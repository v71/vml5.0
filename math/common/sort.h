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

/////////////////////////////////////////////////////////////////////////////
// sort functiuons-

namespace vml
{
	namespace math
	{
		namespace sort
		{
			
			static const unsigned int ASCENDING  = 0;
			static const unsigned int DESCENDING = 1;

			////////////////////////////////////////////////////////////////////////
			// functor for comparisons

			template <typename T>
			class CompareVirtual
			{
				public:	virtual	bool operator() (const T& a, const T& b) = 0;
			};

			template <typename T>
			class CompareGreater : public CompareVirtual<T>
			{
				public:	bool operator() (const T& a, const T& b) { return a<b; }
			};

			template <typename T>
			class CompareLower : public CompareVirtual<T>
			{
				public: bool operator() (const T& a, const T& b) { return a>b; }
			};

			template <typename T>
			class CompareGreaterOrEqual : public CompareVirtual<T>
			{
				public:	bool operator() (const T& a, const T& b) { return a <= b; }
			};

			template <typename T>
			class CompareLowerOrEqual : public CompareVirtual<T>
			{
				public: bool operator() (const T& a, const T& b) { return a >= b; }
			};
			
			/////////////////////////////////////////////////////////////////
			// sort 2 numbers

			template < typename T >
			static void Sort2(T *a, T *b)
			{
				if (*a<*b)
				{
					T temp;
					temp = *a;
					*a = *b;
					*b = temp;
				}
			}

			/////////////////////////////////////////////////////////////////
			// sort 3 numbers

			template < typename T >
			static void Sort3(T *a, T *b, T*c)
			{
				T temp;
				if (*a > *b) { temp = *a; *a = *b; *b = temp; }
				if (*b > *c) { temp = *b; *b = *c; *c = temp; }
				if (*a > *b) { temp = *a; *a = *b; *b = temp; }
			}

			////////////////////////////////////////////////////////
			// flags for ordering policies

			template <typename T>
			class BubbleSort
			{
				public:

					BubbleSort(T *v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						for (size_t i = 0; i < n; ++i)
						{
							for (size_t j = 0; j < n - i - 1; ++j)
							{
								if (policy(v[j + 1], v[j]))
								{
									T temp = v[j];
									v[j] = v[j + 1];
									v[j + 1] = temp;
								}
							}
						}
					}
			};

			//////////////////////////////////////////////////////////////////////////
			// Selection sort

			template< typename T >
			class SelectionSort
			{
				public:

					// ---------------------------------------------------------
					// ctor / dtor

					SelectionSort(T *v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						for (size_t i = 0; i < n; i++)
						{
							size_t min = i;
							for (size_t j = i + 1; j < n; j++)
							{
								if (policy(v[j],v[min]))
								min = j;
							}
							T temp = v[min];
							v[min] = v[i];
							v[i] = temp;
						}
					}

					~SelectionSort()
					{}

			};

			//////////////////////////////////////////////////////////////////////////
			// Insertion sort

			template< typename T >
			class InsertionSort
			{
				public:

					// ---------------------------------------------------------
					// ctor / dtor

					InsertionSort(T *v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						for (size_t i = 0; i < n; i++)
						{
							size_t j = i;
							while (j > 0 && policy(v[j],v[j-1]))
							{
								T temp = v[j];
								v[j] = v[j - 1];
								v[j - 1] = temp;
								j--;
							}
						}
					}

					~InsertionSort()
					{}

			};

			//////////////////////////////////////////////////////////////////////////
			// Recursinve heap sort

			template< typename T >
			class RecursiveHeapSort
			{
				private:

					//------------------------------------------------------------
					// To heapify a subtree rooted with node i which is
					// an index in arr. n is size of heap

					void heapify(T *arr, int n, int i, CompareVirtual<T> &policy)
					{
						int largest = i;  // Initialize largest as root
						int l = 2 * i + 1;  // left = 2*i + 1
						int r = 2 * i + 2;  // right = 2*i + 2

						// If left child is larger than root
						if (l < n && policy(arr[largest],arr[l]))
							largest = l;

						// If right child is larger than largest so far
						if (r < n && policy(arr[largest],arr[r]))
							largest = r;

						// If largest is not root
						if (largest != i)
						{

							T temp = arr[i];
							arr[i] = arr[largest];
							arr[largest] = temp;

							// Recursively heapify the affected sub-tree
							heapify(arr, n, largest, policy);
						}
					}

