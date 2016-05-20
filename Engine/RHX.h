// The MIT License (MIT)
// 
// Copyright (c) 2016 vtdev.com
// This file is part of the CEX Cryptographic library.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Principal Algorithms:
// Cipher implementation based on the Rijndael block cipher designed by Joan Daemen and Vincent Rijmen:
// Rijndael <a href="http://csrc.nist.gov/archive/aes/rijndael/Rijndael-ammended.pdf">Specification</a>.
// AES specification <a href="http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf">Fips 197</a>.
// 
// Implementation Details:
// An implementation based on the Rijndael block cipher, 
// using HKDF with a selectable Message Digest for expanded key generation.
// Rijndael HKDF Extended (RHX)
// Written by John Underhill, November 11, 2014
// contact: develop@vtdev.com

#ifndef _CEXENGINE_RHX_H
#define _CEXENGINE_RHX_H

#include "IBlockCipher.h"

NAMESPACE_BLOCK

/// <summary>
/// RHX: A Rijndael Cipher extended with an (optional) HKDF powered Key Schedule.
/// <para>RHX is a Rijndael implementation that uses a standard configuration on key sizes up to 64 bytes (512 bits). 
/// An optional HKDF Expand bytes generator can be used to expand the user supplied key into a working key integer array
/// for increased rounds and additional security.</para>
/// </summary> 
/// 
/// <example>
/// <description>Example of encrypting a block:</description>
/// <code>
/// CTR cipher(new RHX());
/// // initialize for encryption
/// cipher.Initialize(true, KeyParams(Key, IV));
/// // encrypt a block
/// cipher.Transform(Input, Output);
/// </code>
/// </example>
/// 
/// <seealso cref="CEX::Enumeration::BlockCiphers"/>
/// <seealso cref="CEX::Enumeration::Digests"/>
/// <seealso cref="CEX::Digest::IDigest"/>
/// 
/// <remarks>
/// <description>Implementation Notes:</description>
/// <para>The key schedule in RHX is the defining difference between this and a standard version of Rijndael.
/// The standard Rijndael Key Schedule (128-256 bits), has been extended to accomodate a 512 bit key size.
/// RHX can (optionally) use an HMAC based Key Derivation Function (HKDF) to expand the cipher key to create the internal round key integer array. 
/// This provides better security, and allows for a user assignable number of transformation rounds.
/// When using a the HKDF extended mode, the number of diffusion rounds can be set by the user (through the class constructor). 
/// RHX can run between 10 and 38 rounds.</para>
/// 
/// <list type="bullet">
/// <item><description>When using the standard cipher, the key length the rounds calculation is done automatically: 10, 12, 14, and 22, for key sizes 126, 192, 256, and 512 bits.</description></item>
/// <item><description>HKDF Digest engine is definable through the RHX(uint, uint, Digests) Constructor parameter: KDFEngine.</description></item>
/// <item><description>Key Schedule is powered by a Hash based Key Derivation Function using a user definable Digest.</description></item>
/// <item><description>Minimum key size is the Digests hash return size, and extendable in blocks of (N * Hash Size) bytes.</description></item>
/// <item><description>Valid block sizes are 16 and 32 byte wide.</description></item>
/// <item><description>The rounds can only be assigned if using the HKDF extended mode. Valid Rounds are 10 to 38, default is 22.</description></item>
/// </list>
/// 
/// <para>When using SHA-2 256, a minimum key size for RHX is 32 bytes, further blocks of can be added to the key so long as they align; (n * hash size), ex. 64, 128, 192 bytes.. there is no upper maximum.</para> 
/// 
/// <para>The Digest that powers HKDF, can be any one of the Hash Digests implemented in the CEX library; Blake, Keccak, SHA-2 or Skein.
/// Correct key sizes can be determined at runtime using the <see cref="LegalKeySizes"/> property, based on the digest selected.
/// When using the extended mode, the legal key sizes are determined based on the selected digests hash output size, 
/// ex. SHA256 the minimum legal key size is 256 bits (32 bytes), the recommended size is 2* the hash size, or 512 bits (64 bytes).</para>
/// 
/// <para>The number of diffusion rounds processed within the ciphers rounds function can also be defined; adding rounds creates a more diffused cipher output, making the resulting cipher-text more difficult to cryptanalyze. 
/// RHX is capable of processing up to 38 rounds, that is twenty-four rounds more than the fourteen rounds used in an implementation of AES-256. 
/// Valid rounds assignments can be found in the <see cref="LegalRounds"/> static property.</para>
/// 
/// <description>Guiding Publications:</description>
/// <list type="number">
/// <item><description>NIST <a href="http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf">AES Fips 197</a>.</description></item>
/// <item><description>NIST <a href="http://csrc.nist.gov/archive/aes/rijndael/Rijndael-ammended.pdf">Rijndael ammended</a>.</description></item>
/// <item><description>HMAC <a href="http://tools.ietf.org/html/rfc2104">RFC 2104</a>.</description></item>
/// <item><description>Fips <a href="http://csrc.nist.gov/publications/fips/fips198-1/FIPS-198-1_final.pdf">198.1</a>.</description></item>
/// <item><description>HKDF <a href="http://tools.ietf.org/html/rfc5869">RFC 5869</a>.</description></item>
/// <item><description>NIST <a href="http://csrc.nist.gov/publications/drafts/800-90/draft-sp800-90b.pdf">SP800-90B</a>.</description></item>
/// <item><description>SHA3 <a href="https://131002.net/blake/blake.pdf">The Blake digest</a>.</description></item>
/// <item><description>SHA3 <a href="http://keccak.noekeon.org/Keccak-submission-3.pdf">The Keccak digest</a>.</description></item>
/// <item><description>SHA3 <a href="http://www.skein-hash.info/sites/default/files/skein1.1.pdf">The Skein digest</a>.</description></item>
/// </list>
/// </remarks>
class RHX : public IBlockCipher
{
private:
	static constexpr size_t BLOCK16 = 16;
	static constexpr size_t BLOCK32 = 32;
	static constexpr size_t LEGAL_KEYS = 10;
	static constexpr size_t MAX_ROUNDS = 38;
	static constexpr size_t MIN_ROUNDS = 10;
	static constexpr size_t ROUNDS22 = 22;

