#ifndef CEX_RDP_H
#define CEX_RDP_H

#include "IProvider.h"

NAMESPACE_PROVIDER

/// <summary>
/// An implementation of the Intel RdRand digital random number generator
/// </summary>
/// 
/// <example>
/// <description>Example of getting a seed value:</description>
/// <code>
/// std:vector&lt;byte&gt; output(32);
/// RDP gen;
/// gen.GetBytes(output);
/// </code>
/// </example>
/// 
/// <remarks>
/// <para>The RdRand DRNG uses thermal noise to generate random bits that are buffered into a shift register, then fed into a CBC-MAC to condition the bytes.
/// The output from the CBC-MAC is obtained using the RDSEED api. \n
/// To accommodate large sampling, the system has a built in CTR_DRBG, (as specified in SP800-90), which is continuously reseeded with the output from RDSEED.
/// The output from the CTR Drbg is obtained using the RDRAND api. \n
/// There is some controversy surrounding the security of this mechanism, though the design appears to be sound, and has been reviewed by external auditors, 
/// it is still a proprietary closed system. \n
/// The entropy source itself must therefore be considered to be a 'black box', a source that can not be verified directly, and so must be considered to be of low entropy value. \n
/// For this reason, the DRNG should not be used as the sole source of entropy when creating secret keys, but should be used in concert with other sources of entropy.</para>
/// 
/// <description>Guiding Publications::</description>
/// <list type="number">
/// <item><description>Intel Digital Random Number Digital Random Number Generator: <a href="https://software.intel.com/sites/default/files/m/d/4/1/d/8/441_Intel_R__DRNG_Software_Implementation_Guide_final_Aug7.pdf">DRNG</a>.</description></item>
/// <item><description>NIST <a href="http://csrc.nist.gov/publications/drafts/800-90/draft-sp800-90b.pdf">SP800-90B</a>: Recommendation for the Entropy Sources Used for Random Bit Generation.</description></item>
/// <item><description>NIST <a href="http://csrc.nist.gov/publications/fips/fips140-2/fips1402.pdf">Fips 140-2</a>: Security Requirments For Cryptographic Modules.</description></item>
/// <item><description>NIST <a href="http://csrc.nist.gov/publications/drafts/800-90/draft-sp800-90b.pdf">SP800-90B</a>: Recommendation for the Entropy Sources Used for Random Bit Generation.</description></item>
/// <item><description>ANSI <a href="http://csrc.nist.gov/groups/ST/toolkit/documents/rng/EntropySources.pdf">X9.82: </a>Entropy and Entropy Sources in X9.82.</description></item>
/// <item><description>A Provable Security Analysis of Intel's <a href="http://terashima.us/rdrand-ec2015.pdf">Secure Key RNG</a>.</description></item>
/// </list> 
/// </remarks>
class RDP : public IProvider
{
public:

	/// <summary>
	/// The Intel DRNG providers engine configuration type
	/// </summary>
	enum RdEngines : byte
	{
		/// <summary>
		/// The random number provider
		/// </summary>
		RdRand = 0,
		/// <summary>
		/// The random seed generator
		/// </summary>
		RdSeed = 1
	};

private:

	static const std::string CLASS_NAME;
	// the number of times to read from the RDRAND/RDSEED RNGs; each read generates 32 bits of output
	static const size_t RNGPOLLS = 32;
	// RDRAND is guaranteed to generate a random number within 10 retries on a working CPU
	static const size_t RDRRETRY = 10;
	// RdSeed is not guaranteed to generate a random number within a specific number of retries
	static const size_t RDSRETRY = 20;
	static const size_t RDSEEDMAX = 64 * 1000 * 1000;
	static const size_t RDSUCCESS = 1;

	bool m_isAvailable;
	RdEngines m_engineType;

public:

	RDP(const RDP&) = delete;
	RDP& operator=(const RDP&) = delete;
	RDP& operator=(RDP&&) = delete;

	//~~~Properties~~~//

	/// <summary>
	/// Get: The providers type name
	/// </summary>
	const Providers Enumeral() override;

	/// <summary>
	/// Get: The entropy provider is available on this system
	/// </summary>
	const bool IsAvailable() override;

	/// <summary>
	/// Get: The provider class name
	/// </summary>
	const std::string Name() override;

	//~~~Constructor~~~//

	/// <summary>
	/// Instantiate this class
	/// </summary>
	///
	/// <param name="RdEngine">The providers random output engine configuration type; RdRand (post processed by CTR_DRBG), or RdSeed (conditioned seed value)</param>
	RDP(RdEngines RdEngine = RdEngines::RdRand);

	/// <summary>
	/// Destructor
	/// </summary>
	~RDP() override;

	//~~~Public Functions~~~//

	/// <summary>
	/// Release all resources associated with the object; optional, called by the finalizer
	/// </summary>
	void Destroy() override;

	/// <summary>
	/// Fill a buffer with pseudo-random bytes
	/// </summary>
	///
	/// <param name="Output">The output array to fill</param>
	void GetBytes(std::vector<byte> &Output) override;

	/// <summary>
	/// Fill the buffer with pseudo-random bytes
	/// </summary>
	///
	/// <param name="Output">The output array to fill</param>
	/// <param name="Offset">The starting position within the Output array</param>
	/// <param name="Length">The number of bytes to write to the Output array</param>
	void GetBytes(std::vector<byte> &Output, size_t Offset, size_t Length) override;

	/// <summary>
	/// Return an array with pseudo-random bytes
	/// </summary>
	/// 
	/// <param name="Length">The size of the expected array returned</param>
	/// 
	/// <returns>An array of pseudo-random of bytes</returns>
	std::vector<byte> GetBytes(size_t Length) override;

	/// <summary>
	/// Returns a pseudo-random unsigned 32bit integer
	/// </summary>
	uint Next() override;

	/// <summary>
	/// Reset the internal state
	/// </summary>
	void Reset() override;
};

NAMESPACE_PROVIDEREND
#endif
