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
// string conversion utilities
//
//
////////////////////////////////////////////////////////////////////////////
// documentation
//
//
// static std::string					 BoolToString(bool value)																	// convert bool to string
// static int							 CharToInteger(char c)																		// converts a numeric characther in the 0-128 range to int value
// static std::string					 CharToString(char c) 																		// converts a character to int string
// static const std::string				 CharsToString(int c, ...) 																	// variable number of arguments to string, * terminate with '\0' * or it will crash
// static std::string					 DoubleToString(double value)																// converts double to string
// static std::string					 FloatToString(float value)																	// converts float to string
// static std::string					 FromArray(const char* string, std::size_t length)											// converts a char array to a string
// static bool							 HexCharToValue(unsigned char c, unsigned int *value)										// converts a single hex char to an int value
// static int							 HexStringToValue(const std::string &text)													// return a decimal long from a hexadecimal string must string may be preceeded with 0x or 0X
// static std::string					 IntToString(int value)																		// quicker conversion functions from int to string
// static int							 NumericCharToInteger(char c)																// converts a numeric characther in the 0-9 range to int value
// static int							 NumericIntegerToChar																		// converts a numeric int to a char value
// static const std::string				 OctalToString(int integer) 																// converts an octal interger to a string
// static std::string					 PointerToString(void* p) 																	// converts pointer value into string
// static bool							 StringToBool(const std::string& text)														// convert string to bool
// static int							 StringToInt(const std::string &text)														// quicker conversion functions
// static double						 StringToDouble(const std::string &text)													// converts string to double
// static float							 StringToFloat(const std::string &text)														// converts string to float
// static T								 StringToValue(const std::string &text)														// converts string to value, remember to set precision
// static std::string					 ValueToHexString(unsigned int value)														// Convert decimal number to a hex string in the format 0x00000000
// static std::string					 ValueToString(const T value)																// template function, converts value to string
// static std::string					 UnsignedIntToString(unsigned int value)													// converts an unsigned integer to string

/////////////////////////////////////////////////////////////////

#include <algorithm>
#include <random>

namespace vml
{

	namespace strings
	{

		//////////////////////////////////////////////////////////////////////////////////
		// string conversion class

		class StringConverter
		{
			private:

				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// Conversion from string to value

				//------------------------------------------------------------------------
				// case insensitive compare

				static bool CaseInsensitiveCompare(const char *a, const size_t asize, const char *b,const size_t bsize)
				{
					size_t sz = asize;
					
					if (bsize != sz)
						return false;
					
					for (size_t i = 0; i < sz; ++i)
						if (tolower(a[i]) != tolower(b[i]))
							return false;
					
					return true;
				}
				
				//------------------------------------------------------------------------
				// string conversion templates to unsigned numeric values

				template <typename T>
				static T FromUnsignedDigitsToString(const std::string& s)
				{
					if (s.empty())
						throw std::runtime_error("FromUnsignedDigitsToString : Cannot convert string (1)");

					T result = 0;

					for (char c : s)
					{
						if (!std::isdigit(c))
							throw std::runtime_error("FromUnsignedDigitsToString : Cannot convert string (2)");
		
						const T digit = T(c) - '0';
						
						const T tmp = (result * 10) + digit;
						
						if (result != ((tmp - digit) / 10) || (tmp < result))
							throw std::runtime_error("FromUnsignedDigitsToString : Cannot convert string (3)");
						
						result = tmp;
					}

					return result;
				}

				//------------------------------------------------------------------------
				// string conversion templates to unsigned numeric values

				template <typename T>
				static T UnsignedFromString(std::string s)
				{
					if (s.empty())
						throw std::runtime_error("UnsignedFromString : Cannot convert string");

					if (s[0] == '+')
						s = s.substr(1);

					return FromUnsignedDigitsToString<T>(s);
				}

				//------------------------------------------------------------------------
				// string conversion templates to signed numeric values

				template <typename T>
				static T SignedFromString(std::string s)
				{
					if (s.empty())
						throw std::runtime_error("SignedFromString : Cannot convert string (1)");

					using U = typename std::make_unsigned<T>::type;

					if (s[0] == '-')
					{
						s = s.substr(1);

						const U val = FromUnsignedDigitsToString<U>(s);

						if (val > static_cast<U>(-std::numeric_limits<T>::min()))
							throw std::runtime_error("SignedFromString : Cannot convert string (2)");

						return (-static_cast<T>(val));
					}
					else if (s[0] == '+')
					{
						s = s.substr(1);
					}

					const U val = FromUnsignedDigitsToString<U>(s);

					if (val > std::numeric_limits<T>::max())
						throw std::runtime_error("SignedFromString : Cannot convert string (3)");

					return static_cast<T>(val);
				}