	size_t _blockSize;
	bool _destroyEngine;
	size_t _dfnRounds;
	std::vector<uint> _expKey;
	std::vector<byte> _hkdfInfo;
	bool _isDestroyed;
	bool _isEncryption;
	bool _isInitialized;
	size_t _ikmSize;
	CEX::Enumeration::Digests _kdfEngineType;
	CEX::Digest::IDigest* _kdfEngine;
	std::vector<size_t> _legalKeySizes;
	std::vector<size_t> _legalRounds;

public:

	// *** Properties *** //

	/// <summary>
	/// Get: Unit block size of internal cipher in bytes.
	/// <para>Block size must be 16 or 32 bytes wide.
	/// Value set in class constructor.</para>
	/// </summary>
	virtual const size_t BlockSize() { return _blockSize; }

	/// <summary>
	/// Get/Set: Sets the Info value in the HKDF initialization parameters.
	/// <para>Must be set before <see cref="Initialize(bool, KeyParams)"/> is called.
	/// Changing this code will create a unique distribution of the cipher.
	/// Code can be either a zero byte array, or a multiple of the HKDF digest engines return size.</para>
	/// </summary>
	///
	/// <exception cref="CryptoSymmetricCipherException">Thrown if an invalid distribution code is used</exception>
	const std::vector<byte> &DistributionCode() { return _hkdfInfo; }

	/// <summary>
	/// Get: The block ciphers type name
	/// </summary>
	virtual const CEX::Enumeration::BlockCiphers Enumeral() { return CEX::Enumeration::BlockCiphers::RHX; }

	/// <summary>
	/// Get: Initialized for encryption, false for decryption.
	/// <para>Value set in <see cref="Initialize(bool, KeyParams)"/>.</para>
	/// </summary>
	virtual const bool IsEncryption() { return _isEncryption; }

	/// <summary>
	/// Get: Cipher is ready to transform data
	/// </summary>
	virtual const bool IsInitialized() { return _isInitialized; }

	/// <summary>
	/// Get: Available Encryption Key Sizes in bytes
	/// </summary>
	virtual const std::vector<size_t> &LegalKeySizes() { return _legalKeySizes; }

	/// <summary>
	/// Get: Available diffusion round assignments
	/// </summary>
	virtual const std::vector<size_t> &LegalRounds() { return _legalRounds; }

	/// <summary>
	/// Get: Cipher name
	/// </summary>
	virtual const char* Name() { return "RHX"; }

	/// <summary>
	/// Get: The number of diffusion rounds processed by the transform
	/// </summary>
	virtual const size_t Rounds() { return _dfnRounds; }

	// *** Constructor *** //

