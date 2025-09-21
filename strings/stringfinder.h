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

////////////////////////////////////////////////////////////////////////////
// string searching utilities
//
//
////////////////////////////////////////////////////////////////////////////
// documentation
//
// Rabin-Karp string matcher
//
// std::vector<size_t> RabinKarpMatcher(const std::string &needle, const std::string &haystack)
// 
// given the 'needle' string and the ìhaystack' string , finds all starting point where
// the 'needle string is located in the longer 'haystack' 
//
// Knuth - Morris-Pratt string matcher
//
//	std::vector<size_t> KnuthMorrisPrattSearch(const std::string &needle, const std::string &haystack)
//
// given the 'needle' string and the ìhaystack' string , finds all starting point where
// the 'needle string is located in the longer 'haystack' 
//
// static bool StringContains(const std::string& s, const std::string& substring)										// check if a string is contained inside another
//
// static int StringIndexOf(const std::string& substring, const std::string& s, int startIndex)							// find index of substring
//
// static int StringLastIndexOf(const std::string& s, const std::string& substring, int startIndex)						// find last index of substring
//
// static int MaxBracesDepth(const std::string &text)																	// function takes a string and returns the maximum depth nested parenthesis

namespace vml
{
	namespace strings
	{

		class StringFinder
		{

			public:

			//////////////////////////////////////////////////////////////////////
			// Rabin-Karp string matcher

			static std::vector<size_t> RabinKarpMatcher(const std::string &needle, const std::string &haystack)
			{
				typedef unsigned long long uint64;

				static const uint64 radixLength = 256ULL;
				static const uint64 prime = 72057594037927931ULL;

				std::vector<size_t> matches;

				size_t needleLength = needle.size();
				size_t haystackLength = haystack.size();
				size_t lastIndex = haystackLength - needleLength;
				if (lastIndex <= 0) throw(L"strings mismatch");

				double diff = pow((double)radixLength, (double)(needleLength - 1));
				uint64 differenceHash = (uint64)diff % prime;

				size_t needleHash = 0;
				size_t firstHaystackHash = 0;

				size_t index;

				// preprocessing
				for (index = 0; index < needleLength; index++)
				{
					needleHash = (radixLength * needleHash + needle[index]) % prime;
					firstHaystackHash = (radixLength * firstHaystackHash + haystack[index]) % prime;
				}

				std::vector<uint64> haystackHashes;
				haystackHashes.reserve(lastIndex + 1);
				haystackHashes.push_back(firstHaystackHash);

				// matching

				for (index = 0; index <= lastIndex; index++)
				{
					if (needleHash == haystackHashes[index])
					{
						size_t needleCount = needle.size();
						size_t i;
						for (i = 0; i < needleCount && needle[i] == haystack[index + i]; ++i);
						if (i == needleCount)
							matches.emplace_back(index);
					}

					if (index < lastIndex)
					{
						uint64 newHaystackHash = (radixLength * (haystackHashes[index] - haystack[index] * differenceHash) + haystack[index + needleLength]) % prime;
						haystackHashes.emplace_back(newHaystackHash);
					}
				}

				return matches;
			}

			/////////////////////////////////////////////////////////////////////
			// Knuth - Morris-Pratt string matcher

			static std::vector<size_t> KnuthMorrisPrattSearch(const std::string &needle, const std::string &haystack)
			{
				std::vector<size_t> matches;
				size_t haystackIndex = 0;
				size_t needleIndex = 0;

				size_t haystackSize = haystack.size();
				size_t needleSize = needle.size();

				// create table

				std::vector<size_t> table(needle.size() + 1, -1);

				for (size_t index = 1; index <= needle.size(); index++)
				{
					size_t position = table[index - 1];

					while (position != -1 && needle[position] != needle[index - 1])
						position = table[position];

					table[index] = position + 1;
				}

				// search

				while (haystackIndex < haystackSize)
				{
					while (needleIndex != -1 && (needleIndex == needleSize || needle[needleIndex] != haystack[haystackIndex]))
						needleIndex = table[needleIndex];

					needleIndex++;
					haystackIndex++;

					if (needleIndex == needleSize)
						matches.push_back(haystackIndex - needleSize);
				}

				return matches;
			}

			////////////////////////////////////////////////////////////////////////////////
			// check if a string is contained inside another

			static bool StringContains(const std::string &needle, const std::string &haystack)
			{
				return haystack.find(needle) != std::string::npos;
			}

			////////////////////////////////////////////////////////////////////////////////
			// find index of substring

			static size_t StringIndexOf(const std::string& needle, const std::string& haystack, size_t start)
			{
				if (start >= haystack.size()) start = 0;
				size_t index = haystack.find(needle, (size_t)start);
				if (index == std::string::npos)
					return -1;
				return index;
			}

			////////////////////////////////////////////////////////////////////////////////
			// // find last index of substring

			static int StringLastIndexOf(const std::string &needle, const std::string &haystack, size_t start)
			{
				size_t end = haystack.size();
				if (start >= end) start = 0;
				std::string::size_type result = haystack.rfind(needle, end);
				if (result == std::string::npos ||
					result < (std::string::size_type)start ||
					(result + needle.size() >(std::string::size_type)end))
					return -1;
				return (int)result;
			}

			////////////////////////////////////////////////////////////////////////////////
			// function takes a string and returns the maximum depth nested parenthesis
			// if string contains unbalanced strings, return -1

			static int MaxBracesDepth(const std::string &text)
			{
				int current_max = 0;    // current count
				int max = 0;            // overall maximum count
				size_t n = text.length();

				// Traverse the input string

				for (size_t i = 0; i<n; ++i)
				{
					if (text[i] == '(')
					{
						current_max++;
						// update max if required
						if (current_max> max) max = current_max;
					}
					else if (text[i] == ')')
					{
						if (current_max > 0) current_max--;
						else return -1;
					}
				}

				// finally check for unbalanced string
				if (current_max != 0) return -1;

				return max;
			}
	
			// -------------------------------------------
			// ctor / dtor

			StringFinder()
			{}

			~StringFinder()
			{}
		
		};
	
	} // end of strings namespace

} // end of vml namespace
