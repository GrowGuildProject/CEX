#include "PBR.h"
#include "DigestFromName.h"
#include "IntUtils.h"

NAMESPACE_PRNG

const std::string PBR::CLASS_NAME("PBR");

//~~~Properties~~~//

const Prngs PBR::Enumeral()
{
	return Prngs::PBR;
}

const std::string PBR::Name()
{
	return CLASS_NAME + "-" + m_rngGenerator->Name();
}

//~~~Constructor~~~//

PBR::PBR(std::vector<byte> &Seed, int Iterations, Digests DigestEngine, size_t BufferSize)
	:
	m_bufferIndex(0),
	m_bufferSize(BufferSize),
	m_digestIterations(Iterations),
	m_digestType(DigestEngine),
	m_isDestroyed(false),
	m_rngBuffer(BufferSize),
	m_rndSeed(Seed)
{
	if (Iterations == 0)
		throw CryptoRandomException("PBR:Ctor", "Iterations can not be zero; at least 1 iteration is required!");
	if (GetMinimumSeedSize(DigestEngine) > Seed.size())
		throw CryptoRandomException("PBR:Ctor", "The state seed is too small! must be at least digests block size!");
	if (BufferSize < 64)
		throw CryptoRandomException("PBR:Ctor", "BufferSize must be at least 64 bytes!");

	Reset();
}

PBR::~PBR()
{
	Destroy();
}

//~~~Public Functions~~~//

void PBR::Destroy()
{
	if (!m_isDestroyed)
	{
		m_isDestroyed = true;
		m_bufferIndex = 0;
		m_bufferSize = 0;
		m_digestIterations = 0;

		Utility::IntUtils::ClearVector(m_rngBuffer);
		Utility::IntUtils::ClearVector(m_rndSeed);

		if (m_rngGenerator != 0)
			delete m_rngGenerator;
	}
}

void PBR::Fill(std::vector<ushort> &Output, size_t Offset, size_t Elements)
{
	CexAssert(Output.size() - Offset <= Elements, "the output array is too short");

	size_t bufLen = Elements * sizeof(ushort);
	std::vector<byte> buf(bufLen);
	GetBytes(buf);
	Utility::MemUtils::Copy(buf, 0, Output, Offset, bufLen);
}

void PBR::Fill(std::vector<uint> &Output, size_t Offset, size_t Elements)
{
	CexAssert(Output.size() - Offset <= Elements, "the output array is too short");

	size_t bufLen = Elements * sizeof(uint);
	std::vector<byte> buf(bufLen);
	GetBytes(buf);
	Utility::MemUtils::Copy(buf, 0, Output, Offset, bufLen);
}

void PBR::Fill(std::vector<ulong> &Output, size_t Offset, size_t Elements)
{
	CexAssert(Output.size() - Offset <= Elements, "the output array is too short");

	size_t bufLen = Elements * sizeof(ulong);
	std::vector<byte> buf(bufLen);
	GetBytes(buf);
	Utility::MemUtils::Copy(buf, 0, Output, Offset, bufLen);
}

std::vector<byte> PBR::GetBytes(size_t Size)
{
	std::vector<byte> data(Size);
	GetBytes(data);
	return data;
}

void PBR::GetBytes(std::vector<byte> &Output)
{
	if (Output.size() == 0)
		throw CryptoRandomException("PBR:GetBytes", "Buffer size must be at least 1 byte!");

	if (m_rngBuffer.size() - m_bufferIndex < Output.size())
	{
		size_t bufSize = m_rngBuffer.size() - m_bufferIndex;
		// copy remaining bytes
		if (bufSize != 0)
			Utility::MemUtils::Copy(m_rngBuffer, m_bufferIndex, Output, 0, bufSize);

		size_t rem = Output.size() - bufSize;

		while (rem != 0)
		{
			// fill buffer
			m_rngGenerator->Generate(m_rngBuffer);

			if (rem > m_rngBuffer.size())
			{
				Utility::MemUtils::Copy(m_rngBuffer, 0, Output, bufSize, m_rngBuffer.size());
				bufSize += m_rngBuffer.size();
				rem -= m_rngBuffer.size();
			}
			else
			{
				Utility::MemUtils::Copy(m_rngBuffer, 0, Output, bufSize, rem);
				m_bufferIndex = rem;
				rem = 0;
			}
		}
	}
	else
	{
		Utility::MemUtils::Copy(m_rngBuffer, m_bufferIndex, Output, 0, Output.size());
		m_bufferIndex += Output.size();
	}
}

