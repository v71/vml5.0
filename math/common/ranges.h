#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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

/////////////////////////////////////////////////////////////////////////////
// these are quick templated min amd max functions for 2 and 3 arguments

namespace vml
{
	namespace math
	{
		namespace ranges
		{

				/////////////////////////////////////////////////////////////////////////////
				// these are quick templated min amd max functions for 2 and 3 arguments

				template< typename T >
				static T Max(const T a, const T b)
				{
					if (a>b) return a; return b;
				}

				template< typename T >
				static T Min(const T a, const T b)
				{
					if (a<b) return a; return b;
				}

				template< typename T >
				static T Max(const T x, const T y, const T z)
				{
					T m = x; if (y>m) m = y; if (z>m) m = z; return m;
				}

				template< typename T >
				static T Min(const T x, const T y, const T z)
				{
					T m = x; if (y<m) m = y; if (z<m) m = z; return m;
				}

				///////////////////////////////////////////////////////////////
				// templated clamping functions

				template<typename T>
				static T Clamp(const T x, const T minx, const T maxx)
				{
					return x < minx ? minx : x < maxx ? x : maxx;
				}

				template<typename T>
				static T ClampMax(const T vmax, const T v)
				{
					return v <= vmax ? v : vmax;
				}

				template<typename T>
				static T ClampMin(const T vmin, const T v)
				{
					return v >= vmin ? v : vmin;
				}

				template<typename T>
				static T Blend(const T vmin, const T vmax, const T alpha)
				{
					return vmin*(1 - alpha) + vmax*alpha;
				}

				// normalize value whitin range

				template <typename T>
				static T NormalizeIntoRange(const T& x, const T& start, const T& end)
				{
					return (x - start) / (end - start);
				}

				// checks if rangs overlay , ranges must be ordered

				template<class T>
				static bool Overlay(const T min1, const T max1, const T min2, const T max2)
				{
					return max1 >= min2 && min1 < max2;
				}

				template< typename T >
				static float Saturate(T v)
				{
					return v < 0 ? 0 : (v > 1 ? 1 : v);
				}

				template< typename T >
				static double Saturate(T v)
				{
					return v < 0 ? 0 : (v > 1 ? 1 : v);
				}

				////////////////////////////////////////////////////////////////////
				// ranges functions
		
				// checks if value is between max and min range
				// min and  max must be ordered

				template<class T>
				static bool IsInMinMax(const T v, const T v1, const T v2)
				{
					return(v >= v1) && (v <= v2);
				}

				// checks if value is between max and min range
				// min and max don't need to be ordered

				template<class T>
				static bool IsIn(const T v, const T v1, const T v2)
				{
					return((v >= v1) && (v <= v2)) || ((v >= v2) && (v <= v1));
				}

				template<typename T>
				static bool IsOutMinMax(const T v, const T v1, const T v2)
				{
					return(v < v1) || (v > v2);
				}

				template<typename T>
				static bool IsOut(const T v, const T v1, const T v2)
				{
					return((v < v1) || (v > v2)) && ((v < v2) || (v > v1));
				}
		
				template< typename T >
				static bool FloatEquals(const T value1, const T value2, const double eps = vml::math::EPSILON)
				{
					return fabs(value1 - value2) < eps;
				}

				// Determines whether the two floating-point values f1 and f2 are
				// close enough together that they can be considered equal.
				
				template< typename T >
				static bool CloseEnough(const T f1, const T f2, const double eps = vml::math::EPSILON)
				{
					if (fabs(f1 - f2) < eps)
						return true;
					return fabs(f1 - f2) < eps * Max(fabs(f1), fabs(f2));
				}

				/////////////////////////////////////////////////////////////////////////
				// Snaps a value to the specified grid size. If the value is not
				// a multiple of grid, then value will be rounded to the nearest multiple
				// of a square grid.
				// 
				// param  val   : The value to snap
				// param  grid  : Snap value to use
				// return       : The input value, snapped to a multiple of grid

				template<typename T>
				static T Snap(const T& val, const T& grid)
				{
					T base = val % grid;
					// Account for negatives
					if (base < 0)
						base += grid;
					if (base < (grid / 2))
						return val - base;
					else
						return (grid - base) + val;
					return 0;
				}

		}  // end of namespace ranges

	}	// end of math namespace

}	// end of vml namespace
