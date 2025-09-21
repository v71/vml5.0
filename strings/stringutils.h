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
// string manipulation utilities
//
//
////////////////////////////////////////////////////////////////////////////
// documentation
//
//
// static const std::string					 BackQuote(const std::string &str)															// adds a backquote to a string
// static const std::string					 CapitalizeFirst(const std::string &text)													// capitalize string ( first character to upper case )
// static const std::string					 CapitalizeAll(const std::string &text)														// capitalize string ( all first character to upper case )
// static const std::string					 CaseSeparatorToUnderscoreSeparator(const std::string &str) 								// replaces next character to a separator with an underscore
// static const std::string					 Center(const std::string& text, int width, char fill = ' ')								// center a string given a width
// static const std::string					 DecapitalizeAll(const std::string &text)													// decapitalize string ( all first character to lower case )
// static const std::string					 DoubleQuote(const std::string &str) 														// surrounds string with double quotes
// static const std::string					 ExpandTabs(const std::string& text, int tabsize = 8)										// expand tabs in string with given width
// static const std::string					 Join(const std::string& separator, C container)											// joins string contained in a templated container
// static const std::string					 LeftJustify(const std::string &text, int width, char fillchar = ' ')						// Left justify a string
// static int								 LenghtOfLongestCommonSubstring(const std::string& str1, const std::string& str2)			// lenght of longest common substring
// static const std::string					 LoadText(const std::string &filename)														// load a text file from disk
// static const std::string					 MakeLower(const std::string &text)															// converts string to lower case
// static char								 MakeLower(const char ch)																	// converts single char to lower case
// static void								 MakeLowerInPlace(std::string &str)															// converts a string to lower in place ( source string is modified )
// static char								 MakeUpper(const char ch)																	// converts single char to upper case
// static conststd::string					 MakeUpper(const std::string &text)															// converts string to upper case
// static void								 MakeUpperInPlace(std::string &str)															// converts a string to upper in place ( source string is modified )
// static std::vector<std::string>			 PermuteString(const std::string s)															// compute all strings permutation with repetitions
// static void								 RemoveSubstrings(std::string& text, const std::string& substring)							// remove all substring occurrences from a string
// static const std::string					 RepeatCharToLength(const char ch, int len) 												// repeat string a 'len' number of times
// static const std::string					 RepeatStringToLength(const std::string &s, size_t len)										// repeat a string a number of 'len' times
// static const std::string					 Replace(const std::string& src,const std::string& oldString, const std::string& newString)	// find a substring in the assigned text and inserts the newString in place of oldString
// static const std::string					 Reverse(std::string &text)																	// reverses string
// static const std::string					 RightJustify(const std::string& text, int width, char fillchar = ' ')						// Right justify a string
// static const std::string					 Rotate(const std::string &text, int times)													// rotate string
// static const std::string					 SaveText(const std::string &filename)														// save a text file to disk
// static const std::string					 SingleQuote(const std::string &str) 														// surrounds a string with wingle quotes
// static const std::string					 Surround(const std::string& str, const std::string& wrap)									// Surrounds a string with another string
// static const std::string					 SwapCase(const std::string &text)															// swap case in string
// static const std::string					 ToEnglish(unsigned int i) 																	// convert an unsigned int into a literal string limit is in the million
// static const std::string					 TrimExtraWhiteSpaces(const std::string &text)												// trims whitespaces leaving words separated by one whitespace
// static const std::string					 TrimExtraWhiteSpacesAndUpper(const std::string &text)										// trims whitespaces leaving words separated by one whitespace and converts to upper case
// static const std::string					 TrimExtraWhiteSpacesAndLower(const std::string &text)										// trims whitespaces leaving words separated by one whitespace and converts to lower case
// static const std::string					 TrimWhitesLeft(const std::string& text)													// remove white spaces on the left of the string
// static const std::string					 TrimWhitesRight(const std::string& text)													// remove white spaces on the right of the string
// static const std::string					 TrimWhites(const std::string &text)														// remove white sapces from the string
// static const std::string					 TrimAndLower(const std::string &text)														// trims string and sets all chars to lower case
// static const std::string					 TrimAndUpper(const std::string &text)														// trims string and sets all chars to upper case
// static const std::string					 Truncate(const std::string &text, const int l)												// truncates string to lenght and replaces the rest with '...'
// static const std::string					 Truncate(const std::string & str, int start, int end)										// truncate a string from start to end
// static const std::string					 UnderscoreSeparatorToCaseSeparator(const std::string &str)									// replaces underscore with the next charater in upper case

