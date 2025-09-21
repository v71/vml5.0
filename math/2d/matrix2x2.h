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

//////////////////////////////////////////////////////////////////
// 2x2 matrix class

namespace vml
{
	namespace math
	{

			template <typename T>
			class mat2x2
			{

				private:

					T Data[4];

				public:

					// --------------------------------------------------------------------
					// access operators

					__forceinline T &operator[](const size_t i) 
					{
						return Data[i];
					}

					__forceinline const T &operator[](const size_t i)  const
					{
						return Data[i];
					}

					__forceinline const T &operator()(const size_t i, const size_t j) const
					{
						return Data[i + j * 2];
					}

					// --------------------------------------------------------------
					// casting to pointer

					__forceinline operator T* () const 
					{
						return (T*)Data; 
					}
					
					__forceinline operator const T* () const 
					{
						return (const T*)Data; 
					}

					// --------------------------------------------------------------------
					// unary operators

					__forceinline const mat2x2 operator -()
					{
						return mat2x2(-Data[0], -Data[1],
									  -Data[2], -Data[3]);
					}

					__forceinline const mat2x2 operator +()
					{
						return *this;
					}

					// --------------------------------------------------------------------
					// artimetic operators

					__forceinline const mat2x2& operator +=(const  mat2x2& M)
					{
						Data[0] += M[0]; Data[1] += M[1];
						Data[2] += M[2]; Data[3] += M[3];
						return *this;
					}

					__forceinline const mat2x2& operator +=(const T s)
					{
						Data[0] += s; Data[1] += s;
						Data[2] += s; Data[3] += s;
						return *this;
					}

					__forceinline const mat2x2& operator -=(const mat2x2& M)
					{
						Data[0] -= M[0]; Data[1] -= M[1];
						Data[2] -= M[2]; Data[3] -= M[3];
						return *this;
					}

					__forceinline const mat2x2& operator -=(const T s)
					{
						Data[0] -= s; Data[1] -= s;
						Data[2] -= s; Data[3] -= s;
						return *this;
					}

					__forceinline const mat2x2& operator *=(const mat2x2& M)
					{
						T t[4] = { Data[0] * M[0] + Data[1] * M[2],
								   Data[0] * M[1] + Data[1] * M[3],
								   Data[2] * M[0] + Data[3] * M[2],
								   Data[2] * M[1] + Data[3] * M[3] };
						Data[0] = t[0];	Data[1] = t[1];
						Data[2] = t[2];	Data[3] = t[3];
						return *this;
					}

					__forceinline const mat2x2& operator *=(const T s)
					{
						Data[0] *= s; Data[1] *= s; 
						Data[2] *= s; Data[3] *= s;
						return *this;
					}

					__forceinline const mat2x2& operator /=(const mat2x2& M)
					{
						T det = M[0] * M[3] - M[1] * M[2];

						if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
							vml::os::Message::Error("Mat2x2 Division by zero");

						det = T(1) / det;

						T m00 =  M[0] * det;
						T m01 = -M[1] * det;
						T m10 = -M[2] * det;
						T m11 =  M[3] * det;

						T t[4] = { Data[0] * m11 + Data[1] * m10,
								   Data[0] * m01 + Data[1] * m00,
								   Data[2] * m11 + Data[3] * m10,
								   Data[2] * m01 + Data[3] * m00 };

						Data[0] = t[0];	Data[1] = t[1];
						Data[2] = t[2];	Data[3] = t[3];

						return *this;
					}

					__forceinline const mat2x2& operator /=(const T s)
					{
						if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
							vml::os::Message::Error("Mat2x2 : s is zero");

						float invs = T(1) / s;

						Data[0] *= invs;
						Data[1] *= invs;
						Data[2] *= invs;
						Data[3] *= invs;

						return *this;
					}

					// --------------------------------------------------------------
					// comparison operators

					__forceinline bool operator== (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) == 0;
					}

