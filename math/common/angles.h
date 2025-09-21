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
// helper functions for angle classes

namespace vml
{
	namespace math
	{
		namespace angles
		{

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians (0 to 360 degrees)

			template < typename T >
			static double WrapDegrees(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0) angle = fmod(angle, 360.0) + 360.0;
				else if (angle > 180.0) angle = fmod(angle, 360.0);
				return  angle;
			}

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians with sign (0 to 360 degrees)

			template < typename T >
			static double WrapDegreesSigned(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0) angle = fmod(angle, 360.0);
				else if (angle > 180.0)	angle = fmod(angle, 360.0);
				return  angle;
			}

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians 

			template < typename T >
			static double WrapRadians(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0) angle = fmod(angle, PIMUL2) + PIMUL2;
				else if (angle > PI) angle = fmod(angle, PIMUL2);
				return  angle;
			}

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians with sign (0 to 360 degrees)

			template < typename T >
			static double WrapRadiansSigned(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0) angle = fmod(angle, PIMUL2);
				else if (angle > PI) angle = fmod(angle, PIMUL2);
				return  angle;
			}

			/////////////////////////////////////////////////////////////////////
			// wraps an integer to hi value

			static int WrapInt(int val, int hi)
			{
				if (val < 0) return ((val % hi) + hi) % hi;
				return val % hi;
			}

			/////////////////////////////////////////////////////////////////////
			// wrap number between a range

			template < typename T >
			static double WrapBetween(T val, T lowerBound, T upperBound)
			{
				if (lowerBound > upperBound) std::swap(lowerBound, upperBound);
				//adjust to 0
				val -= lowerBound;
				double rangeSize = (double)upperBound - (double)lowerBound;
				//avoid dividing by 0
				if (rangeSize == 0) return upperBound;
				return (double)val - (rangeSize * std::floor((double)val / rangeSize)) + lowerBound;
			}

			/////////////////////////////////////////////////////////////////////
			// wrap number between a range

			template < typename T >
			static double WrapBetween0And2Pi(T val)
			{
				return (double)val - (PIMUL2 * std::floor((double)val *INV2PI));
			}
		  
			/////////////////////////////////////////////////////////////////////
			// wrap number between a range

			template < typename T >
			static double WrapBetweenMinusPiAndPi(T val)
			{
				return (double)val - (PIMUL2*std::floor(((double)(val)-PI) * INV2PI)) - PIMUL2;
			}

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians with sign (0 to 360 degrees)

			template < typename T >
			static double ClampDegreesSigned(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0f)
					angle = fmod(angle, 360.0);
				else if (angle > 180.0)
					angle = fmod(angle, 360.0);
				return  angle;
			}

			/////////////////////////////////////////////////////////////////////
			// Angles wrap around within the range 0 to 2*PI radians (0 to 360 degrees)

			template < typename T >
			static double ClampDegrees(T a)
			{
				double angle = static_cast<double>(a);
				if (angle < 0.0f)
					angle = fmod(angle, 360.0) + 360.0;
				else if (angle > 180.0)
					angle = fmod(angle, 360.0);
				return  angle;
			}


			/////////////////////////////////////////////////////////////////////
			// give arctan value according to the quadrant the vector is in

			template < typename T >
			static double Atan2(T ay, T ax, const double &eps)
			{
				double dx = static_cast<double>(ax);
				double dy = static_cast<double>(ay);

				if (dx > -eps && dx < eps)
				{
					if (dy > eps)
						return  vml::math::PIDIV2;
					else
						return -vml::math::PIDIV2;
				}

				if (dy > -eps && dy < eps)
				{
					if (dx < eps)
						return  0;
					else
						return -vml::math::PI;
				}

				if (dx > -eps)
				{
					return -atan(dy / dx) + vml::math::PI;
				}
				else
				{
					return -atan(dy / dx);
				}

				// basically the vector has zero dimensions

				return 0;
			}

			///////////////////////////////////////////////////////////////////
			// Computes the sine and cosine of a scalar float.
			// value must be given in radians

			template <typename T>
			static void SinCos(T* scalarsin, T* scalarcos, T value)
			{
				// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.

				double quotient = (vml::math::INVPI*0.5f)*value;
				if (value >= 0.0f)
					quotient = (double)((int)(quotient + 0.5f));
				else
					quotient = (double)((int)(quotient - 0.5f));

				double y = value - (vml::math::PIMUL2)*quotient;

				// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).

				double sign;

				if (y > vml::math::PIDIV2)
				{
					y = vml::math::PI - y;
					sign = -1.0f;
				}
				else if (y < -vml::math::PIDIV2)
				{
					y = -vml::math::PI - y;
					sign = -1.0f;
				}
				else
				{
					sign = +1.0f;
				}

				double y2 = y * y;

				// 11-degree minimax approximation
				*scalarsin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

				// 10-degree minimax approximation
				double p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;

				*scalarcos = sign*p;
			}

			///////////////////////////////////////////////////////////////////

			template <typename T>
			static double ApproxSin(T a)
			{
				double angle = static_cast<double>(a);
				double final_sign = 1.0f;
				if ((angle <= 180.0f) && (angle > 90.0f)) { angle = 180.0f - angle;    final_sign = 1.0f; }
				else if ((angle <= 270.0f) && (angle > 180.0f)) { angle = angle - 180.0f; final_sign = -1.0f; }
				else if ((angle <= 360.0f) && (angle > 270.0f)) { angle = 360.0f - angle;    final_sign = -1.0f; }
				angle *= PI / 180.0f;
				double asqr = angle * angle;
				double result = -2.39e-08;
				result *= asqr;
				result += 2.7526e-06;
				result *= asqr;
				result -= 1.98409e-04;
				result *= asqr;
				result += 8.3333315e-03;
				result *= asqr;
				result -= 1.666666664e-01;
				result *= asqr;
				result += 1.0f;
				result *= angle;
				return result * final_sign;
			}

			///////////////////////////////////////////////////////////////////

			template <typename T>
			static double ApproxCos(T a)
			{
				double angle = static_cast<double>(a);
				double final_sign = 1.0;
				if ((angle <= 180.0) && (angle >     90.0)) { angle = 180.0 - angle;    final_sign = -1.0; }
				else if ((angle <= 270.0) && (angle > 180.0)) { angle = angle - 180.0; final_sign = -1.0; }
				else if ((angle <= 360.0) && (angle > 270.0)) { angle = 360.0 - angle;    final_sign = 1.0; }
				angle *= PI / 180.0;
				double asqr = angle * angle;
				double result = -2.605e-07;
				result *= asqr;
				result += 2.47609e-05;
				result *= asqr;
				result -= 1.3888397e-03;
				result *= asqr;
				result += 4.16666418e-02;
				result *= asqr;
				result -= 4.999999963e-01;
				result *= asqr;
				result += 1.0;
				return result * final_sign;
			}

			///////////////////////////////////////////////////////////////////

			template <typename T>
			static double ApproxTan(T a)
			{
				double angle = static_cast<double>(a);
				double final_sign = 1.0;
				if ((angle <= 180.0) && (angle >     90.0)) { angle = 180.0 - angle;    final_sign = -1.0; }
				else if ((angle <= 270.0) && (angle > 180.0)) { angle = angle - 180.0; final_sign = 1.0; }
				else if ((angle <= 360.0) && (angle > 270.0)) { angle = 360.0 - angle;    final_sign = -1.0; }
				angle *= PI / 180.0;
				double asqr = angle * angle;
				double result = 9.5168091e-03;
				result *= asqr;
				result += 2.900525e-03;
				result *= asqr;
				result += 2.45650893e-02;
				result *= asqr;
				result += 5.33740603e-02;
				result *= asqr;
				result += 1.333923995e-01;
				result *= asqr;
				result += 3.333314036e-01;
				result *= asqr;
				result += 1.0;
				result *= angle;
				return result * final_sign;
			}

			///////////////////////////////////////////////////////////////////

			template <typename T>
			static double Acos(T a)
			{
				double angle = static_cast<double>(a);
				if (-1.0f < angle)
				{
					if (angle < 1.0f)
						return acos(angle);
					else
						return 0.0;
				}
				else
				{
					return vml::math::PI;
				}
			}

			///////////////////////////////////////////////////////////////////
			// the function accepts angles in radians and returns 
			// the direction vector components

			template <typename T>
			static void SphericalCoordinates(const T phi, const T theta, T &rx, T &ry, T &rz)
			{
				float sphi = sin(phi);
				rx = sphi * cos(theta);
				ry = cos(phi);
				rz =-sphi* sin(theta);
			}

		} // end of namespace angles

	}	// end of math namespace

} // end of vml namespace

