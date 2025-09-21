#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2019 v71 
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

/////////////////////////////////////////////////////////////////////////////
// math sequences

namespace vml
{
	namespace math
	{
		namespace sequences
		{

			static double Factorial(int n)
			{
				if (n > 1) return n * Factorial(n - 1);
				return 1;
			}

			static double Fibonacci(int i)
			{
				return round((pow((1 + sqrtf(5)) / 2, i) - pow((1 - sqrtf(5)) / 2, i)) / sqrtf(5));
			}

			static double Lucas(int i)
			{
				return round((pow((1 + sqrtf(5)) / 2, i) + pow((1 - sqrtf(5)) / 2, i)));
			}

			static double Triangular(int i)
			{
				return (i * (i + static_cast<double>(1))) / 2;
			}

			static double Square(int i)
			{
				return i * i;
			}

			static double Pentagonal(int i)
			{
				return (i * (static_cast<double>(3) * i - 1)) / 2;
			}

			static double Hexagonal(int i)
			{
				return i * (2 * i - 1);
			}

			static double Catalan(int i)
			{
				return Factorial(2 * i) / (Factorial(i + 1) * Factorial(i));
			}

			static double Cullen(int i)
			{
				return i * pow(2, i) + 1;
			}

			static double Pronic(int i)
			{
				return i * (i + 1);
			}

			static double Woodall(int i)
			{
				return i * pow(2, i) - 1;
			}
		}
	}
}