#include <algorithm>
#include <random>
#include <fstream>

namespace vml
{
	namespace strings
	{
		
		class StringUtils
		{
			public:
				
			// ----------------------------------------------------------------------
			// adds a backquote to a string

			static const std::string BackQuote(const std::string &text)
			{
				return "`" + text + "`";
			}

			// ----------------------------------------------------------------------
			// replaces next character to a separator with an underscore
			
			static const std::string CaseSeparatorToUnderscoreSeparator(const std::string &text)
			{
				size_t len = text.size();

				std::string str;

				for (size_t i = 0; i < len; ++i)
				{
					if (i > 0 && ::isupper(text[i]))
					{
						str += "_";
						str += tolower(text[i]);
					}
					else
					{
						str += text[i];
					}
				}

				return str;
			}
			
			// ----------------------------------------------------------------------
			// capitalize string ( all first character to upper case )

			static const std::string CapitalizeAll(const std::string &text)
			{
				std::string str = text;
				for (size_t i = 0, isFirst = 1; i < str.size(); ++i)
					str[i] = ::isspace(str[i]) ? (isFirst = 1, str[i]) : isFirst ? (isFirst = 0, ::toupper(str[i])) : ::tolower(str[i]);
				return str;
			}

			// ----------------------------------------------------------------------
			// capitalize string ( first character to upper case )

			static const std::string CapitalizeFirst(const std::string &text)
			{
				std::string str = text;
				str[0] = ::toupper(str[0]);
				return str;
			}

			// ----------------------------------------------------------------------
			// center a string given a width

			static const std::string Center(const std::string& text, int width, char fill = ' ')
			{
				const size_t padding = width - text.length();
				if (padding > 0)
				{
					const size_t before = padding / 2;
					const size_t after = padding - before;
					return std::string(before, fill) + text + std::string(after, fill);
				}
				return text;
			}

			// ----------------------------------------------------------------------
			// decapitalize string ( all first character to lower case )

			static const std::string DecapitalizeAll(const std::string &text)
			{
				std::string str = text;
				for (size_t i = 0, isFirst = 1; i < str.size(); ++i)
					str[i] = ::isspace(str[i]) ? (isFirst = 1, str[i]) : isFirst ? (isFirst = 0, ::tolower(str[i])) : ::toupper(str[i]);
				return str;
			}

			// ----------------------------------------------------------------------
			// surrounds string with double quotes

			static const std::string DoubleQuote(const std::string &text)
			{
				return "\"" + text + "\"";
			}

			// ----------------------------------------------------------------------
			// expand tabs in string with given width

			static const std::string ExpandTabs(const std::string& text, int tabsize = 8)
			{
				std::string str;
				int pos_in_line = 0;
				for (std::string::const_iterator current = text.begin(), end = text.end(); current != end; ++current)
				{
					switch (*current)
					{
					case '\n':
					case '\r':
						str.push_back(*current);
						pos_in_line = 0;
						break;
					case '\t':
						if (tabsize > 0)
						{
							int fillsize = tabsize - (pos_in_line % tabsize);
							str.append(std::string(fillsize, ' '));
							pos_in_line += fillsize;
						}
						break;
					default:
						str.push_back(*current);
						++pos_in_line;
					}
				}
				return str;
			}
			