				// ----------------------------------------------------------------------
				// Converts C++ floating-point type to string , specyfing format base.
				//
				// SourceT   :  Template parameter specifying floating-point type.
				// FormatT   :  Template parameter specifying STL stream manipulator, usually
				//              a function pointer: std::ios_base& ( //ios_base_ ) ( std::ios_base& ).
				// value     :  constant reference to C++ object representing
				//              floating-point input variable to convert to STL string.
				// precision :  an integer specifying desired precision.
				// ios_base  :  STL stream manipulator, such as 
				//              std::showpoint  (always show floating point)
				//              std::scientific (e.g. 1.234568e+003)
				//              std::fixed      (no exponent field)
				//              etc.
				//
				// return     : string that contains result of the conversion.

				// example	  : ValueToString<int, std::ios_base::dec, 8>(info);

				template < typename T, std::ios_base::fmtflags fmtflags = std::ios_base::dec, const unsigned int precision = 8>
				static std::string FromValueToString(const T value)
				{
					std::basic_stringstream<char> stream;
					stream.precision(precision);
					stream.setf(fmtflags);
					stream << value;
					if (stream.fail())
						throw std::runtime_error("FromValueToString : Unknown Char");
					return stream.str();
				}

			public:

				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// Conversion from string to value

				//------------------------------------------------------------------------
				// signed

				static signed char			  FromStringToSignedChar(const std::string& s) { return SignedFromString<signed char>(s); }
				static short int			  FromStringToShortInt(const std::string& s) { return SignedFromString<short int>(s); }
				static int					  FromStringToInt(const std::string& s) { return SignedFromString<int>(s); }
				static long int				  FromStringToLongInt(const std::string& s) { return SignedFromString<long int>(s); }
				static long long int		  FromStringToLongLongInt(const std::string& s) { return SignedFromString<long long int>(s); }

				//------------------------------------------------------------------------
				// unsigned

				static unsigned char	   	  FromStringToUnsignedChar(const std::string& s) { return UnsignedFromString<unsigned char>(s); }
				static unsigned short int	  FromStringToUnsignedShortInt(const std::string& s) { return UnsignedFromString<unsigned short int>(s); }
				static unsigned int			  FromStringToUnsignedInt(const std::string& s) { return UnsignedFromString<unsigned int>(s); }
				static unsigned long int	  FromStringToUnsignedLongInt(const std::string& s) { return UnsignedFromString<unsigned long int>(s); }
				static unsigned long long int FromStringToUnsignedLongLongINt(const std::string& s) { return UnsignedFromString<unsigned long long int>(s); }

				//------------------------------------------------------------------------
				// return a decimal long from a hexadecimal string.
				// string may be preceeded with 0x or 0X

				static int FromHexStringToValue(const std::string& text)
				{
					unsigned int num = 0;
					unsigned int bit_shift = 0;
					char c, ch;

					size_t len = text.length();

					char* buffer = (char*)text.data();

					// check if number is written in 0x form

					if (*buffer == '0')
					{
						if (*(buffer + 1) != 'x' && *(buffer + 1) != 'X')
						{
							throw std::runtime_error("HexToString : Cannot convert string hex string to value");
						}
					}

					// convert number

					while (len--)
					{
						c = *(buffer + len);

						ch = 0;

						if (c >= '0' && c <= '9') ch = c - 48;
						if (c >= 'A' && c <= 'F') ch = c - 55;
						if (c >= 'a' && c <= 'f') ch = c - 87;

						if (c != 'x' && c != 'X')
							if (c < 'a' || c>'f')
								if (c < 'A' || c>'F')
									if (c < '0' || c>'9')
										throw std::runtime_error("HexToString : Cannot convert string hex string to value");

						num += (ch << bit_shift);

						bit_shift += 4;
					}

					return num;
				}

				//------------------------------------------------------------------------
				// float 

				static float FromStringToFloat(const std::string& s)
				{
					if (std::any_of(s.begin(), s.end(), [](char c) {return std::isspace(c); }))
						throw std::runtime_error("ToFloat : Cannot convert string");

					std::string::size_type sz;

					float result = {};

					try
					{
						result = std::stof(s, &sz);
					}
					catch (const std::exception&)
					{
						throw std::runtime_error("ToFloat : Cannot convert string");
					}

					if (sz != s.size())
						throw std::runtime_error("ToFloat : Cannot convert string");

					return result;
				}

