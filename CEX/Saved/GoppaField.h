#ifndef _CEX_GOPPAFIELD_H
#define _CEX_GOPPAFIELD_H

#include "CexDomain.h"

NAMESPACE_MCELIECE

/**
* \internal
*/

//typedef ushort goppa;

/// <summary>
/// An internal McEliece utilities class
/// </summary>
class GoppaField
{
public:

	static void Add(ulong* Output, ulong* X, ulong* Y, size_t Degree)
	{
		for (size_t i = 0; i < Degree; i++)
			Output[i] = X[i] ^ Y[i];
	}

	static void Copy(ulong* Output, const ulong* Input, const size_t Degree)
	{
		for (size_t i = 0; i < Degree; i++)
			Output[i] = Input[i];
	}

	inline static ushort Diff(ushort A, ushort B)
	{
		uint t = (uint)(A ^ B);

		t = ((t - 1) >> 20) ^ 0xFFF;

		return (ushort)t;
	}

	static ushort Invert(ushort X, const size_t Degree)
	{
		ushort tmpA;
		ushort tmpB;
		ushort out = X;

		out = Square(out, Degree);
		tmpA = Multiply(out, X, Degree);
		out = Square(tmpA, Degree);
		out = Square(out, Degree);
		tmpB = Multiply(out, tmpA, Degree);
		out = Square(tmpB, Degree);
		out = Square(out, Degree);
		out = Square(out, Degree);
		out = Square(out, Degree);
		out = Multiply(out, tmpB, Degree);
		out = Square(out, Degree);
		out = Square(out, Degree);
		out = Multiply(out, tmpA, Degree);
		out = Square(out, Degree);
		out = Multiply(out, X, Degree);

		return Square(out, Degree);
	}

	static ushort Multiply(ushort X, ushort Y, const size_t Degree)
	{
		uint t;
		uint t0;
		uint t1;
		uint tmp;

		t0 = X;
		t1 = Y;
		tmp = t0 * (t1 & 1);

		for (size_t i = 1; i < Degree; i++)
			tmp ^= (t0 * (t1 & (1 << i)));

		t = tmp & 0x7FC000;
		tmp ^= t >> 9;
		tmp ^= t >> 12;

		t = tmp & 0x3000;
		tmp ^= t >> 9;
		tmp ^= t >> 12;

		return tmp & ((1 << Degree) - 1);
	}

	static ulong Or(const ulong* Input, const size_t Degree)
	{
		ulong ret = Input[0];

		for (size_t i = 1; i < Degree; i++)
			ret |= Input[i];

		return ret;
	}

	static ushort Square(ushort X, const size_t Degree)
	{
		const uint B[] = { 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF };

		uint y = X;
		uint t;

		y = (y | (y << 8)) & B[3];
		y = (y | (y << 4)) & B[2];
		y = (y | (y << 2)) & B[1];
		y = (y | (y << 1)) & B[0];

		t = y & 0x7FC000;
		y ^= t >> 9;
		y ^= t >> 12;

		t = y & 0x3000;
		y ^= t >> 9;
		y ^= t >> 12;

		return y & ((1 << Degree) - 1);
	}
};

NAMESPACE_MCELIECEEND
#endif