			// ----------------------------------------------------------------------
			// joins string contained in a templated container

			template<typename C>
			static const std::string Join(const std::string &separator, const C &container)
			{
				std::ostringstream joined;
				typename C::const_iterator current = container.begin(), end = container.end();
				joined << *current;
				for (++current; current != end; ++current)
				{
					joined << separator;
					joined << *current;
				}
				return joined.str();
			}

			// ----------------------------------------------------------------------
			// Left justify a string

			static const std::string LeftJustify(const std::string &text, int width, char fillchar = ' ')
			{
				std::string str = text;
				size_t fillsize = width - str.length();
				if (fillsize > 0) str.append(std::string(fillsize, fillchar));
				return str;
			}

			// ----------------------------------------------------------------------
			// load a text file from disk

			static const std::string LoadText(const std::string &filename)
			{
				std::string str;
				std::ifstream in(vml::strings::StringUtils::MakeLower(filename).c_str(),std::ios::binary);
				if (!in.is_open())
					throw std::runtime_error("StringUtils : Cannot open file : "+ filename+" loading");
				str.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				in.close();
				return str;
			}
			
			// ----------------------------------------------------------------------
			// classic algorithm for computing the longest common substrings between two string
			// using dynamic programming

			static int LenghtOfLongestCommonSubstring(const std::string& str1, const std::string& str2)
			{
				size_t size1 = str1.size();
				size_t size2 = str2.size();
				if (size1 == 0 || size2 == 0) return 0;
				
				std::vector<std::vector<int> > table(size1, std::vector<int>(size2, 0));

				// the start position of substring in original string
				int start1 = -1;
				int start2 = -1;
				// the longest length of common substring 
				int longest = 0;

				// record how many comparisons the solution did;
				// it can be used to know which algorithm is better
				int comparisons = 0;
				for (int j = 0; j < size2; ++j)
				{
					++comparisons;
					table[0][j] = (str1[0] == str2[j] ? 1 : 0);
				}

				for (int i = 1; i < size1; ++i)
				{
					++comparisons;
					table[i][0] = (str1[i] == str2[0] ? 1 : 0);

					for (int j = 1; j < size2; ++j)
					{
						++comparisons;
						if (str1[i] == str2[j])
						{
							table[i][j] = table[size_t(i) - 1][size_t(j) - 1] + 1;
						}
					}
				}

				for (int i = 0; i < size1; ++i)
				{
					for (int j = 0; j < size2; ++j)
					{
						if (longest < table[i][j])
						{
							longest = table[i][j];
							start1 = i - longest + 1;
							start2 = j - longest + 1;
						}
					}
				}

//				std::cout << "(first, second, comparisions) = ("
//					<< start1 << ", " << start2 << ", " << comparisons
//					<< ")" << std::endl;

				return longest;
			}

			// ----------------------------------------------------------------------
			// converts single char to lower case

			static char MakeLower(const char ch)
			{
				return tolower(ch);
			}

			// ----------------------------------------------------------------------
			// converts to lower case

			static const std::string MakeLower(const std::string &text)
			{
				std::string str = text;
				transform(str.begin(), str.end(), str.begin(), tolower);
				return str;
			}

			// ----------------------------------------------------------------------
			// converts to lower case in place 

			static void MakeLowerInPlace(std::string &text)
			{
				std::transform(text.begin(), text.end(), text.begin(), tolower);
			}

			// ----------------------------------------------------------------------
			// converts single char to upper case

			static char MakeUpper(const char ch)
			{
				return toupper(ch);
			}

			// ----------------------------------------------------------------------
			// converts to upper case

			static const std::string MakeUpper(const std::string &text)
			{
				std::string str = text;
				std::transform(str.begin(), str.end(), str.begin(), toupper);
				return str;
			}
			