////////////////////////////////////////////////////////////////////////////////////////////////////////
// radians and degrees classes to threat scalar values as rotational values

namespace vml
{
	namespace math
	{
		namespace angles
		{

			/////////////////////////////////////////////////////////////////////////////////////////////
			// classes to handle angles in radians and degrees

			template < typename T > class Degree;
			template < typename T > class Radian;

			/////////////////////////////////////////////////////////////////////////////////////////////
			// class to handle angle in radians

			template< typename T>
			class Radian
			{
				private:

					T Rad;

				public:

					Radian(const Degree<T>& d);
					Radian &operator =  (const Degree<T>& deg);
					Radian  operator +  (const Degree<T>& deg) const;
					Radian &operator += (const Degree<T>& deg);
					Radian &operator -= (const Degree<T>& deg);
					Radian  operator -  (const Degree<T>& deg) const;

					Radian &operator +=  (const Radian& r) { Rad += r.Rad; return *this; }
					Radian &operator -=  (const Radian& r) { Rad -= r.Rad; return *this; }
					Radian &operator *=  (T f) { Rad *= f; return *this; }
					Radian &operator /=  (T f) { Rad /= f; return *this; }
					Radian  operator  +  (const Radian& r) const { return Radian(Rad + r.Rad); }
					Radian  operator  -  () const { return Radian(-Rad); }
					Radian  operator  -  (const Radian& r) const { return Radian(Rad - r.Rad); }
					Radian  operator  *  (T f) const { return Radian(Rad*f); }
					Radian  operator  *  (const Radian& f) const { return Radian(Rad * f.Rad); }
					Radian  operator  /  (T f) const { return Radian(Rad / f); }

