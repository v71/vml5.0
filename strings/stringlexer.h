#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2015 v71 
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

#include <charconv>

namespace vml
{

	namespace strings
	{

		namespace lexer
		{

			///////////////////////////////////////////////////////////////////////////
			// Function to break up a string into tokens
			//
			// Parameters:
			//
			// str = the input string that will be tokenized
			// result = the tokens for str
			// delimiters = the delimiter characters
			// delimiters preserve = same as above, but the delimiter characters
			// will be put into the result as a token
			// quote = characters to protect between the enclosed characters
			// esc = characters to protect between a single character

			static std::vector<std::string> Tokenize( const std::string& str,
													  const std::string& delimiters,
													  const std::string& delimiters_preserve = "",
													  const std::string& quote = "\"",
													  const std::string& esc = "\\" )
			{
				// clear the vector

				std::vector<std::string> result;

				std::string::size_type pos = 0;			    // the current position (char) in the string
				char ch = 0;							    // buffer for the current character
				char delimiter = 0;						    // the buffer for the delimiter char which
															// will be added to the tokens if the delimiter
															// is preserved
				char current_quote = 0;					    // the char of the current open quote
				bool quoted = false;					    // indicator if there is an open quote
				std::string token;							// string buffer for the token
				bool token_complete = false;			    // indicates if the current token is

				// read to be added to the result vector

				std::string::size_type len = str.length();  // length of the input-string

				// for every char in the input-string

				while (len > pos)
				{

					// get the character of the string and reset the delimiter buffer

					ch = str.at(pos);
					delimiter = 0;

					// assume ch isn't a delimiter

					bool add_char = true;

					// check ...
					// ... if the delimiter is an escaped character

					bool escaped = false; // indicates if the next char is protected

					if (false == esc.empty()) // check if esc-chars are  provided
					{
						if (std::string::npos != esc.find_first_of(ch))
						{
							// get the escaped char

							++pos;

							if (pos < len) // if there are more chars left
							{
								// get the next one
								ch = str.at(pos);

								// add the escaped character to the token
								add_char = true;
							}
							else // cannot get any more characters
							{
								// don't add the esc-char
								add_char = false;
							}

							// ignore the remaining delimiter checks
							escaped = true;
						}
					}

					// ... if the delimiter is a quote

					if (false == quote.empty() && false == escaped)
					{
						// if quote chars are provided and the char isn't protected

						if (std::string::npos != quote.find_first_of(ch))
						{
							// if not quoted, set state to open quote and set
							// the quote character

							if (false == quoted)
							{
								quoted = true;
								current_quote = ch;

								// don't add the quote-char to the token
								add_char = false;
							}
							else // if quote is open already
							{
								// check if it is the matching character to close it

								if (current_quote == ch)
								{
									// close quote and reset the quote character
									quoted = false;
									current_quote = 0;

									// don't add the quote-char to the token
									add_char = false;
								}
							} // else
						}
					}

					// ... if the delimiter isn't preserved

					if (false == delimiters.empty() && false == escaped && false == quoted)
					{
						// if a delimiter is provided and the char isn't protected by
						// quote or escape char

						if (std::string::npos != delimiters.find_first_of(ch))
						{
							// if ch is a delimiter and the token string isn't empty
							// the token is complete

							if (false == token.empty()) // BUGFIX: 2006-03-04
							{
								token_complete = true;
							}

							// don't add the delimiter to the token
							add_char = false;
						}
					}

					// ... if the delimiter is preserved - add it as a token

					bool add_delimiter = false;

					if (false == delimiters_preserve.empty() && false == escaped && false == quoted)
					{
						// if a delimiter which will be preserved is provided and the
						// char isn't protected by quote or escape char
						if (std::string::npos != delimiters_preserve.find_first_of(ch))
						{
							// if ch is a delimiter and the token string isn't empty
							// the token is complete
							if (false == token.empty()) // BUGFIX: 2006-03-04
							{
								token_complete = true;
							}

							// don't add the delimiter to the token
							add_char = false;

							// add the delimiter
							delimiter = ch;
							add_delimiter = true;
						}
					}

					// add the character to the token

					if (true == add_char)
					{
						// add the current char
						token.push_back(ch);
					}

					// add the token if it is complete

					if (true == token_complete && false == token.empty())
					{
						// add the token string
						result.emplace_back(token);

						// clear the contents
						token.clear();

						// build the next token
						token_complete = false;
					}

					// add the delimiter

					if (true == add_delimiter)
					{
						// the next token is the delimiter
						std::string delim_token;
						delim_token.push_back(delimiter);
						result.emplace_back(delim_token);
					}

					// repeat for the next character
					++pos;

				} // while

				return result;
			}

