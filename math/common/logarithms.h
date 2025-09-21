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
// powers, logaritmics

namespace vml
{
	namespace math
	{
		namespace logs
		{

			/////////////////////////////////////////////////////////////////////
			// log 2 functions
			// this works only for 32 bits

			static unsigned int Ilog2(unsigned int x)
			{
				unsigned int l = 0;
				if (x >= 1 << 16) { x >>= 16; l |= 16; }
				if (x >= 1 << 8) { x >>= 8; l |= 8; }
				if (x >= 1 << 4) { x >>= 4; l |= 4; }
				if (x >= 1 << 2) { x >>= 2; l |= 2; }
				if (x >= 1 << 1) l |= 1;
				return l;
			}

			/////////////////////////////////////////////////////////////////////
			// this works for every number

			static double Log2(double  f)
			{
				return logf(f) *(double)INVLN2;
			}

			/////////////////////////////////////////////////////////////////////
			// Calculates the equation of "b" to the power of "e".

			static int IPow(int b, int e)
			{
				int result = 1;
				// Loop the multiplication process for each exponent.
				while (e--)
					result *= b;
				return result;
			}

			/////////////////////////////////////////////////////////////////////
			// check if a number is power of 2

			static int IsPowerOf2(int val)
			{
				return (val & (val - 1))==0;
			}

			/////////////////////////////////////////////////////////////////////

			static int IsUint32PowerOf2(unsigned int x)
			{
				return (
						x == 1 || x == 2 || x == 4 || x == 8 || x == 16 || x == 32 ||
						x == 64 || x == 128 || x == 256 || x == 512 || x == 1024 ||
						x == 2048 || x == 4096 || x == 8192 || x == 16384 ||
						x == 32768 || x == 65536 || x == 131072 || x == 262144 ||
						x == 524288 || x == 1048576 || x == 2097152 ||
						x == 4194304 || x == 8388608 || x == 16777216 ||
						x == 33554432 || x == 67108864 || x == 134217728 ||
						x == 268435456 || x == 536870912 || x == 1073741824 ||
						x == 2147483648
					);
			}

			/////////////////////////////////////////////////////////////////////
			// Find the closest power of 2 that is >= N

			static unsigned int NextPowerOf2(unsigned int x)
			{
				if (x == 0) return 0;
				x -= 1;
				x |= x >> 1;
				x |= x >> 2;
				x |= x >> 4;
				x |= x >> 8;
				x |= x >> 16;
				return x + 1;
			}

			/////////////////////////////////////////////////////////////////////
			// Find previous power of 2 to an integer.
			
			static unsigned int PrevPowerOf2(unsigned int x) 
			{
				x |= x >> 1;
				x |= x >> 2;
				x |= x >> 4;
				x |= x >> 8;
				x |= x >> 16;
				return x - (x >> 1);
			}

			/////////////////////////////////////////////////////////////////////
			// Function to find the closest power of 2 to an integer.

			static unsigned int closest_power_of_2(unsigned int x) 
			{
				unsigned int nx = NextPowerOf2(x);
				unsigned int px = PrevPowerOf2(x);
				return (nx - x) > (x - px) ? px : nx;
			}

			/////////////////////////////////////////////////////////////////////
			// Get a shift value from a power of 2.

			static int GetShiftFromPowerOf2(unsigned int bits)
			{
				for (unsigned int i = 0; i < 32; i++) 
					if (bits == (unsigned int)(1 << i)) 
						return i;
				return -1;
			}

			/////////////////////////////////////////////////////////////////////
			// Function to find the nearest (bigger) power of 2 to an integer.

			static unsigned int NearestShift(unsigned int n) 
			{
				for (int i = 30; i >= 0; i--) 
					if (n & (1 << i)) 
						return i + 1;
				return 0;
			}

			/////////////////////////////////////////////////////////////////////
			// Returns the closest multiple of value that isn't less than value.

			static int NextMultipleOf(int multiple, int value)
			{
				return (multiple + value) - (multiple % value);
			}
		
		} // end of namespaced logs

	} // end of math namespace

}	// end of vml namespace
