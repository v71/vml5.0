#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
//4
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
// dependencies

#include <math.h>

///////////////////////////////////////////////////////////////
// a complex number class

namespace vml
{
	namespace math
	{
		namespace complex
		{
			///////////////////////////////////////////////////////////////
			// a complex number class

			class Complex
			{

				public:

					double real;       // Real Part
					double imag;      //  Imaginary Part

					Complex()
					{
						real = 0.0f;
						imag = 0.0f;
					}

					Complex(double r, double im)
					{
						real = r;
						imag = im;
					}

					Complex(const Complex &c)
					{
						real = c.real;
						imag = c.imag;
					}

					~Complex()
					{
					}

					const Complex &operator =(const Complex &c)
					{
						if (this == &c)
							return *this;
						real = c.real;
						imag = c.imag;
						return *this;
					}

					void Set(double r, double im)
					{
						real = r;
						imag = im;
					}

					void SetReal(double r)
					{
						real = r;
					}

					void SetIm(double im)
					{
						imag = im;
					}

					double GetReal()
					{
						return real;
					}

					double GetIm()
					{
						return imag;
					}

					double GetRealTrigForm()
					{
						if ( real > -vml::math::EPSILON && real < vml::math::EPSILON)
						{
							double f;
							if (imag < 0.0f) f = -1.5707963268f; // -pi / 2
							else  f = 1.5707963268f; // pi / 2
						}
						else f = atan2(imag, real);
						return real * cos(f);
					}

					double GetImTrigForm()
					{
						if (real > -vml::math::EPSILON && real < vml::math::EPSILON)
						{
							double f;
							if (imag < 0.0f) f = -1.5707963268f; // -pi / 2
							else  f = 1.5707963268f; // pi / 2
						}
						else f = atan2(imag, real);
						return real * sin(f);
					}

					const Complex operator-()
					{
						real = -real;
						imag = -imag;
						return *this;
					}

					const Complex& operator+()
					{
						return *this;
					}

					const Complex &operator +=(const Complex &c)
					{
						real += c.real;
						imag += c.imag;
						return *this;
					}

					const Complex &operator -=(const Complex &c)
					{
						real -= c.real;
						imag -= c.imag;
						return *this;
					}

					const Complex operator *=(const double s)
					{
						real = s * real;
						imag = s * imag;
						return *this;
					}

					const Complex operator *=(const Complex &c)
					{
						double mreal = (real*c.real) - (imag*c.imag);
						double mimag = (real*c.imag) + (imag*c.real);
						real = mreal;
						imag = mimag;
						return *this;
					}

					const Complex operator /=(const Complex &c)
					{
						double div = (c.real*c.real) + (c.imag*c.imag);
						double mreal = (real*c.real) + (imag*c.imag);
						double mimag = (imag*c.real) - (real*c.imag);
						mreal /= div;
						mimag /= div;
						real = mreal;
						imag = mimag;
						return *this;
					}

					const Complex operator /=(const double s)
					{
						real = real / s;
						imag = imag / s;
						return *this;
					}

					const Complex GetConjugate()
					{
						Complex tmp;
						tmp.real = real;
						tmp.imag = -imag;
						return tmp;
					}

					const Complex GetReciprocal()
					{
						Complex t;
						t.real = real;
						t.imag = -imag;
						double div;
						div = (real*real) + (imag*imag);
						t.real /= div;
						t.imag /= div;
						return t;
					}

					const double GetModulus()
					{
						double z;
						z = (real*real) + (imag*imag);
						z = sqrt(z);
						return z;
					}

					const double GetPhase()
					{
						if ((real > -vml::math::EPSILON && real < vml::math::EPSILON) && imag < vml::math::EPSILON) return -1.5707963268f; // -pi / 2
						if ((real > -vml::math::EPSILON && real < vml::math::EPSILON) && imag > vml::math::EPSILON) return  1.5707963268f; //  pi / 2
						if ((real > -vml::math::EPSILON && real < vml::math::EPSILON) && (imag > -vml::math::EPSILON && imag < vml::math::EPSILON)) return  0.0f;
						return double(atan2(imag, real));
					}

					bool operator==(const Complex &c)
					{
						return (real == c.real) && (imag == c.imag) ? 1 : 0;
					}

					bool operator !=(const Complex &c)
					{
						return (real != c.real) && (imag != c.imag) ? 1 : 0;
					}
			};

			static const Complex operator *(const Complex& c, const double s)
			{
				return Complex(c.real*s, c.imag*s);
			}

			static const Complex operator *(const double s, const Complex& c)
			{
				return Complex(c.real*s, c.imag*s);
			}

			static const Complex operator *(const Complex& c1, const Complex& c2)
			{
				return Complex((c1.real*c2.real) - (c1.imag*c2.imag),(c1.real*c2.imag) + (c1.imag*c2.real));
			}

			static const Complex operator /(const Complex& c, const double s)
			{
				return Complex(c.real / s, c.imag / s);
			}

			static const Complex operator /(const double s, const Complex& c)
			{
				double sc = s / (c.real*c.real + c.imag*c.imag);
				return Complex(sc*c.real, sc*c.imag);
			}

			static const Complex operator /(const Complex& c1, const Complex& c2)
			{
				double sc = 1.0f / (c2.real*c2.real + c2.imag*c2.imag);
				return Complex(sc*(c1.real*c2.real + c1.imag*+c2.imag), sc*(c1.imag*c2.real - c2.imag*c1.real));
			}

			static const Complex operator +(const Complex& c1, const Complex& c2)
			{
				return Complex(c1.real + c2.real, c1.imag + c2.imag);
			}

			static const Complex operator -(const Complex& c1, const Complex& c2)
			{
				return Complex(c1.real - c2.real, c1.imag - c2.imag);
			}

		}

	}
}