			// -----------------------------------------------------------------
			// split text along lines

			static std::vector<std::string> SplitStringInLines(const std::string &text)
			{
				std::vector<std::string> tokens;
				tokens = vml::strings::lexer::Tokenize(text, "\n\r\t", "", "", "");
				return tokens;
			}

			////////////////////////////////////////////////////////
			// enums used by the lexer

			enum BasicTokenEnum : long long
			{

				// built in token

				TOKEN_EOL = 0,					// end of line
				TOKEN_CR,						// end of line ( carriage return )
				TOKEN_TAB,						// horizontal tab
				TOKEN_BACKSPACE,
				TOKEN_EOF,						// end of file
				TOKEN_UNKNOWN,		    		// unkown 
				TOKEN_STRING,					// terminal token
				TOKEN_INT,						// integer token
				TOKEN_FLOAT,					// float token
				TOKEN_HEX,						// hex token						

				// built in math operators

				TOKEN_PLUS,		   				// operator +
				TOKEN_MINUS,	     			// operator -
				TOKEN_MUL,		     			// operator *
				TOKEN_DIV,		     			// operator /
				TOKEN_ASSIGN,					// operator =
				TOKEN_AND,						// operator &
				TOKEN_OR,						// operator |
				TOKEN_NOT,						// operator !
				TOKEN_MOD,						// operator %
				TOKEN_XOR,						// operator ^
				TOKEN_LESSER_THAN,				// < operator
				TOKEN_GREATER_THAN,				// > operator
				TOKEN_TILDE,					// ~ operator

				// symbols 

				TOKEN_AT,						// @
				TOKEN_UNDERSCORE,				// _
				TOKEN_QUOTE,					// "
				TOKEN_ACCENT,					// ' 
				TOKEN_DOT,						// .
				TOKEN_COMMA,					// ,
				TOKEN_SEMICOLON,				// ;
				TOKEN_COLON,					// :
				TOKEN_QUESTION,					// ?
				TOKEN_NUMBER,					// # 
				TOKEN_BACKSLASH,				// \
														
				// brackets

				TOKEN_ROUND_OPENED,   			// bracket (
				TOKEN_ROUND_CLOSED,   			// bracket )
				TOKEN_SQUARE_OPENED,  			// bracket [
				TOKEN_SQUARE_CLOSED,  			// bracket ]
				TOKEN_CURLY_OPENED,				// bracket {
				TOKEN_CURLY_CLOSED,				// bracket }

			};

			////////////////////////////////////////////////////////////////////////////
			// lexeme, stores token name and metadata

			class CLexeme
			{

				private:

					std::string		Token;			// string contatining the token
					int				IValue;			// value for numeric int token
					double		    FValue;			// value for numeric float token 
					int				TokenType;		// token type
					std::string		Synopsys;		// explains what the token is
					int				Line;			// line where the token is found in the source code

				public:

					// ---------------------------------------------------------------

					int					GetInt() const { return IValue; }
					double				GetFloat() const { return FValue; }
					int					GetTokenType() const { return TokenType; }
					const std::string  &GetToken() const { return Token; }
					const std::string  &GetSynopsysString() const { return Synopsys; }
					int					GetLine() const { return Line; }
					bool				IsString() const { return TokenType == TOKEN_STRING; }