			// ----------------------------------------------------------------------
			// converts single char to upper case in place

			static void MakeUpperInPlace(std::string &text)
			{
				std::transform(text.begin(), text.end(), text.begin(), toupper);
			}
			
			// ----------------------------------------------------------------------
			// compute all strings permutation

			static const std::vector<std::string> PermuteString(const std::string &text)
			{
				if (text.length() == 0)
					throw std::runtime_error("StringUtils : empty string");

				size_t length = 1;

				for (size_t i = 1; i <= text.length(); ++i)
					length *= i;

				std::vector< std::string > sb;

				sb.resize(length);

				for (size_t i = 0; i < text.length(); i++)
				{

					char ch = text[i];

					size_t times = length / (i + 1);

					for (size_t j = 0; j < times; j++)
					{
						for (size_t k = 0; k < length / times; k++)
						{
							sb[length - 1 - j * length / times - k].insert(k, 1, ch);
						}
					}
				}

				return sb;
			}

			// ----------------------------------------------------------------------
			// find a substring in the assigned text and inserts the newString with oldString
			// if the substring to be replaced is not found, the source line is left untouched

			static const std::string Replace(const std::string &src, const std::string &oldString, const std::string &newString)
			{

				std::string text = src;
				const size_t oldSize = oldString.length();

				// do nothing if line is shorter than the string to find

				if (oldSize > text.length()) return text;
				const size_t newSize = newString.length();
				for (size_t pos = 0;; pos += newSize)
				{
					pos = text.find(oldString, pos);
					if (pos == std::string::npos) return text;
					if (oldSize == newSize)
					{
						// if they're same size, use std::string::replace
						text.replace(pos, oldSize, newString);
					}
					else
					{
						// if not same size, replace by erasing and inserting
						text.erase(pos, oldSize);
						text.insert(pos, newString);
					}
				}
				return text;
			}

			// ----------------------------------------------------------------------
			// remove all substring occurrences from a string

			static const std::string RemoveSubstring(const std::string &str, const std::string& subtext)
			{
				std::string text = str;
				std::string::size_type n = subtext.length();
				for (std::string::size_type i = text.find(subtext); i != std::string::npos; i = text.find(subtext))
					text.erase(i, n);
				return text;
			}

			// ----------------------------------------------------------------------
			// repeat string a 'len' number of times

			static const std::string RepeatCharToLength(const char ch, int len)
			{
				std::string r;
				for (int i = 0; i < len; ++i)
					r.push_back(ch);
				return r;
			}

			// ----------------------------------------------------------------------
			// repeat string a 'len' number of times

			static const std::string RepeatStringToLength(const std::string &text, size_t len)
			{
				std::string r, str = text;
				if ((size_t)len <= text.size())
					throw std::runtime_error("StringUtils : lenght must be geater than source lenght");
				// add all the whole multiples of text.
				for (size_t q = len / text.size(); q > 0; q >>= 1)
				{
					if (q & 1) r += str; // add si to r if the low bit of q is 1
					str += str;			// double si
				}
				r += text.substr(0, len - r.size()); // add any remainder
				return r;
			}

			// ----------------------------------------------------------------------
			// reverses string 

			static const std::string Reverse(const std::string &text)
			{
				std::string str = text;
				std::reverse(str.begin(), str.end());
				return str;
			}
			
			// ----------------------------------------------------------------------
			// Right justify a string

			static const std::string RightJustify(const std::string& text, int width, char fillchar = ' ')
			{
				std::string str;
				size_t fillsize = width - text.length();
				if (fillsize > 0) str = std::string(fillsize, fillchar);
				return str + text;
			}
			
			// ----------------------------------------------------------------------
			// rotate string

			static const std::string Rotate(const std::string &text, int times)
			{
				std::string str = text;
				for (int i = 0; i < times; ++i) std::rotate(str.begin(), str.begin() + 1, str.end());
				return str;
			}

