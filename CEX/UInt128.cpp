#include "UInt128.h"

NAMESPACE_NUMERIC

void UInt128::LoadBE(uint X0, uint X1, uint X2, uint X3)
{
	Swap().LoadLE(X0, X1, X2, X3);
}

void UInt128::LoadBE16(const std::vector<byte> &Input, size_t Offset, UInt128 &X0, UInt128 &X1, UInt128 &X2, UInt128 &X3)
{
	X0.LoadBE(Input, Offset);
	X1.LoadBE(Input, Offset + 16);
	X2.LoadBE(Input, Offset + 32);
	X3.LoadBE(Input, Offset + 48);
	Transpose(X0, X1, X2, X3);
}

void UInt128::LoadBE256(const std::vector<byte> &Input, size_t InOffset, std::vector<UInt128> &Output, size_t OutOffset)
{
	Output[OutOffset].LoadBE(Input, InOffset);
	Output[OutOffset + 1].LoadBE(Input, InOffset + 16);
	Output[OutOffset + 2].LoadBE(Input, InOffset + 32);
	Output[OutOffset + 3].LoadBE(Input, InOffset + 48);
	Output[OutOffset + 4].LoadBE(Input, InOffset + 64);
	Output[OutOffset + 5].LoadBE(Input, InOffset + 80);
	Output[OutOffset + 6].LoadBE(Input, InOffset + 96);
	Output[OutOffset + 7].LoadBE(Input, InOffset + 112);
	Output[OutOffset + 8].LoadBE(Input, InOffset + 128);
	Output[OutOffset + 9].LoadBE(Input, InOffset + 144);
	Output[OutOffset + 10].LoadBE(Input, InOffset + 160);
	Output[OutOffset + 11].LoadBE(Input, InOffset + 176);
	Output[OutOffset + 12].LoadBE(Input, InOffset + 192);
	Output[OutOffset + 13].LoadBE(Input, InOffset + 208);
	Output[OutOffset + 14].LoadBE(Input, InOffset + 224);
	Output[OutOffset + 15].LoadBE(Input, InOffset + 240);
}

void UInt128::LoadLE(uint X0, uint X1, uint X2, uint X3)
{
	Register = _mm_set_epi32(X0, X1, X2, X3);
}

void UInt128::LoadLE16(const std::vector<byte> &Input, size_t Offset, UInt128 &X0, UInt128 &X1, UInt128 &X2, UInt128 &X3)
{
	X0.LoadLE(Input, Offset);
	X1.LoadLE(Input, Offset + 16);
	X2.LoadLE(Input, Offset + 32);
	X3.LoadLE(Input, Offset + 48);
	Transpose(X0, X1, X2, X3);
}

void UInt128::LoadLE256(std::vector<byte> &Input, size_t InOffset, std::vector<UInt128> &Output, size_t OutOffset)
{
	Output[OutOffset].LoadLE(Input, InOffset);
	Output[OutOffset + 1].LoadLE(Input, InOffset + 16);
	Output[OutOffset + 2].LoadLE(Input, InOffset + 32);
	Output[OutOffset + 3].LoadLE(Input, InOffset + 48);
	Output[OutOffset + 4].LoadLE(Input, InOffset + 64);
	Output[OutOffset + 5].LoadLE(Input, InOffset + 80);
	Output[OutOffset + 6].LoadLE(Input, InOffset + 96);
	Output[OutOffset + 7].LoadLE(Input, InOffset + 112);
	Output[OutOffset + 8].LoadLE(Input, InOffset + 128);
	Output[OutOffset + 9].LoadLE(Input, InOffset + 144);
	Output[OutOffset + 10].LoadLE(Input, InOffset + 160);
	Output[OutOffset + 11].LoadLE(Input, InOffset + 176);
	Output[OutOffset + 12].LoadLE(Input, InOffset + 192);
	Output[OutOffset + 13].LoadLE(Input, InOffset + 208);
	Output[OutOffset + 14].LoadLE(Input, InOffset + 224);
	Output[OutOffset + 15].LoadLE(Input, InOffset + 240);
}

