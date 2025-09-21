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

namespace vml
{
	namespace math
	{
		namespace trigs
		{

			//////////////////////////////////////////////////////////////////
			//	arcsin

			static float Asin(const float val)
			{
				if (-1.0f < val)
				{
					if (val < 1.0f)
						return asin(val);
					else
						return vml::math::PIDIV2;
				}
				else
				{
					return -vml::math::PIDIV2;
				}
			}

			//////////////////////////////////////////////////////////////////
			//	taylor sinus  function

			static float TaylorSin(float a)
			{
				float a2  =  a*a,
					  a3  = a2*a,
					  a5  = a3*a2,
					  a7  = a5*a2,
					  a9  = a7*a2,
					  a11 = a9*a2;

				return (  a -  a3*0.166666666f +
							   a5*0.008333333f -
							   a7*0.000198412f +
							   a9*0.000002755f -
							  a11*0.000000015f ) ;
			}

			//////////////////////////////////////////////////////////////////
			//	taylor cosinus  function

			static float TaylorCos(float a)
			{
				float a2  = a *a,
					  a4  = a2*a2,
					  a6  = a4*a2,
					  a8  = a6*a2,
					  a10 = a8*a2;

				return ( 1.0f -  a2*0.500000000f +
								 a4*0.041666666f -
								 a6*0.001388888f +
								 a8*0.000024801f -
								a10*0.000000275f );
			}

			static float FastSin0(float val)
			{
				float angleSqr = val*val;
				float result = 7.61e-03f;
				result *= angleSqr;
				result -= 1.6605e-01f;
				result *= angleSqr;
				result += 1.0f;
				result *= val;
				return result;
			}

			static float FastSin1(float val)
			{
				float angleSqr = val*val;
				float result = -2.39e-08f;
				result *= angleSqr;
				result += 2.7526e-06f;
				result *= angleSqr;
				result -= 1.98409e-04f;
				result *= angleSqr;
				result += 8.3333315e-03f;
				result *= angleSqr;
				result -= 1.666666664e-01f;
				result *= angleSqr;
				result += 1.0f;
				result *= val;
				return result;
			}

			static float FastCos0(float val)
			{
				float angleSqr = val*val;
				float result = 3.705e-02f;
				result *= angleSqr;
				result -= 4.967e-01f;
				result *= angleSqr;
				result += 1.0f;
				return result;
			}

			static float FastCos1(float val)
			{
				float angleSqr = val*val;
				float result = -2.605e-07f;
				result *= angleSqr;
				result += 2.47609e-05f;
				result *= angleSqr;
				result -= 1.3888397e-03f;
				result *= angleSqr;
				result += 4.16666418e-02f;
				result *= angleSqr;
				result -= 4.999999963e-01f;
				result *= angleSqr;
				result += 1.0f;
				return result;
			}

			static float FastTan0(float val)
			{
				float angleSqr = val*val;
				float result = 2.033e-01f;
				result *= angleSqr;
				result += 3.1755e-01f;
				result *= angleSqr;
				result += 1.0f;
				result *= val;
				return result;
			}

			static float FastTan1(float val)
			{
				float angleSqr = val*val;
				float result = 9.5168091e-03f;
				result *= angleSqr;
				result += 2.900525e-03f;
				result *= angleSqr;
				result += 2.45650893e-02f;
				result *= angleSqr;
				result += 5.33740603e-02f;
				result *= angleSqr;
				result += 1.333923995e-01f;
				result *= angleSqr;
				result += 3.333314036e-01f;
				result *= angleSqr;
				result += 1.0f;
				result *= val;
				return result;
			}

			static float FastASin0(float val)
			{
				float root = sqrt(abs(1.0f - val));
				float result = -0.0187293f;
				result *= val;
				result += 0.0742610f;
				result *= val;
				result -= 0.2121144f;
				result *= val;
				result += 1.5707288f;
				result = vml::math::PIDIV2 - root*result;
				return result;
			}