				public:

					//------------------------------------------------------------
					// ctor / dtor

					RecursiveHeapSort(T *v, int n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"array is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						// Build heap (rearrange array)

						for (int i = n / 2 - 1; i >= 0; i--)
							heapify(v, n, i, policy);

						// One by one extract an element from heap

						for (int i = n - 1; i >= 0; i--)
						{
							// Move current root to end
							//		swap(arr[0], arr[i]);

							T temp = v[i];
							v[i] = v[0];
							v[0] = temp;

							// call max heapify on the reduced heap
							heapify(v, i, 0, policy);
						}
					}

					~RecursiveHeapSort()
					{}

			};

			/////////////////////////////////////////////////////////////////////////
			// Heap sort

			template<typename T>
			class HeapSort
			{

			private:

				void Heapify(T* v, size_t i, const size_t heapsize, CompareVirtual<T> &policy)
				{
					while (true)
					{
						size_t l = 2 * i + 1;
						size_t r = 2 * i + 2;
						size_t largest;

						largest = (l < heapsize && policy(v[i],v[l])) ? l : i;

						if (r < heapsize && policy(v[largest],v[r]))
							largest = r;

						if (largest != i)
						{
							T temp = v[i];
							v[i] = v[largest];
							v[largest] = temp;
							i = largest;
						}
						else break;
					}
				}

			public:

				//------------------------------------------------------------
				// ctor / dtor

				HeapSort(T* v, const size_t n, CompareVirtual<T> &policy)
				{
					if (v == nullptr) vml::os::Error(L"arrays pointer is null");
					if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

					for (size_t i = n / 2; i > 0;)
					{
						--i;
						Heapify(v, i, n, policy);
					}

					for (size_t i = n - 1; i > 0; --i)
					{
						T temp = v[0];
						v[0] = v[i];
						v[i] = temp;
						Heapify(v, 0, i, policy);
					}
				}

			};

			//////////////////////////////////////////////////////////////////////////
			// Shell sort

			template < typename T>
			class ShellSort
			{
				public:

					//------------------------------------------------------------
					// ctor / dtor

					ShellSort(T* v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						bool flag = true;
						size_t d = n;

						while (flag || (d > 1))
						{
							flag = false;
							d = (d + 1) / 2;
							for (size_t i = 0; i < (n - d); i++)
							{
								if ( policy(v[i + d],v[i]))
								{
									T temp = v[i + d];
									v[i + d] = v[i];
									v[i] = temp;
									flag = true;
								}
							}
						}
				}

				~ShellSort()
				{}
			};

			////////////////////////////////////////////////////////////////////////
			// Gnome sort

			template< typename T >
			class GnomeSort
			{
				public:

					// ---------------------------------------------------------
					// ctor / dtor

					GnomeSort(T *v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						size_t i = 1;
						size_t j = 2;

						while (i < n)
						{
							if (!( policy(v[i],v[i-1])))
							{
								i = j;
								++j;
							}
							else
							{
								T temp = v[i - 1];
								v[i - 1] = v[i];
								v[i] = temp;

								--i;

								if (i == 0)
								{
									i = j;
									++j;
								}
							}
						}

					}

					~GnomeSort()
					{}

			};

			/////////////////////////////////////////////////////////////////////////
			// Merge sort

			template<typename T>
			class MergeSort
			{
				private:

					//------------------------------------------------------------
					// Perform one pass through the two arrays, invoking Merge() above
					// Merge adjacent segments of size s.

					void mergepass(T *x, T *y, int s, int n, CompareVirtual<T> &policy)
					{
						int i = 0;

						//Merge two adjacent segments of size s

						while (i <= n - 2 * s)
						{
							merge(x, y, i, i + s - 1, i + 2 * s - 1, policy);
							i = i + 2 * s;
						}
						// fewer than 2s elements remain

						if (i + s < n)
							merge(x, y, i, i + s - 1, n - 1, policy);
						else
							for (int j = i; j <= n - 1; j++)
								y[j] = x[j];   // copy last segment to y
					}