					// ---------------------------------------------------------------

					bool IsNumeric() const
					{
						return (TokenType == TOKEN_INT ||
								TokenType == TOKEN_FLOAT ||
								TokenType == TOKEN_HEX);
					}

					// ---------------------------------------------------------------

					bool IsOperand() const
					{
						return (TokenType == TOKEN_PLUS ||
								TokenType == TOKEN_MINUS ||
								TokenType == TOKEN_MUL ||
								TokenType == TOKEN_DIV ||
								TokenType == TOKEN_XOR ||
								TokenType == TOKEN_MOD ||
								TokenType == TOKEN_AND ||
								TokenType == TOKEN_OR ||
								TokenType == TOKEN_NOT ||
								TokenType == TOKEN_LESSER_THAN ||
								TokenType == TOKEN_GREATER_THAN ||
								TokenType == TOKEN_TILDE);
					}

					// ---------------------------------------------------------------

					const double GetNumericValue() const
					{
						switch (TokenType)
						{
							case TOKEN_INT: return (double)(IValue);
							case TOKEN_HEX: return (double)(IValue);
							case TOKEN_FLOAT: return (double)(FValue);
						}
					
						return std::numeric_limits<double>::quiet_NaN();
					}

					// -----------------------------------------------------------
					// construcor, destructor

					CLexeme() : TokenType(TOKEN_UNKNOWN),
								IValue(0),
								FValue(0.0f),
								Token("Unknown"),
								Synopsys("Unknown"),
								Line(0)
					{
					}

					CLexeme(const int tokentype,
							const int ivalue,
							const double fvalue,
							const std::string &token,
							const std::string& synopsys, const int line) :  TokenType(tokentype),
																			IValue(ivalue),
																			FValue(fvalue),
																			Token(token),
																			Synopsys(synopsys),
																			Line(line)
					{
					}

					~CLexeme()
					{
					}

			};
			
			/////////////////////////////////////////////////////////////////////
			// lexer class , tokenizes the input and classifies each token

			class CLexer
			{

				private:

					// ---------------------------------------------------------------

					std::unordered_map< std::string, CLexeme >	LexemeBank;
					std::string									LastError;
					int											LineNo;
					std::string									Source;
					std::string									Output;
					std::string									FileName;
					unsigned int								FileSizeInBytes;
					vml::os::Timer								Timer;
					float										LoadingTime;
					float										LexingTime;
					std::string									CharsToLex;
					std::string									CharsToPreserve;
					bool										SkipWhites;
					unsigned int								Flags;
				
					// -------------------------------------------------------------------------
					// check if token is a number or is a n hex format

					bool CheckNumericToken(const std::string &str, double *out)
					{
						// -----------------------------------------------------------
						// check if a token is an hexadecimal 
						// number with the '0x' prefix
						// out is the decimal value

						if (str.size() > 3)
						{
							if (str[0] == '0')
							{
								if (str[1] == 'x' || str[1] == 'X')
								{
									for (size_t i = 2; i < str.size(); i++)
									{
										char current = str[i];
										if (current >= '0' && current <= '9')  continue;
										if (current >= 'A' && current <= 'F')  continue;
										if (current >= 'a' && current <= 'f')  continue;
										return false;
									}

									*out = strtoul(str.c_str(), NULL, 16);
									
									return true;
								}
							}
						}

						double value = 0;
						const auto format = std::chars_format::general;
						const auto res = std::from_chars(str.data(),str.data() + str.size(),value,format);
						if (res.ec == std::errc()) { *out = value; return true; }
						return false;
					}

					// -----------------------------------------------------------------
					// check if token is an operator