void UInt128::StoreBE16(std::vector<byte> &Output, size_t Offset, UInt128 &X0, UInt128 &X1, UInt128 &X2, UInt128 &X3)
{
	Transpose(X0, X1, X2, X3);
	X0.StoreBE(Output, Offset);
	X1.StoreBE(Output, Offset + 16);
	X2.StoreBE(Output, Offset + 32);
	X3.StoreBE(Output, Offset + 48);
}

void UInt128::StoreBE256(std::vector<UInt128> &Input, size_t InOffset, std::vector<byte> &Output, size_t OutOffset)
{
	__m128i T0 = _mm_unpacklo_epi32(Input[InOffset].Register, Input[InOffset + 1].Register);
	__m128i T1 = _mm_unpacklo_epi32(Input[InOffset + 2].Register, Input[InOffset + 3].Register);
	__m128i T2 = _mm_unpacklo_epi32(Input[InOffset + 4].Register, Input[InOffset + 5].Register);
	__m128i T3 = _mm_unpacklo_epi32(Input[InOffset + 6].Register, Input[InOffset + 7].Register);
	__m128i T4 = _mm_unpacklo_epi32(Input[InOffset + 8].Register, Input[InOffset + 9].Register);
	__m128i T5 = _mm_unpacklo_epi32(Input[InOffset + 10].Register, Input[InOffset + 11].Register);
	__m128i T6 = _mm_unpacklo_epi32(Input[InOffset + 12].Register, Input[InOffset + 13].Register);
	__m128i T7 = _mm_unpacklo_epi32(Input[InOffset + 14].Register, Input[InOffset + 15].Register);
	__m128i T8 = _mm_unpackhi_epi32(Input[InOffset].Register, Input[InOffset + 1].Register);
	__m128i T9 = _mm_unpackhi_epi32(Input[InOffset + 2].Register, Input[InOffset + 3].Register);
	__m128i T10 = _mm_unpackhi_epi32(Input[InOffset + 4].Register, Input[InOffset + 5].Register);
	__m128i T11 = _mm_unpackhi_epi32(Input[InOffset + 6].Register, Input[InOffset + 7].Register);
	__m128i T12 = _mm_unpackhi_epi32(Input[InOffset + 8].Register, Input[InOffset + 9].Register);
	__m128i T13 = _mm_unpackhi_epi32(Input[InOffset + 10].Register, Input[InOffset + 11].Register);
	__m128i T14 = _mm_unpackhi_epi32(Input[InOffset + 12].Register, Input[InOffset + 13].Register);
	__m128i T15 = _mm_unpackhi_epi32(Input[InOffset + 14].Register, Input[InOffset + 15].Register);

	Input[InOffset].Register = _mm_unpacklo_epi64(T0, T1);
	Input[InOffset + 1].Register = _mm_unpacklo_epi64(T2, T3);
	Input[InOffset + 2].Register = _mm_unpacklo_epi64(T4, T5);
	Input[InOffset + 3].Register = _mm_unpacklo_epi64(T6, T7);
	Input[InOffset + 4].Register = _mm_unpackhi_epi64(T0, T1);
	Input[InOffset + 5].Register = _mm_unpackhi_epi64(T2, T3);
	Input[InOffset + 6].Register = _mm_unpackhi_epi64(T4, T5);
	Input[InOffset + 7].Register = _mm_unpackhi_epi64(T6, T7);
	Input[InOffset + 8].Register = _mm_unpacklo_epi64(T8, T9);
	Input[InOffset + 9].Register = _mm_unpacklo_epi64(T10, T11);
	Input[InOffset + 10].Register = _mm_unpacklo_epi64(T12, T13);
	Input[InOffset + 11].Register = _mm_unpacklo_epi64(T14, T15);
	Input[InOffset + 12].Register = _mm_unpackhi_epi64(T8, T9);
	Input[InOffset + 13].Register = _mm_unpackhi_epi64(T10, T11);
	Input[InOffset + 14].Register = _mm_unpackhi_epi64(T12, T13);
	Input[InOffset + 15].Register = _mm_unpackhi_epi64(T14, T15);

	Input[InOffset].StoreBE(Output, OutOffset);
	Input[InOffset + 1].StoreBE(Output, OutOffset + 16);
	Input[InOffset + 2].StoreBE(Output, OutOffset + 32);
	Input[InOffset + 3].StoreBE(Output, OutOffset + 48);
	Input[InOffset + 4].StoreBE(Output, OutOffset + 64);
	Input[InOffset + 5].StoreBE(Output, OutOffset + 80);
	Input[InOffset + 6].StoreBE(Output, OutOffset + 96);
	Input[InOffset + 7].StoreBE(Output, OutOffset + 112);
	Input[InOffset + 8].StoreBE(Output, OutOffset + 128);
	Input[InOffset + 9].StoreBE(Output, OutOffset + 144);
	Input[InOffset + 10].StoreBE(Output, OutOffset + 160);
	Input[InOffset + 11].StoreBE(Output, OutOffset + 176);
	Input[InOffset + 12].StoreBE(Output, OutOffset + 192);
	Input[InOffset + 13].StoreBE(Output, OutOffset + 208);
	Input[InOffset + 14].StoreBE(Output, OutOffset + 224);
	Input[InOffset + 15].StoreBE(Output, OutOffset + 240);
}

