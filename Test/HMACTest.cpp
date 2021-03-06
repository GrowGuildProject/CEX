#include "HMACTest.h"
#include "../CEX/HMAC.h"
#include "../CEX/SHA256.h"
#include "../CEX/SHA512.h"
#include "../CEX/SymmetricKey.h"

namespace Test
{
	using Key::Symmetric::SymmetricKey;

	const std::string HMACTest::DESCRIPTION = "RFC 4321 Test Vectors for HMAC SHA224, SHA256, SHA384, and SHA512.";
	const std::string HMACTest::FAILURE = "FAILURE! ";
	const std::string HMACTest::SUCCESS = "SUCCESS! All HMAC tests have executed succesfully.";

	HMACTest::HMACTest()
		:
		m_expected256(0),
		m_expected512(0),
		m_keys(0),
		m_input(0),
		m_progressEvent()
	{
	}

	HMACTest::~HMACTest()
	{
	}

	std::string HMACTest::Run()
	{
		try
		{
			Initialize();

			CompareVector256(m_keys[0], m_input[0], m_expected256[0]);
			CompareVector256(m_keys[1], m_input[1], m_expected256[1]);
			CompareVector256(m_keys[2], m_input[2], m_expected256[2]);
			CompareVector256(m_keys[3], m_input[3], m_expected256[3]);
			CompareVector256(m_keys[4], m_input[4], m_expected256[4]);
			CompareVector256(m_keys[5], m_input[5], m_expected256[5]);
			CompareVector256(m_keys[6], m_input[6], m_expected256[6]);
			OnProgress(std::string("HMACTest: Passed SHA-2 256 bit known answer vector tests.."));

			CompareVector512(m_keys[0], m_input[0], m_expected512[0]);
			CompareVector512(m_keys[1], m_input[1], m_expected512[1]);
			CompareVector512(m_keys[2], m_input[2], m_expected512[2]);
			CompareVector512(m_keys[3], m_input[3], m_expected512[3]);
			CompareVector512(m_keys[4], m_input[4], m_expected512[4]);
			CompareVector512(m_keys[5], m_input[5], m_expected512[5]);
			CompareVector512(m_keys[6], m_input[6], m_expected512[6]);
			OnProgress(std::string("HMACTest: Passed SHA-2 512 bit known answer vector tests.."));

			CompareAccess(m_keys[3]);
			OnProgress(std::string("Passed Finalize/Compute methods output comparison.."));

			return SUCCESS;
		}
		catch (TestException const &ex)
		{
			throw TestException(FAILURE + std::string(" : ") + ex.Message());
		}
		catch (...)
		{
			throw TestException(std::string(FAILURE + std::string(" : Unknown Error")));
		}
	}

	void HMACTest::CompareAccess(std::vector<byte> &Key)
	{
		std::vector<byte> hash1(32);
		Digest::SHA256* eng = new Digest::SHA256();
		Mac::HMAC mac(eng);
		SymmetricKey kp(Key);

		mac.Initialize(kp);

		std::vector<byte> input(256);
		for (size_t i = 0; i < input.size(); ++i)
			input[i] = (byte)i;

		mac.Compute(input, hash1);

		std::vector<byte> hash2(32);
		mac.Update(input, 0, 128);
		mac.Update(input, 128, 128);
		mac.Finalize(hash2, 0);
		delete eng;

		if (hash1 != hash2)
			throw TestException("CMAC is not equal!");
	}

	void HMACTest::CompareVector256(std::vector<byte> &Key, std::vector<byte> &Input, std::vector<byte> &Expected)
	{
		std::vector<byte> hash(32, 0);
		Digest::SHA256* eng = new Digest::SHA256();
		Mac::HMAC mac(eng);
		SymmetricKey kp(Key);

		mac.Initialize(kp);
		mac.Compute(Input, hash);

		delete eng;

		// truncated output, test case #5
		if (Expected.size() != 32)
		{
			std::vector<byte> tmph;
			tmph.resize(Expected.size(), 0);
			std::memcpy(&tmph[0], &hash[0], Expected.size());

			if (Expected != tmph)
				throw TestException("HMACTest: return code is not equal!");
		}
		else
		{
			if (Expected != hash)
				throw TestException("HMACTest: return code is not equal!");
		}
	}