				//------------------------------------------------------------------------
				// double

				static double FromStringToDouble(const std::string& s)
				{
					if (std::any_of(s.begin(), s.end(), [](char c) {return std::isspace(c); }))
						throw std::runtime_error("ToDouble : Cannot convert string");

					std::string::size_type sz;

					double result = {};

					try
					{
						result = std::stod(s, &sz);
					}
					catch (const std::exception&)
					{
						throw std::runtime_error("ToDouble : Cannot convert string");
					}

					if (sz != s.size())
						throw std::runtime_error("ToDouble : Cannot convert string");

					return result;
				}

				//------------------------------------------------------------------------
				// long double

				static long double FromStringToLongDouble(const std::string& s)
				{
					if (std::any_of(s.begin(), s.end(), [](char c) {return std::isspace(c); }))
						throw std::runtime_error("ToLongDouble : Cannot convert string");

					std::string::size_type sz;

					long double result = {};

					try
					{
						result = std::stold(s, &sz);
					}
					catch (const std::exception&)
					{
						throw std::runtime_error("ToLongDouble : Cannot convert string");
					}

					if (sz != s.size())
						throw std::runtime_error("ToLongDouble : Cannot convert string");

					return result;
				}

				//------------------------------------------------------------------------
				// return a decimal long from a hexadecimal string.
				// string may be preceeded with 0x or 0X

				static int FromStringToHex(const std::string& s)
				{
					unsigned int num = 0;

					unsigned int bit_shift = 0;

					size_t len = s.length();

					char* buffer = (char*)s.data();

					// check if number is written in 0x form

					if (*buffer == '0')
					{
						if (*(buffer + 1) != 'x' && *(buffer + 1) != 'X')
						{
							throw std::runtime_error("ToHex : Unknown Char");
						}
					}

					// convert number

					while (len--)
					{
						char c = *(buffer + len);

						char ch = 0;

						if (c >= '0' && c <= '9') ch = c - 48;
						if (c >= 'A' && c <= 'F') ch = c - 55;
						if (c >= 'a' && c <= 'f') ch = c - 87;

						if (c != 'x' && c != 'X')
							if (c < 'a' || c>'f')
								if (c < 'A' || c>'F')
									if (c < '0' || c>'9')
										throw std::runtime_error("ToHex : Unknown Char");

						num += (ch << bit_shift);

						bit_shift += 4;
					}

					return num;
				}

				//------------------------------------------------------------------------
				// variable number of arguments to string, * terminate with '\0' * or it will crash

				static const std::string FromCharsToString(char c, ...)
				{
					std::string str;
					va_list vargs;
					va_start(vargs, c);
					while (c != '\0')
					{ str += c;
					  c = va_arg(vargs, char); }
					va_end(vargs);
					return str;
				}

				//------------------------------------------------------------------------
				// converts a string into a wide string

				static const std::wstring FromStringToWString(const std::string& s)
				{
					return std::wstring(s.begin(), s.end());
				}

				// ---------------------------------------------------------------------
				// converts pointer value into string

				static std::string FromPointerToString(void* p)
				{
					if (!p)throw std::runtime_error("PointerToString : Cannot convert null pointer");
					std::ostringstream stream;
					stream << std::hex;
					stream << "0x" << p;
					return stream.str();
				}

				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// -----------------------------------------------------------------------
				// Conversion from value to string

				// -----------------------------------------------------------------------
				// convert an unsigned int into a literal string limit is in the million

