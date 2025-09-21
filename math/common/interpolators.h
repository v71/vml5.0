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
//  interpolatopr function

namespace vml
{
	namespace math
	{
		namespace interpolators
		{
				//////////////////////////////////////////////////////////////
				// interpolators
				// usage :
				// Create array
				// double p[4][4] = {{1,3,3,4}, {7,2,3,4}, {1,6,3,6}, {2,5,7,2}};
				// Interpolate
				// std::cout << bicubicInterpolate(p, 0.1, 0.2) << '\n';
				// Or use the nCubicInterpolate function
				// double co[2] = {0.1, 0.2};
				// std::cout << nCubicInterpolate(2, (double*) p, co) << '\n';

				//////////////////////////////////////////////////////////////
				//	Simple linear interpolation between two entities

				static double LinearInterpolate( const double a, const double b, double t )
				{
					return a + (b - a) * t;
				}

				//////////////////////////////////////////////////////////////
				// simple linear interpolation

				 static double InverseLinearInterpolate( const double a, const double b, const double x)
				 {
						 return (double)(x - a) / (double)(b - a);
				 }

				 //////////////////////////////////////////////////////////////
				 // bilinear interpolator

				 static double BilinearInterpolate(const double a, const double b, const double c, const double d, double u, double v)
				 {
					// Performs a bilinear interpolation.
					//  P(u,v) = e + v(f - e)
					//  
					//  where
					//  e = a + u(b - a)
					//  f = c + u(d - c)
					//  u in range [0,1]
					//  v in range [0,1]

					return a * ((1.0f - u) * (1.0f - v))
						 + b * (u * (1.0f - v))
		 				 + c * (v * (1.0f - u))
		  				 + d * (u * v);
				 }
				
				 //////////////////////////////////////////////////////////////
				 // cubic interpolator

				 static double CubicInterpolate ( double p[4], double x) 
				 {
					return p[1] + 0.5f * x*(p[2] - p[0] + x*(2.0f*p[0] - 5.0f*p[1] + 4.0f*p[2] - p[3] + x*(3.0f*(p[1] - p[2]) + p[3] - p[0])));
			 	 }
				
				 //////////////////////////////////////////////////////////////
				 // bicubic interpolator

				 static double BicubicInterpolate ( double p[4][4], double x, double y) 
				 {
					double arr[4]={0,0,0,0};
					arr[0] = CubicInterpolate(p[0], y);
					arr[1] = CubicInterpolate(p[1], y);
					arr[2] = CubicInterpolate(p[2], y);
					arr[3] = CubicInterpolate(p[3], y);
					return CubicInterpolate(arr, x);
				 }
				
				 //////////////////////////////////////////////////////////////
				 // tricubic interpolator

				 static double TricubicInterpolate ( double p[4][4][4], double x, double y, double z)
				 {
					double arr[4] = { 0,0,0,0 };
					arr[0] = BicubicInterpolate(p[0], y, z);
					arr[1] = BicubicInterpolate(p[1], y, z);
					arr[2] = BicubicInterpolate(p[2], y, z);
					arr[3] = BicubicInterpolate(p[3], y, z);
					return CubicInterpolate(arr, x);
				 }
				
				 //////////////////////////////////////////////////////////////
				 // n-cubic interpolator

				 static double nCubicInterpolate (int n, double* p, double coordinates[]) 
				 {
	//				if (n == 0) vml::os::Error(L"value for n must be greater than zero");
					if (n == 1) 
						return CubicInterpolate(p, *coordinates);
					else 
					{
						double arr[4] = { 0,0,0,0 };
						int skip = 1 << (n - 1) * 2;
						arr[0] = nCubicInterpolate(n - 1, p, coordinates + 1);
						arr[1] = nCubicInterpolate(n - 1, p + skip, coordinates + 1);
						arr[2] = nCubicInterpolate(n - 1, p + 2*skip, coordinates + 1);
						arr[3] = nCubicInterpolate(n - 1, p + 3*skip, coordinates + 1);
						return CubicInterpolate(arr, *coordinates);
					}
				 }
				