			// ----------------------------------------------------------------------
			// save a text file to disk

			static bool SaveText(const std::string &filename, const std::string &text)
			{
				std::ofstream out(filename.c_str());
				if (!out.is_open())
					throw std::runtime_error("StringUtils : Cannot open file : " + filename + " for writing");
				out << text;
				return true;
			}

			// ----------------------------------------------------------------------
			// surrounds a string with wingle quotes

			static const std::string SingleQuote(const std::string &text)
			{
				return "'" + text + "'";
			}

			// ----------------------------------------------------------------------
			// Surrounds a string with another string.

			static std::string Surround(const std::string& text, const std::string& wrap)
			{
				return wrap + text + wrap;
			}

			// ----------------------------------------------------------------------
			// swap case in string 

			static const std::string SwapCase(const std::string &text)
			{
				std::string str = text;
				std::string::size_type len = str.size(), i;
				for (i = 0; i < len; ++i)
				{
					if (::islower(str[i])) str[i] = (char) ::toupper(str[i]);
					else if (::isupper(str[i])) str[i] = (char) ::tolower(str[i]);
				}
				return str;
			}

			// ----------------------------------------------------------------------
			// trims string and sets all chars to lower case
			
			static const std::string TrimAndLower(const std::string &text)
			{
				std::string str = text;
				transform(str.begin(), str.end(), str.begin(), tolower);
				str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
				return str;
			}
			
			// ----------------------------------------------------------------------
			// trims right trailing whites spaces

			static const std::string TrimWhitesRight(const std::string& text)
			{
				std::string str = text;
				const char* c = str.c_str();
				const char* t = c + str.size() - 1;
				while (c < t && isspace((unsigned char)(*t))) --t;
				str.erase(t - c + 1);
				return str;
			}

			// ----------------------------------------------------------------------
			// trims all whitespaces

			static const std::string TrimWhites(const std::string &text)
			{
				std::string str = text;
				str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
				return str;
			}
			
			// ----------------------------------------------------------------------
			// trims left trailing whites spaces

			static const std::string TrimWhitesLeft(const std::string& text)
			{
				std::string str = text;
				const char* c = str.c_str();
				const char* t = c + str.size();
				while (c < t && isspace((unsigned char)(*c))) ++c;
				str.erase(0, c - str.c_str());
				return str;
			}

			// ----------------------------------------------------------------------
			// trims extra white spaces, leaving words separated by exactly one space

			static const std::string TrimExtraWhiteSpaces(const std::string &text)
			{
				std::string str = text;
				for (size_t i = 0; i<str.length(); i++)
				{
					if (i == 0)
					{
						if (str[i] == ' ')
						{
							str.erase(str.begin(), str.begin() + 1);
							i--;
						}
					}
					else if ((str[i] == ' ') && (str[i + 1] == ' ' || (str[i + 1] == '\0')))
					{
						str.erase(str.begin() + i, str.begin() + i + 1);
						i--;
					}
				}
				return str;
			}

			// ----------------------------------------------------------------------
			// trims extra white spaces, and lowers the strings, leaving words 
			// separated by exactly one space and in lowercase

			static const std::string TrimExtraWhiteSpacesAndLower(const std::string &text)
			{
				std::string str = text;
				std::transform(str.begin(), str.end(), str.begin(), tolower);
				for (size_t i = 0; i<str.length(); i++)
				{
					if (i == 0)
					{
						if (str[i] == ' ')
						{
							str.erase(str.begin(), str.begin() + 1);
							i--;
						}
					}
					else if ((str[i] == ' ') && (str[i + 1] == ' ' || (str[i + 1] == '\0')))
					{
						str.erase(str.begin() + i, str.begin() + i + 1);
						i--;
					}
				}
				return str;
			}
			
			// ----------------------------------------------------------------------
			// trims extra white spaces, and uppers the strings, leaving words 
			// separated by exactly one space and in uppercase

