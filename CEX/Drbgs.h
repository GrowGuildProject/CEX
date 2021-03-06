#ifndef CEX_DRBGS_H
#define CEX_DRBGS_H

#include "CexDomain.h"

NAMESPACE_ENUMERATION

/// <summary>
/// Pseudo Random Generator enmumeration names
/// </summary>
enum class Drbgs : byte
{
	/// <summary>
	/// No generator is specified
	/// </summary>
	None = 0,
	/// <summary>
	/// An implementation of a Block Counter mode Generator
	/// </summary>
	BCG = 1,
	/// <summary>
	/// An implementation of a Digest Counter Generator
	/// </summary>
	DCG = 2,
	/// <summary>
	/// An implementation of an HMAC Counter Generator
	/// </summary>
	HCG = 3
};

NAMESPACE_ENUMERATIONEND
#endif