					bool operator <  (const Radian& r) const { return Rad  < r.Rad; }
					bool operator <= (const Radian& r) const { return Rad <= r.Rad; }
					bool operator == (const Radian& r) const { return Rad == r.Rad; }
					bool operator != (const Radian& r) const { return Rad != r.Rad; }
					bool operator >= (const Radian& r) const { return Rad >= r.Rad; }
					bool operator >  (const Radian& r) const { return Rad  > r.Rad; }

					T GetValue() const
					{
						return vml::math::angles::WrapDegreesSigned(Rad);
					}

					T GetDegreeValue() const
					{
						return vml::math::angles::WrapDegreesSigned(Rad)*vml::math::RADTODEG;
					}

					////////////////////////////////////////////////////////////////////////////
					// ctor / dtor

					explicit Radian(T r = 0)
					{
						Rad = vml::math::angles::WrapDegreesSigned(r);
					}

					~Radian()
					{
					}

					Radian& operator = (const T r)
					{
						Rad = vml::math::angles::WrapDegreesSigned(r);
						return *this;
					}

					Radian& operator = (const Radian& r)
					{
						Rad = vml::math::angles::WrapDegreesSigned(r.Rad);
						return *this;
					}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// class to handle angles in degrees

			template < typename T >
			class Degree
			{
				private:

					T Deg;

				public:

					Degree& operator = (const Radian<T>& r);
					Degree(const Radian<T>& r);
					Degree operator +   (const Radian<T>& r) const;
					Degree& operator += (const Radian<T>& r);
					Degree& operator -= (const Radian<T>& r);
					Degree operator -   (const Radian<T>& r) const;

