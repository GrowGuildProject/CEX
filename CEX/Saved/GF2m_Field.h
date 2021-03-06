#ifndef _CEX_GF2MFIELD_H
#define _CEX_GF2MFIELD_H

#include "CexDomain.h"
#include "IDigest.h"
#include "IPrng.h"
#include "gf2m_small_m.h"

NAMESPACE_MCELIECE
/**
* GF(2^m) field for m = [2...16]
*/
class GF2m_Field
{
#define MAX_EXT_DEG 16

public:

	unsigned int prim_poly[MAX_EXT_DEG + 1] =
	{
		01,		/* extension degree 0 (!) never used */
		03,		/* extension degree 1 (!) never used */
		07, 		/* extension degree 2 */
		013, 		/* extension degree 3 */
		023, 		/* extension degree 4 */
		045, 		/* extension degree 5 */
		0103, 		/* extension degree 6 */
		0203, 		/* extension degree 7 */
		0435, 		/* extension degree 8 */
		01041, 		/* extension degree 9 */
		02011,		/* extension degree 10 */
		04005,		/* extension degree 11 */
		010123,		/* extension degree 12 */
		020033,		/* extension degree 13 */
		042103,		/* extension degree 14 */
		0100003,		/* extension degree 15 */
		0210013		/* extension degree 16 */
	};

	gf2m m_gf_extension_degree, m_gf_multiplicative_order;
	const std::vector<gf2m>& m_gf_log_table;
	const std::vector<gf2m>& m_gf_exp_table;


	GF2m_Field(size_t extdeg) : m_gf_extension_degree(extdeg),
		m_gf_multiplicative_order((1 << extdeg) - 1),
		m_gf_log_table(log_table(m_gf_extension_degree)),
		m_gf_exp_table(exp_table(m_gf_extension_degree))
	{
	}

	std::vector<gf2m> gf_exp_table(size_t deg, gf2m prime_poly)
	{
		// construct the table gf_exp[i]=alpha^i

		std::vector<gf2m> tab((1 << deg) + 1);

		tab[0] = 1;
		for (size_t i = 1; i < tab.size(); ++i)
		{
			const bool overflow = tab[i - 1] >> (deg - 1);
			tab[i] = (tab[i - 1] << 1) ^ (overflow ? prime_poly : 0);
		}

		return tab;
	}

	const std::vector<gf2m>& exp_table(size_t deg)
	{
		static std::vector<gf2m> tabs[MAX_EXT_DEG + 1];

		//if (deg < 2 || deg > MAX_EXT_DEG)
		//	throw Exception("GF2m_Field does not support degree " + std::to_string(deg));

		if (tabs[deg].empty())
			tabs[deg] = gf_exp_table(deg, prim_poly[deg]);

		return tabs[deg];
	}

	std::vector<gf2m> gf_log_table(size_t deg, const std::vector<gf2m>& exp)
	{
		std::vector<gf2m> tab(1 << deg);

		tab[0] = (1 << deg) - 1; // log of 0 is the order by convention
		for (size_t i = 0; i < tab.size(); ++i)
		{
			tab[exp[i]] = i;
		}
		return tab;
	}

	const std::vector<gf2m>& log_table(size_t deg)
	{
		static std::vector<gf2m> tabs[MAX_EXT_DEG + 1];

		//if (deg < 2 || deg > MAX_EXT_DEG)
		//	throw Exception("GF2m_Field does not support degree " + std::to_string(deg));

		if (tabs[deg].empty())
			tabs[deg] = gf_log_table(deg, exp_table(deg));

		return tabs[deg];
	}

public:

	explicit GF2m_Field(size_t extdeg);

	gf2m gf_mul(gf2m x, gf2m y) const
	{
		return ((x) ? gf_mul_fast(x, y) : 0);
	}

	gf2m gf_square(gf2m x) const
	{
		return ((x) ? gf_exp(_gf_modq_1(gf_log(x) << 1)) : 0);
	}

	gf2m square_rr(gf2m x) const
	{
		return _gf_modq_1(x << 1);
	}

	gf2m gf_mul_fast(gf2m x, gf2m y) const
	{
		return ((y) ? gf_exp(_gf_modq_1(gf_log(x) + gf_log(y))) : 0);
	}

	/*
	naming convention of GF(2^m) field operations:
	l logarithmic, unreduced
	r logarithmic, reduced
	n normal, non-zero
	z normal, might be zero
	*/

	gf2m gf_mul_lll(gf2m a, gf2m b) const
	{
		return (a + b);
	}

	gf2m gf_mul_rrr(gf2m a, gf2m b) const
	{
		return (_gf_modq_1(gf_mul_lll(a, b)));
	}

