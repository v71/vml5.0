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

//////////////////////////////////////////////////////////////////////////////////////
//  equatiuons solvers

namespace vml
{
	namespace math
	{
		namespace solvers
		{

			/////////////////////////////////////////////////////
			// equation solvers

			static const unsigned int NO_ROOTS								      = 0;
			static const unsigned int ONE_REAL_ROOT						          = 1;
			static const unsigned int TWO_REAL_ROOTS						      = 2;
			static const unsigned int TWO_IDENTICAL_REAL_ROOTS			          = 3;
			static const unsigned int TWO_COMPLEX_CONJUGATED_ROOTS				  = 4;
			static const unsigned int THREE_REAL_ROOTS							  = 5;
			static const unsigned int THREE_IDENTICAL_REAL_ROOTS				  = 6;
			static const unsigned int ONE_REAL_ROOT_TWO_COMPLEX_CONIJUGATES_ROOTS = 7;

			/////////////////////////////////////////////////////

			static const std::string GetReturnValueString(const unsigned int retval)
			{
				switch (retval)
				{
					case NO_ROOTS								     : return "No Roots";										break;
					case ONE_REAL_ROOT								 : return "One Real Solution";								break;
					case TWO_REAL_ROOTS								 : return "Two Real Roots";									break;
					case TWO_IDENTICAL_REAL_ROOTS					 : return "Two Identical Real Roots";						break;
					case TWO_COMPLEX_CONJUGATED_ROOTS				 : return "Two Complex Conjugated Roots";					break;
					case THREE_REAL_ROOTS							 : return "Three Real Roots";								break;
					case THREE_IDENTICAL_REAL_ROOTS				     : return "Three Identical Real Roots";					    break;
					case ONE_REAL_ROOT_TWO_COMPLEX_CONIJUGATES_ROOTS : return "One Real Root and Two Complex Conjugated Roots"; break;
				}
				
				// we never get here

				return	"Undefined";
			}

			/////////////////////////////////////////////////////
			// linear equation solver

			template < typename T>
			static int SolveLinear(const T a, const T b, T& lc,const T eps=vml::math::EPSILON)
			{
				if ( fabs(a) <eps)
				{
					return NO_ROOTS;		
				}
				else
				{
					if (fabs(a) < eps)
					{
						lc = -b / a;

						return ONE_REAL_ROOT;	
					}
				}

				return NO_ROOTS;		
			}

			/////////////////////////////////////////////////////
			// quadratic equation solver

			template < typename T>
			static int SolveQuadratic(const T a, const T b, const T c,
									  T& r1, T& c1,
									  T& r2, T& c2,
									  const T eps = vml::math::EPSILON)
			{
				r1 = 0;
				c1 = 0;
				r2 = 0;
				c2 = 0;

				// spurious term is lacking

				if (fabs(a) <eps)
				{

					if (fabs(b) <eps )
					{
						return NO_ROOTS;	
					}

					// 1 real solution

					r1 = -c / b;

					return ONE_REAL_ROOT;	
				}

				T det = b*b - 4*a*c;

				if (fabs(det) <eps)
				{
					// two identical solutiuons

					if (fabs(b) < eps)
					{

						r1 = 0;
						r2 = 0;

						return TWO_IDENTICAL_REAL_ROOTS;
					}

					r1 = -b*0.5 / a;
					r2 = r1;

					return TWO_IDENTICAL_REAL_ROOTS;
			
				}
				else
				{

					if (det > eps)
					{
						// two real solutions

						T s = 0.5*sqrt(det) / a;

						// store solutions

						r1 = r2 = -0.5*b / a;
						r1 += s;
						r2 -= s;

						return TWO_REAL_ROOTS;
					}
					else if (det < -eps)
					{
						// complex and coniugate slutions

						T s = 0.5f*sqrt(-det) / a;

						// store solutions

						r1 =  r2 = -0.5*b / a;
						c1 =  s;
						c2 = -s;

						return TWO_COMPLEX_CONJUGATED_ROOTS;
					}

				}

				return NO_ROOTS;
			}

			// -----------------------------------------------------
			// cubic equation solver using Cardano method
			
			template < typename T>
			static int SolveCubic(T a, T b, T c, T d,
								  T& r1, T& c1,
								  T& r2, T& c2,
								  T& r3, T& c3,
								  const double eps=vml::math::EPSILON)
			{
				r1 = 0;
				c1 = 0;
				r2 = 0;
				c2 = 0;
				r3 = 0;
				c3 = 0;

				// if a element is zero , we have a quadratic equations 

				if (fabs(a) < eps)
				{
					return SolveQuadratic<T>(b, c, d, r1, c1, r2, c2, eps);
				}

				// compute cubic equation

				T f = ( 3 * c/a - b*b/a/a ) *0.33333333;
				T g = ( 2 * b*b*b/a/a/a - 9*b*c/a/a + 27*d/a) * 0.0370370370;
				T h = g*g *0.25 + f*f*f * 0.0370370370;

				// evaluate discriminant

				if ( fabs(f) < eps && fabs(g) < eps && fabs(h)< eps)
				{
					// 3 equal roots

					T x, y;

					// when f, g, and h all equal 0 the roots 
					// can be found by the following line

					y = d / a;

					if (y < -eps)
						x = -cbrt(-y);
					else
						x = cbrt(y);

					r1 = -x;
					r2 = -x;
					r3 = -x;

					return THREE_IDENTICAL_REAL_ROOTS; 
				}
				else if (h < eps)
				{

					// 3 real roots

					T i, j, k, m, n, p;

					i = sqrt(g*g / 4 - h);

					if (i < -eps)
						j = -cbrt(-i);
					else
						j = cbrt(i);

					k = acos(-(g / (2 * i)));
					m = cos(k * 0.3333333);
					n = sqrt(3) * sin(k *0.3333333);
					p = -b / (3 * a);

					r1 =  2 * j *  m + p;
					r2 = -j * (m + n) + p;
					r3 = -j * (m - n) + p;
					
					return THREE_REAL_ROOTS;

				}
				else if (h > eps)
				{
					// 1 real root and 2 complex roots

					T r, s, t, u, p;

					r = -(g *0.5) + sqrtf(h);

					if (r < 0.0f)
						s = -cbrt(-r);
					else
						s = cbrt(r);

					t = -(g *0.5) - sqrtf(h);

					if (t < 0.0f)
						u = -cbrt(-t);
					else
						u = cbrt(t);

					p = -(b / (3 * a));

					r1 = (s + u) + p;
					r2 = -(s + u) / 2 + p;
					r3 = r2;
					c2 = (s - u) * sqrtf(3) *0.5f;
					c3 = -c2;

					return ONE_REAL_ROOT_TWO_COMPLEX_CONIJUGATES_ROOTS;
				}

				// if we get here there was an error 
				// this is virtually never returned

				return NO_ROOTS;
			}

		} // end of eqolvers namespace
	} // end of math namespace
}// end of vml namespace