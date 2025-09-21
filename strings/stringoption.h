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

#include <regex>

namespace vml
{
	namespace strings
	{

		// option parameter string parser

		class StringOptionParser
		{
			private:

				// ---------------------------------------------------------------------------------

				std::vector<std::string> Tokens;
				std::string ErrorString;
				std::vector<double> NumericParms;
				std::vector<std::string> StringParms;
				std::string VariableString;
				int ParmsType;
				bool Initted;

				// ---------------------------------------------------------------------------------

				bool ParseNumericElement(const std::string &text)
				{
					double parm;

					if (std::regex_match(text, std::regex("[+-]?[0-9]+")))
					{
						std::stringstream ss(text);
						ss >> parm;
						NumericParms.emplace_back(parm);
						return true;
					}

					if (std::regex_match(text, std::regex("[+]?[0-9]+")))
					{
						std::stringstream ss(text);
						ss >> parm;
						NumericParms.emplace_back(parm);
						return true;
					}

					if (std::regex_match(text, std::regex("[+-]?[0-9]+[.]?[0-9]+")))
					{
						std::stringstream ss(text);
						ss >> parm;
						NumericParms.emplace_back(parm);
						return true;
					}

					if (std::regex_match(text, std::regex("[+]?[0-9]+[.]?[0-9]+")))
					{
						std::stringstream ss(text);
						ss >> parm;
						NumericParms.emplace_back(parm);
						return true;
					}

					return false;
				}

				// ---------------------------------------------------------------------------------

				bool ParseStringElement(const std::string &text)
				{
					if (std::regex_match(text, std::regex("^[a-zA-Z0-9_]*$")))
					{
						StringParms.emplace_back(text);
						return true;
					}

					return false;
				}

				// ---------------------------------------------------------------------------------
				//

				std::string IntToString(int value)
				{
					std::stringstream ss;
					ss << value;
					return ss.str();
				}

				// ---------------------------------------------------------------------------------
				// case insensitive compare

				bool CaseInsensitiveCompare(const std::string& a, const std::string& b)
				{
					size_t sz = a.size();
					if (b.size() != sz)
						return false;
					for (size_t i = 0; i < sz; ++i)
						if (tolower(a[i]) != tolower(b[i]))
							return false;
					return true;
				}

			public:

				// ---------------------------------------------------------------------------------

				static const unsigned int		ALLOW_NUMBERS = 0;
				static const unsigned int		ALLOW_STRINGS = 1;

				// ---------------------------------------------------------------------------------

