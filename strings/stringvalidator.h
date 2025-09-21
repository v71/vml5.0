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
// string validation utilities
//
//
////////////////////////////////////////////////////////////////////////////
// documentation
//
// template <class out_type, class in_type>
// static out_type NumericConverter(in_type const& sval)																// Numeric conversion function internal usage
// static bool BeginsWith(const std::string &input, const std::string & match)											// Checks if a string start with a matching substring case sensitive
// static bool EndsWith(std::string const &input, std::string const &match)												// Checks if a string ends with a matching substring case sensitive
// static char FirstNonRepeatedChar(const std::string& str)																// Determines first non-repeated character in string 
// static bool HasStringOneCharUpperCase(const std::string &text)														// Test if string has just one character in upper case
// static bool HasStringOneCharLowerCase(const std::string &text)														// Test if string has just one character in lower case
// static bool HasStringOneSpace(const std::string &str)																// Tests if string has at least one space
// static bool IsStringHex(const std::string& str, int *out)															// Test if a number is in hex format
// static bool IsStringBinary(const std::string &str)																	// Tests if string is binary
// static bool IsStringAlNum(const std::string &str)																	// Check if string is alphanumeric
// static bool IsStringAlpha(const std::string &str)																	// Check if string is alphanumeric
// static bool IsStringDigit(const std::string &str)																	// Check if string is decimal digit
// static bool IsStringLower( const std::string & str )																	// Tests if string is in lower case
// static bool IsStringPalindrome(const std::string &s)																	// Tests if string is palindrome
// static float IsStringSimilarTo(const std::string &s1, const std::string & s2, unsigned int *editedchars = nullptr)	// Returns string similarity ( Levenstein distance )
// static bool IsStringTitle(const std::string & str) 																	//	All first letters are uppercase, and the other letters are lowercase
// static bool IsStringUpper(const std::string & str)																	// Test if string is in upper case

namespace vml
{
	namespace strings
	{

		class StringValidator
		{
			public:

				// ------------------------------------------
				// numerical converter templated function

				template <class out_type, class in_type>
				static out_type NumericConverter(in_type const& sval)
				{
					std::stringstream ss;
					ss << sval;
					out_type val;
					ss >> val;
					if (ss.fail())
						vml::os::Error(L"fail when translating value");
					return val;
				}

				// ------------------------------------------
				// checks if a string start with a matching substring
				// case sensitive

				static bool BeginsWith(const std::string &input, const std::string & match)
				{
					return input.size() >= match.size() && std::equal(match.begin(), match.end(), input.begin());
				}

				// ------------------------------------------
				// checks if a string ends with a matching substring
				// case sensitive

				static bool EndsWith(std::string const &input, std::string const &match)
				{
					return input.size() >= match.size() && std::equal(match.rbegin(), match.rend(), input.rbegin());
				}

				// ------------------------------------------
				// Determines first non-repeated character in string 

				static char FirstNonRepeatedChar(const std::string& str)
				{
					// Cycle through each character int he string and check if it's repeated
					for (size_t i = 0; i<str.size(); ++i)
					{
						char c = str[i];
						int mycount = count(str.begin(), str.end(), c);
						// If character not repeated elsewhere in string, return that character
						if (mycount == 1) return c;
					}
					// Return a blank if no non-repeated character found in string s
					return ' ';
				}

				// ------------------------------------------
				// test if string has just one character in upper case

				static bool HasStringOneCharUpperCase(const std::string &text)
				{
					return (std::any_of(std::begin(text), std::end(text), [](char c) {return(isupper(c)); }));
				}

				// ------------------------------------------
				// test if string has just one character in lower case

				static bool HasStringOneCharLowerCase(const std::string &text)
				{
					return (std::any_of(std::begin(text), std::end(text), [](char c) {return(islower(c)); }));
				}

				// ------------------------------------------
				// tests if string has at least one space

				static bool HasStringOneSpace(const std::string & str)
				{
					std::string::size_type len = str.size(), i;
					if (len == 0) return false;
					if (len == 1) return ::isspace(str[0]);

					for (i = 0; i < len; ++i)
					{
						if (!::isspace(str[i])) return false;
					}
					return true;
				}

				// ------------------------------------------
				// check if a token is an hexadecimal 
				// number with the '0x' prefix
				// out is the decimal value

				static bool IsStringHex(const std::string& str, int *out)
				{
					if (str.length() < 3 || str[0] != '0')
						return false;

					if (str[1] != 'x' && str[1] != 'X')
						return false;

					char current;

					for (size_t i = 2; i < str.length(); i++)
					{
						current = str[i];

						if (current >= '0' && current <= '9')  continue;
						if (current >= 'A' && current <= 'F')  continue;
						if (current >= 'a' && current <= 'f')  continue;

						return false;
					}

					if (*out) *out = strtoul(str.c_str(), NULL, 16);

					return true;
				}