	void HMACTest::CompareVector512(std::vector<byte> &Key, std::vector<byte> &Input, std::vector<byte> &Expected)
	{
		std::vector<byte> hash(32, 0);
		Digest::SHA512* eng = new Digest::SHA512();
		Mac::HMAC mac(eng);
		SymmetricKey kp(Key);

		mac.Initialize(kp);
		mac.Compute(Input, hash);

		delete eng;

		if (Expected.size() != 64)
		{
			std::vector<byte> tmph;
			tmph.resize(Expected.size(), 0);
			std::memcpy(&tmph[0], &hash[0], Expected.size());

			if (Expected != tmph)
				throw TestException("HMACTest: return code is not equal!");
		}
		else
		{
			if (Expected != hash)
				throw TestException("HMACTest: return code is not equal!");
		}
	}

	void HMACTest::Initialize()
	{
		const char* keysEnc[7] =
		{
			("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"),
			("4a656665"),
			("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
			("0102030405060708090a0b0c0d0e0f10111213141516171819"),
			("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"),
			("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
			("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
		};
		HexConverter::Decode(keysEnc, 7, m_keys);

		const char* plainTextEnc[7] =
		{
			("4869205468657265"),
			("7768617420646f2079612077616e7420666f72206e6f7468696e673f"),
			("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
			("cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd"),
			("546573742057697468205472756e636174696f6e"),
			("54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a65204b6579202d2048617368204b6579204669727374"),
			("5468697320697320612074657374207573696e672061206c6172676572207468616e20626c6f636b2d73697a65206b657920616e642061206c61726765722074" \
				"68616e20626c6f636b2d73697a6520646174612e20546865206b6579206e6565647320746f20626520686173686564206265666f7265206265696e6720757365" \
				"642062792074686520484d414320616c676f726974686d2e")
		};
		HexConverter::Decode(plainTextEnc, 7, m_input);

		const char* expected256Enc[7] =
		{
			("b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7"),
			("5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843"),
			("773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe"),
			("82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b"),
			("a3b6167473100ee06e0c796c2955552b"),
			("60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54"),
			("9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2")
		};
		HexConverter::Decode(expected256Enc, 7, m_expected256);

		const char* expected512Enc[7] =
		{
			("87aa7cdea5ef619d4ff0b4241a1d6cb02379f4e2ce4ec2787ad0b30545e17cdedaa833b7d6b8a702038b274eaea3f4e4be9d914eeb61f1702e696c203a126854"),
			("164b7a7bfcf819e2e395fbe73b56e0a387bd64222e831fd610270cd7ea2505549758bf75c05a994a6d034f65f8f0e6fdcaeab1a34d4a6b4b636e070a38bce737"),
			("fa73b0089d56a284efb0f0756c890be9b1b5dbdd8ee81a3655f83e33b2279d39bf3e848279a722c806b485a47e67c807b946a337bee8942674278859e13292fb"),
			("b0ba465637458c6990e5a8c5f61d4af7e576d97ff94b872de76f8050361ee3dba91ca5c11aa25eb4d679275cc5788063a5f19741120c4f2de2adebeb10a298dd"),
			("415fad6271580a531d4179bc891d87a6"),
			("80b24263c7c1a3ebb71493c1dd7be8b49b46d1f41b4aeec1121b013783f8f3526b56d037e05f2598bd0fd2215d6a1e5295e64f73f63f0aec8b915a985d786598"),
			("e37b6a775dc87dbaa4dfa9f96e5e3ffddebd71f8867289865df5a32d20cdc944b6022cac3c4982b10d5eeb55c3e4de15134676fb6de0446065c97440fa8c6a58")
		};
		HexConverter::Decode(expected512Enc, 7, m_expected512);
	}

	void HMACTest::OnProgress(std::string Data)
	{
		m_progressEvent(Data);
	}
}