					//------------------------------------------------------------
					// Merge from one array into another
					// Merge c[lt:md] and c[md+1:rt] to d[lt:rt]

					void merge(T *c, T* d, int lt, int md, int rt, CompareVirtual<T> &policy)
					{
						int i = lt,       // cursor for first segment
							j = md + 1,   // cursor for second
							k = lt;       // cursor for result

										  // merge until i or j exits its segment

							while ((i <= md) && (j <= rt))
								if ( policy(c[i],c[j]))d[k++] = c[i++];
								else  d[k++] = c[j++];

						// take care of left overs ,only one while loop actually runs

						while (i <= md)	d[k++] = c[i++];
						while (j <= rt)	d[k++] = c[j++];
					}

				public:

					//------------------------------------------------------------
					// ctor / dtor

					MergeSort(T *v, const size_t n, CompareVirtual<T> &policy)
					{
						if (v == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						// Sort a[0:n-1] using merge sort.

						int  s = 1;						// segment size

						T *b = new T[n];

						while (s < n)
						{
							mergepass(v, b, s, n, policy);		// merge from a to b
							s += s;						// double the segment size
							mergepass(b, v, s, n, policy);		// merge from b to a
							s += s;						// again, double the segment size
						}

						delete b;
					}

					~MergeSort()
					{}
			};

			///////////////////////////////////////////////////////////////////////////////
			// imperative version of bitonic sort

			template< typename T >
			class BitonicSort
			{

				public:

					// -------------------------------------------------
					// ctor / dtor

					BitonicSort(T *a, const size_t n, CompareVirtual<T> &policy)
					{
						if ((n&(n - 1))) vml::os::Error(L"n is not a power of two");
						if (a == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						int i, j, k;

						for (k = 2; k <= n; k = 2 * k)
						{
							for (j = k >> 1; j > 0; j = j >> 1)
							{
								for (i = 0; i < n; i++)
								{
									int ij = i^j;

										if ( policy(i,ij))	// policy is here
										{
											if ((i&k) == 0 && a[i] > a[ij])
											{
												T temp;
												temp = a[i];
												a[i] = a[ij];
												a[ij] = temp;
											}

											if ((i&k) != 0 && a[i] < a[ij])
											{
												T temp;
												temp = a[i];
												a[i] = a[ij];
												a[ij] = temp;
											}
										}

									}
								
							}
						}
					}

					~BitonicSort()
					{}
			};

			/////////////////////////////////////////////////////////////////////////
			// recursive bitonic sort

			template<typename T>
			class RecursiveBitonicSort
			{
				private:

					// -------------------------------------------------
					//It recursively sorts a bitonic sequence in ascending order,
					//if dir = ASCENDING, and in descending order otherwise.
					//The sequence to be sorted starts at index position lo,
					//the parameter cbt is the number of elements to be sorted.

					void bitonicMerge(T *a, int lo, int cnt, int dir, CompareVirtual<T> &policy)
					{
						if (cnt>1)
						{
							int k = cnt / 2;

							for (int i = lo; i < lo + k; i++)
							{

								if (dir == (int)policy(a[i+k],a[i] ) )
								{
									T temp;
									temp = a[i];
									a[i] = a[i + k];
									a[i + k] = temp;
								}

							}

							bitonicMerge(a, lo, k, dir, policy);
							bitonicMerge(a, lo + k, k, dir, policy);
						}
					}

					// -------------------------------------------------
					//first produces a bitonic sequence by recursively sorting
					//its two halves in opposite sorting orders, and then
					//calls bitonicMerge to make them in the same order

					void recBitonicSort(T *a, int lo, int cnt, int dir, CompareVirtual<T> &policy)
					{
						if (cnt>1)
						{
							int k = cnt / 2;
							recBitonicSort(a, lo, k, ASCENDING, policy);
							recBitonicSort(a, lo + k, k, DESCENDING, policy);
							bitonicMerge(a, lo, cnt, dir, policy);
						}
					}

				public:

					// -------------------------------------------------
					// ctor / dtor

					RecursiveBitonicSort(T *a, const size_t n, CompareVirtual<T> &policy)
					{
						if ((n&(n - 1))) vml::os::Error(L"n is not a power of two");
						if (a == nullptr) vml::os::Error(L"arrays pointer is null");
						if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						// call sorting function recursively
						recBitonicSort(a, 0, n, ASCENDING, policy);
					}