void UInt128::StoreLE16(std::vector<byte> &Output, size_t Offset, UInt128 &X0, UInt128 &X1, UInt128 &X2, UInt128 &X3)
{
	Transpose(X0, X1, X2, X3);
	X0.StoreLE(Output, Offset);
	X1.StoreLE(Output, Offset + 16);
	X2.StoreLE(Output, Offset + 32);
	X3.StoreLE(Output, Offset + 48);
}

void UInt128::StoreLE256(std::vector<UInt128> &Input, size_t InOffset, std::vector<byte> &Output, size_t OutOffset)
{
	__m128i T0 = _mm_unpacklo_epi32(Input[InOffset].Register, Input[InOffset + 1].Register);
	__m128i T1 = _mm_unpacklo_epi32(Input[InOffset + 2].Register, Input[InOffset + 3].Register);
	__m128i T2 = _mm_unpacklo_epi32(Input[InOffset + 4].Register, Input[InOffset + 5].Register);
	__m128i T3 = _mm_unpacklo_epi32(Input[InOffset + 6].Register, Input[InOffset + 7].Register);
	__m128i T4 = _mm_unpacklo_epi32(Input[InOffset + 8].Register, Input[InOffset + 9].Register);
	__m128i T5 = _mm_unpacklo_epi32(Input[InOffset + 10].Register, Input[InOffset + 11].Register);
	__m128i T6 = _mm_unpacklo_epi32(Input[InOffset + 12].Register, Input[InOffset + 13].Register);
	__m128i T7 = _mm_unpacklo_epi32(Input[InOffset + 14].Register, Input[InOffset + 15].Register);
	__m128i T8 = _mm_unpackhi_epi32(Input[InOffset].Register, Input[InOffset + 1].Register);
	__m128i T9 = _mm_unpackhi_epi32(Input[InOffset + 2].Register, Input[InOffset + 3].Register);
	__m128i T10 = _mm_unpackhi_epi32(Input[InOffset + 4].Register, Input[InOffset + 5].Register);
	__m128i T11 = _mm_unpackhi_epi32(Input[InOffset + 6].Register, Input[InOffset + 7].Register);
	__m128i T12 = _mm_unpackhi_epi32(Input[InOffset + 8].Register, Input[InOffset + 9].Register);
	__m128i T13 = _mm_unpackhi_epi32(Input[InOffset + 10].Register, Input[InOffset + 11].Register);
	__m128i T14 = _mm_unpackhi_epi32(Input[InOffset + 12].Register, Input[InOffset + 13].Register);
	__m128i T15 = _mm_unpackhi_epi32(Input[InOffset + 14].Register, Input[InOffset + 15].Register);

	Input[InOffset].Register = _mm_unpacklo_epi64(T0, T1);
	Input[InOffset + 1].Register = _mm_unpacklo_epi64(T2, T3);
	Input[InOffset + 2].Register = _mm_unpacklo_epi64(T4, T5);
	Input[InOffset + 3].Register = _mm_unpacklo_epi64(T6, T7);
	Input[InOffset + 4].Register = _mm_unpackhi_epi64(T0, T1);
	Input[InOffset + 5].Register = _mm_unpackhi_epi64(T2, T3);
	Input[InOffset + 6].Register = _mm_unpackhi_epi64(T4, T5);
	Input[InOffset + 7].Register = _mm_unpackhi_epi64(T6, T7);
	Input[InOffset + 8].Register = _mm_unpacklo_epi64(T8, T9);
	Input[InOffset + 9].Register = _mm_unpacklo_epi64(T10, T11);
	Input[InOffset + 10].Register = _mm_unpacklo_epi64(T12, T13);
	Input[InOffset + 11].Register = _mm_unpacklo_epi64(T14, T15);
	Input[InOffset + 12].Register = _mm_unpackhi_epi64(T8, T9);
	Input[InOffset + 13].Register = _mm_unpackhi_epi64(T10, T11);
	Input[InOffset + 14].Register = _mm_unpackhi_epi64(T12, T13);
	Input[InOffset + 15].Register = _mm_unpackhi_epi64(T14, T15);

	Input[InOffset].StoreLE(Output, OutOffset);
	Input[InOffset + 1].StoreLE(Output, OutOffset + 16);
	Input[InOffset + 2].StoreLE(Output, OutOffset + 32);
	Input[InOffset + 3].StoreLE(Output, OutOffset + 48);
	Input[InOffset + 4].StoreLE(Output, OutOffset + 64);
	Input[InOffset + 5].StoreLE(Output, OutOffset + 80);
	Input[InOffset + 6].StoreLE(Output, OutOffset + 96);
	Input[InOffset + 7].StoreLE(Output, OutOffset + 112);
	Input[InOffset + 8].StoreLE(Output, OutOffset + 128);
	Input[InOffset + 9].StoreLE(Output, OutOffset + 144);
	Input[InOffset + 10].StoreLE(Output, OutOffset + 160);
	Input[InOffset + 11].StoreLE(Output, OutOffset + 176);
	Input[InOffset + 12].StoreLE(Output, OutOffset + 192);
	Input[InOffset + 13].StoreLE(Output, OutOffset + 208);
	Input[InOffset + 14].StoreLE(Output, OutOffset + 224);
	Input[InOffset + 15].StoreLE(Output, OutOffset + 240);
}