			static float FastASin1(float val)
			{
				float root = sqrt(abs(1.0f - val));
				float result = -0.0012624911f;
				result *= val;
				result += 0.0066700901f;
				result *= val;
				result -= 0.0170881256f;
				result *= val;
				result += 0.0308918810f;
				result *= val;
				result -= 0.0501743046f;
				result *= val;
				result += 0.0889789874f;
				result *= val;
				result -= 0.2145988016f;
				result *= val;
				result += 1.5707963050f;
				result = vml::math::PIDIV2 - root*result;
				return result;
			}

			static float FastACos0(float val)
			{
				float root = sqrt(abs(1.0f - val));
				float result = -0.0187293f;
				result *= val;
				result += 0.0742610f;
				result *= val;
				result -= 0.2121144f;
				result *= val;
				result += 1.5707288f;
				result *= root;
				return result;
			}

			static float FastACos1(float val)
			{
				float root = sqrt(abs(1.0f - val));
				float result = -0.0012624911f;
				result *= val;
				result += 0.0066700901f;
				result *= val;
				result -= 0.0170881256f;
				result *= val;
				result += 0.0308918810f;
				result *= val;
				result -= 0.0501743046f;
				result *= val;
				result += 0.0889789874f;
				result *= val;
				result -= 0.2145988016f;
				result *= val;
				result += 1.5707963050f;
				result *= root;
				return result;
			}

			static float FastATan0(float val)
			{
				float valueSqr = val*val;
				float result = 0.0208351f;
				result *= valueSqr;
				result -= 0.085133f;
				result *= valueSqr;
				result += 0.180141f;
				result *= valueSqr;
				result -= 0.3302995f;
				result *= valueSqr;
				result += 0.999866f;
				result *= val;
				return result;
			}

			static float FastATan1(float val)
			{
				float valueSqr = val*val;
				float result = 0.0028662257f;
				result *= valueSqr;
				result -= 0.0161657367f;
				result *= valueSqr;
				result += 0.0429096138f;
				result *= valueSqr;
				result -= 0.0752896400f;
				result *= valueSqr;
				result += 0.1065626393f;
				result *= valueSqr;
				result -= 0.1420889944f;
				result *= valueSqr;
				result += 0.1999355085f;
				result *= valueSqr;
				result -= 0.3333314528f;
				result *= valueSqr;
				result += 1.0f;
				result *= val;
				return result;
			}

			// ----------------------------------------------------------
			// return a 'safe' acos 

			static float SafeAcos(float value)
			{
				if (value < -1.0)
				{
					return vml::math::PI;
				}
				else if (value > 1.0)
				{
					return 0;
				}
				else
				{
					return acos(value);
				}
			}

		} // end of trigs namespace

	} // end of math namespace

}	// end of vml namespace


namespace vml
{
	namespace math
	{
		namespace trigs
		{

			//////////////////////////////////////////////////////////////////
			// Polar cooordiantes

			class Polar
			{

				public:

					float R;     // Distance coordinate from origin.
					float Theta; // Counter-clockwise rotation from x-axis.

					// ---------------------------------------------------------
					// Convert to cartesian.
				
					void ToCartesian(float vec[2])
					{
						vec[0] = R * cos(Theta);
						vec[1] = R * sin(Theta);
					}

					// ---------------------------------------------------------
					// ctor / dtor

					Polar(float r, float theta)
					{
						R	  = r;
						Theta = theta;
					}

					Polar(const float cartesian[2])
					{
						R = sqrtf(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1]);
						Theta = atan2(cartesian[1], cartesian[0]);
					}

					~Polar()
					{
					}

			};

			//////////////////////////////////////////////////////////////////
			// Spherical cooordiantes
			
			class Spherical
			{
				public:

					float R;     // Distance coordinate from origin.
					float Phi;   // counter-clockwise rotation from xy plane to z (z being up-axis in conventional math).
					float Theta; // counter-clockwise rotation from x axis on xy plane.

					// ---------------------------------------------------------------------
					// convert from spherical to cartesian coordinates
					// All angles are in radians.
					// rho = distance from origin O to point P (i.e., length of OP)
					// phi = angle between OP and the XZ plane
					// theta = angle between X-axis and OP projected onto XZ plane


