#include "stdafx.h"
#include "Math.hpp"
#include <time.h>

using namespace Quicksand;

/////////////////////////////////////////////////////////////////////////////
//	CRandom Implimentation
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//	Constructor
/////////////////////////////////////////////////////////////////////////////

CRandom::CRandom(void)
{
	m_Seed = 1;
	// safe0 start
	m_Seed_sp = 0;
	m_MTI = CMATH_N + 1;
	// safe0 end
}


/////////////////////////////////////////////////////////////////////////////
//	CRandom::Random
/////////////////////////////////////////////////////////////////////////////

unsigned int CRandom::Random(unsigned int n)
{
	unsigned long y;
	static unsigned long mag01[2] = { 0x0, CMATH_MATRIX_A };

	if (n == 0)
		return(0);

	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (m_MTI >= CMATH_N) { /* generate N words at one time */
		int kk;

		if (m_MTI == CMATH_N + 1)   /* if sgenrand() has not been called, */
			SetRandomSeed(4357); /* a default initial seed is used   */

		for (kk = 0; kk<CMATH_N - CMATH_M; kk++) {
			y = (m_MT[kk] & CMATH_UPPER_MASK) | (m_MT[kk + 1] & CMATH_LOWER_MASK);
			m_MT[kk] = m_MT[kk + CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk<CMATH_N - 1; kk++) {
			y = (m_MT[kk] & CMATH_UPPER_MASK) | (m_MT[kk + 1] & CMATH_LOWER_MASK);
			m_MT[kk] = m_MT[kk + (CMATH_M - CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_MT[CMATH_N - 1] & CMATH_UPPER_MASK) | (m_MT[0] & CMATH_LOWER_MASK);
		m_MT[CMATH_N - 1] = m_MT[CMATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		m_MTI = 0;
	}

	y = m_MT[m_MTI++];
	y ^= CMATH_TEMPERING_SHIFT_U(y);
	y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
	y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
	y ^= CMATH_TEMPERING_SHIFT_L(y);


	return (y%n);
}


float CRandom::Random()
{
	float r = (float)Random(MAXINT);
	float divisor = (float)MAXINT;
	return (r / divisor);
}



void CRandom::SetRandomSeed(unsigned int n)
{
	/* setting initial seeds to m_MT[N] using         */
	/* the generator Line 25 of Table 1 in          */
	/* [KNUTH 1981, The Art of Computer Programming */
	/*    Vol. 2 (2nd Ed.), pp102]                  */
	m_MT[0] = n & 0xffffffff;
	for (m_MTI = 1; m_MTI<CMATH_N; m_MTI++)
		m_MT[m_MTI] = (69069 * m_MT[m_MTI - 1]) & 0xffffffff;

	m_Seed = n;
}

unsigned int CRandom::GetRandomSeed(void)
{
	return(m_Seed);
}

void CRandom::Randomize(void)
{
	SetRandomSeed((unsigned int)time(NULL));
}