UInt128 UInt128::AndNot(const UInt128 &Value)
{
	return UInt128(_mm_andnot_si128(Register, Value.Register));
}

const size_t UInt128::Length()
{
	return 16;
}

void UInt128::Rotl32(const int Shift)
{
	Register = _mm_or_si128(_mm_slli_epi32(Register, static_cast<int>(Shift)), _mm_srli_epi32(Register, static_cast<int>(32 - Shift)));
}

UInt128 UInt128::Rotl32(const UInt128 &Value, const int Shift)
{
	return UInt128(_mm_or_si128(_mm_slli_epi32(Value.Register, static_cast<int>(Shift)), _mm_srli_epi32(Value.Register, static_cast<int>(32 - Shift))));
}

void UInt128::Rotr32(const int Shift)
{
	Rotl32(32 - Shift);
}

UInt128 UInt128::Rotr32(const UInt128 &Value, const int Shift)
{
	return Rotl32(Value, 32 - Shift);
}

UInt128 UInt128::ShuffleLoadBE(const std::vector<byte> &Input, size_t Offset, size_t Shift)
{
	return UInt128(
		(Input[Offset] << 24) | (Input[Offset + 1] << 16) | (Input[Offset + 2] << 8) | Input[Offset + 3],
		(Input[Offset + Shift] << 24) | (Input[Offset + 1 + Shift] << 16) | (Input[Offset + 2 + Shift] << 8) | Input[Offset + 3 + Shift],
		(Input[Offset + Shift * 2] << 24) | (Input[Offset + 1 + Shift * 2] << 16) | (Input[Offset + 2 + Shift * 2] << 8) | Input[Offset + 3 + Shift * 2],
		(Input[Offset + Shift * 3] << 24) | (Input[Offset + 1 + Shift * 3] << 16) | (Input[Offset + 2 + Shift * 3] << 8) | Input[Offset + 3 + Shift * 3]);
}

