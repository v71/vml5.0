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
		namespace swap
		{
	
			static const int SWAPIFAEB  = vml::bits32::BIT0;	// swap if a is equal to b
			static const int SWAPIFAGB  = vml::bits32::BIT1;	// swap if a is greather than to b
			static const int SWAPIFAGEB = vml::bits32::BIT2;	// swap if a is greater than or equal to b
			static const int SWAPIFBGA  = vml::bits32::BIT3;	// swap if a is lower to b
			static const int SWAPIFBGEA = vml::bits32::BIT4;	// swap if a is lower than or equal to b

			/////////////////////////////////////////////////////////////////////
			// swaps 2 numbers

			template < typename T >
			static void Swap(T &a, T &b)
			{
				T c = a;
				a = b;
				b = c;
			}

			/////////////////////////////////////////////////////////////////////
			// swaps 2 numbers
			// base on comparison between a and b 

			template < typename T >
			static void Swap(T &a, T &b, unsigned int flags)
			{
				if ((flags & SWAPIFAEB)  != 0) { if (a == b) { T c = a; a = b; b = c; } } 	// swap if a is equalt to b
				if ((flags & SWAPIFAGB)  != 0) { if (a  > b) { T c = a; a = b; b = c; } } 	// swap if a is greater than b
				if ((flags & SWAPIFAGEB) != 0) { if (a >= b) { T c = a; a = b; b = c; } }	// swap if a is greater than or equal to b
				if ((flags & SWAPIFBGA)  != 0) { if (a  < b) { T c = a; a = b; b = c; } }	// swap if a is greater than or equal to b
				if ((flags & SWAPIFBGEA) != 0) { if (a <= b) { T c = a; a = b; b = c; } }	// swap if a is greater than or equal to b
			}
		
		} // end of namespace swap

	}	// end of math namespace

}	// end of vml namespace