					Degree& operator += (const Degree& r) { Deg += r.Deg; return *this; }
					Degree& operator -= (const Degree& r) { Deg -= r.Deg; return *this; }
					Degree& operator *= (T f) { Deg *= f; return *this; }
					Degree& operator /= (T f) { Deg /= f; return *this; }
					Degree operator   + (const Degree& r) const { return Degree(Deg + r.Deg); }
					Degree operator   - () const { return Degree(-Deg); }
					Degree operator   - (const Degree& r) const { return Degree(Deg - r.Deg); }
					Degree operator   * (T f) const { return Degree(Deg * f); }
					Degree operator   * (const Degree& f) const { return Degree(Deg * f.Deg); }
					Degree operator   / (T f) const { return Degree(Deg / f); }

					bool operator <  (const Degree& r) const { return Deg < r.Deg; }
					bool operator <= (const Degree& r) const { return Deg <= r.Deg; }
					bool operator == (const Degree& r) const { return Deg == r.Deg; }
					bool operator != (const Degree& r) const { return Deg != r.Deg; }
					bool operator >= (const Degree& r) const { return Deg >= r.Deg; }
					bool operator >  (const Degree& r) const { return Deg > r.Deg; }

					T GetValue() const
					{
						return vml::math::angles::WrapDegreesSigned(Deg);
					}

					T GetRadianValue() const
					{
						return vml::math::angles::WrapDegreesSigned(Deg)*vml::math::DEGTORAD;
					}

					////////////////////////////////////////////////////////////////////////////
					// ctor / dtor

					explicit Degree(T d = 0)
					{
						Deg = vml::math::angles::WrapDegreesSigned(d);
					}

					~Degree()
					{
					}

					Degree& operator = (const T d)
					{
						Deg = vml::math::angles::WrapDegreesSigned(d);
						return *this;
					}

					Degree& operator = (const Degree& d)
					{
						Deg = vml::math::angles::WrapDegreesSigned(d.Deg);
						return *this;
					}

			};

			/////////////////////////////////////////////////////////////////////
			// these functions could not be defined within the class definition of class
			// Radian because they required class Degree to be defined

			template < typename T >
			Radian<T>::Radian(const Degree<T>& deg) : Rad(deg.GetRadianValue())
			{
			}

			template < typename T >
			Radian<T>& Radian<T>::operator = (const Degree<T>& deg)
			{
				Rad = deg.GetRadianValue(); return *this;
			}

			template < typename T >
			Radian<T> Radian<T>::operator + (const Degree<T>& deg) const
			{
				return Radian(Rad + deg.GetRadianValue());
			}

			template < typename T >
			Radian<T>& Radian<T>::operator += (const Degree<T>& deg)
			{
				Rad += deg.GetRadianValue();
				return *this;
			}

			template < typename T >
			Radian<T> Radian<T>::operator - (const Degree<T>& deg) const
			{
				return Radian(Rad - deg.GetRadianValue());
			}

			template < typename T >
			Radian<T>& Radian<T>::operator -= (const Degree<T>& deg)
			{
				Rad -= deg.GetRadianValue();
				return *this;
			}

			/////////////////////////////////////////////////////////////////////
			// these functions could not be defined within the class definition of class
			// Radian because they required class Degree to be defined

			template < typename T >
			Degree<T>::Degree(const Radian<T>& rad) : Degree(rad.GetDegreeValue())
			{
			}

			template < typename T >
			Degree<T>& Degree<T>::operator = (const Radian<T>& rad)
			{
				Deg = rad.GetDegreeValue(); return *this;
			}

			template < typename T >
			Degree<T> Degree<T>::operator + (const Radian<T>& rad) const
			{
				return Degree(Deg + rad.GetDegreeValue());
			}

			template < typename T >
			Degree<T>& Degree<T>::operator += (const Radian<T>& rad)
			{
				Deg += rad.GetDegreeValue();
				return *this;
			}

			template < typename T >
			Degree<T> Degree<T>::operator - (const Radian<T>& rad) const
			{
				return Degree(Deg - rad.GetDegreeValue());
			}

			template < typename T >
			Degree<T>& Degree<T>::operator -= (const Radian<T>& rad)
			{
				Deg -= rad.GetDegreeValue();
				return *this;
			}

		} // end of namespace angles

	}	// end of math namespace

} // end of vml namespace