					bool CheckTerminals(std::vector<std::string> &tokens, size_t &it)
					{
						LastError = "No error";

						// reached eof ?

						if (it == tokens.size()) return true;

						// check for known operators

						std::unordered_map< std::string, CLexeme >::iterator jt;

						jt = LexemeBank.find(tokens[it]);

						if (jt == LexemeBank.end())
						{
							// if we don't find the current
							// token among the known lexemes
							// store it as a terminal
							// store the string as a lexeme
							
							if (!IsAllStrings())
							{

								double fvalue = -FLT_MAX;

								if (CheckNumericToken(tokens[it], &fvalue))
								{
									if (fvalue != floor(fvalue))
										Lexemes.emplace_back(CLexeme(TOKEN_FLOAT, 0, fvalue, tokens[it], "Double", LineNo));
									else
										Lexemes.emplace_back(CLexeme(TOKEN_INT, (int)fvalue, 0, tokens[it], "Integer", LineNo));
								}
								else
								{
									Lexemes.emplace_back(CLexeme(TOKEN_STRING, 0, 0, tokens[it], "String", LineNo));
								}
							}
							else
							{
								Lexemes.emplace_back(CLexeme(TOKEN_STRING, 0, 0, tokens[it], "String", LineNo));
							}

							it++;

							return true;
						}

						// if we get here , we need to store the new lexeme
						// we have just found

						CLexeme l((*jt).second.GetTokenType(),
								  (*jt).second.GetInt(),
								  (*jt).second.GetFloat(),
								  (*jt).second.GetToken(),
								  (*jt).second.GetSynopsysString(),
								  LineNo);

						if (SkipWhites)
						{
							if ((*jt).second.GetTokenType() != TOKEN_EOL &&
								(*jt).second.GetTokenType() != TOKEN_TAB &&
								(*jt).second.GetTokenType() != TOKEN_CR)
							{
								Lexemes.push_back(l);
							}
						}
						else if (!SkipWhites)
						{
							Lexemes.emplace_back(l);
						}

						// advance line if a '\n' or '\r' has been encountered

						if (l.GetTokenType() == TOKEN_EOL || l.GetTokenType() == TOKEN_CR)
							LineNo++;

						it++;

						return true;
					}

					// -----------------------------------------------------------
					// given the string token, return the lexeme

					CLexeme *GetLexeme(const std::string &token)
					{
						std::unordered_map< std::string, CLexeme >::iterator jt;
						jt = LexemeBank.find(token);
						if (jt != LexemeBank.end())
							return &((*jt).second);
						return nullptr;
					}

					// -----------------------------------------------------------
					// given the string token, return the lexeme
					
					const std::string RemoveCommentsFromSourceFile0(const std::string &source)
					{
						std::string text = source;
						
						while (text.find("/*") != std::string::npos)
						{
							size_t pos = text.find("/*");
							text.erase(pos, (text.find("*/", pos) - pos) + 2);
						}

						while (text.find("//") != std::string::npos)
						{
							size_t pos = text.find("//");
							text.erase(pos, text.find("\n", pos) - pos);
						}
						
						return text;

					}
		
					const std::string RemoveCommentsFromSourceFile(const std::string &prgm)
					{
						size_t n = prgm.length();
						
						std::string res;

						// Flags to indicate that single line and multiple line comments
						// have started or not.
						
						bool singlecomment = false;
						bool multicommnt = false;

						// Traverse the given program
						
						for (size_t i = 0; i < n; i++)
						{
							// If single line comment flag is on, then check for end of it

							if (singlecomment && prgm[i] == '\n')
								singlecomment = false;

							// If multiple line comment is on, then check for end of it

							else if (multicommnt && prgm[i] == '*' && prgm[i + 1] == '/')
								multicommnt = false, i++;

							// If this character is in a comment, ignore it

							else if (singlecomment || multicommnt)
								continue;

							// Check for beginning of comments and set the appropriate flags

							else if (prgm[i] == '/' && prgm[i + 1] == '/')
							{
								singlecomment = true;
								i++;
							}

							else if (prgm[i] == '/' && prgm[i + 1] == '*')
							{
								multicommnt = true;
								i++;
							}

							// If current character is a non-comment character, append it to res
							
							else  res += prgm[i];
						}

						return res;
					}