					__forceinline bool operator!= (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) != 0;
					}

					__forceinline bool operator<= (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) <= 0;
					}

					__forceinline bool operator>= (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) >= 0;
					}

					__forceinline bool operator> (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) > 0;
					}

					__forceinline bool operator< (const mat2x2& M) const
					{
						return memcmp(Data, M.Data, 4 * sizeof(T)) < 0;
					}

					// --------------------------------------------------------------------
					// query functions
					
					__forceinline const T* GetData() const
					{
						return Data; 
					}
					
					__forceinline T* GetData()
					{
						return Data;
					}
				
					// index at i,j coords

					__forceinline int Index(const int i, const int j)
					{
						return  i + j * 2; 
					}

					// --------------------------------------------------------------
					// ctor / dtor

					mat2x2()
					{
						Data[0] = T(1); Data[1] = T(0);
						Data[2] = T(0); Data[3] = T(1);
					}

					mat2x2(T m11, T m12,
						   T m21, T m22)
					{
						Data[0] = m11; Data[1] = m12;
						Data[2] = m21; Data[3] = m22;
					}

					// ---------------------------------------------------------------
					// assignment operator
					// if you assign different order matrices
					// the dest matrix will take the source's order

					mat2x2& operator= (const mat2x2 &M)
					{
						if (&M == this)
							return *this;
						Data[0] = M[0]; Data[1] = M[1];
						Data[2] = M[2]; Data[3] = M[3];
						return *this;
					}

					// ---------------------------------------------------------------
					// copy constructor
					// if you assign different order matrices
					// the dest matrix will take the source's order

					mat2x2(const mat2x2& M)
					{
						Data[0] = M[0]; Data[1] = M[1];
						Data[2] = M[2]; Data[3] = M[3];
					}

					~mat2x2()
					{
					}
					
				};
				
				/////////////////////////////////////////////////////////////////////////////////
				// friend functions
				
				template <typename T>
				__forceinline const mat2x2<T> operator *(const mat2x2<T> &R, const T s)
				{
					return mat2x2<T>(R[0] * s, R[1] * s, R[2] * s, R[3] * s);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator *(const T s,const mat2x2<T> &R)
				{
					return mat2x2<T>(R[0] * s, R[1] * s, R[2] * s, R[3] * s);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator *(const mat2x2<T> &R, const mat2x2<T> &L)
				{
					return mat2x2<T>(R[0] * L[0] + R[1] * L[2], R[0] * L[1] + R[1] * L[3],
									 R[2] * L[0] + R[3] * L[2], R[2] * L[1] + R[3] * L[3]);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator /(const mat2x2<T> &R, const T s)
				{
					if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
						vml::os::Message::Error("Mat2x2 : scalar is null");
					T invs = T(1) / T(s);
					return mat2x2<T>(R[0] * invs, R[1] * invs, R[2] * invs, R[3] * invs);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator /(const T s,const mat2x2<T> &L)
				{
					T det = L[0] * L[3] - L[1] * L[2];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						vml::os::Message::Error("Mat2x2 : determinant is null");
					det = s / det;
					return mat2x2<T>( L[3] * det, -L[1] * det,
						 			 -L[2] * det,  L[0] * det);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator /(const mat2x2<T> &R, const mat2x2<T> &L)
				{
					T det = L[0] * L[3] - L[1] * L[2];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						vml::os::Message::Error("Mat2x2 : determinant is null");
					det = T(1) / det;
					T m00 = L[0] * det , m01 = L[1] * det;
					T m10 = L[2] * det , m11 = L[3] * det;
					return mat2x2<T>(R[0] * m11 - R[1] * m10, -R[0] * m01 + R[1] * m00,
									 R[2] * m11 - R[3] * m10, -R[2] * m01 + R[3] * m00);
				}
				
				template <typename T>
				__forceinline const mat2x2<T> operator +(const mat2x2<T> &R, const mat2x2<T> &L)
				{
					return mat2x2<T>(R[0] + L[0], R[1] + L[1],
							 		 R[2] + L[2], R[3] + L[3]);
				}
			
				template <typename T>
				__forceinline const mat2x2<T> operator +(const mat2x2<T>& R, const T s)
				{
					return mat2x2<T>(R[0] + s, R[1] + s,
									 R[2] + s, R[3] + s);
				}

				template <typename T>
				__forceinline const mat2x2<T> operator +(const T s,const mat2x2<T>& R)
				{
					return mat2x2<T>(R[0] + s, R[1] + s,
									 R[2] + s, R[3] + s);
				}

				template <typename T>
				__forceinline const mat2x2<T> operator -(const mat2x2<T>& R, const mat2x2<T>& L)
				{
					return mat2x2<T>(R[0] - L[0], R[1] - L[1],
									 R[2] - L[2], R[3] - L[3]);
				}

				template <typename T>
				__forceinline const mat2x2<T> operator -(const mat2x2<T>& R, const T s)
				{
					return mat2x2<T>(R[0] - s, R[1] - s,
									 R[2] - s, R[3] - s);
				}

				template <typename T>
				__forceinline const mat2x2<T> operator -(const T s, const mat2x2<T>& R)
				{
					return mat2x2<T>(R[0] - s, R[1] - s,
									 R[2] - s, R[3] - s);
				}

				///////////////////////////////////////////////////////////////////////
				// vector operations

				// row vector by a matrix

				template <typename T>
				__forceinline const vec2<T> operator *(const vec2<T> &L, const mat2x2<T> &R)
				{
					return vec2<T>(L.x * R[0]+ L.y * R[2], L.x * R[1] + L.y * R[3]);
				}

				// column vector by a matrix

				template <typename T>
				__forceinline const vec2<T> operator *(const mat2x2<T> &R, const vec2<T> &L)
				{
					return vec2<T>(L.x * R[0] + L.y * R[2], L.x * R[1] + L.y * R[3]);
				}

				template <typename T >
				std::ostream& operator<< (std::ostream& os, const vml::math::mat2x2<T>& M)
				{
					std::string strbuffer[4];
					size_t strlength[4] = { 0,0,0,0 };

					size_t maxlenght;
					size_t lenght;

					maxlenght = 0;

					std::ostringstream ts;

					ts.str("");
					ts.clear();
					ts << M[0];
					lenght = ts.str().length();
					strbuffer[0] = ts.str();
					strlength[0] = lenght;
					if (lenght > maxlenght) maxlenght = lenght;
				
					ts.str("");
					ts.clear();
					ts << M[1];
					lenght = ts.str().length();
					strbuffer[1] = ts.str();
					strlength[1] = lenght;
					if (lenght > maxlenght) maxlenght = lenght;

					ts.str("");
					ts.clear();
					ts << M[2];
					lenght = ts.str().length();
					strbuffer[2] = ts.str();
					strlength[2] = lenght;
					if (lenght > maxlenght) maxlenght = lenght;

					ts.str("");
					ts.clear();
					ts << M[3];
					lenght = ts.str().length();
					strbuffer[3] = ts.str();
					strlength[3] = lenght;
					if (lenght > maxlenght) maxlenght = lenght;

					maxlenght += 2;
					
					os.clear();

					for (size_t j = 0; j < 2; ++j)
					{
						for (size_t i = 0; i < 2; ++i)
						{
							size_t index = i + j * 2;

							os << strbuffer[index] << std::setw(maxlenght - strlength[index]);
						}

						os << "\n";
					}

					return os;
				}

	} // end of namespace math

} // end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////
// expand matrix helpers with the newly defined matrix nxn class

namespace vml
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////////
		// predefined types

		typedef mat2x2<unsigned char>	mat2x2b;	// 8-bit  unsigned.
		typedef mat2x2<uint8_t>			mat2x2ui8;	// 8-bit  unsigned.
		typedef mat2x2<uint16_t>		mat2x2ui16;	// 16-bit unsigned.
		typedef mat2x2<uint32_t>		mat2x2ui32;	// 32-bit unsigned.
		typedef mat2x2<uint64_t>		mat2x2ui64;	// 64-bit unsigned.
		typedef mat2x2<int8_t>			mat2x2i8;	// 8-bit  signed.
		typedef mat2x2<int16_t>			mat2x2i16;	// 16-bit signed.
		typedef mat2x2<int32_t>			mat2x2i32;	// 32-bit signed.
		typedef mat2x2<int64_t>			mat2x2i64;	// 64-bit signed.
		typedef mat2x2<float_t>			mat2x2f;	// 32-bit float
		typedef mat2x2<double_t>		mat2x2d;	// 64-bit float

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// matrix2x2 utilities

namespace vml
{
	namespace math
	{
		namespace mat2x2utils
		{
			
			// --------------------------------------------------------
			// metrics functions

			template <typename T>
			__forceinline T Determinant(const mat2x2<T>& R)
			{
				return R[0] * R[3] - R[1] * R[2];
			}

			// matrix norm
			template <typename T>
			__forceinline T Norm(const mat2x2<T>& R)
			{
				return sqrtf(R[0] * R[0] + R[1] * R[1] + R[2] * R[2] + R[3] * R[3]);
			}

			// computes the trace for anxn matrix
			// the sum of all elelemnts of the main diagonal

			template <typename T>
			__forceinline T Trace(const mat2x2<T>& R)
			{
				return R[0] + R[3];
			}

			// computes the rank for a 2x2 matrix

			template <typename T>
			__forceinline int Rank(const mat2x2<T>& R)
			{
				T det = R[0] * R[3] - R[1] * R[2];
				if (det < -vml::math::EPSILON || det > vml::math::EPSILON)	return 2;
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)	return 1;
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)	return 1;
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)	return 1;
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)	return 1;
				return 0;
			}

			// compute normalized distance from maximum eleement

			template <typename T>
			__forceinline mat2x2<T> NormalizedDistance(const mat2x2<T>& R)
			{
				T invmax = R[0];

				if (R[1] > invmax) invmax = R[1];
				if (R[2] > invmax) invmax = R[2];
				if (R[3] > invmax) invmax = R[3];

				if (invmax > -vml::math::EPSILON && invmax < vml::math::EPSILON)
					vml::os::Message::Error("Mat2x2 : max is null");

				invmax = T(1) / invmax;

				return mat2x2<T>(R[0] * invmax, R[1] * invmax, R[2] * invmax, R[3] * invmax);

			}

			// --------------------------------------------------------
			// invert a nxn matrix

			template <typename T>
			__forceinline mat2x2<T> Invert(const mat2x2<T> &R)
			{
				T det = R[0] * R[3] - R[1] * R[2];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Message::Error("Mat2x2 : matrix is singular");
				det = T(1) / det;
				return mat2x2<T>(R[3] * det, -R[1] * det, -R[2] * det, R[0] * det);
			}

			// --------------------------------------------------------
			// transposes a nxn matrix

			template <typename T>
			__forceinline mat2x2<T> Transpose(const mat2x2<T> &R)
			{
				return mat2x2<T>(R[0],R[2],R[1],R[3]);
			}
			
			// --------------------------------------------------------
			// zero a nxn matrix

			template <typename T>
			__forceinline mat2x2<T> Zero()
			{
				return mat2x2<T>(T(0), T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// matrix identity

			template <typename T>
			__forceinline mat2x2<T> Identity()
			{
				return mat2x2<T>(T(1),T(0),T(0),T(1));
			}
			
			// --------------------------------------------------------
			// initialise matrix with random values

			template <typename T>
			__forceinline mat2x2<T> Random(const int min, const int max)
			{
				return mat2x2<T>(T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))));
			} 
	
			// --------------------------------------------------------
			// initialise matrix with diagonal values

			template <typename T>
			__forceinline mat2x2<T> Diagonal(const T value)
			{
				return mat2x2<T>(T(value), T(0), T(0), T(value));
			}
			
			// --------------------------------------------------------
			// initialise matrix with a upper triangular values

			template <typename T>
			__forceinline mat2x2<T> UpperTriangular(const T value)
			{
				return mat2x2<T>(T(0), T(value), T(0), T(0));
			}

			// --------------------------------------------------------
			// initialise matrix with random values

			template <typename T>
			__forceinline mat2x2<T> UpperTriangular(const int min, const int max)
			{
				return mat2x2<T>(T(0),T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),T(0),T(0));
			}

			// --------------------------------------------------------
			// initialise matrix with a lower triangular values

			template <typename T>
			__forceinline mat2x2<T> LowerTriangular(const T value)
			{
				return mat2x2<T>(T(0), T(0), T(value), T(0));
			}

			// --------------------------------------------------------
			// initialise matrix with random values

			template <typename T>
			__forceinline mat2x2<T> LowerTriangular(const int min, const int max)
			{
				return mat2x2<T>(T(0), T(0),T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))), T(0));
			}

			// --------------------------------------------------------
			// compute the absolute value of a matriz

			template <typename T>
			__forceinline mat2x2<T> Abs(const mat2x2<T> &R)
			{
				return mat2x2<T>(fabs(R[0]), fabs(R[1]), fabs(R[2]), fabs(R[3]));
			}

			// --------------------------------------------------------
			// normalise matrix, all values forced into the [0..1] range

			template <typename T>
			__forceinline mat2x2<T> Flat(const mat2x2<T> &R)
			{
				// flat matrix value in the the [0..1] range

				T min = T(R[0]);

				if (R[1] < min)min = R[1];
				if (R[2] < min)min = R[2];
				if (R[3] < min)min = R[3];

				T max = T(R[0]);

				if (R[1] > max)max = R[1];
				if (R[2] > max)max = R[2];
				if (R[3] > max)max = R[3];

				T invdenum = max - min;

				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Message::Error("Mat2x2 : max is null");

				invdenum = T(1) / invdenum;

				return mat2x2<T>( (R[0] - min) * invdenum, (R[1] - min) * invdenum, (R[2] - min) * invdenum,(R[3] - min) * invdenum);

			}
	
			// --------------------------------------------------------
			// get cofactors of a given nxn matrix

			template <typename T>
			__forceinline T CoFactor(const mat2x2<T> &R, const size_t i, const size_t j,const bool evalsign=true)
			{
				// size_t cannot be negative

				if ( i>1 )
					vml::os::Message::Error("Mat2x2 : index i out of range");
				if ( j>1)
					vml::os::Message::Error("Mat2x2 : index i out of range");

				if (i == 0 && j == 0 &&  evalsign) return  R[3];
				if (i == 1 && j == 0 &&  evalsign) return -R[2];
				if (i == 0 && j == 1 &&  evalsign) return -R[1];
				if (i == 1 && j == 1 &&  evalsign) return  R[0];
				if (i == 0 && j == 0 && !evalsign) return  R[3];
				if (i == 1 && j == 0 && !evalsign) return  R[2];
				if (i == 0 && j == 1 && !evalsign) return  R[1];
				if (i == 1 && j == 1 && !evalsign) return  R[0];
				return T(0);
			}

			// --------------------------------------------------------
			// get cofactors of a given nxn matrix

			template <typename T>
			__forceinline std::vector<T> CoFactors(const mat2x2<T>& R, const bool evalsign = true)
			{
				std::vector <T> cofactors;
				cofactors.emplace_back(CoFactor(R, 0, 0, evalsign));
				cofactors.emplace_back(CoFactor(R, 1, 0, evalsign));
				cofactors.emplace_back(CoFactor(R, 0, 1, evalsign));
				cofactors.emplace_back(CoFactor(R, 1, 1, evalsign));
				return cofactors;
			}

			// --------------------------------------------------------
			// calculate the condition number
			// which is the norm multiplied
			// byt the norm of the matrix
			// inverse

			template <typename T>
			__forceinline T ConditionNumber(const mat2x2<T>& R)
			{
				T det = R[0] * R[3] - R[1] * R[2];

				// cannot inRert a singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Message::Error("Mat2x2 : determinant is null");

				det = T(1) / det;

				return (R[0] * R[0] + R[1] * R[1] + R[2] * R[2] + R[3] * R[3]) * sqrtf(det);
			}

			// --------------------------------------------------------
			// get cofactors of a given nxn matrix

			template <typename T>
			__forceinline T Adjoint(const mat2x2<T> &R, const size_t i, const size_t j)
			{
				if (i == 0 && j == 0) return  R[0];
				if (i == 1 && j == 0) return -R[1];
				if (i == 0 && j == 1) return -R[2];
				if (i == 1 && j == 1) return  R[3];
				// we never get here but vs complains about return path
				return T(0);
			}

			// --------------------------------------------------------
			// get cofactors of a given nxn matrix

			template <typename T>
			__forceinline std::vector<T> Adjoints(const mat2x2<T> &R)
			{
				std::vector <T> cofactors;
				cofactors.emplace_back(Adjoint(R,1, 1));
				cofactors.emplace_back(Adjoint(R,0, 1));
				cofactors.emplace_back(Adjoint(R,1, 0));
				cofactors.emplace_back(Adjoint(R,0, 0));
				return cofactors;
			}
			
			// --------------------------------------------------------
			// create a submatrix given an nxn matrix

			template <typename T>
			__forceinline T SubMatrix(const mat2x2<T> &R, const size_t i, const size_t j)
			{
				return CoFactor(R, i, j, false);
			}

			// --------------------------------------------------------
			// create a distance matrix given an 2x2 matrix

			template <typename T>
			__forceinline T DistanceFromMatrix(const mat2x2<T> &R, const mat2x2<T>& M)
			{
				T t[4] = { R[0] - M[0],R[1] - M[1],R[2] - M[2],R[3] - M[3] };
				return sqrtf(t[0] * t[0] + t[1] * t[1] + t[2] * t[2] + t[3] * t[3]);
			}

			// --------------------------------------------------------
			// create a normalized distance matrix given an 2x2 matrix

			template <typename T>
			__forceinline T NormalizedDistanceFromMatrix(const mat2x2<T> &R, const mat2x2<T>& M)
			{
				T t[4] = { R[0] - M[0],R[1] - M[1],R[2] - M[2],R[3] - M[3] };
				T max = t[0];
				if (t[1] > max)max = t[1];
				if (t[2] > max)max = t[2];
				if (t[3] > max)max = t[3];
				return sqrtf(t[0] * t[0] + t[1] * t[1] + t[2] * t[2] + t[3] * t[3])/max;
			}

			// --------------------------------------------------------
			// create a symmetric matrix given an 2x2 matrix

			template <typename T>
			__forceinline mat2x2<T> MakeSimmetric(const vml::math::mat2x2<T> &R)
			{
				return mat2x2<T>(R[0] + R[0],R[1] + R[2], R[2] + R[1],R[3] + R[3] );
			}

			// --------------------------------------------------------
			// normalize matrix

			template <typename T>
			__forceinline mat2x2<T> Normalize(const vml::math::mat2x2<T>& R)
			{
				T det= R[0] * R[3] - R[1] * R[2];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Message::Error("Mat2x2 : max is null");
				det = T(1) / det;
				return mat2x2<T>(R[0]*det, R[1]*det, R[2]*det, R[3]*det);
			}

			template <typename T>
			void GetEigenValues(const vml::math::mat2x2<T>& m, T& l1, T& l2, T& c1, T& c2)
			{
				T b = -m[0] - m[3];
				T c = m[0] * m[3] - m[1] * m[2];
				T delta = b * b - 4 * c;

				if (delta > 0)
				{
					delta = sqrtf(delta);
					l1 = (-b + delta) * 0.5f;
					l2 = (-b - delta) * 0.5f;
					c1 = 0.0f;
					c2 = 0.0f;
				}
				else
				{
					delta = sqrtf(-delta);
					T l = -b * 0.5f;
					l1 = l;
					l2 = l;
					c1 = delta;
					c2 = -delta;
				}
			}

			// --------------------------------------------------------
			// extracting colums as vector

			template <typename T>
			__forceinline vec2<T> GetColumn(const vml::math::mat2x2<T> &R,const int i)
			{
				if (i > 1)
					vml::os::Message::Error("Mat2x2 : index i out of range");
				if (i == 0) return vec2<T>(R[0], R[2]);
				if (i == 1) return vec2<T>(R[1], R[3]);
				return vec2<T>(T(0), T(0));
			}

			// --------------------------------------------------------
			// extracting row as vector

			template <typename T>
			__forceinline vec2<T> GetRow(const vml::math::mat2x2<T> &R, const int i)
			{
				if (i > 1)
					vml::os::Message::Error("Mat2x2 : index i out of range");
				if (i == 0) return vec2<T>(R[0], R[1]);
				if (i == 1) return vec2<T>(R[2], R[3]);
				return vec2<T>(T(0), T(0));
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetMin(const vml::math::mat2x2<T> &R)
			{
				T min = R[0];
				if (R[1] < min)min = R[1];
				if (R[2] < min)min = R[2];
				if (R[3] < min)min = R[3];
				return min;
			}

			template <typename T>
			__forceinline T GetAbsMin(const vml::math::mat2x2<T>& R)
			{
				T min = fabs(R[0]);
				if (fabs(R[1]) < min)min = fabs(R[1]);
				if (fabs(R[2]) < min)min = fabs(R[2]);
				if (fabs(R[3]) < min)min = fabs(R[3]);
				return min;
			}

			// --------------------------------------------------------
			// compute the maximum value in the matrix

			template <typename T>
			__forceinline T GetMax(const vml::math::mat2x2<T> &R)
			{
				T max = R[0];
				if (R[1] > max)max = R[1];
				if (R[2] > max)max = R[2];
				if (R[3] > max)max = R[3];
				return max;
			}

			template <typename T>
			__forceinline T GetAbsMax(const vml::math::mat2x2<T>& R)
			{
				T max = fabs(R[0]);
				if (fabs(R[1]) > max)max = fabs(R[1]);
				if (fabs(R[2]) > max)max = fabs(R[2]);
				if (fabs(R[3]) > max)max = fabs(R[3]);
				return max;
			}

			// --------------------------------------------------------
			// compute the floor of a given matrix

			template <typename T>
			__forceinline mat2x2<T> Floor(const vml::math::mat2x2<T> &R)
			{
				return mat2x2<T>(floor(R[0]),floor(R[1]),floor(R[2]),floor(R[3]));
			}

			// --------------------------------------------------------
			// compute the ceil of a given matrix

			template <typename T>
			__forceinline mat2x2<T> Ceil(const vml::math::mat2x2<T> &R)
			{
				return mat2x2<T>(ceil(R[0]),ceil(R[1]),ceil(R[2]),ceil(R[3]));
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat2x2<T> Saturate(const vml::math::mat2x2<T> &R)
			{
				T P[4]={R[0],R[1],R[2],R[3]};
				if (P[0] > T(1)) P[0] = T(1);
				if (P[1] > T(1)) P[1] = T(1);
				if (P[2] > T(1)) P[2] = T(1);
				if (P[3] > T(1)) P[3] = T(1);
				if (P[0] < T(0)) P[0] = T(0);
				if (P[1] < T(0)) P[1] = T(0);
				if (P[2] < T(0)) P[2] = T(0);
				if (P[3] < T(0)) P[3] = T(0);
				return mat2x2<T>(P[0],P[1],P[2],P[3]);
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat2x2<T> SaturateToZero(const vml::math::mat2x2<T> &R)
			{
				T P[4] = { R[0],R[1],R[2],R[3] };
				if (P[0] < T(0)) P[0] = T(0);
				if (P[1] < T(0)) P[1] = T(0);
				if (P[2] < T(0)) P[2] = T(0);
				if (P[3] < T(0)) P[3] = T(0);
				return mat2x2<T>(P[0], P[1], P[2], P[3]);
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat2x2<T> SaturateToOne(const vml::math::mat2x2<T> &R)
			{
				T P[4] = { R[0],R[1],R[2],R[3] };
				if (P[0] > T(1)) P[0] = T(1);
				if (P[1] > T(1)) P[1] = T(1);
				if (P[2] > T(1)) P[2] = T(1);
				if (P[3] > T(1)) P[3] = T(1);
				return mat2x2<T>(P[0], P[1], P[2], P[3]);
			}

			// --------------------------------------------------------
			// compute the rotation matrix

			template <typename T>
			__forceinline mat2x2<T> Rot(vml::math::mat2x2<T> &R,const T yaw)
			{
				yaw *= vml::math::DEGTORAD;
				T sinx = sin(yaw);
				T cosx = cos(yaw);
				return mat2x2<T>(cosx,-sinx,sinx,cosx);
			}

			// --------------------------------------------------------
			// return bool if two matrices are equal

			template <typename T>
			__forceinline bool IsEqual(const vml::math::mat2x2<T> &R, const vml::math::mat2x2<T> &P)
			{
				return fabs(R[0] - P[0]) < vml::math::EPSILON &&
					   fabs(R[1] - P[1]) < vml::math::EPSILON &&
					   fabs(R[2] - P[2]) < vml::math::EPSILON &&
					   fabs(R[3] - P[3]) < vml::math::EPSILON;
			}


			// --------------------------------------------------------
			// return bool if matrix is singular

			template <typename T>
			__forceinline bool IsSingular(const vml::math::mat2x2<T> &R)
			{
				T det = R[0] * R[3] - R[1] * R[2];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is upper triangulat

			template <typename T>
			__forceinline bool IsUpperTriangular(const vml::math::mat2x2<T> &R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
					return true;
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is lower triangulat

			template <typename T>
			__forceinline bool IsLowerTriangular(const vml::math::mat2x2<T> &R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
					return true;
				if (R[0] > -vml::math::EPSILON && R[0] < vml::math::EPSILON)
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is diagonal

			template <typename T>
			__forceinline bool IsDiagonal(const vml::math::mat2x2<T> &R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
					return true;
				return false;
			}
			
			// --------------------------------------------------------
			// return bool if matrix is identity

			template <typename T>
			__forceinline bool IsIdentity(const vml::math::mat2x2<T> &R)
			{
				if (R[0] > 1 - vml::math::EPSILON && R[0] < 1 + vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] > 1 - vml::math::EPSILON && R[2] < 1 + vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is zero

			template <typename T>
			__forceinline bool IsZero(const vml::math::mat2x2<T> &R)
			{
				if ((R[0] > -vml::math::EPSILON && R[0] < vml::math::EPSILON) &&
					(R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON) &&
					(R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSimmetric(const vml::math::mat2x2<T> &R)
			{
				T det = R[1] - R[2];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSkewSimmetric(const vml::math::mat2x2<T> &R)
			{
				T det = R[1] + R[2];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					return true;
				return false;
			}
			
		
		}

	} // end of namespace math

} // end of namesapce vml