	/// <summary>
	/// Initialize the class with a Digest instance (HKDF mode)
	/// </summary>
	///
	/// <param name="KdfEngine">The Key Schedule KDF digest engine instance; can be any one of the Digest implementations.</param>
	/// <param name="Rounds">Number of diffusion rounds. The <see cref="LegalRounds"/> property contains available sizes.  Default is 22 rounds.</param>
	/// <param name="BlockSize">Cipher input Block Size. Default is 16 bytes.</param>
	///
	/// <exception cref="CEX::Exception::CryptoSymmetricCipherException">Thrown if an invalid block size or invalid rounds count are used</exception>
	RHX(CEX::Digest::IDigest *KdfEngine, size_t Rounds = ROUNDS22, size_t BlockSize = BLOCK16)
		:
		_blockSize(BlockSize),
		_destroyEngine(false),
		_dfnRounds(Rounds),
		_hkdfInfo(0, 0),
		_ikmSize(0),
		_isDestroyed(false),
		_isEncryption(false),
		_isInitialized(false),
		_kdfEngine(KdfEngine),
		_kdfEngineType(CEX::Enumeration::Digests::SHA512),
		_legalKeySizes(LEGAL_KEYS, 0),
		_legalRounds(15, 0)
	{
		if (KdfEngine == 0)
			throw CryptoSymmetricCipherException("RHX:CTor", "Invalid null parameter! The digest instance can not be null.");
		if (BlockSize != BLOCK16 && BlockSize != BLOCK32)
			throw CryptoSymmetricCipherException("RHX:CTor", "Invalid block size! Supported block sizes are 16 and 32 bytes.");
		if (Rounds < MIN_ROUNDS || Rounds > MAX_ROUNDS || Rounds % 2 > 0)
			throw CryptoSymmetricCipherException("RHX:CTor", "Invalid rounds size! Sizes supported are even numbers between 10 and 38.");

		std::string info = "information string RHX version 1";
		_hkdfInfo.reserve(info.size());
		for (size_t i = 0; i < info.size(); ++i)
			_hkdfInfo.push_back(info[i]);

		_legalRounds = { 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38 };
		_kdfEngineType = KdfEngine->Enumeral();
		// set the hmac key size
		_ikmSize = _kdfEngine->DigestSize();

		// add standard key lengths
		_legalKeySizes[0] = 16;
		_legalKeySizes[1] = 24;
		_legalKeySizes[2] = 32;
		_legalKeySizes[3] = 64;

		for (size_t i = 4; i < _legalKeySizes.size(); ++i)
			_legalKeySizes[i] = (_legalKeySizes[3] + _ikmSize * (i - 3));
	}

	/// <summary>
	/// Initialize the class
	/// </summary>
	/// 
	/// <param name="BlockSize">Cipher input <see cref="BlockSize"/>. The <see cref="LegalBlockSizes"/> property contains available sizes. Default is 16 bytes.</param>
	/// <param name="Rounds">Number of diffusion rounds. The <see cref="LegalRounds"/> property contains available sizes. 
	/// Default is based on the key size; defining rounds requires HKDF extended mode.</param>
	/// <param name="KdfEngineType">The Key Schedule HKDF digest engine; can be any one of the <see cref="CEX::Enumeration::Digests">Digest</see> 
	/// implementations. The default engine is None, which invokes the standard key schedule mechanism.</param>
	/// 
	/// <exception cref="CEX::Exception::CryptoSymmetricCipherException">Thrown if an invalid block size or invalid rounds count are used</exception>
	RHX(size_t BlockSize = BLOCK16, size_t Rounds = ROUNDS22, CEX::Enumeration::Digests KdfEngineType = CEX::Enumeration::Digests::None)
		:
		_blockSize(BlockSize),
		_destroyEngine(true),
		_dfnRounds(Rounds),
		_hkdfInfo(0, 0),
		_ikmSize(0),
		_isDestroyed(false),
		_isEncryption(false),
		_isInitialized(false),
		_kdfEngine(0),
		_kdfEngineType(KdfEngineType),
		_legalKeySizes(LEGAL_KEYS, 0),
		_legalRounds(0, 0)
	{
		if (BlockSize != BLOCK16 && BlockSize != BLOCK32)
			throw CryptoSymmetricCipherException("RHX:CTor", "Invalid block size! Supported block sizes are 16 and 32 bytes.");

		// add standard key lengths
		_legalKeySizes[0] = 16;
		_legalKeySizes[1] = 24;
		_legalKeySizes[2] = 32;
		_legalKeySizes[3] = 64;

		if (KdfEngineType != CEX::Enumeration::Digests::None)
		{
			if (Rounds < MIN_ROUNDS || Rounds > MAX_ROUNDS || Rounds % 2 != 0)
				throw CryptoSymmetricCipherException("RHX:CTor", "Invalid rounds size! Sizes supported are even numbers between 10 and 38.");

			std::string info = "information string RHX version 1";
			_hkdfInfo.reserve(info.size());
			for (size_t i = 0; i < info.size(); ++i)
				_hkdfInfo.push_back(info[i]);

			_legalRounds.resize(15);
			_legalRounds = { 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38 };
			// set the hmac key size
			_ikmSize = GetIkmSize(KdfEngineType);

			// hkdf extended key sizes
			for (size_t i = 4; i < _legalKeySizes.size(); ++i)
				_legalKeySizes[i] = (_legalKeySizes[3] + _ikmSize * (i - 3));
		}
		else
		{
			_legalKeySizes.resize(4);
			_legalRounds.resize(4);
			_legalRounds = { 10, 12, 14, 22 };
		}
	}

