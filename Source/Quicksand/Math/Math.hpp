#ifndef QSE_RANDOM_H
#define QSE_RANDOM_H

//Math period functions
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / QSE_PI)
#define DEGREES_TO_RADIANS(x) ((x) * QSE_PI / 180.0f)

/////////////////////////////////////////////
//	class CCRandom
//
//	a random number generating class
//
//
//
//

namespace Quicksand
{

	class CRandom
	{
	private:

		//DATA:
		unsigned int	m_Seed;
		unsigned int	m_Seed_sp;
		unsigned long	m_MT[CMATH_N];//this is an aray for the state vector
		int				m_MTI;//whether m_MT is initialized

	public:

		//FUNCTIONS
		CRandom( void );

		unsigned int	Random( unsigned int n );//returns a number 0 to n-1 inclusive
		float			Random();//random float
		void			SetRandomSeed( unsigned int n );
		unsigned int	GetRandomSeed( void );
		void			Randomize( void );
	};


	//some glm extensions that are useful
	const vec3& GetTransMatPosition(const mat4& matrix);
	void        SetTransMatPosition(mat4 *matrix, vec3 position);

	const vec3& GetTransMatDirection(const mat4& matrix);

}
#endif