				static const std::string FromUnsignedIntToEnglish(unsigned int i)
				{
					if (i <= 100)
					{
						switch (i)
						{
							case   0: return "zero";
							case   1: return "one";
							case   2: return "two";
							case   3: return "three";
							case   4: return "four";
							case   5: return "five";
							case   6: return "six";
							case   7: return "seven";
							case   8: return "eight";
							case   9: return "nine";
							case  10: return "ten";
							case  11: return "eleven";
							case  12: return "twelve";
							case  13: return "thirteen";
							case  14: return "fourteen";
							case  15: return "fifteen";
							case  16: return "sixteen";
							case  17: return "seventeen";
							case  18: return "eighteen";
							case  19: return "nineteen";
							case  20: return "twenty";
							case  30: return "thirty";
							case  40: return "fourty";
							case  50: return "fifty";
							case  60: return "sixty";
							case  70: return "seventy";
							case  80: return "eighty";
							case  90: return "ninety";
							case 100: return "one hundred";

							default:
							{
								int tens = i / 10;
								return FromUnsignedIntToEnglish(10 * tens) + " " + FromUnsignedIntToEnglish(i - 10 * tens);
							}
						}
					}

					if (i < 1000)
					{
						int hundreds = i / 100;
						int units = i - 100 * hundreds;
						if (units > 0)
							return FromUnsignedIntToEnglish(hundreds) + " hundred and " + FromUnsignedIntToEnglish(units);
						else
							return FromUnsignedIntToEnglish(hundreds) + " hundred";
					}

					if (i < 1000000)
					{
						int thousands = i / 1000;
						int units = i - 1000 * thousands;
						if (units > 0)
							return FromUnsignedIntToEnglish(thousands) + " thousand " + FromUnsignedIntToEnglish(units);
						else
							return FromUnsignedIntToEnglish(thousands) + " thousand";
					}

					int millions = i / 1000000;
					int units = i - 1000000 * millions;
					if (units > 0)
						return FromUnsignedIntToEnglish(millions) + " million " + FromUnsignedIntToEnglish(units);
					else
						return FromUnsignedIntToEnglish(millions) + " million";

					// returns null string if conversion fails

					return "";
				}

				//------------------------------------------------------------------------
				// Convert decimal number to a hex string in the format 0x00000000

				static const std::string FromHexToString(unsigned int value)
				{
					std::string str = "0X00000000"; //default format 
					char* buffer = (char*)str.data() + str.length();//seekend
					while (value)
					{
						*--buffer = "0123456789ABCDEF"[value % 16];
						value >>= 4;
					}
					return str;
				}

				static std::string FromByteToHex(const unsigned char b)
				{
					static const char hexval[] = "0123456789ABCDEF";
					char ret[3] = { hexval[(b >> 4) & 0xF],hexval[(b >> 0) & 0xF],0 };
					return ret;
				}

				// ----------------------------------------------------
				// get a uuid generated screenname

				static std::string GetUUID(const int digits = 16)
				{
					std::random_device dev;
					std::mt19937 rng(dev());
					std::uniform_int_distribution<int> dist(0, 15);
					const char* v = "0123456789abcdef";
					std::string res = "Object-";
					for (int i = 0; i < digits; i++)
						res += v[dist(rng)];
					return res;
				}

				//------------------------------------------------------------------------
				// signed

				static const std::string FromSignedCharToString(const signed char value) { return FromValueToString< signed char, std::ios_base::dec, 8>(value); }
				static const std::string FromShortIntToString(const short int value) { return FromValueToString< unsigned int, std::ios_base::dec, 8>(value); }
				static const std::string FromIntToString(const int value) { return FromValueToString< int, std::ios_base::dec, 8>(value); }
				static const std::string FromLongIntToString(const long int value) { return FromValueToString< long int, std::ios_base::dec, 8>(value); }
				static const std::string FromLongLongToString(const long long int value) { return FromValueToString< long long int, std::ios_base::dec, 8>(value); }

				//------------------------------------------------------------------------
				// unsigned

				static const std::string FromUnsignedCharToString(const unsigned char value) { return FromValueToString< unsigned char, std::ios_base::dec, 8>(value); }
				static const std::string FromUnsignedShortIntToString(const unsigned short int value) { return FromValueToString< unsigned short int, std::ios_base::dec, 8>(value); }
				static const std::string FromUnsignedIntToString(const unsigned int value) { return FromValueToString< unsigned int, std::ios_base::dec, 8>(value); }
				static const std::string FromUnsignedLongIntToString(const unsigned long int value) { return FromValueToString< unsigned long int, std::ios_base::dec, 8>(value); }
				static const std::string FromUnsignedLongLongToString(const unsigned long long int value) { return FromValueToString< unsigned long long int, std::ios_base::dec, 8>(value); }

				//------------------------------------------------------------------------
				// float

				static const std::string FromFloatToString(const float value) { return FromValueToString< float, std::ios_base::dec, 8>(value); }
				static const std::string FromDoubleToString(const double value) { return FromValueToString< double, std::ios_base::dec, 8>(value); }

				// ----------------------------------------------------------------------
				// ctor / dtor

				StringConverter()
				{}

				~StringConverter()
				{}

		};

	} // ens of namespace strings

} // end of namespace vml