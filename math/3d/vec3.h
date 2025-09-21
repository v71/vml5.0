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
		// vec3 class

		template <typename T>
		class vec3 
		{
				
			public:

				T x, y, z;
	
				// --------------------------------------------------------------------
				// artimetic operators

				__forceinline vec3<T>& operator*=(const T s)
				{
					x *= s;
					y *= s;
					z *= s;
					return *this;
				}

				__forceinline vec3<T>& operator/=(const T s)
				{
					T invs;
					if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
						invs = vml::math::EPSILON;
					else
						invs = T(1) / T(s);
					x *= invs;
					y *= invs;
					z *= invs;
					return *this;
				}

				__forceinline vec3<T>& operator+=(const vec3<T>& v)
				{
					x += v.x;
					y += v.y;
					z += z.y;
					return *this;
				}

				__forceinline vec3<T>& operator+=(const T s)
				{
					x += s;
					y += s;
					z += s;
					return *this;
				}

				__forceinline vec3<T>& operator-=(const vec3<T>& v)
				{
					x -= v.x;
					y -= v.y;
					z -= v.z;
					return *this;
				}

				__forceinline vec3<T>& operator-=(const T s)
				{
					x -= s;
					y -= s;
					z -= s;
					return *this;
				}

				__forceinline vec3<T>& operator+()
				{
					return *this;
				}

				__forceinline vec3<T> operator-()
				{
					return vec3<T>(-x, -y,-z);
				}

				__forceinline vec3<T>& operator+() const
				{
					return *this;
				}

				__forceinline vec3<T> operator-() const
				{
					return vec3<T>(-x, -y,-z);
				}

				// --------------------------------------------------------------------
				// comparison operators

				__forceinline bool operator== (const vec3<T>& v) const
				{
					return fabs(v.x - x) < vml::math::EPSILON && fabs(v.y - y) < vml::math::EPSILON && fabs(v.z - z) < vml::math::EPSILON;
				}

				__forceinline bool operator!= (const vec3<T>& v) const
				{
					return fabs(v.x - x) > vml::math::EPSILON || fabs(v.y - y) > vml::math::EPSILON || fabs(v.z - z) > vml::math::EPSILON;
				}

				// --------------------------------------------------------------------
				// ctor / dtor

				vec3() 
				{ 
					x = T(0);
					y = T(0);
					z = T(0);
				}

				vec3(const T a, const T b, const T c)
				{
					x = a;
					y = b;
					z = c;
				}

				// ---------------------------------------------------------------
				// assignment operator

				vec3& operator= (const vec3<T>& v)
				{
					if (&v == this)
						return *this;
					x = v.x;
					y = v.y;
					z = v.z;
					return *this;
				}

				// ---------------------------------------------------------------
				// copy constructor

				vec3(const vec3<T>& v)
				{
					x = v.x;
					y = v.y;
					z = v.z;
				}

				~vec3()
				{}

		};
			
		/////////////////////////////////////////////////////////////////////////////////
		// friend functions
			
		template <typename T>
		__forceinline vec3<T> operator+(const vec3<T> &v, const vec3<T> &w)
		{
			return vec3<T>(v.x + w.x, v.y + w.y, v.z + w.z);
		}
			
		template <typename T>
		__forceinline vec3<T> operator-(const vec3<T>& v, const vec3<T>& w)
		{
			return vec3<T>(v.x - w.x, v.y - w.y, v.z - w.z );
		}

		template <typename T>
		__forceinline vec3<T> operator+(const vec3<T>& v, const T s)
		{
			return vec3<T>(v.x + s, v.y + s, v.z + s);
		}

		template <typename T>
		__forceinline vec3<T> operator-(const vec3<T>& v, const T s)
		{
			return vec3<T>(v.x - s, v.y - s, v.z - s);
		}

		template <typename T>
		__forceinline vec3<T> operator*(const vec3<T>& v, const T s)
		{
			return vec3<T>(v.x * s, v.y * s, v.z * s);
		}

		template <typename T>
		__forceinline vec3<T> operator*(const T s, const vec3<T>& v)
		{
			return vec3<T>(s * v.x, s * v.y, s * v.z);
		}

		template <typename T>
		__forceinline vec3<T> operator/(const vec3<T>& v, const T s)
		{
			T invs;
			if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
				invs = vml::math::EPSILON;
			else
				invs = T(1) / s;
			return vec3<T>(v.x * invs, v.y * invs, v.z * invs);
		}

		template <typename T >
		std::ostream& operator<< (std::ostream& output, const vml::math::vec3<T>& v)
		{
			std::ostringstream os;
			os << " [ " << v.x << " , " << v.y << " , " << v.z << " ]";
			return os.str();
		}

		///////////////////////////////////////////////////////////////////////////////////
		// predefined types

		typedef vec3<unsigned char>		vec3b;		// 8-bit  unsigned.
		typedef vec3<uint8_t>			vec3ui8;	// 8-bit  unsigned.
		typedef vec3<uint16_t>			vec3ui16;	// 16-bit unsigned.
		typedef vec3<uint32_t>			vec3ui32;	// 32-bit unsigned.
		typedef vec3<uint64_t>			vec3ui64;	// 64-bit unsigned.
		typedef vec3<int8_t>			vec3i8;		// 8-bit  signed.
		typedef vec3<int16_t>			vec3i16;	// 16-bit signed.
		typedef vec3<int32_t>			vec3i32;	// 32-bit signed.
		typedef vec3<int64_t>			vec3i64;	// 64-bit signed.
		typedef vec3<float_t>			vec3f;		// 32-bit float
		typedef vec3<double_t>			vec3d;		// 64-bit float

}	// end of namespace math

}	// end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////
// expand vector helpers with the newly defined vector n class