					~RecursiveBitonicSort()
					{}
			};

			/////////////////////////////////////////////////////////////////////
			// Quick sort

			template< typename T>
			class QuickSort
			{
				private:

					// ----------------------------------------------------
					// Find and return the index of pivot element

					int pivot(T *a, int first, int last, CompareVirtual<T> &policy)
					{
						int  p = first;
						int pivotElement = a[first];

						for (int i = first + 1; i <= last; i++)
						{

							if ( policy(a[i],pivotElement))
							{
								p++;

								T temp;
								temp = a[i];
								a[i] = a[p];
								a[p] = temp;

							}

						}

						T temp;
						temp = a[first];
						a[first] = a[p];
						a[p] = temp;

						return p;
					}

					// ----------------------------------------------------
					// Quicksort main function

					void quickSort(T *a, int first, int last, CompareVirtual<T> &policy)
					{
						if (first < last)
						{
							int pivotElement = pivot(a, first, last, policy);
							quickSort(a, first, pivotElement - 1, policy);
							quickSort(a, pivotElement + 1, last, policy);
						}
					}

				public:

					// -------------------------------------------------
					// ctor / dtor


					QuickSort(int *a, const size_t n, CompareVirtual<T> &policy)
					{
						quickSort(a, 0, n - 1, policy);
					}

					~QuickSort()
					{
					}
			};

			////////////////////////////////////////////////////////////////////////////
			// The array is considered unsorted
			// if a successor has a greater value than its predecessor.
			// If we reach the end of the loop without finding that the array is unsorted,
			// then it must be sorted instead.
			// Note that we are always comparing an element to its successor.
			// Because of this, we can end the loop after comparing 
			// the second-last element to the last one.
			// This means the loop iterator will end as an index of the second-last
			// element of the array instead of the last one.

			template< typename T>
			static const bool IsSortedHigh(const T *a, const size_t n)
			{
				// if we get out of the cycle It is proven that the 
				// array is not sorted in ascending order
				for (size_t i = 0; i <n - 1; ++i)
					if (a[i] > a[i + 1])
						return false;
				return true; // If this part has been reached, the array must be sorted
			}

			template< typename T>
			static const bool IsSortedLow(const T *a, const size_t n)
			{
				// if we get out of the cycle It is proven that the 
				// array is not sorted in descending order
				for (size_t i = 0; i <n - 1; ++i)
					if (a[i] < a[i + 1])
						return false;
				return true; // If this part has been reached, the array must be sorted
			}

			template< typename T>
			static const int IsSorted(const T *a, const size_t n)
			{
				if (IsSortedHigh(a, n))
					return 1;
				else if (IsSortedLow(a, n)) return 2;
				return 0;
			}

			////////////////////////////////////////////////////////////////////////////
			// check if all elements in the array are negative

			template< typename T>
			static const bool IsNegative(const T *a, const size_t n)
			{
				// if we get out of the cycle It is proven that the 
				// array at least contains a positive elelemnt
				for (size_t i = 0; i <n - 1; ++i)
					if (a[i]>0)
						return false;
				// If this part has been reached, all elements in the array are <0
				return true; 
			}

			////////////////////////////////////////////////////////////////////////////
			// check if all elements in the array are negative

			template< typename T>
			static const bool IsPositive(const T *a, const size_t n)
			{
				// if we get out of the cycle It is proven that the 
				// array at least contains a negative elelemnt
				for (size_t i = 0; i <n - 1; ++i)
					if (a[i]<0)
						return false;
				// If this part has been reached, all elements in the array are >0
				return true; 
			}

			///////////////////////////////////////////////////////////////////////////
			// comparing policy must match the sorted vector
			// else you will get incorrect results

			template < typename T >
			static int BinarySearch(const T *a, const size_t n, T key, CompareVirtual<T> &policy)
			{
				int left = 0;
				int right = n;
				while (left <= right)
				{
					int middle = (left + right) >> 1;
					if (a[middle] == key)
						return middle;
					else if ( policy(key,a[middle])) 
					// example for ascending order, policy is : a[middle] > key
						right = middle - 1;
					else
						left = middle + 1;
				}
				return -1;
			}
						
