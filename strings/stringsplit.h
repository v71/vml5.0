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

//#include <regex>

namespace vml
{
	namespace strings
	{

		// option parameter string parser

		class RegExStringSplit
		{
			private:

				// ---------------------------------------------------------------------------------
				//

				std::vector<std::string> Tokens;
				std::vector<std::string> StringParms;
				std::string ErrorString;
				bool Initted;

			public:

				// ---------------------------------------------------------------------------------
				//

				bool Split()
				{
					if (!Initted)
					{
						ErrorString = "Splitter not initted";
						return false;
					}

					if (Tokens.size() == 0)
					{
						ErrorString = "No input string";
						return false;
					}

					StringParms.clear();

					size_t curlocation = 0;

					for (;;)
					{

						if (std::regex_match(Tokens[curlocation], std::regex("^[A-Za-z]+$")))
						{
							StringParms.emplace_back(Tokens[curlocation]);

							curlocation++;

							if (curlocation == Tokens.size())
							{

								// sort and erase duplicates

								std::sort(std::begin(StringParms), std::end(StringParms));
								StringParms.erase(std::unique(std::begin(StringParms), std::end(StringParms)), std::end(StringParms));

								return true;
							}
						}
						else
						{
							ErrorString = "Expecting alphanumeric element";
							return false;
						}

						if (Tokens[curlocation] == ",")
						{
							if (curlocation == Tokens.size() - 1)
							{
								ErrorString = "Expecting alphanumeric element";
								return false;
							}
							else
							{
								curlocation++;
							}
						}
					}

					return false;
				}

				// ---------------------------------------------------------------------------------
				//

				void Begin(const std::string &text)
				{
					Initted = true;
					ErrorString = "No error";
					std::string CharsToLex = " ,;\"\n\r\t";
					std::string CharsToPreserve = ",";
					Tokens = vml::strings::lexer::Tokenize(text, CharsToLex, CharsToPreserve, "", "");
				}

				// ---------------------------------------------------------------------------------
				//

				void End()
				{
				}

				// ---------------------------------------------------------------------------------
				//

				const bool IsInitted() const { return Initted; }
				const std::string &GetErrorString() const { return ErrorString; }
				const std::vector<std::string> &GetStringsArray() const { return StringParms; }

				// ---------------------------------------------------------------------------------
				// ctor / dtor

				RegExStringSplit()
				{
					Initted = false;
					ErrorString = "No error";
				}

				RegExStringSplit(const std::string &text)
				{
					Initted = false;
					ErrorString = "No error";
					Begin(text);
					Split();
					End();
				}

				~RegExStringSplit()
				{}

		};
	}
}