namespace vml
{
	namespace math
	{

		// --------------------------------------------------------------------
		// metrics functions

		template <typename T>
		__forceinline [[nodiscard]] T SquaredLength(const vec3<T>& v)
		{
			return v.x * v.x + v.y * v.y + v.z * v.z;
		}

		template <typename T>
		__forceinline [[nodiscard]] T Length(const vec3<T>& v)
		{
			return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		}

		// --------------------------------------------------------
		// return the dot product between two vectors

		template <typename T>
		__forceinline [[nodiscard]] T Dot(const vec3<T>& v, const vec3<T>& w)
		{
			return (v.x * w.x + v.y * w.y + v.z * w.z);
		}
		/*
		// --------------------------------------------------------
		// reciprocal of a vector
		
		template <typename T>
		__forceinline vec3<T> Reciprocal(const vec3<T>& v)
		{
			if (v.x > -vml::math::EPSILON && v.x < vml::math::EPSILON) v.x = vml::math::EPSILON;
			if (v.y > -vml::math::EPSILON && v.y < vml::math::EPSILON) v.y = vml::math::EPSILON;
			if (v.z > -vml::math::EPSILON && v.z < vml::math::EPSILON) v.z = vml::math::EPSILON;
			return vec3<T>(T(1) / v.x, T(1) / v.y, T(1) / v.z);
		}
		*/
		// --------------------------------------------------------
		// normalize vector

		template <typename T>
		__forceinline [[nodiscard]] vec3<T> Normalize(const vec3<T>& v)
		{
			T d = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
			if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
				d = vml::math::EPSILON;
			d = T(1) / d;
			return vec3<T>(v.x * d, v.y * d, v.z * d);
		}
		
		// --------------------------------------------------------
		// cross product

		template <typename T>
		__forceinline [[nodiscard]] vec3<T> Cross(const vec3<T>& v, const vec3<T>& w)
		{
			return vec3<T>(v.y * w.z - v.z * w.y,
						   v.z * w.x - v.x * w.z,
						   v.x * w.y - v.y * w.x);
		}

		// --------------------------------------------------------
		// returns angles computed from spherical coordinates 

		template <typename T>
		__forceinline [[nodiscard]] vec3<T> Ortho(const vec3<T>& v,const float eps= vml::math::EPSILON)
		{
			vml::math::vec3f ref(1, 0, 0);
			vml::math::vec3f n = vml::math::Cross(ref, v);
			if (std::fabs(n.x) < eps && std::fabs(n.y) < eps && std::fabs(n.z) < eps)
			{
				ref = vml::math::vec3f(1, 0, 0);
				n = vml::math::Cross(ref, v);
				if (std::fabs(n.x) < eps && std::fabs(n.y) < eps && std::fabs(n.z) < eps)
				{
					ref = vml::math::vec3f(0, 0, 1);
					n = vml::math::Cross(ref, v);
				}
			}
			float denum = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
			if (denum > -eps && denum < eps)
				denum = eps;
			denum = 1.0f / denum;
			n.x *= denum;
			n.y *= denum;
			n.z *= denum;
			return n;
		}

		// --------------------------------------------------------
		// zeros a vector

		template <typename T>
		__forceinline vec3<T> Zero()
		{
			return vec3<T>(T(0), T(0), T(0));
		}

		// --------------------------------------------------------
		// returns a random vector in the min - max range

