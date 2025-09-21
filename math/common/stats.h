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
		namespace stats
		{
		
			///////////////////////////////////////////////////////////////
			// calculate the averag value in an array ( mean value )

			template < class T >
			static double Avg(const T *score, const size_t N)
			{
				double total = 0.0f;
				for (register size_t i = 0; i < N; ++i)
					total += (double)score[i];
				return (total / (double)N);
			}
			
			///////////////////////////////////////////////////////////////
			// accumulate values in an array
			// init is the initial value

			template < class T >
			static double Accumulate( T init,const T *score, const size_t N)
			{
				double ret = (double)init;
				for (register size_t i = 0; i < N; ++i)
					ret += score[i];
				return ret;
			}

			///////////////////////////////////////////////////////////////
			// accumulate values in an array

			template < class T >
			static double Product(const T *score, const size_t N)
			{
				double ret = 1.0;
				for (register size_t i = 0; i < N; ++i)
					ret *= (double)score[i];
				return ret;
			}
			
			///////////////////////////////////////////////////////////////
			// accumulate values in an array

			template < class T >
			static double DeProduct(const T *score, const size_t N)
			{
				double ret = 1.0;
				for (register size_t i = 0; i < N; ++i)
					ret /= (double)score[i];
				return ret;
			}

			///////////////////////////////////////////////////////////////
			// accumulate with minus operator values in an array

			template < class T >
			static T DeAccumulate(T init, const T *score, const size_t N)
			{
				double ret = init;
				for (register size_t i = 0; i < N; ++i)
					ret -= score[i];
				return ret;
			}

			///////////////////////////////////////////////////////////////
			// Calculate the standard deviation using the formula:
			// sqrt(n(sum_of_squares) - square of sum) / (n(n - 1))

			template < class T >
			static double Sigma(const T *array, const size_t N)
			{
				// sum is used to calculate the sum of elements in the array
				// squares_sum is used to calculate the sum of squared elements
				double sum = 0.0f, squares_sum = 0.0f;
				double sigma; // holds the result
				double n = (double)N;
				// Let's calculate the sums:
				for (register size_t i = 0; i < N; i++)
				{
					sum += (double)array[i];
					squares_sum += (double)array[i] * (double)array[i];
				}
				sigma = sqrt((n * squares_sum - sum * sum) / (n * (n - 1)));
				return sigma;
			}

			///////////////////////////////////////////////////////////////
			//This is the faster median determination method.
			//Algorithm from Numerical recipes in C of 1992

			template < class T >
			static T Median(T *arr, const size_t N)
			{
				int low, high;
				int median;
				int middle, ll, hh;
				low = 0; high = N - 1; median = (low + high) / 2;

				for (;;)
				{
					// One element only 
					if (high <= low) return arr[median];

					if (high == low + 1)
					{
						// Two elements only 
						if (arr[low] > arr[high])
							vml::math::swap::Swap(arr[low], arr[high]);
						return arr[median];
					}

					// Find median of low, middle and high items; swap into position low 

					middle = (low + high) / 2;
					if (arr[middle] > arr[high])
						vml::math::swap::Swap(arr[middle], arr[high]);
					if (arr[low] > arr[high])
						vml::math::swap::Swap(arr[low], arr[high]);
					if (arr[middle] > arr[low])
						vml::math::swap::Swap(arr[middle], arr[low]);

					// Swap low item (now in position middle) into position (low+1) 

					vml::math::swap::Swap(arr[middle], arr[low + 1]);

					// Nibble from each end towards middle, swapping items when stuck 

					ll = low + 1;
					hh = high;

					for (;;)
					{
						do ll++; while (arr[low] > arr[ll]);
						do hh--; while (arr[hh] > arr[low]);
						if (hh < ll)
							break;
						vml::math::swap::Swap(arr[ll], arr[hh]);
					}

					// Swap middle item (in position low) back into correct position 

					vml::math::swap::Swap(arr[low], arr[hh]);

					// Re-set active partition 

					if (hh <= median)
						low = ll;
					if (hh >= median)
						high = hh - 1;
				}

				return arr[median];
			}

			/////////////////////////////////////////////////////////////////////////////
			// distance between two vectors, also known as correlaction

			template<typename T1, typename T2>
			double VectorDistance(const T1* first, const T2* second, const size_t n)
			{
				if (first == nullptr || second == nullptr || n == 0)
					throw std::runtime_error("pointers mut be non mull and sizes must match");
				double ret = 0.0;
				for (register size_t i = 0; i < n; ++i)
				{
					double dist = (double)first[i] - (double)second[i];
					ret += dist * dist;
				}
				return ret > 0.0 ? sqrt(ret) : 0.0;
			}

			/////////////////////////////////////////////////////////////////////////////
			// these are templated min amd max functions for n vector 

			template < typename T >
			static const double Max(const T* data, const size_t N)
			{
				double max = (double)data[0];
				for (register size_t i = 1; i <N; ++i)
					if (data[i] > max)max = data[i];
				return max;
			}

			template < typename T >
			static const double Min(const T* data, const size_t N)
			{
				double min = (double)data[0];
				for (register size_t i = 1; i <N; ++i)
					if (data[i] < min)min = data[i];
				return min;
			}

			/////////////////////////////////////////////////////
			// computes the faux average for value array

			template< typename T >
			static double FauxAvg(const T* data, const size_t N)
			{
				double m = data[0];
				for (register size_t i = 1; i <N; ++i)
					if (data[i] < m)m = data[i];
				double M = data[0];
				for (register size_t i = 1; i <N; ++i)
					if (data[i] > M)M = data[i];
				return m + (M - m) / (double)N;
			}

			//////////////////////////////////////////////////////
			// computes the faux average for 2 values

			template< typename T >
			static double FauxAvg(const T a, const T b)
			{
				if (a>b) return (double)b + ((double)a - (double)b) / 2.0f;
				return a + (b - a) / (T)2.0f;
			}

			/////////////////////////////////////////////////////
			// computes the faux average for 3 values

			template< typename T >
			static double FauxAvg(const T a, const T b, const T c)
			{
				double M = (double)a; if (b>M) M = (double)b; if (c>M) M = c;
				double m = (double)a; if (b<m) m = (double)b; if (c<m) m = c;
				return (double)m + ((double)M - (double)m) / 2.0f;
			}

			/////////////////////////////////////////////////////
			// computes repetitions in a vector
			// returns the indices where value repetitions occurs

			template < typename T >
			std::vector<size_t> GetRepetitions(const T value,const T* data, const size_t N)
			{
				std::vector<size_t> indices;
				for (size_t i = 0; i < N; ++i)
					if (data[i] == value) indices.push_back(i);
				return indices;
			}

			/////////////////////////////////////////////////////////////////////////////
			// these are quick templated min amd max functions for 2 and 3 arguments

			template< typename T >
			static double Max(const T a, const T b)
			{
				if (a>b) return (double)a; return (double)b;
			}

			template< typename T >
			static double Min(const T a, const T b)
			{
				if (a<b) return (double)a; return (double)b;
			}

			template< typename T >
			static double Max(const T x, const T y, const T z)
			{
				double m = (double)x; if (y>m) m = (double)y; if (z>m) m = (double)z; return m;
			}

			template< typename T >
			static double Min(const T x, const T y, const T z)
			{
				double m = (double)x; if (y<m) m = (double)y; if (z<m) m = (double)z; return m;
			}

			template< typename T >
			static double Avg(const T a, const T b)
			{
				return ((double)a + (double)b) * 0.5f;
			}

			template< typename T >
			static double Avg(const T a, const T b, const T c)
			{
				return ((double)a + (double)b + (double)c) * 0.333f;
			}

			////////////////////////////////////////////////////////////////////////////////////////////
			// stl wrappers

			template< typename T1, typename T2>
			double VectorDistance(const std::vector<T1> &a, const std::vector<T2> &b)
			{
				if (a.size() == 0 || b.size() == 0 || a.size() != b.size())
					throw std::runtime_error("sizes must be non zero and matching");
				// since a and b vectors shares the same size, there is no difference
				// sending a.size or b.size as vectors size
				return vml::math::stats::VectorDistance(&a[0], &b[0], a.size());
			}

			template < class T >
			static double Avg(const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return Avg(&a[0], a.size());
			}

			template < class T >
			static double Sigma(const std::vector<T> &a)
			{
				if (a.size() == 0 )
					throw std::runtime_error("sizes must be non zero and matching");
				return Sigma(&a[0], a.size());
			}
	
			template < class T >
			static double Accumulate(T init, const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return Accumulate(init,&a[0], a.size());
			}

			template < class T >
			static double DeAccumulate(T init, const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return DeAccumulate(init,&a[0], a.size());
			}

			template < class T >
			static double Product(const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return Product(&a[0], a.size());
			}

			template < class T >
			static double DeProduct(const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return DeProduct(&a[0], a.size());
			}

			// * this function alters vector value *

			template < class T >
			static double NonConstMedian(std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				return Median(&a[0], a.size());
			}
			
			// * this function copies the vector into a temporary vector
			//  this is just to maintain consistency with the other wrappers
			// but you should conside this extra overhead if you are intned
			// to use it.

			template < class T >
			static double Median(const std::vector<T> &a)
			{
				if (a.size() == 0)
					throw std::runtime_error("sizes must be non zero and matching");
				std::vector<T> temp(a);
				return Median(&temp[0], temp.size());
			}

			////////////////////////////////////////////////////////////////////////////////////////////
			// end of stats lib

		}	// end of stats namespace

	}	// end of math namespace

}	// end of vml namespace