	gf2m gf_mul_nrr(gf2m a, gf2m b) const
	{
		return (gf_exp(gf_mul_rrr(a, b)));
	}

	gf2m gf_mul_rrn(gf2m a, gf2m y) const
	{
		return _gf_modq_1(gf_mul_lll(a, gf_log(y)));
	}

	gf2m gf_mul_rnr(gf2m y, gf2m a) const
	{
		return gf_mul_rrn(a, y);
	}

	gf2m gf_mul_lnn(gf2m x, gf2m y) const
	{
		return (gf_log(x) + gf_log(y));
	}

	gf2m gf_mul_rnn(gf2m x, gf2m y) const
	{
		return _gf_modq_1(gf_mul_lnn(x, y));
	}

	gf2m gf_mul_nrn(gf2m a, gf2m y) const
	{
		return gf_exp(_gf_modq_1((a)+gf_log(y)));
	}

	/**
	* zero operand allowed
	*/
	gf2m gf_mul_zrz(gf2m a, gf2m y) const
	{
		return ((y == 0) ? 0 : gf_mul_nrn(a, y));
	}

	gf2m gf_mul_zzr(gf2m a, gf2m y) const
	{
		return gf_mul_zrz(y, a);
	}

	/**
	* non-zero operand
	*/
	gf2m gf_mul_nnr(gf2m y, gf2m a) const
	{
		return gf_mul_nrn(a, y);
	}

	gf2m gf_sqrt(gf2m x) const
	{
		return ((x) ? gf_exp(_gf_modq_1(gf_log(x) << (get_extension_degree() - 1))) : 0);
	}

	gf2m gf_div_rnn(gf2m x, gf2m y) const
	{
		return _gf_modq_1(gf_log(x) - gf_log(y));
	}

	gf2m gf_div_rnr(gf2m x, gf2m b) const
	{
		return _gf_modq_1(gf_log(x) - b);
	}

	gf2m gf_div_nrr(gf2m a, gf2m b) const
	{
		return gf_exp(_gf_modq_1(a - b));
	}

	gf2m gf_div_zzr(gf2m x, gf2m b) const
	{
		return ((x) ? gf_exp(_gf_modq_1(gf_log(x) - b)) : 0);
	}

	gf2m gf_inv(gf2m x) const
	{
		return gf_exp(gf_ord() - gf_log(x));
	}

	gf2m gf_inv_rn(gf2m x) const
	{
		return (gf_ord() - gf_log(x));
	}

	gf2m gf_square_ln(gf2m x) const
	{
		return gf_log(x) << 1;
	}

	gf2m gf_square_rr(gf2m a) const
	{
		return a << 1;
	}

	gf2m gf_l_from_n(gf2m x) const
	{
		return gf_log(x);
	}

	gf2m GF2m_Field::gf_div(gf2m x, gf2m y) const
	{
		const int32_t sub_res = static_cast<int32_t>(gf_log(x) - static_cast<int32_t>(gf_log(y)));
		const int32_t modq_res = static_cast<int32_t>(_gf_modq_1(sub_res));
		const int32_t div_res = static_cast<int32_t>(x) ? static_cast<int32_t>(gf_exp(modq_res)) : 0;
		return static_cast<gf2m>(div_res);
	}

	gf2m gf_pow(gf2m x, int i) const;

	gf2m gf_exp(gf2m i) const
	{
		return m_gf_exp_table.at(i); /* alpha^i */
	}

	gf2m gf_log(gf2m i) const
	{
		return m_gf_log_table.at(i); /* return i when x=alpha^i */
	}

	gf2m gf_ord() const
	{
		return m_gf_multiplicative_order;
	}

	gf2m get_extension_degree() const
	{
		return m_gf_extension_degree;
	}

	gf2m get_cardinality() const
	{
		return static_cast<gf2m>(1 << get_extension_degree());
	}

private:
	gf2m _gf_modq_1(int32_t d) const
	{
		/* residual modulo q-1
		when -q < d < 0, we get (q-1+d)
		when 0 <= d < q, we get (d)
		when q <= d < 2q-1, we get (d-q+1)
		*/
		return (((d)& gf_ord()) + ((d) >> get_extension_degree()));
	}

	// we suppose i >= 0. Par convention 0^0 = 1
	gf2m GF2m_Field::gf_pow(gf2m x, int i) const
	{
		if (i == 0)
			return 1;
		else if (x == 0)
			return 0;
		else
		{
			// i mod (q-1)
			while (i >> get_extension_degree())
				i = (i & (gf_ord())) + (i >> get_extension_degree());
			i *= gf_log(x);
			while (i >> get_extension_degree())
				i = (i & (gf_ord())) + (i >> get_extension_degree());
			return gf_exp(i);
		}
	}

};

NAMESPACE_MCELIECEEND

#endif