		template <typename T>
		__forceinline vec3<T> Random(const int min, const int max)
		{
			return (vec3<T>(min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
							min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
							min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))));
		}
		/*
		// --------------------------------------------------------
		// returns the distance from two vectors

		template <typename T>
		__forceinline T DistanceFrom(const vec3<T>& v, const vec3<T>& w)
		{
			T dx = v.x - w.x;
			T dy = v.y - w.y;
			T dz = v.z - w.z;
			return sqrtf(dx * dx + dy * dy + dz * dz);
		}

		// --------------------------------------------------------
		// return the trace of a given vector

		template <typename T>
		__forceinline T Trace(const vec3<T>& v)
		{
			return v.x + v.y + v.z;
		}

		// --------------------------------------------------------
		// transpose a vector

		template <typename T>
		__forceinline vec3<T> Transpose(const vec3<T>& v)
		{
			return vec3<T>(v.z, v.y, v.x);
		}

		// --------------------------------------------------------------------
		// check if vector is nearly zero

		// --------------------------------------------------------
		// tests if a vector is zero whitin a given epsilon range

		template <typename T>
		__forceinline bool IsZero(const vec3<T>& v)
		{
			if ((v.x > -vml::math::EPSILON && v.x < vml::math::EPSILON) &&
				(v.y > -vml::math::EPSILON && v.y < vml::math::EPSILON) &&
				(v.z > -vml::math::EPSILON && v.z < vml::math::EPSILON))
				return true;
			return false;
		}

		// --------------------------------------------------------------------
		// checks if vector is nearly identticaly to (1,1)
			
		template <typename T>
		__forceinline bool IsIdentity(const vec3<T>& v)
		{
			if ((v.x > T(1) - vml::math::EPSILON && v.x < T(1) + vml::math::EPSILON) &&
				(v.y > T(1) - vml::math::EPSILON && v.y < T(1) + vml::math::EPSILON) &&
				(v.z > T(1) - vml::math::EPSILON && v.z < T(1) + vml::math::EPSILON))
				return true;
			return false;
		}
		// --------------------------------------------------------
		// checks if two vectors are the same whithin an epsilon range

		template <typename T>
		__forceinline bool IsEqual(const vec3<T>& v, const vec3<T>& w)
		{
			return fabs(v.x - w.x) < vml::math::EPSILON &&
					fabs(v.y - w.y) < vml::math::EPSILON &&
					fabs(v.z - w.z) < vml::math::EPSILON;
		}

		// --------------------------------------------------------
		// checks if two vectors are not the same whithin an epsilon range

		template <typename T>
		__forceinline bool IsNotEqual(const vec3<T>& v, const vec3<T>& w)
		{
			return fabs(v.x - w.x) > vml::math::EPSILON ||
				fabs(v.y - w.y) > vml::math::EPSILON ||
				fabs(v.z - w.z) > vml::math::EPSILON;
		}

		// --------------------------------------------------------
		// gets the maximum value component of a given vector

		template <typename T>
		__forceinline T GetMax(const vec3<T>& v)
		{
			T max = v.x;
			if (v.y > max) max = v.y;
			if (v.z > max) max = v.z;
			return max;
		}

		// --------------------------------------------------------
		// gets the absolute maximum value component of a given vector

		template <typename T>
		__forceinline T GetAbsMax(const vec3<T>& v)
		{
			T max = fabs(v.x);
			if (fabs(v.y) > max) max = fabs(v.y);
			if (fabs(v.z) > max) max = fabs(v.z);
			return max;
		}

		// --------------------------------------------------------
		// gets the minimum value component of a given vector

		template <typename T>
		__forceinline T GetMin(const vec3<T>& v)
		{
			T min = v.x;
			if (v.y < min) min = v.y;
			if (v.z < min) min = v.z;
			return min;
		}

		// --------------------------------------------------------
		// gets the absolute maximum value component of a given vector

		template <typename T>
		__forceinline T GetAbsMin(const vec3<T>& v)
		{
			T max = fabs(v.x);
			if (fabs(v.y) < max) max = fabs(v.y);
			if (fabs(v.z) < max) max = fabs(v.z);
			return max;
		}

		// --------------------------------------------------------
		// vector flatting

		template <typename T>
		__forceinline vec3<T> Flat(const vec3<T>& v)
		{
			T min = T(v.x);
			if (v.y < min)min = v.y;
			if (v.z < min)min = v.z;
			T max = T(v.x);
			if (v.y > max)max = v.y;
			if (v.z > max)max = v.z;
			T invdenum = max - min;
			if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
				invdenum = vml::math::EPSILON;
			invdenum = T(1) / invdenum;
			return vec3<T>((v.x - min) * invdenum, (v.y - min) * invdenum, (v.z - min) * invdenum);
		}

		// --------------------------------------------------------
		// return the angle between two vectors

		template <typename T>
		__forceinline T AngleBetween(const vec3<T>& v, const vec3<T>& w)
		{
			T num = v.x * w.x + v.y * w.y + v.z * w.z;
			T r = num;
			T denum = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
			if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
				denum = vml::math::EPSILON;
			r /= denum;
			denum = sqrtf(w.x * w.x + w.y * w.y + w.z * w.z);
			if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
				denum = vml::math::EPSILON;
			r /= denum;
			if (r < -T(1)) r = -T(1);
			if (r > T(1)) r = T(1);
			return acos(r);
		}

		// --------------------------------------------------------
		// returns angles computed from spherical coordinates 

		template <typename T>
		__forceinline vml::math::vec2<T> Angle(const vec3<T>& v)
		{
			T radius = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
			if (radius > -vml::math::EPSILON && radius < vml::math::EPSILON)
				radius = vml::math::EPSILON;
			return vec2<T>(atan2(v.y, v.x), acos(v.z / radius));
		}

		

		*/	
	} // end of namespace math

} // end of namesapce vml