////////////////////////////////////////////////////////////////////////////////////////////
// histogram computation
////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace math
	{
		namespace stats
		{
			
			//////////////////////////////////////////////////////////////////////////////
			// node for templated array hystogram class

			template < typename T >
			class CArrayHystogramNode
			{

				private:
	
						T Element;
						unsigned int Frequency;

				public:

						// ----------------------------------------------------
						// getters

						const T GetElement() const { return Element; }
						const unsigned int GetFrequency() const { return Frequency; }

						// ---------------------------------------------------

						void IncFrequency() { Frequency++; }

						// ---------------------------------------------------
						// ctor / dtor

						CArrayHystogramNode(const T element)
						{
							Element = element;
							Frequency = 1;
						}

						~CArrayHystogramNode()
						{}

			};

			///////////////////////////////////////////////////////////////////////////
			// tempalted array hystogram class
			
			template < typename T >
			class ArrayHystogram
			{

				private:

						// -----------------------------------------------------------------
						// resets everything

						void Reset()
						{
							for (std::unordered_map<T, CArrayHystogramNode<T>*>::iterator<T> it = ElementMap.begin(); it != ElementMap.end(); ++it)
							{
								delete ((*it).second);
							}
						}

						// ----------------------------------------------------------------

						struct SortByName
						{
							inline bool operator() (const std::pair<T, CArrayHystogramNode<T>*>& struct1,
													const std::pair<T, CArrayHystogramNode<T>*>& struct2)
							{
								return (struct1.second->GetElement() < struct2.second->GetElement());
							}
						};

						struct SortByFreq
						{
							inline bool operator() (const std::pair<T, CArrayHystogramNode<T>*>& struct1,
													const std::pair<T, CArrayHystogramNode<T>*>& struct2)
							{
								return (struct1.second->GetFrequency() < struct2.second->GetFrequency());
							}
						};

						// ------------------------------------------------------------------

						unsigned int									SortMode;
						std::unordered_map<T, CArrayHystogramNode<T>*>	ElementMap;

				public:

						std::vector<std::pair<T, CArrayHystogramNode<T>*>>	ElementVec;
						
						// ------------------------------------------------------

						static const unsigned int SORT_BY_ELEMENT   = 1;
						static const unsigned int SORT_BY_FREQUENCY = 2;
						
						// -----------------------------------------------------------------
						// creates hystogram from a text string

						const bool Compute(const T *v, const size_t n, unsigned int flags = 0)
						{
							if (n <= 0) vml::os::Error(L"size is zero");

							Reset();

							if (n != 0)
							{

								// add tokens

								for (size_t i = 0; i<n; ++i)
								{

									std::unordered_map<T, CArrayHystogramNode<T>*>::iterator<T> it = ElementMap.find(v[i]);

									if (it == ElementMap.end())
									{
										ElementMap[v[i]] = new CArrayHystogramNode<T>(v[i]);
									}
									else
									{
										(*it).second->IncFrequency();
									}

								}

								// Initialize and sort vector with data_t items from link_map    

								for (std::unordered_map<T, CArrayHystogramNode<T>*>::iterator<T> it = ElementMap.begin(); it != ElementMap.end(); ++it)
								{
									ElementVec.emplace_back(*it);
								}

								// sort according to sort mode

								Sort();

							}
							else
							{
								return false;
							}

							// we never get here

							return false;

						}
						
						// ---------------------------------------------------------------

						void Sort()
						{
							switch (SortMode)
							{
								case SORT_BY_ELEMENT:   std::sort(ElementVec.begin(), ElementVec.end(), SortByName()); break;
								case SORT_BY_FREQUENCY: std::sort(ElementVec.begin(), ElementVec.end(), SortByFreq()); break;
							}
						}

						// ----------------------------------------
						// changes sorting mode

						void SetSortMode(unsigned int mode)
						{
							if (mode != SORT_BY_ELEMENT && mode != SORT_BY_FREQUENCY)
							{
								SortMode = SORT_BY_ELEMENT;
							}
							else
							{
								SortMode = mode;
							}
						}
						
				public:

						// ----------------------------------------
						// ctor / dtor

						ArrayHystogram()
						{
							SortMode = SORT_BY_ELEMENT;
						}

						~ArrayHystogram()
						{
							Reset();
						}

			};
			

		}	// end of stats namespace

	}	// end of math namespace

}	// end of vml namespace