					void ToCartesian(float cartesian[3])
					{
						cartesian[0] = R * sin(Phi) * cos(Theta); // project Phi onto xy plane and then polar coordinate conversion (r*cos(Theta))	
						cartesian[1] = R * sin(Phi) * sin(Theta), // project Phi onto xy plane and then polar coordinate conversion (r*sin(Theta))
						cartesian[2] = R * cos(Phi);
					}

					///////////////////////////////////////////////////////
					// convert from cartesian to spherical coordinates
					// All angles are in radians.
					// rho = distance from origin O to point P (i.e., length of OP)
					// phi = angle between OP and the XZ plane
					// theta = angle between X-axis and OP projected onto XZ plane

					void ToSpherical(const float cartesian[3], float& rho, float& phi, float& theta)
					{
						R   = sqrtf(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1] + cartesian[2] * cartesian[2]);
						Phi = sqrtf(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1]) / cartesian[2];
						if (cartesian[0] > vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0]);
						if (cartesian[0] < vml::math::EPSILON && cartesian[1] > vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0]) + vml::math::PI;
						if (cartesian[0] < vml::math::EPSILON && cartesian[1] < vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0]) - vml::math::PI;
						if (fabs(cartesian[0]) > vml::math::EPSILON && cartesian[1] > vml::math::EPSILON)
							Theta = vml::math::PI;
						if (fabs(cartesian[0]) > vml::math::EPSILON && cartesian[1] < vml::math::EPSILON)
							Theta = -vml::math::PI;
					}

					// ---------------------------------------------------------------------
					// convert to cartesian.

					Spherical(const float r, const float phi, const float theta)
					{
						R     = r;
						Phi   = phi;
						Theta = theta;
					}

					Spherical(float cartesian[3])
					{
						R	= sqrtf(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1] + cartesian[2] * cartesian[2]);
						Phi = sqrtf(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1]) / cartesian[2];
						if (cartesian[0] > vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0]);
						if (cartesian[0] < vml::math::EPSILON && cartesian[1] > vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0])+vml::math::PI;
						if (cartesian[0] < vml::math::EPSILON && cartesian[1] < vml::math::EPSILON)
							Theta = atan(cartesian[1] / cartesian[0]) - vml::math::PI;
						if (fabs(cartesian[0]) > vml::math::EPSILON && cartesian[1]> vml::math::EPSILON)
							Theta =  vml::math::PI;
						if (fabs(cartesian[0]) > vml::math::EPSILON && cartesian[1] < vml::math::EPSILON)
							Theta = -vml::math::PI;
					}

					~Spherical()
					{

					}
			};
			

		} // end of trigs namespace

	} // end of math namespace

}	// end of vml namespace


/*

// Convert matrix to Euler angles (in radians). 
EulerAngles Eul_FromHMatrix(HMatrix M, int order)
{
	EulerAngles ea;
	int i, j, k, h, n, s, f;
	EulGetOrd(order, i, j, k, h, n, s, f);
	if (s == EulRepYes) {
		double sy = sqrt(M[i][j] * M[i][j] + M[i][k] * M[i][k]);
		if (sy > 16 * FLT_EPSILON) {
			ea.x = atan2(M[i][j], M[i][k]);
			ea.y = atan2(sy, M[i][i]);
			ea.z = atan2(M[j][i], -M[k][i]);
		}
		else {
			ea.x = atan2(-M[j][k], M[j][j]);
			ea.y = atan2(sy, M[i][i]);
			ea.z = 0;
		}
	}
	else {
		double cy = sqrt(M[i][i] * M[i][i] + M[j][i] * M[j][i]);
		if (cy > 16 * FLT_EPSILON) {
			ea.x = atan2(M[k][j], M[k][k]);
			ea.y = atan2(-M[k][i], cy);
			ea.z = atan2(M[j][i], M[i][i]);
		}
		else {
			ea.x = atan2(-M[j][k], M[j][j]);
			ea.y = atan2(-M[k][i], cy);
			ea.z = 0;
		}
	}
	if (n == EulParOdd) { ea.x = -ea.x; ea.y = -ea.y; ea.z = -ea.z; }
	if (f == EulFrmR) { float t = ea.x; ea.x = ea.z; ea.z = t; }
	ea.w = order;
	return (ea);
}
*/