			//////////////////////////////////////////////////////////////////////////
			// Radix Sort
			// only for non negative integer numbers

			class RadixSort
			{

				public:

					// -----------------------------------------------------------
					// ctor / dtor

					RadixSort(int *v, const int n)
					{
			//			if (v == nullptr) vml::os::Error(L"arrays pointer is null");
			//			if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						// Base 10 is used

						int i;
						int *semiSorted = new int[n];
						int significantDigit = 1;

						int largestNum = -1;
						
						for (i = 0; i < n; i++)
							if (v[i] > largestNum)
								largestNum = v[i];

						// Loop until we reach the largest significant digit

						while (largestNum / significantDigit > 0)
						{

							int bucket[10] = { 0 };

							// Counts the number of "keys" or digits that will go into each bucket

							for (i = 0; i < n; i++)
								bucket[(v[i] / significantDigit) % 10]++;

							// Add the count of the previous buckets,
							// Acquires the indexes after the end of each bucket location in the array
							// Works similar to the count sort algorithm

							for (i = 1; i < 10; i++)
								bucket[i] += bucket[i - 1];

							// Use the bucket to fill a "semiSorted" array

							for (i = n - 1; i >= 0; i--)
								semiSorted[--bucket[(v[i] / significantDigit) % 10]] = v[i];

							for (i = 0; i < n; i++)
								v[i] = semiSorted[i];

							// Move to next significant digit

							significantDigit *= 10;

						}

						delete []semiSorted;

					}

					~RadixSort()
					{}

			};
			
			//////////////////////////////////////////////////////////////////////////
			// Counting sort
			// only for non negative int numbers

			class CountingSort
			{

				public:

					//------------------------------------------------------------
					// ctor / dtor

					CountingSort(int* v, const size_t n, const unsigned int policy = ASCENDING)
					{
				//		if (v == nullptr) vml::os::Error(L"arrays pointer is null");
				//		if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						int largestNum = -1;
						for (int i = 0; i < n; i++)
							if (v[i] > largestNum)
								largestNum = v[i];

						int *counts = new int[largestNum + 1];

						memset(counts, 0, sizeof(size_t)*(static_cast<unsigned long long>(largestNum) + 1));

						// do the counting sort itself.

						for (int i = 0; i < n; i++)
							++counts[v[i]];

						// write out the result.

						int k = 0;

						for (int i = 0; i < largestNum + 1; i++)
							for (int j = 0; j < counts[i]; j++)
								v[k++] = i;

						delete []counts;

						// if we need to sort in a descending order
						// we need to invert the array

						if (policy == DESCENDING)
						{
							size_t i, j;
							
							for (i = 0, j = n - 1; i<n / 2; i++, j--)
							{
								int temp = v[i];
								v[i] = v[j];
								v[j] = temp;
							}
						}

					}

					~CountingSort()
					{}
			};

			////////////////////////////////////////////////////////////////
			// Bucket sort
			// only for non negative int numbers

			class BucketSort
			{
				public:

					// -------------------------------------------------
					// ctor / dtor

					BucketSort(int *v, const size_t n, const unsigned int policy = ASCENDING)
					{
					//	if (v == nullptr) vml::os::Error(L"arrays pointer is null");
					//	if (n <= 1) vml::os::Error(L"number of elements must be greater than 1");

						int largestNum = -1;
						for (int i = 0; i < n; i++)
							if (v[i] > largestNum)
								largestNum = v[i];

						int *b = new int[long(largestNum + 1)];

						int i, k, j = -1;

						memset(b, 0, sizeof(int) * (static_cast<unsigned long long>((largestNum)) + 1));

						for (i = 0; i < n; ++i)
							b[v[i]] = b[v[i]] + 1;

						for (i = 0; i < largestNum; ++i)
							for (k = b[i]; k > 0; --k)
								v[++j] = i;

						delete []b;

						// if we need to sort in a descending order
						// we need to invert the array

						if (policy == DESCENDING)
						{
							size_t i, j;
							
							for (i = 0, j = n - 1; i<n / 2; i++, j--)
							{
								int temp = v[i];
								v[i] = v[j];
								v[j] = temp;
							}
						}

					}

					~BucketSort()
					{}

			};
			
		} // end of sort namespace
	} // end of math namespace
} // end of vml namespace