UInt128 UInt128::ShuffleLoad32(const std::vector<uint> &Input, size_t Offset, size_t Shift)
{
	return UInt128(Input[Offset], Input[Offset + Shift], Input[Offset + Shift * 2], Input[Offset + Shift * 3]);
}

UInt128 UInt128::ShuffleLoadLE(const std::vector<byte> &Input, size_t Offset, size_t Shift)
{
	return UInt128(
		Input[Offset] | (Input[Offset + 1] << 8) | (Input[Offset + 2] << 16) | (Input[Offset + 3] << 24),
		Input[Offset + Shift] | (Input[Offset + 1 + Shift] << 8) | (Input[Offset + 2 + Shift] << 16) | (Input[Offset + 3 + Shift] << 24),
		Input[Offset + Shift * 2] | (Input[Offset + 1 + Shift * 2] << 8) | (Input[Offset + 2 + Shift * 2] << 16) | (Input[Offset + 3 + Shift * 2] << 24),
		Input[Offset + Shift * 3] | (Input[Offset + 1 + Shift * 3] << 8) | (Input[Offset + 2 + Shift * 3] << 16) | (Input[Offset + 3 + Shift * 3] << 24));
}

UInt128 UInt128::Swap() const
{
	__m128i T = Register;

	T = _mm_shufflehi_epi16(T, _MM_SHUFFLE(2, 3, 0, 1)); // ?
	T = _mm_shufflelo_epi16(T, _MM_SHUFFLE(2, 3, 0, 1));

	return UInt128(_mm_or_si128(_mm_srli_epi16(T, 8), _mm_slli_epi16(T, 8)));
}

UInt128 UInt128::Swap(UInt128 &X)
{
	__m128i T = X.Register;

	T = _mm_shufflehi_epi16(T, _MM_SHUFFLE(2, 3, 0, 1));
	T = _mm_shufflelo_epi16(T, _MM_SHUFFLE(2, 3, 0, 1));

	return UInt128(_mm_or_si128(_mm_srli_epi16(T, 8), _mm_slli_epi16(T, 8)));
}

void UInt128::ToUint8(std::vector<byte> &Output, size_t Offset)
{
	memcpy(&Output[Offset], &Register.m128i_u8[0], 16);
}

void UInt128::ToUint16(std::vector<ushort> &Output, size_t Offset)
{
	memcpy(&Output[Offset], &Register.m128i_u16[0], 16);
}

void UInt128::ToUint32(std::vector<uint> &Output, size_t Offset)
{
	memcpy(&Output[Offset], &Register.m128i_u32[0], 16);
}

void UInt128::ToUint64(std::vector<ulong> &Output, size_t Offset)
{
	memcpy(&Output[Offset], &Register.m128i_u64[0], 16);
}

void UInt128::Transpose(UInt128 &X0, UInt128 &X1, UInt128 &X2, UInt128 &X3)
{
	__m128i T0 = _mm_unpacklo_epi32(X0.Register, X1.Register);
	__m128i T1 = _mm_unpacklo_epi32(X2.Register, X3.Register);
	__m128i T2 = _mm_unpackhi_epi32(X0.Register, X1.Register);
	__m128i T3 = _mm_unpackhi_epi32(X2.Register, X3.Register);
	X0.Register = _mm_unpacklo_epi64(T0, T1);
	X1.Register = _mm_unpackhi_epi64(T0, T1);
	X2.Register = _mm_unpacklo_epi64(T2, T3);
	X3.Register = _mm_unpackhi_epi64(T2, T3);
}

NAMESPACE_NUMERICEND