	/// <summary>
	/// Finalize objects
	/// </summary>
	virtual ~RHX()
	{
		Destroy();
	}

	// *** Public Methods *** //

	/// <summary>
	/// Decrypt a single block of bytes.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called with the Encryption flag set to <c>false</c> before this method can be used.
	/// Input and Output arrays must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Encrypted bytes</param>
	/// <param name="Output">Decrypted bytes</param>
	virtual void DecryptBlock(const std::vector<byte> &Input, std::vector<byte> &Output);

	/// <summary>
	/// Decrypt a block of bytes with offset parameters.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called with the Encryption flag set to <c>false</c> before this method can be used.
	/// Input and Output arrays with Offsets must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Encrypted bytes</param>
	/// <param name="InOffset">Offset in the Input array</param>
	/// <param name="Output">Decrypted bytes</param>
	/// <param name="OutOffset">Offset in the Output array</param>
	virtual void DecryptBlock(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);

	/// <summary>
	/// Clear the buffers and reset
	/// </summary>
	virtual void Destroy();

	/// <summary>
	/// Encrypt a block of bytes.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called with the Encryption flag set to <c>true</c> before this method can be used.
	/// Input and Output array lengths must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Input bytes to Transform</param>
	/// <param name="Output">Output product of Transform</param>
	virtual void EncryptBlock(const std::vector<byte> &Input, std::vector<byte> &Output);

	/// <summary>
	/// Encrypt a block of bytes with offset parameters.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called with the Encryption flag set to <c>true</c> before this method can be used.
	/// Input and Output arrays with Offsets must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Input bytes to Transform</param>
	/// <param name="InOffset">Offset in the Input array</param>
	/// <param name="Output">Output product of Transform</param>
	/// <param name="OutOffset">Offset in the Output array</param>
	virtual void EncryptBlock(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);

	/// <summary>
	/// Initialize the Cipher.
	/// </summary>
	///
	/// <param name="Encryption">Using Encryption or Decryption mode</param>
	/// <param name="KeyParam">Cipher key container. <para>The <see cref="LegalKeySizes"/> property contains valid sizes.</para></param>
	///
	/// <exception cref="CryptoSymmetricCipherException">Thrown if a null or invalid key is used</exception>
	virtual void Initialize(bool Encryption, const CEX::Common::KeyParams &KeyParam);

	/// <summary>
	/// Transform a block of bytes.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called before this method can be used.
	/// Input and Output array lengths must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Input bytes to Transform or Decrypt</param>
	/// <param name="Output">Output product of Transform</param>
	virtual void Transform(const std::vector<byte> &Input, std::vector<byte> &Output);

	/// <summary>
	/// Transform a block of bytes with offset parameters.
	/// <para><see cref="Initialize(bool, KeyParams)"/> must be called before this method can be used.
	/// Input and Output arrays with Offsets must be at least <see cref="BlockSize"/> in length.</para>
	/// </summary>
	///
	/// <param name="Input">Input bytes to Transform</param>
	/// <param name="InOffset">Offset in the Input array</param>
	/// <param name="Output">Output product of Transform</param>
	/// <param name="OutOffset">Offset in the Output array</param>
	virtual void Transform(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);

private:
	void Decrypt16(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);
	void Decrypt32(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);
	void Encrypt16(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);
	void Encrypt32(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset);
	void ExpandKey(bool Encryption, const std::vector<byte> &Key);
	void ExpandRotBlock(std::vector<uint> &Key, size_t KeyIndex, size_t KeyOffset, size_t RconIndex);
	void ExpandSubBlock(std::vector<uint> &Key, size_t KeyIndex, size_t KeyOffset);
	CEX::Digest::IDigest* GetDigest(CEX::Enumeration::Digests DigestType);
	int GetIkmSize(CEX::Enumeration::Digests DigestType);
	void SecureExpand(const std::vector<byte> &Key);
	void StandardExpand(const std::vector<byte> &Key);
	uint SubByte(uint Rot);
};

NAMESPACE_BLOCKEND
#endif