				// ------------------------------------------
				// Check if string is alphanumeric

				static bool IsStringAlNum(const std::string &str)
				{
					std::string::size_type len = str.size(), i;
					if (len == 0) return false;
					for (i = 0; i < len; ++i)
						if (!::isalnum(str[i]))	return false;
					return true;
				}

				// ------------------------------------------
				// Check if string is alphabetic

				static bool IsStringLiteral(const std::string &str)
				{
					std::string::size_type len = str.size(), i;
					if (len == 0) return false;
					for (i = 0; i < len; ++i)
						if (!::isalpha((int)str[i])) return false;
					return true;
				}

				// ------------------------------------------
				// Check if string is decimal digit

				static bool IsStringDigit(const std::string &str)
				{
					std::string::size_type len = str.size(), i;
					if (len == 0) return false;
					for (i = 0; i < len; ++i)
						if (!::isdigit(str[i])) return false;
					return true;
				}

				// ------------------------------------------
				// tests if string in binary format is valid
				// no test for whitespaces is done

				static bool IsStringBinary(const std::string &str)
				{
					const char  *p = str.data();
					while (*p != '\0')
						if (((*p) >= '0') && ((*p) <= '1')) p++;
						else return false;
						return true;
				}

				// ------------------------------------------
				// test if string is in lower case
			
				static bool IsStringLower( const std::string & str )
				{
					std::string::size_type len = str.size();
					if ( len == 0 ) return false;
					for ( size_t i = 0; i < len; ++i )
						if ( !::islower( str[i] ) ) return false;
					return true;
				}

				// ------------------------------------------
				// checkf if a string is palindrome

				static bool IsStringPalindrome(const std::string &s,bool casesensitive=false)
				{
					int start = 0;
					int end = int(s.size()) - 1;
			
					if (!casesensitive)
					{
						while (start < end)
						{
							if (s[start] != s[end])
								return false;
							start++;
							end--;
						}
					}
					else if (casesensitive)
					{
						while (start < end)
						{
							if (tolower(s[start]) != tolower(s[end]))
								return false;
							start++;
							end--;
						}
					}
					return true;
				}

				// ------------------------------------------
				// compute levensthein functions for string similarity
				// case sensitive

				static float IsStringSimilarTo(const std::string &s1, const std::string &s2)
				{
					const size_t m=s1.size();
					const size_t n=s2.size();

					if (m == 0 || n == 0) return 0.0f;

					size_t Max = m;
					if (n>Max) Max = n;
				
					size_t *costs = new size_t[Max + 8];	// '8' is for padding, wtf
				
					memset(costs, 0, sizeof(size_t) * (Max + 8));

					for (size_t k = 0; k <= n; k++) costs[k] = k;

					size_t i = 0;

					for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
					{
						costs[0] = i + 1;

						size_t corner = i;

						size_t j = 0;
				
						for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
						{
							size_t upper = costs[j + 1];
					
							if (*it1 == *it2)
							{
								costs[j + 1] = corner;
							}
							else
							{
								size_t t=upper<corner ? upper : corner;
							
								costs[j + 1] = (costs[j]<t ? costs[j] : t) + 1;
							}

							corner = upper;
						}
					}

					size_t result = costs[n];
					delete[] costs;
				
					return 100.0f - 100.0f*(float)result/(float)Max;
				}

				// ------------------------------------------
				//	All first letters are uppercase, and the other letters are lowercase

				static bool IsStringTitle(const std::string & str)
				{
					std::string::size_type len = str.size(), i;

					if (len == 0) return false;
					if (len == 1) return ::isupper(str[0]);

					bool wait_new_word = true; 

					for (i = 0; i < len; ++i)
					{
						if (::isupper(str[i]))
						{
							if (!wait_new_word)
								return false;
							wait_new_word = false;
						}
						else if (::islower(str[i]))
						{
							if (wait_new_word)
								return false;
						}
						else 
						{
							wait_new_word = true;
						}
					}
					return true;
				}

				// ------------------------------------------
				// test if string is in upper case

				static bool IsStringUpper(const std::string & str)
				{
					std::string::size_type len = str.size();
					if (len == 0) return false;
					for (size_t i = 0; i < len; ++i)
						if (!::isupper(str[i])) return false;
					return true;
				}

				// ------------------------------------------
				// cotr / dtor

				StringValidator()
				{}

				~StringValidator()
				{}

			};
	
	} // end of strings namespace

} // end of vml namespace

