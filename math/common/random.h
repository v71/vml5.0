#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////
// polynomails

namespace vml
{
	namespace math
	{
		namespace random
		{ 

			///////////////////////////////////////////////////////////
			// random numbers generators

			class Random
			{
					private:

							long m_nSeed;
							long m_nQuotient;
							long m_nRemainder;

					public:

							void Init(unsigned int nSeed) 
							{
								srand(nSeed); 
							}

							double Rand()
							{
								return (double)rand() / (double)RAND_MAX; 
							}

							double RandD(double dMin, double dMax)
							{
								double dInterval = dMax - dMin;
								double d = dInterval * Rand();
								return dMin + vml::math::ranges::Min(d, dInterval);
							}

							unsigned int RandI(unsigned int nMin, unsigned int nMax)
							{
								unsigned int nInterval = nMax - nMin;
								unsigned int i = (unsigned int)((nInterval + 1.0) * Rand());
								return nMin + vml::math::ranges::Min(i, nInterval);
							}

							// -------------------------------------------------------
							// ctor / dtor

							Random() 
							{
								m_nSeed      =0;
								m_nQuotient  =0;
								m_nRemainder =0;
							}
				
							Random(unsigned int nSeed) 
							{
								Init(nSeed); 
							}

							~Random()
							{
							}

			};

			////////////////////////////////////////////////////////////
			// random number using the lcg algoritm

			class RandomLCG
			{
				private:

					unsigned int m_nSeed;
					unsigned int m_nQuotient;
					unsigned int m_nRemainder;

				public:

					unsigned int rand()
					{
						if (m_nSeed <= m_nQuotient)
							m_nSeed = (m_nSeed * 16807L) % LONG_MAX;
						else
						{
							int nHigh = m_nSeed / m_nQuotient;
							int nLow = m_nSeed % m_nQuotient;

							int test = 16807L * nLow - m_nRemainder * nHigh;

							if (test > 0)
								m_nSeed = test;
							else
								m_nSeed = test + LONG_MAX;
						}

						return m_nSeed;
					}

					// ---------------------------------------------------------------------
					// ctor / dtor

					RandomLCG()
					{
						m_nSeed=0;
						m_nQuotient=0;
						m_nRemainder=0;
					}

					RandomLCG(unsigned int nSeed)
					{
						m_nSeed = nSeed;
						m_nQuotient = LONG_MAX / 16807L;
						m_nRemainder = LONG_MAX % 16807L;
					}

					~RandomLCG()
					{
					}

			};

			////////////////////////////////////////////////////////
			// random number using the r250 algoritm

			class RandomR250
			{
				private:

					static const unsigned int BITS			= 32;
					static const unsigned int MSB			= 0x80000000L;
					static const unsigned int ALL_BITS		= 0xffffffffL;
					static const unsigned int HALF_RANGE	= 0x40000000L;
					static const unsigned int STEP			= 7;

					unsigned int m_nR250buffer[250];
					int m_nR250index;

				public:

					unsigned int Rand250()
					{
						int j;
						unsigned int new_rand;

						if (m_nR250index >= 147)
							j = m_nR250index - 147;
						else
							j = m_nR250index + 103;

						new_rand = m_nR250buffer[m_nR250index] ^ m_nR250buffer[j];
						m_nR250buffer[m_nR250index] = new_rand;

						if (m_nR250index >= 249)
							m_nR250index = 0;
						else
							m_nR250index++;

						return new_rand;

					}

					double Rand250D()
					{
						int j;
						unsigned int new_rand;

						if (m_nR250index >= 147)
							j = m_nR250index - 147;
						else
							j = m_nR250index + 103;

						new_rand = m_nR250buffer[m_nR250index] ^ m_nR250buffer[j];
						m_nR250buffer[m_nR250index] = new_rand;

						if (m_nR250index >= 249)	/* increment pointer for next time */
							m_nR250index = 0;
						else
							m_nR250index++;

						return (double)new_rand / ALL_BITS;
					}

					// ---------------------------------------------------------------------
					// ctor / dtor

					RandomR250()
					{
						memset( m_nR250buffer,0,sizeof(unsigned int)*250);
						m_nR250index=0;
					}

					RandomR250(unsigned int nSeed)
					{
						memset(m_nR250buffer, 0, sizeof(unsigned int) * 250);

						RandomLCG lcg(nSeed);

						int j, k;
						unsigned int mask, msb;
						m_nR250index = 0;
						for (j = 0; j < 250; j++)
							m_nR250buffer[j] = rand();
						for (j = 0; j < 250; j++)	// set some MSBs to 1
							if (lcg.rand() > HALF_RANGE)
								m_nR250buffer[j] |= MSB;

						msb = MSB;	        // turn on diagonal bit
						mask = ALL_BITS;	// turn off the leftmost bits

						for (j = 0; j < BITS; j++)
						{
							k = STEP * j + 3;	// select a word to operate on
							m_nR250buffer[k] &= mask; // turn off bits left of the diagonal
							m_nR250buffer[k] |= msb;	// turn on the diagonal bit
							mask >>= 1;
							msb >>= 1;
						}
					}

					~RandomR250()
					{
					}

			};
		
			//////////////////////////////////////////////////////////////
			// Generates two random numbers with a gaussian distribution.
			// Algorithm taken from 
			// http://www.taygeta.com/random/gaussian.html

			template<typename T >
			class GaussRandF
			{

				public:

					T Rand()
					{
						T x1, x2;
						T y1, y2;
						T w;
						T c;

						c = 2.0f / (T)RAND_MAX;
						
						w = 0;

						do
						{
							x1 = c * rand() - 1.0f;
							x2 = c * rand() - 1.0f;
							w = x1 * x1 + x2 * x2;
						} while (w >= 1.0f);

						w = sqrt((-2.0f * log(w)) / w);
						y1 = x1 * w;
						y2 = x2 * w;

						return y1;
					}

					// ---------------------------------------------------------------------
					// ctor / dtor
			
					GaussRandF()
					{}

					~GaussRandF()
					{}

			};

			//////////////////////////////////////////////////////////////////////////
			// van der corput quasi random generator 

			static std::vector<double> GenerateVanDerCorputSequence(int base, int n)
			{
				if (base < 2) vml::os::Error("Base must be greater than 2");

				std::vector<double> p;

				for (int i = 0; i < n; ++i)
				{
					double f = 0;
					double denom = 1;

					int j = i;

					while (j)
					{

						denom*= double(base);

						f += fmod(j, base) / denom;
						
						j /= base;
					}

					p.emplace_back(f);

				}

				return p;
			}
			
		} // end of random namespace
	} // end of math namespace 
} // end of vml namesapce 
