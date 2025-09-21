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
// numerics 

namespace vml
{
	namespace math
	{
		namespace numerics
		{

			////////////////////////////////////////////////////////////////
			// checks if a number has a decimal part

			static const bool HasDecPart(double val)
			{
				double temp = static_cast<double>(val); // type upcast
				if (floor(temp) != temp)
					return true;
				return false;
			}

			/////////////////////////////////////////////////////////////////////
			// Returns 1 if odd else 0
			// uses type downcast

			template < typename T >
			static const bool IsOdd(const T val)
			{
				int temp = static_cast<int>(val);	// type downcast
				if ((temp % 2) == 1)
					return true;
				return false;
			}

			///////////////////////////////////////////////////////////////////
			// Returns 1 if even else 0
			// uses type downcast

			template < typename T >
			static const bool IsEven(const T val)
			{
				int temp = static_cast<int>(val);	// type downcast
				if ((temp % 2) == 1)
					return false;
				return true;
			}

			///////////////////////////////////////////////////////////////////
			// gets sign of a number

			template <typename T>
			static const int Sign(const T val)
			{
				return (T(0) < val) - (val < T(0));
			}

			//////////////////////////////////////////////////////////////////
			// gets absolute value

			template <typename T>
			static T Abs(T x)
			{
				return (x >= 0 ? x : -x);
			}

			//////////////////////////////////////////////////////////////////
			// NOTE(Nabil/htmlboss): The STL has an overloaded abs()
			
			inline bool EpsCompare(const float a, const float b, const float epsilon)
			{
				const float diff = abs(a - b);
				// NOTE(Joey): originally we want to take the absolute max of either
				// a or b or 1.0 (s.t. we don't scale epsilon down) to make sure we 
				// scale epsilon by the biggest value; however, max() can be 
				// expensive so we approximate by adding all 3 values together.
				const float compare = epsilon * (abs(a) + abs(b) + 1.0f);
				return diff <= compare;
			}

			//////////////////////////////////////////////////////////////////
			// NOTE(Joey): the above comparrison is more general, but can still be too
			// slow in performance critical code. Here we use a more efficient absolute
			// epsilon check which is fine if one takes the relative magnitude of the
			// inputs into account.

			inline bool EpsAbsCompare(const float a, const float b, const float epsilon)
			{
				return abs(a - b) <= epsilon;
			}

			//////////////////////////////////////////////////////////////////
			// simple squaring ad cubing of numbers

			template <typename T>
			static T Square(T x)
			{
				return x * x;
			}

			template <typename T>
			static T Cube(T x)
			{
				return x * x * x;
			}

			///////////////////////////////////////////////////////////////////
			// Checks if the value is zero.In the case of floating point values,
			// this method will check if the value is precisely zero or within an
			// acceptable distance(epsilon) of zero.
			// param  a  Value to check
			// return True if the value is within error tolerance of zero

			template< typename T >
			static bool IsZero(T x)
			{
				return x >-vml::math::EPSILON && x < vml::math::EPSILON;
			}

			//////////////////////////////////////////////////////////////////////////
			// get fractional parto f a float or double number

			template< typename T >
			static float Fraction(T a)
			{
				return a - std::floor((float)a);
			}

			//////////////////////////////////////////////////////////////////////////
			// wrap a number in a min and max range

			template < typename T >
			static T Wrap(T value, T min, T max)
			{
				max -= min;
				value = std::fmod(value - min, max);
				if (value < T(0)) value += max;
				return min + value;
			}

			//////////////////////////////////////////////////////////////////////////
			// find cloeset power of two to a number

			static uint32 NextPowerOfTwo(uint32 n)
			{
				const int mantissamask = (1 << 23) - 1;
				(*(float*)&n) = (float)n;
				n = n + mantissamask & ~mantissamask;
				n = (unsigned)(*(float*)&n);
				return n;
			}

			///////////////////////////////////////////////////////////////////
			// Allows you to set your FPU to single precision mode and back. 
			// This especially speeds up divisions and square roots. 
			// Be careful with this instructions, some of the
			// optimized  functions won't work in single 
			// precision mode

			///////////////////////////////////////////////////////////////////
			// set fpu precision to high

			static int SetFpuHighPrec()
			{
				unsigned int control_word;
				return _controlfp_s(&control_word, _PC_24, MCW_PC);
			}

			///////////////////////////////////////////////////////////////////
			// set fpu precision to low

			static int SetFpuLowPrec()
			{
				unsigned int control_word;
				return _controlfp_s(&control_word, _CW_DEFAULT, MCW_PC);
			}

			///////////////////////////////////////////////////////////////////
			// round number to 8 bytes

			static int RoundTo8Bytes(const int x)
			{
				return ((x + 7) & (-8));
			}

			///////////////////////////////////////////////////////////////////
			// round number to 16 bytes

			static int RoundTo16Bytes(const int x)
			{
				return ((x + 15) & (-16));
			}

			///////////////////////////////////////////////////////////////////
			// round number to 32 bytes

			static int RoundTo32Bytes(const int x)
			{
				return ((x + 31) & (-32));
			}

		} // end of conversion namespace

	} // end of math namespace

} // end of vml namespace