ushort PBR::NextUShort()
{
	return Utility::IntUtils::LeBytesTo16(GetBytes(2), 0);
}

ushort PBR::NextUShort(ushort Maximum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");

	ushort num;

	do
	{
		num = (ushort)GetRanged(Maximum, sizeof(ushort));
	} while (num > Maximum);

	return num;
}

ushort PBR::NextUShort(ushort Maximum, ushort Minimum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");
	CexAssert(Maximum > Minimum, "minimum can not be more than maximum");

	uint num = 0;
	while ((num = NextUShort(Maximum)) < Minimum) {}
	return num;
}

uint PBR::Next()
{
	return Utility::IntUtils::LeBytesTo32(GetBytes(4), 0);
}

uint PBR::Next(uint Maximum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");

	uint num;

	do
	{
		num = (uint)GetRanged(Maximum, sizeof(uint));
	} while (num > Maximum);

	return num;
}

uint PBR::Next(uint Maximum, uint Minimum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");
	CexAssert(Maximum > Minimum, "minimum can not be more than maximum");

	uint num = 0;
	while ((num = Next(Maximum)) < Minimum) {}
	return num;
}

ulong PBR::NextULong()
{
	return Utility::IntUtils::LeBytesTo64(GetBytes(8), 0);
}

ulong PBR::NextULong(ulong Maximum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");

	ulong num;

	do
	{
		num = GetRanged(Maximum, sizeof(ulong));
	} while (num > Maximum);

	return num;
}

ulong PBR::NextULong(ulong Maximum, ulong Minimum)
{
	CexAssert(Maximum != 0, "maximum can not be zero");
	CexAssert(Maximum > Minimum, "minimum can not be more than maximum");

	ulong num = 0;
	while ((num = NextULong(Maximum)) < Minimum) {}
	return num;
}

void PBR::Reset()
{
	m_rngGenerator = new Kdf::PBKDF2(m_digestType, m_digestIterations);
	m_rngGenerator->Initialize(m_rndSeed);
	m_rngGenerator->Generate(m_rngBuffer);
	m_bufferIndex = 0;
}

//~~~Private Functions~~~//

ulong PBR::GetRanged(ulong Maximum, size_t Length)
{
	std::vector<byte> rand;

	if (Maximum < 256)
		rand = GetBytes(1);
	else if (Maximum < 65536)
		rand = GetBytes(2);
	else if (Maximum < 16777216)
		rand = GetBytes(3);
	else if (Maximum < 4294967296)
		rand = GetBytes(4);
	else if (Maximum < 1099511627776)
		rand = GetBytes(5);
	else if (Maximum < 281474976710656)
		rand = GetBytes(6);
	else if (Maximum < 72057594037927936)
		rand = GetBytes(7);
	else
		rand = GetBytes(8);

	ulong val = 0;
	Utility::MemUtils::CopyToValue(rand, 0, val, rand.size());

	ulong bits = Length * 8;
	while (val > Maximum && bits != 0)
	{
		val >>= 1;
		bits--;
	}

	return val;
}

uint PBR::GetMinimumSeedSize(Digests RngEngine)
{
	switch (RngEngine)
	{
		case Digests::Blake256:
			return 64;
		case Digests::Blake512:
			return 128;
		case Digests::Keccak256:
			return 136;
		case Digests::Keccak512:
		case Digests::Keccak1024:
			return 72;
		case Digests::SHA256:
			return 55;
		case Digests::SHA512:
			return 111;
		case Digests::Skein1024:
			return 128;
		case Digests::Skein256:
			return 32;
		case Digests::Skein512:
			return 64;
		default:
			return 128;
	}
}

NAMESPACE_PRNGEND