			static const std::string TrimExtraWhiteSpacesAndUpper(const std::string &text)
			{
				std::string str = text;
				std::transform(str.begin(), str.end(), str.begin(), toupper);
				for (size_t i = 0; i<str.length(); i++)
				{
					if (i == 0)
					{
						if (str[i] == ' ')
						{
							str.erase(str.begin(), str.begin() + 1);
							i--;
						}
					}
					else if ((str[i] == ' ') && (str[i + 1] == ' ' || (str[i + 1] == '\0')))
					{
						str.erase(str.begin() + i, str.begin() + i + 1);
						i--;
					}
				}
				return str;
			}

			// ----------------------------------------------------------------------
			// trims string and sets all chars to upper case

			static const std::string TrimAndUpper(const std::string &text)
			{
				std::string str = text;
				transform(str.begin(), str.end(), str.begin(), toupper);
				str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
				return str;
			}

			// ----------------------------------------------------------------------
			// truncate a string to lenght

			static const std::string Truncate(const std::string &text, const int l)
			{
				if (l<0) vml::os::Message::Error("StringUtils : ","lenght must be non negative");
				// constant 3 is related to the number of dots
				// replacing the truncate 'text' part
				if (l>3 && text.size()>size_t(l)-3)
					return text.substr(0, size_t(l) - 3) + "...";
				return text;
			}

			// ----------------------------------------------------------------------
			// truncate a string from start to end

			static const std::string Truncate(const std::string & str, size_t start, size_t end)
			{
				size_t len = str.size();

				if (end > len)
				{
					end = len;
				}
				else if (end < 0)
				{
					end += len;
					if (end < 0)
						end = 0;
				}

				if (start < 0)
				{
					start += len;
					if (start < 0)
						start = 0;
				}

				if (start >= end) return "";

				return str.substr(start, end - start);
			}

			// ----------------------------------------------------------------------
			// replaces underscore with the next charater in upper case

			static const std::string UnderscoreSeparatorToCaseSeparator(const std::string &text)
			{
				size_t len = text.size();

				std::string str;

				for (size_t i = 0; i < len; ++i)
				{
					if (i > 0 && i < len - 1 && text[i] == '_' && (::islower(text[i + 1]) || ::isdigit(text[i + 1])))
					{
						++i;
						str += toupper(text[i]);
					}
					else
					{
						str += text[i];
					}
				}

				return str;
			}

			// ----------------------------------------------------------------------
			// string flattening

			static const std::string Flatten(const std::vector<std::string> &array, size_t start, size_t end)
			{
				std::string str;
				if (end >= array.size()) end = array.size() - 1;
				size_t n = end - 1;
				for (size_t i = start; i < n; ++i)
					str += array[i] + " ";
				str += array[n];
				return str;
			}

			// ----------------------------------------------------------------------
			// string splitting
			// splits a string , into a vector of string separated by a delimiter

			static std::vector<std::string> SplitString(const std::string& s, const std::string& delimiter)
			{
				size_t pos_start, pos_end, delim_len;
				std::string token;
				std::vector<std::string> res;
				pos_start = 0;
				delim_len = delimiter.length();
				while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
				{
					token = s.substr(pos_start, pos_end - pos_start);
					pos_start = pos_end + delim_len;
					res.push_back(token);
				}
				res.push_back(s.substr(pos_start));
				return res;
			}
				
			// ----------------------------------------------------------------------
			// generate a random string

			static std::string GenRandom(const int len)
			{
				static const char alphanum[] = "0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";
				std::string tmp_s;
				tmp_s.reserve(len);
				for (int i = 0; i < len; ++i)
					tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
				return tmp_s;
			}

	public:

			// ----------------------------------------------------------------------
			// cotr / dtor

			StringUtils()
			{}

			~StringUtils()
			{}

		};
	
	} // end of strings namespace

} // end of vml namespace