				bool ParseStringLocation(const std::string &text, int allowedparms, int parmscount, std::initializer_list<std::string> s = {})
				{
					if (!Initted)
					{
						ErrorString = "Parser not initted";
						return false;
					}

					if (parmscount <= 0)
					{
						ErrorString = "Parameters must be greater than 0";
						return false;
					}

					if (allowedparms != ALLOW_NUMBERS && allowedparms != ALLOW_STRINGS)
					{
						ErrorString = "Only string or numeric values are accepted";
						return false;
					}

					int variablelocation = -1;

					// find text location in the tokens vector

					for (size_t i = 0; i < Tokens.size() && variablelocation == -1; ++i)
						if (Tokens[i] == text) variablelocation = int(i);

					int curparms = 0;

					if (variablelocation != -1)
					{
						VariableString = Tokens[variablelocation];

						ParmsType = allowedparms;

						ErrorString = VariableString + " : No Error";

						NumericParms.clear();
						StringParms.clear();

						// we have found our string of interest
						// search for '=' symbol

						int assignlocation = variablelocation + 1;

						// look for '=' symbol

						if (assignlocation == Tokens.size())
						{
							ErrorString = VariableString + " : Expecting '=' symbol";
							return false;
						}

						size_t n = Tokens.size();

						// we have found the '=' symbol

						int paramlocation = assignlocation + 1;

						if (paramlocation == n)
						{
							if (ParmsType == ALLOW_NUMBERS)
								ErrorString = VariableString + " : Expecting numeric value";
							if (ParmsType == ALLOW_STRINGS)
								ErrorString = VariableString + " : Expecting string value";
							return false;
						}

						bool eol = false;

						do
						{
							if (ParmsType == ALLOW_NUMBERS)
							{
								if (ParseNumericElement(Tokens[paramlocation]))
								{
									// check parameters are of the correct number

									if (curparms > parmscount)
									{
										ErrorString = VariableString + " : This variable requires " + IntToString(parmscount) + " elements";
										return false;
									}

									curparms++;
								}
								else
								{
									ErrorString = VariableString + " : Expecting numeric value, found : " + Tokens[paramlocation];
									return false;
								}
							}
							else if (ParmsType == ALLOW_STRINGS)
							{
								if (ParseStringElement(Tokens[paramlocation]))
								{
									// check parameters are of the correct number

									if (curparms > parmscount)
									{
										ErrorString = VariableString + " : This variable requires " + IntToString(parmscount) + " elements";
										return false;
									}

									curparms++;
								}
								else
								{
									ErrorString = VariableString + " : Expecting string value, found : " + Tokens[paramlocation];
									return false;
								}
							}

							int nextlocation = paramlocation + 1;

							if (nextlocation == n)
							{
								ErrorString = VariableString + " : Expecting comma or semicolon";
								return false;
							}

							if (Tokens[nextlocation] == ",")
							{
								paramlocation++;

								if (paramlocation == n)
								{
									if (ParmsType == ALLOW_NUMBERS)
										ErrorString = VariableString + " : Expecting numeric value";
									else if (ParmsType == ALLOW_STRINGS)
										ErrorString = VariableString + " : Expecting string value";
									return false;
								}

								paramlocation++;

								if (paramlocation == n)
								{
									if (ParmsType == ALLOW_NUMBERS)
										ErrorString = VariableString + " : Expecting numeric value";
									else if (ParmsType == ALLOW_STRINGS)
										ErrorString = VariableString + " : Expecting string value";
									return false;
								}

							}

							if (Tokens[nextlocation] == ";")
							{
								eol = true;
							}

						} while (eol == false);

						// check if parms are allowed according to the initilizer list

						if (ParmsType == ALLOW_STRINGS)
						{
							if (s.size() == 0)
								return true;

							bool found = false;

							for (size_t i = 0; i < StringParms.size(); ++i)
							{
								found = false;

								for (auto it = s.begin(); it != s.end() && !found; ++it)
								{
									if (CaseInsensitiveCompare(StringParms[i], *it))
										found = true;
								}
							}

							if (!found)
							{
								ErrorString = VariableString + " : No accepted values found";
								return false;
							}

						}

						return true;
					}

					// string has not been found

					ErrorString = "Missing " + text + " using default";

					return false;
				}

				// ---------------------------------------------------------------------------------

				void Begin(const std::string &text)
				{
					Initted = true;
					ParmsType = -1;
					ErrorString = "No error";
					std::string CharsToLex = " ,;=\"\n\r\t";
					std::string CharsToPreserve = ",;=\"\n\r\t";
					Tokens = vml::strings::lexer::Tokenize(text, CharsToLex, CharsToPreserve, "", "");
				}

				// ---------------------------------------------------------------------------------

				void End()
				{
				}

				// ---------------------------------------------------------------------------------

				const std::string &GetErrorString() const
				{
					return ErrorString;
				}

				const bool IsInitted() const
				{
					return Initted;
				}

				const std::string &GetVariableString() const
				{
					return VariableString;
				}

				const std::vector<double> &GetNumericParms()
				{
					return NumericParms;
				}

				const std::vector<std::string> &GetStringParms()
				{
					return StringParms;
				}

				const int GetParmsType() const
				{
					return ParmsType;
				}

				// ---------------------------------------------------------------------------------
				// ctor / dtor

				StringOptionParser()
				{
					Initted = false;
					ParmsType = -1;
					ErrorString = "No error";
				}

				~StringOptionParser()
				{}

		};

	}
}
