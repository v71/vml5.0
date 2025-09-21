#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
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
	namespace strings
	{
		namespace parser
		{

			/////////////////////////////////////////////////////////////////////
			// enum used by parser nodes

			enum class PARSERELEMENTTYPE : int
			{
				PARSER_NODE_ROOT		= 0,
				PARSER_NODE_PARAGRAPH   = 1,
				PARSER_NODE_ARRAY       = 2,
				PARSER_NODE_STRING		= 3,
				PARSER_NODE_VALUE		= 4
			};

			/////////////////////////////////////////////////////////////////////////
			// parser node

			class ParserNode
			{
				protected:

					int						   Level;	// node's depth level
					double					   Value;	// numerical value stored by the node
					std::string				   Text;	// string value stored by the node
					std::string				   Name;	// node's name
					std::string				   Path;	// node's path
					PARSERELEMENTTYPE		   Type;	// element type
					ParserNode				  *Parent;	// parent node
					std::vector<ParserNode*>   Child;	// node's children

				public:

					//-------------------------------------------------------------------------
					// getters

					const std::string &GetPath() const { return Path; }
					const std::string &GetName() const { return Name; }
					const std::string &GetText() const { return Text; }
					const ParserNode *GetParent() const { return Parent; }
					const ParserNode *GetChildAt(size_t pos) const { return Child[pos]; }
					double GetValue() const { return Value; }
					bool IsLeaf() const { return Child.size() == 0; }
					int GetLevel() const { return Level; }
					size_t GetChildrenCount() const { return Child.size(); }

					const ParserNode* GetChildByName(const std::string &name) const
					{
						for (size_t i = 0; i < Child.size(); ++i)
							if (Child[i]->Name == name) return Child[i];
						return nullptr;
					}

					//-------------------------------------------------------------------------
					// virtuals

					virtual const std::string DumpInfo() const = 0;
					virtual const PARSERELEMENTTYPE GetType() const = 0;
					virtual const std::string GetTypeString() const = 0;

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserNode(ParserNode *parent,
							   const std::string &name,
							   const std::string& path,
							   const enum class PARSERELEMENTTYPE type)
					{
						Path   = path;
						Name   = name;
						Parent = parent;
						Level  = 0;
						Value  = 0;
						Type   = type;

						if (Parent)
						{
							Parent->Child.push_back(this);
							Level = Parent->Level + 1;
						}
					}

					virtual ~ParserNode()
					{
					}
			};

			////////////////////////////////////////////////////////////////////////
			// root node for parser

			class ParserRoot : public ParserNode
			{
				public:

					//-------------------------------------------------------------------------
					// getters

					const PARSERELEMENTTYPE GetType() const { return PARSERELEMENTTYPE::PARSER_NODE_ROOT; }
					const std::string GetTypeString() const { return "Root node"; }

					//-------------------------------------------------------------------------
					// dumps info in a string

					const std::string DumpInfo() const
					{
						std::string text;
						text = "Name	      : " + Name + "\n";
						text += "Path	      : " + Path + "\n";
						if (Parent)
						{
							text += "Parent name			: " + Parent->GetName() + "\n";
							text += "Parent path			: " + Parent->GetPath() + "\\\n";
						}
						text += "Type          : " + GetTypeString() + "\n";
						text += vml::strings::StringFormat::Text("Level         : {0}\n", Level);
						text += vml::strings::StringFormat::Text("Children      : {0}\n", Child.size());
						for (size_t i = 0; i < Child.size(); ++i)
							text += vml::strings::StringFormat::Text("Child [ {0} ]   : {1}\n", i, Child[i]->GetName().c_str());
						return text;
					}

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserRoot(const std::string &path) : ParserNode(nullptr, path, path, PARSERELEMENTTYPE::PARSER_NODE_ROOT)
					{
					}

					~ParserRoot()
					{}
			};

			////////////////////////////////////////////////////////////////////////
			// paragraph node for parser

			class ParserParagraph : public ParserNode
			{
				public:

					//-------------------------------------------------------------------------
					// getters

					const PARSERELEMENTTYPE GetType() const { return PARSERELEMENTTYPE::PARSER_NODE_PARAGRAPH; }
					const std::string GetTypeString() const { return "Paragraph node"; }

					//-------------------------------------------------------------------------
					// dumps info in a string

					const std::string DumpInfo() const
					{
						std::string text;
						text = "Name	      : " + Name + "\n";
						text += "Path	      : " + Path + "\n";
						text += "Parent name   : " + Parent->GetName() + "\n";
						text += "Parent path   : " + Parent->GetPath() + "\\\n";
						text += "Type	      : " + GetTypeString() + "\n";
						text += vml::strings::StringFormat::Text("Level         : {0}\n", Level);
						text += vml::strings::StringFormat::Text("Children      : {0}\n", Child.size());
						for (size_t i = 0; i < Child.size(); ++i)
							text += vml::strings::StringFormat::Text("Child [ {0} ]   : {1}\n", i, Child[i]->GetName().c_str());
						return text;
					}

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserParagraph(ParserNode *parent,
									const std::string &name,
									const std::string &path) : ParserNode(parent, name, path, PARSERELEMENTTYPE::PARSER_NODE_PARAGRAPH)
					{
					}

					~ParserParagraph()
					{
					}
			};

			////////////////////////////////////////////////////////////////////////
			// array node for parser

			class ParserArray : public ParserNode
			{

				public:

					//-------------------------------------------------------------------------
					// getters

					const PARSERELEMENTTYPE GetType() const { return PARSERELEMENTTYPE::PARSER_NODE_ARRAY; }
					const std::string GetTypeString() const { return "Array node"; }

					//-------------------------------------------------------------------------
					// dumps info in a string

					const std::string DumpInfo() const
					{
						std::string text;
						text = "Name	      : " + Name + "\n";
						text += "Path	      : " + Path + "\n";
						text += "Parent name   : " + Parent->GetName() + "\n";
						text += "Parent path   : " + Parent->GetPath() + "\\\n";
						text += "Type	      : " + GetTypeString() + "\n";
						text += vml::strings::StringFormat::Text("Level         : {0}\n", Level);
						text += vml::strings::StringFormat::Text("Children      : {0}\n", Child.size());
						for (size_t i = 0; i < Child.size(); ++i)
							text += vml::strings::StringFormat::Text("Child [ {0} ]   : {1}\n", i, Child[i]->GetName().c_str());
						return text;
					}

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserArray(ParserNode *parent,
								const std::string &name,
								const std::string &path) : ParserNode(parent, name, path, PARSERELEMENTTYPE::PARSER_NODE_ARRAY)
					{
					}

					~ParserArray()
					{}

			};


			////////////////////////////////////////
			// value node for parser

			class ParserString : public ParserNode
			{
				public:

					//-------------------------------------------------------------------------
					// getters

					const PARSERELEMENTTYPE GetType() const { return PARSERELEMENTTYPE::PARSER_NODE_STRING; }
					const std::string GetTypeString() const { return "String node"; }

					//-------------------------------------------------------------------------
					// dumps info in a string

					const std::string DumpInfo() const
					{
						std::string text;
						text = "Name	      : " + Name + "\n";
						text += "Path	      : " + Path + "\n";
						text += "Parent name   : " + Parent->GetName() + "\n";
						text += "Parent path   : " + Parent->GetPath() + "\\\n";
						text += "Type	      : " + GetTypeString() + "\n";
						text += "Text	      : " + Text + "\n";
						text += vml::strings::StringFormat::Text("Level         : {0}\n", Level);
						text += vml::strings::StringFormat::Text("Children      : {0}\n", Child.size());
						for (size_t i = 0; i < Child.size(); ++i)
							text += vml::strings::StringFormat::Text("Child [ {0} ]   : {1}\n", i, Child[i]->GetName().c_str());
						return text;
					}

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserString(ParserNode *parent,
								 const std::string &name,
								 const std::string &path,
								 const std::string &text) : ParserNode(parent, name, path, PARSERELEMENTTYPE::PARSER_NODE_STRING)
					{
						Text = text;
					}

					~ParserString()
					{}

			};

			////////////////////////////////////////////////////////////////////////
			// value node for parser

			class ParserValue : public ParserNode
			{
				public:

					//-------------------------------------------------------------------------
					// getters

					const PARSERELEMENTTYPE GetType() const { return PARSERELEMENTTYPE::PARSER_NODE_VALUE; }
					const std::string GetTypeString() const { return "Value node"; }

					//-------------------------------------------------------------------------
					// dumps info in a string

					const std::string DumpInfo() const
					{
						std::string text;
						text = "Name	      : " + Name + "\n";
						text += "Path 	      : " + Path + "\n";
						text += "Parent name   : " + Parent->GetName() + "\n";
						text += "Parent path   : " + Parent->GetPath() + "\\\n";
						text += "Type          : " + GetTypeString() + "\n";
						text += vml::strings::StringFormat::Text("Value         : {0}\n", Value);
						text += vml::strings::StringFormat::Text("Level         : {0}\n", Level);
						text += vml::strings::StringFormat::Text("Children      : {0}\n", Child.size());
						for (size_t i = 0; i < Child.size(); ++i)
							text += vml::strings::StringFormat::Text("Child [ {0} ]   : {1}\n", i, Child[i]->GetName().c_str());
						return text;
					}

					//-------------------------------------------------------------------------
					// ctor / dtor

					ParserValue(ParserNode *parent,
								const std::string &name,
								const std::string &path,
								const double value) : ParserNode(parent, name, path, PARSERELEMENTTYPE::PARSER_NODE_VALUE)
					{
						Value = value;
					}

					~ParserValue()
					{}

			};

			///////////////////////////////////////////////////////////////////////////////////////////
			// vson document parser

			class Parser
			{

				private:

					// -------------------------------------------------------------------

					std::unordered_map<std::string, ParserNode*>	NodeMap;					// hash map ,nodes are stored here for fast retrival
					std::vector<ParserNode*>						NodeStack;					// stack used for parsing nested paragraphs and /or header
					int												CurrentStack;				// current node stack
					int												CurrentSubScript;			// current lexeme
					vml::os::Timer									Timer;						// timer class this is windows dependant				
					float											ParsingTime;				// estimates parsing time
					float											TotalTime;					// estimates lexing time plus parsing time
					ParserNode									   *Root;						// root node
					std::string										LastError;					// error reporting string
					std::string										Output;						// output string 
					unsigned int									Flags;						// flags storing		
					std::string										CharsToLex;					// set of chars to lex
					std::string										CharsToPreserve;			// set of chars to preserve during lexing
					std::string										CharsNotAllowedForLabel;	// sets chars which string label won't accept
					int												CurrentLexeme;				// current lexeme

					//--------------------------------------------------------------------
					// setters

					void SetVerbose() { Flags |= VERBOSE; Flags &= ~QUIET; }
					void SetQuiet() { Flags |= QUIET; Flags &= ~VERBOSE; }
					void AllowEmptyNodes() { Flags |= ALLOW_EMPTY_NODES; }
					void DisallowEmptyNodes() { Flags &= ~ALLOW_EMPTY_NODES; }

					// -------------------------------------------------------------------
					// get current token

					int GetCurrentToken() const
					{
						return Lexer.GetLexemeAt(CurrentLexeme).GetTokenType();
					}

					// -------------------------------------------------------------------
					// get token string

					const std::string &GetCurrentTokenString()
					{
						return Lexer.GetLexemeAt(CurrentLexeme).GetToken();
					}

					// -------------------------------------------------------------------
					// get token line

					int ReadCurrentTokenLine()
					{
						return Lexer.GetLexemeAt(CurrentLexeme).GetLine();
					}

					// -------------------------------------------------------------------
					// advance current lexeme index and returns token

					int GetNextToken()
					{
						return Lexer.GetLexemeAt(++CurrentLexeme).GetTokenType();
					}

					// -------------------------------------------------------------------
					// advance current lexeme index *copy* and returns token
					// actually this function read the next token without modifying
					// the current lexeme index 

					int ReadNextToken()
					{
						return Lexer.GetLexemeAt(CurrentLexeme + 1).GetTokenType();
					}

					// -------------------------------------------------------------------
					// get token line

					int ReadNextTokenLine()
					{
						unsigned int pos = CurrentLexeme;
						size_t n = Lexer.GetLexemeCount();
						if (pos >= n) pos = int(n - 1);
						return Lexer.GetLexemeAt(pos).GetLine() + 1;
					}

					// -------------------------------------------------------------------
					// checks if a node has been already declared

					bool CheckNodeAvailability(const std::string &nodestringpath)
					{
						if (NodeMap.find(nodestringpath) == NodeMap.end()) return true;
						return false;
					}

					// -------------------------------------------------------------------
					// validate paragraph label

					bool ValidateLabel(const std::string &label)
					{
						
						for (size_t i = 0; i < label.size(); ++i)
						{
							char ch = label[i];

							// unallowed chars

							if (ch == '{'  || ch == '}'  ||
								ch == '('  || ch == ')'  ||
								ch == '['  || ch == ']'  ||
								ch == '<'  || ch == '>'  ||
								ch == '\\' || ch == '|'  ||
								ch == '+'  || ch == '-'  ||
								ch == '*'  || ch == '/'  ||
								ch == '='  || ch == '.'  ||
								ch == '%'  || ch == '&'  ||
								ch == '@'  || ch == '\'' ||
								ch == '#'  || ch == '$'  ||
								ch == '°'  || ch == ':'  ||
								ch == ';'  || ch == ','  ||
								ch == '^'  || ch == '!'  ||
								ch == '?'  || ch == '"')
									return false;
							
						}

						return true;
					
					}

					// -------------------------------------------------------------------
					// validate string token

					bool ValidateString(const std::string &src)
					{

						size_t p = src.size() - 1;

						if ( (p == 1) && ((Flags & ALLOW_EMPTY_NODES) == 0) )
							return false;

						// skip the double quaotes at the beginngin and at the ned of the string

						if (src[0] == '"' && src[p] == '"')
						{

							for (size_t i = 1; i < p; ++i)
							{
								char ch = src[i];
								
								// '.' ,'/', are allowed here becasue we might express a filename

								if (ch == '{' || ch == '}' ||
									ch == '[' || ch == ']' ||
									ch == '|' || ch == '%' ||
									ch == '&' || ch == '@' ||
									ch == '#' || ch == '$' ||
									ch == '°' || ch == ':' ||
									ch == ';' || ch == ',' ||
									ch == '^' || ch == '!' ||
									ch == '?' || ch == '"')
										return false;

							}

							return true;
						}

						return false;
					}

					// --------------------------------------------------------
					// 

					bool CheckIfStringIsAMathExpr(const std::string &src)
					{
						for (size_t i = 0; i < src.size(); ++i)
						{
							char ch = src[i];

							// unallowed chars

							if (ch == '{'  || ch == '}'  ||
								ch == '['  || ch == ']'  ||
								ch == '\\' || ch == '|'  ||
								ch == '%'  || ch == '&'  ||
								ch == '@'  || ch == '\'' ||
								ch == '#'  || ch == '$'  ||
								ch == '°'  || ch == ':'  ||
								ch == ';'  || ch == ','  ||
								ch == '!'  ||
								ch == '?'  || ch == '"')
									return false;

							// check if a char is alphanumeric

							if (std::isalpha(ch))
								return false;
						}

						return true;

					}

					// -------------------------------------------------------------------
					// release the allocated nodes
					// and reset everything

					void ReleaseTree()
					{
						for (auto it = NodeMap.begin(); it != NodeMap.end(); ++it)
							delete ((*it).second);
						NodeMap.clear();
						NodeStack.clear();
						CurrentLexeme = 0;
						CurrentStack = 0;
						LastError = "No Error";
						Output = "";
					}

					// -------------------------------------------------------------------
					// error exiting function

					bool ExitWithError(const std::string &err)
					{
						LastError = err;
						return false;
					}

					// -------------------------------------------------------------------
					// parse comma token

					bool ParseComma()
					{
						// look ahead of current token

						switch (ReadNextToken())
						{

							case vml::strings::lexer::TOKEN_STRING:

								GetNextToken();

							break;

							case vml::strings::lexer::TOKEN_EOF:

								return ExitWithError(vml::strings::StringFormat::Text("Parse comma(1) : Premature end of file at line {0}", ReadNextTokenLine()));

							break;

							default:

								return ExitWithError(vml::strings::StringFormat::Text("Parse comma(2) : Expecting value or semicolon at line {0}", ReadNextTokenLine()));

							break;

						}

						return true;
					}

					// -------------------------------------------------------------------
					// parse string

					bool ParseStringToken(const std::string &name, const std::string &path)
					{

						// look ahead of current token

						switch (ReadNextToken())
						{
							case vml::strings::lexer::TOKEN_COMMA:
							case vml::strings::lexer::TOKEN_SEMICOLON:

								{
									// store string and check if its a numerical expression
									
									std::string result = vml::strings::StringUtils::TrimWhites(Lexer.GetLexemeAt(CurrentLexeme).GetToken());
									std::string sub = "[" + vml::strings::StringConverter::FromIntToString(CurrentSubScript) + "]";
									std::string subpath = NodeMap[path]->GetPath() + sub;

									if (CheckIfStringIsAMathExpr(result))
									{

										ExpressionParser expr;

										if (!expr.Parse(result))
											return ExitWithError(vml::strings::StringFormat::Text("Parse math string(1) : Invalid String at line {0} ,{1}", 
												                                                  ReadNextTokenLine(), expr.GetLastErrorString()));

										if (CheckNodeAvailability(subpath))
											NodeMap[subpath] = new ParserValue(NodeMap[path], name + sub, subpath, expr.GetAnswer());

									}
									else
									{

										if (!ValidateString(result))
											return ExitWithError(vml::strings::StringFormat::Text("Parse string(2) : Invalid String at line {0}", ReadNextTokenLine()));

										if (CheckNodeAvailability(subpath))
											NodeMap[subpath] = new ParserString(NodeMap[path], name + sub, subpath, result);

									}

									CurrentSubScript++;

									// step next token

									GetNextToken();

								}

							break;

							case vml::strings::lexer::TOKEN_EOF:

									return ExitWithError(vml::strings::StringFormat::Text("Parse string(2) : Premature end of file at line {0}", ReadNextTokenLine()));

							break;

							default:

									return ExitWithError(vml::strings::StringFormat::Text("Parse string(3) : Expecting value, comma or semicolon at line {0}", ReadNextTokenLine()));

							break;
						}

						return true;
					}

					// -------------------------------------------------------------------
					// parse token after the assign symbol

					bool ParseElement(ParserNode *parent, const std::string &name, const std::string &path)
					{

						int token = GetNextToken();

						if (token == vml::strings::lexer::TOKEN_EOF)
							return ExitWithError(vml::strings::StringFormat::Text("Parse element(1) : Empty Assignement at line {0}", ReadCurrentTokenLine()));

						if (token != vml::strings::lexer::TOKEN_ASSIGN)
							return ExitWithError(vml::strings::StringFormat::Text("Parse element(1) : Expected an Assignement at line {0}", ReadCurrentTokenLine()));

						token = GetNextToken();

						if (token == vml::strings::lexer::TOKEN_EOF)
							return ExitWithError(vml::strings::StringFormat::Text("Parse element(2) : Empty Assignement at line {0}", ReadCurrentTokenLine()));

						if (token == vml::strings::lexer::TOKEN_COMMA)
							return ExitWithError(vml::strings::StringFormat::Text("Parse element(2) : Expected a numeric or literal at line {0}", ReadCurrentTokenLine()));

						if (token == vml::strings::lexer::TOKEN_SEMICOLON)
							return ExitWithError(vml::strings::StringFormat::Text("Parse element(2) : Empty Assignmenet at line {0}", ReadCurrentTokenLine()));

						CurrentSubScript = 0;

						NodeMap[path] = new ParserArray(parent, name, path);

						for (;;)
						{
							switch (GetCurrentToken())
							{

								case vml::strings::lexer::TOKEN_STRING:

									if (!ParseStringToken(name, path))
										return false;

								break;

								case vml::strings::lexer::TOKEN_COMMA:

									if (!ParseComma())
										return false;

								break;

								case vml::strings::lexer::TOKEN_EOF:

									return ExitWithError(vml::strings::StringFormat::Text("Parse element(3) : Premature end of file at line {0}", ReadNextTokenLine()));

								break;

								case vml::strings::lexer::TOKEN_SEMICOLON:

									return true;

								break;

								default:

									return ExitWithError(vml::strings::StringFormat::Text("Parse element(4) : Unaccepted token at line {0}", ReadNextTokenLine()));

								break;

							}

						}

						return false;
					}


				public:

					// -------------------------------------------------------------------

					vml::strings::lexer::CLexer	Lexer;			// Lexer

					//--------------------------------------------------------------------
					// public bit flags

					static const unsigned int QUIET				      = vml::bits32::BIT0;
					static const unsigned int VERBOSE			      = vml::bits32::BIT1;
					static const unsigned int ALLOW_EMPTY_NODES	      = vml::bits32::BIT2;
					static const unsigned int ALLOW_EMPTY_STRINGS     = vml::bits32::BIT4;

					//--------------------------------------------------------------------
					// getters

					bool AreEmptyNodesAllowed() { return ((Flags & ALLOW_EMPTY_NODES) != 0); }
					bool AreEmptyStringsAllowed() { return ((Flags & ALLOW_EMPTY_NODES) == 0); }
					bool IsVerbose() { return ((Flags & VERBOSE) != 0); }
					bool IsQuiet() { return ((Flags & QUIET) != 0); }

					//--------------------------------------------------------------------
					// load file to parse from disk

					bool FromFile(const std::string &filename, unsigned int flags)
					{

						// start with a clean state

						ReleaseTree();

						// start lexing , skips whites ( second parameter )
						// define which characters to lex and which to preserve

						Lexer.SetCharsToLex(CharsToLex);
						Lexer.SetCharsToPreserve(CharsToPreserve);

						// lex the source file
						// lexer consider all numeric vsalues as strings 
						// becasue the parser will parse the math experssion itself
					
						unsigned int lexerflags = vml::strings::lexer::CLexer::ALLSTRINGS | vml::strings::lexer::CLexer::REMOVECOMMENTS;

						if ((flags & VERBOSE) != 0)
							lexerflags |= vml::strings::lexer::CLexer::VERBOSE;
						else
							lexerflags |= vml::strings::lexer::CLexer::QUIET;

						if (Lexer.FromFile(filename, lexerflags ))
						{

							int topofstack;
							int currenttoken;
							int nexttoken;
							int token;
							std::string path;
							std::string name;

							ParsingTime = 0.0f;

							Timer.Init();

							// gets flags

							Flags = flags;

							if ((IsVerbose() && IsQuiet()) || (!IsVerbose() && !IsQuiet()))
								SetQuiet();

							// initialize stack with root node

							NodeStack.resize(Lexer.GetLexemeCount());

							// allocate root node

							Root = new ParserRoot("Root");

							// puts root node in node map

							NodeMap["Root"] = Root;

							// put root node in top of stack

							NodeStack[CurrentStack++] = Root;

							// start parsing cycle

							for (;;)
							{
								// get current token

								currenttoken = GetCurrentToken();

								if (currenttoken == vml::strings::lexer::TOKEN_STRING)
								{
									// get parent node on top of the stack

									topofstack = CurrentStack - 1;

									// look for token ahead

									token = ReadNextToken();

									if (token == vml::strings::lexer::TOKEN_EOF)
										return ExitWithError(vml::strings::StringFormat::Text("ReadNextToken : Premature eof found at line {0}", 
											                                                  ReadNextTokenLine()));

									name = vml::strings::StringUtils::TrimWhites(GetCurrentTokenString());

									if (!ValidateLabel(name))
										return ExitWithError(vml::strings::StringFormat::Text("Label :' {0} ' is not a valid label", name.c_str()));

									path = NodeStack[topofstack]->GetPath() + "\\" + name;

									if (!CheckNodeAvailability(path))
										return ExitWithError(vml::strings::StringFormat::Text("CheckNodeAvailability : Node ' {0} ' already defined at line {1}",path.c_str(), 
											                                                  ReadCurrentTokenLine()));

									// if we have an open curly parentesis , we
									// have entered another node enclosed by curly brackets
									// store the new node as a child of the parent node 
									// currently at top of the stack

									if (token == vml::strings::lexer::TOKEN_CURLY_OPENED)
									{
										nexttoken = GetNextToken();

										if (nexttoken == vml::strings::lexer::TOKEN_EOF)
											return ExitWithError(vml::strings::StringFormat::Text("GetNextToken : Premature eof found at line {0}", ReadNextTokenLine()));

										NodeMap[path] = new ParserParagraph(NodeStack[topofstack], name, path);

										NodeStack[CurrentStack++] = NodeMap[path];
									}

									// check if there is an unwanted token
									
									if (token != vml::strings::lexer::TOKEN_CURLY_OPENED &&
										token != vml::strings::lexer::TOKEN_ASSIGN)
									{

										return ExitWithError(vml::strings::StringFormat::Text("ParseParagraph(1) : Incomplete expression started with ' {0} ' at line {1}",
																						      GetCurrentTokenString().c_str(), ReadCurrentTokenLine()));
									}
									
									// check if token is an element

									if (token == vml::strings::lexer::TOKEN_ASSIGN)
									{
										if (!ParseElement(NodeStack[topofstack], name, path))
											return false;
									}

								}

								// if current token is different from these token, we have an error

								if (currenttoken != vml::strings::lexer::TOKEN_CURLY_CLOSED &&
									currenttoken != vml::strings::lexer::TOKEN_STRING &&
									currenttoken != vml::strings::lexer::TOKEN_EOF)
								{
									return ExitWithError(vml::strings::StringFormat::Text("ParseParagraph(2) : Unaccepted symbol ' {0} ' at line {1}",
										                                                  GetCurrentTokenString().c_str(), ReadCurrentTokenLine()));
								}

								// if current token is a curly closed parentesis , the stack
								// is stepped back to its parent

								if (currenttoken == vml::strings::lexer::TOKEN_CURLY_CLOSED)
								{
									CurrentStack--;

									if (!AreEmptyNodesAllowed())
									{
										if (NodeStack[CurrentStack]->GetChildrenCount() == 0)
											return ExitWithError(vml::strings::StringFormat::Text("ParseParagraph(3) : Empty node ' {0} ' at line {1}",
																								  NodeStack[CurrentStack]->GetName().c_str(),
																								  ReadCurrentTokenLine()));
									}

								}

								// if current token is eof the file has been scanned completely

								if (currenttoken == vml::strings::lexer::TOKEN_EOF)
								{
									break;
								}
								else
								{
									// else we continue to scan the lexemes
									// until eof is encountered

									CurrentLexeme++;
								}

							}

							// check if curly brackets are matched

							if (CurrentStack - 1 != 0)
								return ExitWithError("ParseParagraph(4) : Unbalanced parenthesys");

						}
						else
						{
							return ExitWithError(Lexer.GetLastError());
						}

						// get elapsed time after parsing

						ParsingTime = Timer.GetElapsedTime();

						TotalTime = Lexer.GetLexingTime() + ParsingTime;

						return true;
					}

					// -------------------------------------------------------------
					// getters

					const std::string &GetLexerOutput() const
					{
						return Lexer.GetOutput();
					}

					const std::string &GetLastError() const
					{
						return LastError;
					}

					const std::string &GetOutput() const
					{
						return Output;
					}

					unsigned int GetFlags() const
					{
						return Flags;
					}

					const ParserNode *GetRoot() const
					{
						return Root;
					}

					float GetTiming()
					{
						return TotalTime;
					}

					const ParserNode *GetNodeByName(const std::string &text) 
					{
						auto it = NodeMap.find(text);
						if (it != NodeMap.end())
							return NodeMap[text];
						return nullptr;
					}

					// ------------------------------------------------------------------
					// prints each node in a string

					void PrintNode(const ParserNode *pNode, std::string &str, int &level)
					{
						std::string text;

						for (int i = 0; i < level; i++) text += "| ";

						if (!pNode->IsLeaf()) text += "+";

						if (pNode->GetType() == PARSERELEMENTTYPE::PARSER_NODE_ROOT)
							text += vml::strings::StringFormat::Text("{0} ({1})\n", pNode->GetName().c_str(), pNode->GetPath().c_str());

						if (pNode->GetType() == PARSERELEMENTTYPE::PARSER_NODE_PARAGRAPH)
							text += vml::strings::StringFormat::Text("{0} ({1})\n", pNode->GetName().c_str(), pNode->GetPath().c_str());

						if (pNode->GetType() == PARSERELEMENTTYPE::PARSER_NODE_ARRAY)
							text += vml::strings::StringFormat::Text("{0} ({1})\n", pNode->GetName().c_str(), pNode->GetPath().c_str());

						if (pNode->GetType() == PARSERELEMENTTYPE::PARSER_NODE_STRING)
							text += vml::strings::StringFormat::Text("{0} = {1} ({2}) ({3})\n",
																pNode->GetName().c_str(),
																pNode->GetText().c_str(),
																pNode->GetTypeString().c_str(),
																pNode->GetPath().c_str());

						if (pNode->GetType() == PARSERELEMENTTYPE::PARSER_NODE_VALUE)
							text += vml::strings::StringFormat::Text("{0} = {1} ({2}) ({3})\n",
																pNode->GetName().c_str(),
																pNode->GetValue(),
																pNode->GetTypeString().c_str(),
																pNode->GetPath().c_str());

						str += text;

						for (size_t i = 0; i < pNode->GetChildrenCount(); ++i)
						{
							level++;
							PrintNode(pNode->GetChildAt(i), str, level);
							level--;
						}

					}

					// -----------------------------------------------------------------------------------
					// dump parser results in the output string

					const std::string &DumpToString()
					{
						if (Root)
						{
							if (IsVerbose())
							{
								int level = 0;
								Output.clear();
								Output = "Source file : \n\n" + Lexer.GetSource();
								Output += "\n\n" + Lexer.GetOutput();
								Output += "\nParser : Started...\n\n";
								PrintNode(Root, Output, level);
								Output += vml::strings::StringFormat::Text("\nParser : Ended in {0} millisecs\n", ParsingTime);
								Output += vml::strings::StringFormat::Text("\nParser : {0}\n\n", LastError.c_str());
							}
							else
							{
								Output = "Parser is set on quiet mode : no output available.\n";
							}
						}
						else
						{
							Output = LastError;
						}

						return Output;
					}

					// ------------------------------------------------------------------
					// ctor / dtor

					Parser()
					{
						Flags					= VERBOSE;
						Root					= nullptr;
						CurrentLexeme			= 0;
						CurrentStack		    = 0;
						CurrentSubScript	    = 0;
						ParsingTime			    = 0.0f;
						TotalTime				= 0.0f;
						Output                  = "";
						LastError               = "No Error";
						CharsToLex              = " {};,=\n\r\t\b";
						CharsToPreserve         = "{};,=\n\r\t\b";
						CharsNotAllowedForLabel = "{}()[]<>\\|+-%&@#*$°/:;,.'=^!?\"";		// space is not a valid label
					}

					virtual ~Parser()
					{
						ReleaseTree();
					}

			};

		}	// end of parser namespace

	}	// end of strings namespace 

}	// end of vml namespace
