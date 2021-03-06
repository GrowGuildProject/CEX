#include "MPKCPrivateKey.h"
#include "IntUtils.h"

NAMESPACE_ASYMMETRICKEY

//~~~Properties~~~//

const AsymmetricEngines MPKCPrivateKey::CipherType()
{
	return AsymmetricEngines::McEliece;
}

const MPKCParams MPKCPrivateKey::Parameters()
{
	return m_mpkcParameters;
}


//~~~Constructor~~~//

MPKCPrivateKey::MPKCPrivateKey(MPKCParams Params, std::vector<byte> &S)
	:
	m_isDestroyed(false),
	m_mpkcParameters(Params),
	m_sCoeffs(S)
{
}

MPKCPrivateKey::MPKCPrivateKey(const std::vector<byte> &KeyStream)
	:
	m_isDestroyed(false)
{
	m_mpkcParameters = static_cast<MPKCParams>(KeyStream[0]);
	uint sLen = Utility::IntUtils::LeBytesTo32(KeyStream, 1);
	m_sCoeffs.resize(sLen);
	Utility::MemUtils::Copy(KeyStream, 5, m_sCoeffs, 0, sLen);
}

MPKCPrivateKey::~MPKCPrivateKey()
{
	Destroy();
}

//~~~Public Functions~~~//

void MPKCPrivateKey::Destroy()
{
	if (!m_isDestroyed)
	{
		m_isDestroyed = true;
		m_mpkcParameters = MPKCParams::None;

		if (m_sCoeffs.size() > 0)
			Utility::IntUtils::ClearVector(m_sCoeffs);
	}
}

std::vector<byte> MPKCPrivateKey::ToBytes()
{
	uint sLen = static_cast<uint>(m_sCoeffs.size());
	std::vector<byte> s(sLen + 5);
	s[0] = static_cast<byte>(m_mpkcParameters);
	Utility::IntUtils::Le32ToBytes(sLen, s, 1);
	Utility::MemUtils::Copy(s, 5, m_sCoeffs, 0, sLen);

	return s;
}

NAMESPACE_ASYMMETRICKEYEND