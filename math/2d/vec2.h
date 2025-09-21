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

		///////////////////////////////////////////////////////////////////////////////////
		// vec2 class

		template <typename T>
		class vec2 
		{
			public:

			  T x,y;

			  // --------------------------------------------------------------------
			  // artimetic operators

			  __forceinline vec2<T>& operator*=(const T s)
			  {
				  x *= s;
				  y *= s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator/=(const T s)
			  {
				  T invs;
				  if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					  invs = vml::math::EPSILON;
				  else
					  invs = T(1) / T(s);
				  x *= invs;
				  y *= invs;
				  return *this;
			  }

			  __forceinline vec2<T>& operator+=(const vec2<T> &v)
			  {
				  x += v.x;
				  y += v.y;
				  return *this;
			  }

			  __forceinline vec2<T>& operator+=(const T s)
			  {
				  x += s;
				  y += s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator-=(const T s)
			  {
				  x -= s;
				  y -= s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator-=(const vec2<T> &v)
			  {
				  x -= v.x;
				  y -= v.y;
				  return *this;
			  }

			  __forceinline vec2<T> &operator+() 
			  {
				  return *this;
			  }

			  __forceinline vec2<T> operator-() 
			  {
				  return vec2<T>(-x, -y);
			  }

			  __forceinline vec2<T>& operator+() const
			  {
				  return *this;
			  }

			  __forceinline vec2<T> operator-() const
			  {
				  return vec2<T>(-x, -y);
			  }

			  // --------------------------------------------------------------------
			  // comparison operators
			 
			  __forceinline bool operator== ( const vec2<T> &v ) const
			  {
				  return fabs(v.x - x) < vml::math::EPSILON && fabs(v.y - y) < vml::math::EPSILON;
			  }

			  __forceinline bool operator!= ( const vec2<T> &v ) const
			  {
				  return fabs(v.x - x) > vml::math::EPSILON || fabs(v.y - y) > vml::math::EPSILON;
			  }

			// --------------------------------------------------------------------
			// ctor / dtor

			vec2()
			{ 
				x = T(0);
				y = T(0);
			}

			vec2(const T a, const T b)
			{
				x = a;
				y = b;
			}

			// ---------------------------------------------------------------
			// assignment operator

			vec2& operator= (const vec2<T> &v)
			{
				if (&v == this)
					return *this;
				x = v.x;
				y = v.y;
				return *this;
			}

			// ---------------------------------------------------------------
			// copy constructor

			vec2(const vec2<T> &v)
			{
				x = v.x;
				y = v.y;
			}

			~vec2()
			{}

		};

		/////////////////////////////////////////////////////////////////////////////////
		// friend functions
	
		template <typename T>
		__forceinline vec2<T> operator+(const vec2<T> &v, const vec2<T> &w)
		{
			return vec2<T>(v.x + w.x, v.y + w.y);
		}

		template <typename T>
		__forceinline vec2<T> operator-(const vec2<T> &v, const vec2<T> &w)
		{
			return vec2<T>(v.x - w.x, v.y - w.y);
		}

		template <typename T>
		__forceinline vec2<T> operator+(const vec2<T>& v, const T s)
		{
			return vec2<T>(v.x + s, v.y + s);
		}

		template <typename T>
		__forceinline vec2<T> operator-(const vec2<T>& v, const T s)
		{
			return vec2<T>(v.x - s, v.y - s);
		}

		template <typename T>
		__forceinline vec2<T> operator*(const vec2<T> &v, const T s)
		{
			return vec2<T>(v.x * s, v.y * s);
		}

		template <typename T>
		__forceinline vec2<T> operator*(const T s, const vec2<T> &v)
		{
			return vec2<T>(s*v.x, s*v.y);
		}

		template <typename T>
		__forceinline vec2<T> operator/(const vec2<T> &v, const T s)
		{
			T invs;
			if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
				invs = vml::math::EPSILON;
			else
				invs = T(1) / s;
			return vec2<T>(v.x * invs, v.y * invs);
		}

		template <typename T >
		std::ostream& operator<< (std::ostream& output, const vml::math::vec2<T> &v)
		{
			std::ostringstream os;
			os << " [ " << v.x << " , " << v.y << " ]";
			return os.str();
		}

		///////////////////////////////////////////////////////////////////////////////////
		// predefined types
		
		typedef vec2<unsigned char>		vec2b;		// 8-bit  unsigned.
		typedef vec2<uint8_t>			vec2ui8;	// 8-bit  unsigned.
		typedef vec2<uint16_t>			vec2ui16;	// 16-bit unsigned.
		typedef vec2<uint32_t>			vec2ui32;	// 32-bit unsigned.
		typedef vec2<uint64_t>			vec2ui64;	// 64-bit unsigned.
		typedef vec2<int8_t>			vec2i8;		// 8-bit  signed.
		typedef vec2<int16_t>			vec2i16;	// 16-bit signed.
		typedef vec2<int32_t>			vec2i32;	// 32-bit signed.
		typedef vec2<int64_t>			vec2i64;	// 64-bit signed.
		typedef vec2<float_t>			vec2f;		// 32-bit float
		typedef vec2<double_t>			vec2d;		// 64-bit float
		
	} // end of namespace math
}
// end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace math
	{
			// --------------------------------------------------------------------
			// metrics functions

			template <typename T>
			__forceinline [[nodiscard]] T SquaredLength(const vec2<T> &v)
			{
				return v.x * v.x + v.y * v.y;
			}

			template <typename T>
			__forceinline [[nodiscard]] T Length(const vec2<T>& v)
			{
				return sqrtf(v.x * v.x + v.y * v.y);
			}

			// --------------------------------------------------------
			// return sthe dot product between two vectors

			template <typename T>
			__forceinline [[nodiscard]] T Dot(const vec2<T>& v, const vec2<T>& w)
			{
				return (v.x * w.x + v.y * w.y);
			}

			template <typename T>
			__forceinline [[nodiscard]] vec2<T> Normalize(const vec2<T>& v,const float eps=vml::math::EPSILON)
			{
				T d = sqrtf(v.x * v.x + v.y * v.y);
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / d;
				return vec2<T>(v.x * d, v.y * d); 
			}

			// --------------------------------------------------------
			// cross product

			template <typename T>
			__forceinline [[nodiscard]] vec2<T> Cross(const vec2<T>& v, const vec2<T>& w)
			{
				return vec2<T>(v.x * w.y, -v.y * w.x);
			}

			// --------------------------------------------------------
			// return a vector orthogonal to a given vector

			template <typename T>
			__forceinline [[nodiscard]] vec2<T> Ortho(const vec2<T>& v)
			{
				return vec2<T>(-v.y, v.x);
			}

			// --------------------------------------------------------
			// zeros a vector

			template <typename T>
			__forceinline [[nodiscard]] vec2<T> Zero()
			{
				return vec2<T>(T(0), T(0));
			}

			// --------------------------------------------------------
			// returns a random vector in the min - max range

			template <typename T>
			__forceinline [[nodiscard]] vec2<T> Random(const int min, const int max)
			{
				return vec2<T>(T(min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))),
							   T(min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))));
			}

	} // end of namespace math
}
// end of namesapce vml
