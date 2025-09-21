#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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
		namespace primes
		{
	
			class PrimeGenerator
			{

				//////////////////////////////////////////////////////////////////////////////////
				// prime number check look up table

				std::vector<bool> PrimeLUT;

				//////////////////////////////////////////////////////////////////////////////////
				// A function to check if a given number is prime

				bool CheckIfPrime(unsigned long long n)
				{
					if (n == 0 || n == 1) return false;
					for (unsigned long long i = 2; i <= n / 2; ++i)
						if (n % i == 0)
							return false;
					return true;
				}

				//////////////////////////////////////////////////////////////////////////////////
				// generate look up table for prime numbers
				// the larger the array the longer the generation time

				void GeneratePrimeNumbersLUT(size_t n = 1024)
				{
					// create an array of 16 entried at least

					if (n <= 16) n = 16;

					// for each number in the range 0..n create 
					// a boolean array to check if that
					// corresponfing number is prime or not

					PrimeLUT.clear();

					for (size_t i = 0; i < n; ++i)
						PrimeLUT.emplace_back(CheckIfPrime(i));
				}

				//////////////////////////////////////////////////////////////////////////////////
				// A function to check if a given number is prime
				// if number is in the 0...n where n is the primelut size
				// then a simple check is performed, else the 
				// prime check function is called

				bool CheckIfPrimeLUT(unsigned long long val)
				{
					size_t n = PrimeLUT.size();
//					if (n == 0) vml::os::Error(L"Generate lut first, calling  GeneratePrimeNumbersLUT in this namespace ( vml::math::primes )");
					if (val >= 0 && val < n)
						return PrimeLUT[val];
					else return CheckIfPrime(val);
				}

				//////////////////////////////////////////////////////////////////////////////////
				// A function to check if two numbers are coprime ( relatively prime )

				bool AreCoprime(unsigned long long u, unsigned long long v)
				{
					if (((u | v) & 1) == 0) return false;

					while ((u & 1) == 0) u >>= 1;

					if (u == 1) return true;

					do
					{
						while ((v & 1) == 0) v >>= 1;

						if (v == 1) return true;

						if (u > v)
						{
							unsigned long long t = v;
							v = u;
							u = t;
						}

						v -= u;

					} while (v != 0);

					return false;
				}

				//////////////////////////////////////////////////////////////////////////////////
				// A function to print all prime factors of a given number n

				std::vector <unsigned long long> GetPrimeFactors(unsigned long long n)
				{
					std::vector <unsigned long long> factors;

					// Print the number of 2s that divide n

					if (n == 0) { factors.emplace_back(0); return factors; }
					if (n == 1) { factors.emplace_back(1); return factors; }

					while (n % 2 == 0)
					{
						factors.emplace_back(2);
						n = n / 2;
					}

					// n must be odd at this point.  So we can skip one element (Note i = i +2)

					for (unsigned long long i = 3; i <= sqrtf(n); i = i + 2)
					{
						// While i divides n, print i and divide n

						while (n % i == 0)
						{
							factors.emplace_back(i);
							n = n / i;
						}
					}

					// This condition is to handle the case whien n is a prime number
					// greater than 2

					if (n > 2)
					{
						factors.emplace_back(n);
					}

					return factors;
				}
			
				// -----------------------------------------------------
				// ctor / dtor

				PrimeGenerator()
				{}

				~PrimeGenerator()
				{}

			};

		}  // end of primefactors namespace

	}  // end of math namespace

} // end of vml namespace