				// -----------------------------------------------------------------
				// add end of line lexeme

				void InstallKeyWord(const std::string &token, int tokentype, const std::string &sinopsys)
				{
					// check if lexeme is already in bank
					if (LexemeBank.find(token) != LexemeBank.end())
						return;
					// create lexeme
					LexemeBank[token] = CLexeme(tokentype, 0, 0.0f, token, sinopsys, 0);
				}

				// -----------------------------------------------------------------
				// truncate a string to lenght

				const std::string Truncate(const std::string &text, const int l)
				{
					if (l<=3) 
						throw std::runtime_error("lenght must be greather than 3");
					// constant 3 is related to the number of dots
					// replacing the truncate 'text' part
					if (text.size()>( size_t(l) - 3))
					{
						size_t n = text.size();
						if (text[n - 1] != ' ' && text[n - 2] != ' ' && text[n - 3] != ' ')
							return text.substr(0, long(l) - 3) + "...";
						else
							return text.substr(0, l);
					}
					return text;
				}

				// -----------------------------------------------------------------
				// dump lexer results in the output string
				
				void OutputToString()
				{
					
					if ( IsVerbose() )
					{
					
						std::vector<CLexeme>::iterator it;
						std::string tokentype;
						std::string tokensize;
						std::string tokenline;
						std::string tokenvalue;
						std::string tokensinopsys;
						std::string tokenstring;
						std::string text[6];
						int colsize = 20;

						Output = vml::strings::StringFormat::Text("Lexer : Loaded ' {0} ' in {1} millisecs ( {2} bytes )\n",
																 FileName.c_str(), LoadingTime, Source.size());
				
						Output += "\nLexer : Started...\n\n";
					
						text[0] = "Token"       + vml::strings::StringUtils::RepeatCharToLength(' ', colsize -  5);
						text[1] = "Type"        + vml::strings::StringUtils::RepeatCharToLength(' ', colsize -  4);
						text[2] = "Value"       + vml::strings::StringUtils::RepeatCharToLength(' ', colsize -  5);
						text[3] = "Lexcode"     + vml::strings::StringUtils::RepeatCharToLength(' ', colsize -  7);
						text[4] = "Size(Chars)" + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - 11);
						text[5] = "Line\n\n";

						Output += text[0] + text[1] + text[2] + text[3] + text[4] + text[5];
						
						for (it = Lexemes.begin(); it != Lexemes.end(); ++it)
						{
							
							tokentype	  = vml::strings::StringFormat::Text("{0}", (*it).GetTokenType());
							tokensize	  = vml::strings::StringFormat::Text("{0}", (*it).GetToken().size());
							tokenline	  = vml::strings::StringFormat::Text("{0}", (*it).GetLine());
							tokensinopsys = (*it).GetSynopsysString();
							tokenstring   = (*it).GetToken();
							
							switch ((*it).GetTokenType())
							{
								case TOKEN_INT:    tokenvalue = vml::strings::StringFormat::Text("{0}", (*it).GetInt()); break;
								case TOKEN_FLOAT:  tokenvalue = vml::strings::StringFormat::Text("{0}", (*it).GetFloat()); break;
								case TOKEN_HEX:    tokenvalue = vml::strings::StringFormat::Text("{0}", (*it).GetInt()); break;
								default:		   tokenvalue = "None"; break;
							}

							if ((*it).GetTokenType() != TOKEN_EOL &&
								(*it).GetTokenType() != TOKEN_CR &&
								(*it).GetTokenType() != TOKEN_TAB)
							{
								Output += Truncate(tokenstring   + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenstring.size()))  , colsize)+
										  Truncate(tokensinopsys + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensinopsys.size())), colsize)+
										  Truncate(tokenvalue    + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenvalue.size()))   , colsize)+
										  Truncate(tokentype     + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokentype.size()))    , colsize)+
										  Truncate(tokensize     + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensize.size()))    , colsize)+
										  Truncate(tokenline, colsize)+ "\n";
							}
							else
							{
								
								switch ((*it).GetTokenType())
								{

									case TOKEN_EOL:

										Output += Truncate("EOL" + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - 3), colsize) +
												  Truncate(tokensinopsys + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensinopsys.size())), colsize) +
												  Truncate(tokenvalue + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenvalue.size())), colsize) +
												  Truncate(tokentype + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokentype.size())), colsize) +
												  Truncate(tokensize + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensize.size())), colsize) +
												  Truncate(tokenline, colsize) + "\n";
									break;
									
									case TOKEN_CR:

										Output += Truncate("CR" + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - 2), colsize) +
												  Truncate(tokensinopsys + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensinopsys.size())), colsize) +
												  Truncate(tokenvalue + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenvalue.size())), colsize) +
												  Truncate(tokentype + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokentype.size())), colsize) +
												  Truncate(tokensize + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensize.size())), colsize) +
												  Truncate(tokenline, colsize) + "\n";
									break;

									case TOKEN_TAB:

										Output += Truncate("TAB" + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - 3), colsize) +
												  Truncate(tokensinopsys + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensinopsys.size())), colsize) +
												  Truncate(tokenvalue + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenvalue.size())), colsize) +
												  Truncate(tokentype + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokentype.size())), colsize) +
												  Truncate(tokensize + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensize.size())), colsize) +
												  Truncate(tokenline, colsize) + "\n";
									break;
								
									case TOKEN_BACKSPACE:

										Output += Truncate("BKSPC" + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - 3), colsize) +
												  Truncate(tokensinopsys + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensinopsys.size())), colsize) +
												  Truncate(tokenvalue + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokenvalue.size())), colsize) +
												  Truncate(tokentype + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokentype.size())), colsize) +
												  Truncate(tokensize + vml::strings::StringUtils::RepeatCharToLength(' ', colsize - int(tokensize.size())), colsize) +
												  Truncate(tokenline, colsize) + "\n";
										break;

								}
								
							}
							
						}
					
						Output += vml::strings::StringFormat::Text("\nLexer : Ended in {0} millisecs\n\nLexer : {1}\n",LexingTime, LastError.c_str());
					
					}
					else
					{
						Output = "Lexer is set on quiet mode : no output available.\n";
					}
					
				}

				// -----------------------------------------------------------------

				long GetFileSize(const char *filename)
				{
					struct stat stat_buf;
					int rc = stat(filename, &stat_buf);
					return rc == 0 ? stat_buf.st_size : -1;
				}

				// -----------------------------------------------------------------
				// functions

				void InstallLexemes()
				{
					// built in operators

					InstallKeyWord("+",   TOKEN_PLUS,		   "Operator plus");
					InstallKeyWord("-",   TOKEN_MINUS,		   "Operator minus");
					InstallKeyWord("*",   TOKEN_MUL,		   "Operator mul");
					InstallKeyWord("/",   TOKEN_DIV,		   "Operator div");
					InstallKeyWord("=",   TOKEN_ASSIGN,		   "Operator assign");
					InstallKeyWord("<",   TOKEN_LESSER_THAN,   "Operator <");
					InstallKeyWord(">",   TOKEN_GREATER_THAN,  "Operator >");
					InstallKeyWord("&",   TOKEN_AND,		   "Operator and");
					InstallKeyWord("!",   TOKEN_NOT,		   "Operator not");
					InstallKeyWord("|",   TOKEN_OR,			   "Operator or");
					InstallKeyWord("%",   TOKEN_MOD,		   "Operator mod");
					InstallKeyWord("^",   TOKEN_XOR,		   "Operator xor");
					InstallKeyWord("~",   TOKEN_TILDE,		   "Operator tilde");

					// built in symbols

					InstallKeyWord("@",   TOKEN_AT,            "At symbol");
					InstallKeyWord("\"",  TOKEN_QUOTE,         "Quote symbol");
					InstallKeyWord(".",   TOKEN_DOT,           "Dot symbol");
					InstallKeyWord("'",   TOKEN_ACCENT,        "Accent symbol");
					InstallKeyWord("’",   TOKEN_ACCENT,		   "Accent symbol");
					InstallKeyWord(",",   TOKEN_COMMA,         "Comma symbol");
					InstallKeyWord(":",   TOKEN_COLON,         "Colon symbol");
					InstallKeyWord(";",   TOKEN_SEMICOLON,     "Semicolon symbol");
					InstallKeyWord("\\",  TOKEN_BACKSLASH,     "BackSlash symbol");
					InstallKeyWord("_",   TOKEN_UNDERSCORE,    "Underscore symbol");
					InstallKeyWord("?",   TOKEN_QUESTION,      "Question symbol");

					// built in brackets

					InstallKeyWord("(",   TOKEN_ROUND_OPENED,  "Round opened parenthesys");
					InstallKeyWord(")",   TOKEN_ROUND_CLOSED,  "Round closed parenthesys");
					InstallKeyWord("[",   TOKEN_SQUARE_OPENED, "Square opened parenthesys");
					InstallKeyWord("]",   TOKEN_SQUARE_CLOSED, "Square closed parenthesys");
					InstallKeyWord("{",   TOKEN_CURLY_OPENED,  "Curly opened parenthesys");
					InstallKeyWord("}",   TOKEN_CURLY_CLOSED,  "Curly closed parenthesys");

					// end of line and end of file

					InstallKeyWord("\n",  TOKEN_EOL,           "End of line");
					InstallKeyWord("\r",  TOKEN_CR,			   "Carriage return");
					InstallKeyWord("\t",  TOKEN_TAB,		   "Tab");
					InstallKeyWord("\b",  TOKEN_BACKSPACE,	   "BackSpace");
					InstallKeyWord("eof", TOKEN_EOF,		   "End of file");

				}

				// -----------------------------------------------------------------
				// resets everything

				void Reset()
				{
					LineNo = 0;
					LastError = "No error\n";
					Source.clear();
					Output.clear();
					FileName.clear();
					FileSizeInBytes = 0;
					Lexemes.clear();
					Timer.Init();
					LexingTime = 0.0f;
					LoadingTime = 0.0f;
				}

				// ---------------------------------------------------------------
				// starts lexer, call this function and pass your text to have it lexed

				bool LexString(std::string &text, unsigned int flags)
				{
					// empty file

					if (text.empty())
					{
						LastError = "Empty Source file\n";
						return false;
					}

					// start lexing file

					std::vector<std::string> tokens;
					size_t it;

					it         = 0;
					LineNo     = 0;
					SkipWhites = true;							// always skip white spaces
					Flags      = flags;

					if ((Flags & REMOVECOMMENTS) != 0)
						text = RemoveCommentsFromSourceFile(text);

					// the empty string at the end of the tokenizer function,
					// denotes that the double quote symbol "" is not preserved

					tokens = vml::strings::lexer::Tokenize(text, CharsToLex, CharsToPreserve, "", "");
					tokens.emplace_back("eof");

					if ((IsVerbose() && IsQuiet()) || (!IsVerbose() && !IsQuiet()))
					{
						SetQuiet();
					}

					LastError = "No error\n";
					LexingTime = 0.0f;

					Timer.Init();

					Lexemes.clear();

					do
					{

						if (!CheckTerminals(tokens, it))
						{
							return false;
						}

					} while (it != tokens.size());

					// gets elapsed time

					LexingTime = Timer.GetElapsedTime();

					// dump out tokens

					OutputToString();

					return true;
				}
				
				// -----------------------------------------------------------------
				// lexemes array

				std::vector<CLexeme> Lexemes;

			public:

				// -----------------------------------------------------------------
				// flags

				static const unsigned int QUIET		     = vml::bits32::BIT0;
				static const unsigned int VERBOSE	     = vml::bits32::BIT1;
				static const unsigned int ALLSTRINGS     = vml::bits32::BIT2;
				static const unsigned int REMOVECOMMENTS = vml::bits32::BIT3;

				// -----------------------------------------------------------------
				// getters

				int GetSourceSize() const { return int(Source.size()); }
				const std::string &GetSource() const { return Source; }
				const std::string &GetOutput() const { return Output; }
				const std::string &GetLastError() const { return LastError; }
				const std::string &GetFileName() const { return FileName; }
				unsigned int GetFileSize() const { return	FileSizeInBytes;}
				float GetLoadingTime() const { return LoadingTime; }
				float GetLexingTime() const { return LexingTime; }
				const std::string &GetCharsTolex() const { return CharsToLex; }
				const std::string &GetCharsToPreserve() const { return CharsToPreserve; }
				unsigned int GetFlags() const { return Flags; }
				bool IsQuiet() { return ((Flags & QUIET) != 0); }
				bool IsVerbose() { return ((Flags & VERBOSE) != 0); }
				bool IsAllStrings() { return ((Flags & ALLSTRINGS) != 0); }
				const CLexeme &GetLexemeAt(int pos) const { return Lexemes[pos]; }
				size_t GetLexemeCount() const { return Lexemes.size(); }
				const CLexeme *GetLexemeAtPtr(int pos) const { return &Lexemes[pos]; }
				const std::vector<CLexeme> &GetLexemes() const{ return Lexemes; }

				// -----------------------------------------------------------------
				// sets character to be transformed into lexemes

				void SetCharsToLex(const std::string &l)
				{
					CharsToLex = l;
				}

				// -----------------------------------------------------------------
				// sets character to be preserved when transformed into lexemes

				void SetCharsToPreserve(const std::string &p)
				{
					CharsToPreserve = p;
				}

				// -----------------------------------------------------------------
				// set flags

				void SetVerbose() { Flags |= VERBOSE; Flags &= ~QUIET; }
				void SetQuiet() { Flags |= QUIET; Flags &= ~VERBOSE; }

				// ---------------------------------------------------------------
				// load a source text from a file 

				bool FromFile(const std::string &filename, unsigned int flags)
				{
					Reset();
					Timer.Init();
					LoadingTime = 0.0f;
					FileName = filename;
					std::ifstream ifs(FileName.c_str());

					if (ifs.is_open())
					{
						Source.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
						FileSizeInBytes = int(Source.size());
						LoadingTime = Timer.GetElapsedTime();
						LastError = "No Error";
						ifs.close();
						return LexString(Source, flags);
					}
					else
					{
						LastError = vml::strings::StringFormat::Text("Couldn't open ' {0} '\n", filename.c_str());
						return false;
					}
		
					// we never get here

					return false;
				}

				// -----------------------------------------------------------------

				bool FromString(const std::string &text, unsigned int flags)
				{
					Reset();
					Timer.Init();
					LoadingTime = 0.0f;
					FileName = "string";
					
					if (text.size()!=0)
					{
						Source=text;
						FileSizeInBytes = int(Source.size());
						LoadingTime = Timer.GetElapsedTime();
						LastError = "No Error";
						return LexString(Source, flags);
					}
					else
					{
						LastError = "Couldn't lex string";
						return false;
					}

					// we never get here

					return false;
				}
				
				// ---------------------------------------------------------------
				// ctor / dtor

				CLexer()
				{
					InstallLexemes();
					Timer.Init();
					LineNo			= 0;
					LastError		= "No Error\n";
					LexingTime		= 0.0f;
					LoadingTime		= 0.0f;
					SkipWhites		= true;
					Flags			= 0;
					FileSizeInBytes = 0;
					CharsToLex		=" {}()[]<>\\|+-~%&@#*$°/:;,._'’=^!?\"\n\r\t\b";
					CharsToPreserve = "{}()[]<>\\|+-~%&@#*$°/:;,._'’=^!?\"\n\r\t\b";
				}
				
				~CLexer()
				{
				}

			};

		}	// end of lexer namespace

	}	// end of strings namespace 

}	// end of vml namespace