				 //////////////////////////////////////////////////////////////
				 // Returns a gradual transition of 'x' from 0 to 1 beginning at
				 // threshold 'a' and ending at threshold 'b'.
				 //
				 // References:
				 //  [1] http://www.rendermanacademy.com/docs/smoothstep.htm
				 //  [2] http://www.brlcad.org/doxygen/d8/d33/noise_8c-source.html
				 //  [3] Natalya Tatarchuk, "Efficient High-Level Shader Development",
				 //      Game Developers Conference Europe, August 2003.
			
				 static double SmoothStep( double a, double b, double x)
				 {
					if (x < a)
						return 0.0f;
					else if (x >= b)
						return 1.0f;
					else
					{
						x = (x - a) / (b - a);
						return x * x * (3.0f - 2.0f * x);
					}
				 }
				 
				 //////////////////////////////////////////////////////////////
				 // cosinusoidal step

				 static double CosInterpolate ( const double t, const double a, const double b)
				 {
					double f= (1.0f - cos(t * vml::math::PI)) * 0.5f;
					return a*(1-f) + b*f;
				 }

				 //////////////////////////////////////////////////////////////
				 // sinusoidal step

				 static double SinInterpolate ( const double t, const double a, const double b)
				 {
					double f= (1.0f - sin(t * vml::math::PI)) * 0.5f;
					return a*(1-f) + b*f;
				 }

				 //////////////////////////////////////////////////////////////
				 /// Hermite polynomial interpolation function.

				 static double HermiteInterpolate ( const double t, const double p0, const double m0, const double p1, const double m1)
				 {
					double t3 = t*t*t, t2 = t*t;
					double h00 = 2*t3 - 3*t2 + 1;
					double h10 = t3 - 2*t2 + t;
					double h01 = -2*t3 + 3*t2;
					double h11 = t3 - t2;
					return p0 * h00 + m0 * h10 + p1 * h01 + m1 * h11;
				 }

				 //////////////////////////////////////////////////////////////
				 /// Lagrange interpolation function.

				 static double Lagrange(double *x, double *y, size_t n, double ix)
				 {
					 double L = 0, l = 0;
					 for (size_t i = 0; i < n; i++) 
					 {
						 l = 1;
						 for (size_t j = 0; j < n; j++) 
						 {
							 if (i != j) l *= (ix - x[j]) / (x[i] - x[j]);
						 }
						 L += y[i] * l;
					 }
					 return L;
				 }

				/////////////////////////////////////////////////////////////////////////////////////
				// Performs a "boing!" interpolation on the range [a,b] using s as the
				// interpolation percent (0=a, 1=b). Berp will cause the interpolated
				// value to overshoot the upper limit, waver back and forth before finally
				// converging on the upper limit.
				
				static double berp(double a, double b, double s)
				{
					double v =s < 0.0f ? 0.0f : s < 1.0f ? s : 1.0f;
					v = sin( s * vml::math::PI * (0.2f + 2.5f * s * s * s) * pow(1.0f - s, 2.2f) + v) * (1.0f + 1.2f * (1.0f - v));
					return a + (b - a) * s;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Performs a bouncing interpolation on the range [a,b] using s as the
				// interpolation percent (0=a, 1=b). The value will exhibit a "bouncing"
				// behavior as it bounces from its lower limit to upper limit before
				// slowly settling on its lower limit as s approaches 1.
		
				static double bounce(const double& a, const double& b, const double& s)
				{
					double x = abs(sin(6.28f * (s + 1.0f) * (s + 1.0f)) * (1.0f - s));
					return a + (b - a) * x;
				}

		}	// end of interpolators namespace

	}	// end of math namespace

} // end of vml namespace

