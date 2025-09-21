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
//
////////////////////////////////////////////////////////////////////////////////////////
//  Example of usage :
//
//	vml::strings::hysto::CStringHistogram h;
//
//	h.FromFile("ReadMe.txt");
//
//	const vml::strings::hysto::WordVector &Vec=h.GetWords();
//
//	std::string text;
//
//	for ( size_t i=0; i<Vec.size(); ++i )
//		text+=vml::strings::Format("%d %-15s %d\n",i,Vec[i].first.c_str(),Vec[i].second->GetFrequency() );
//

#include <vml\strings\stringlexer.h>

namespace vml
{
	namespace strings
	{
		namespace hysto
		{
			class StringHistogramNode
			{
						std::string Word;
						unsigned int Frequency;

					public:

						// ----------------------------------------------------
						// getters

						const std::string &GetWord() const { return Word; }
						const unsigned int GetFrequency() const { return Frequency; }
			
						// ---------------------------------------------------

						void IncFrequency() { Frequency++; }

						// ---------------------------------------------------
						// ctor / dtor

						StringHistogramNode( const std::string &word )
						{
							Word=word;
							Frequency=1;
						}

						~StringHistogramNode()
						{}

			};

			//////////////////////////////////////////////////////////////////////////
			// create vector of words along with their use frequency

			typedef std::vector<std::pair<std::string,StringHistogramNode*>> WordVector;

			//////////////////////////////////////////////////////////////////////////
			// string hystogram class

			class StringHistogram
			{

				private:
		
					// ----------------------------------------------------------------

					std::string												  LastError;
					std::string												  Source;
					std::string												  Output;
					std::string												  FileName;
					vml::os::Timer											  Timer;
					float													  LoadingTime;
					float													  LexingTime;
					unsigned int											  SortMode;
					std::string												  CharsToLex;
					std::string												  CharsToPreserve;
					std::unordered_map<std::string, StringHistogramNode*>	  WordMap;
					std::vector<std::pair<std::string,StringHistogramNode*>> WordVec;
			
					// ----------------------------------------------------------------

					struct SortByName
					{
						inline bool operator() (const std::pair<std::string,StringHistogramNode*>& struct1, 
												const std::pair<std::string,StringHistogramNode*>& struct2)
						{
							return (struct1.second->GetWord() < struct2.second->GetWord());
						}
					};

					struct SortByFreq
					{
						inline bool operator() (const std::pair<std::string,StringHistogramNode*>& struct1, 
												const std::pair<std::string,StringHistogramNode*>& struct2)
						{
							return (struct1.second->GetFrequency() < struct2.second->GetFrequency());
						}
					};
	
					// -----------------------------------------------------------------
					// resets everything

					void Reset()
					{
						LastError = "No error";
						Source.clear();
						Output.clear();
						FileName.clear();
						Timer.Init();
						LexingTime = 0.0f;
						LoadingTime = 0.0f;
						for ( std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.begin(); it!=WordMap.end(); ++it )
							delete ( (*it).second );
					}
		
				public:

					// ------------------------------------------------------

					static const unsigned int SORT_BY_WORD_NAME	=1;
					static const unsigned int SORT_BY_WORD_FREQ	=2;

					// -------------------------------------------------------------------
					// getters

					const std::string &GetLastError() const { return LastError; }
					const std::string &GetFileName() const { return FileName; }
					const std::string &GetCharsTolex() const { return CharsToLex; }
					const std::string &GetCharsToPreserve() const { return CharsToPreserve; }
					unsigned int GetSortMode() const { return SortMode; }
					const std::vector<std::pair<std::string,StringHistogramNode*>> &GetWords() { return WordVec; }

					const unsigned int GetWordFreq( const std::string &word ) 
					{
						std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.find( word );
						if ( it!=WordMap.end() ) return (*it).second->GetFrequency();
						return 0;
					}

					// -----------------------------------------------------------------
					// sets character to be transformed into lexemes

					void SetCharsToLex(const std::string &l) { CharsToLex = l; }

					// -----------------------------------------------------------------
					// sets character to be preserved when transformed into lexemes

					void SetCharsToPreserve(const std::string &p) { CharsToPreserve = p; }

					// ---------------------------------------------------------------

					void Sort()
					{
						switch ( SortMode )
						{
							case SORT_BY_WORD_NAME : std::sort(WordVec.begin(),WordVec.end(),SortByName()); break;
							case SORT_BY_WORD_FREQ : std::sort(WordVec.begin(),WordVec.end(),SortByFreq()); break;
						}
					}
	
					// ----------------------------------------
					// changes sorting mode

					void SetSortMode( unsigned int mode )
					{
						if ( mode!=SORT_BY_WORD_NAME && 
							 mode!=SORT_BY_WORD_FREQ )
						{
							SortMode=SORT_BY_WORD_NAME;
						}
						else
						{
							SortMode=mode;
						}
					}
		
					// -------------------------------------------------------------------
					// creates hystogram from a text file

					bool FromFile(const std::string &filename)
					{
						Reset();
						Timer.Init();
						LoadingTime = 0.0f;
						FileName = vml::strings::StringUtils::TrimAndLower(filename);
						std::ifstream ifs(FileName.c_str());

						if (ifs.is_open())
						{
							Source.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
							LoadingTime = Timer.GetElapsedTime();
							LastError = "No Error";

							// tokenize input file

							std::vector < std::string > tokens;
			
							tokens=vml::strings::lexer::Tokenize( vml::strings::StringUtils::LoadText(FileName),
																  CharsToLex,
																  CharsToPreserve );
				
							// add tokens

							for ( size_t i=0; i<tokens.size(); ++i )
							{
								std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.find( tokens[i] );
			
								if ( it==WordMap.end() )
								{
									WordMap[ tokens[i] ]= new StringHistogramNode(tokens[i]);
								}
								else
								{
									(*it).second->IncFrequency();
								}
							}

							// Initialize and sort vector with data_t items from link_map    
	
							for ( std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.begin(); it!=WordMap.end(); ++it )
								WordVec.emplace_back( *it ) ;

							// sort according to sort mode

							Sort();

							// gets elapsed time

							LexingTime = Timer.GetElapsedTime();

							return true;
						}
						else
						{
							LastError = vml::strings::StringFormat::Text("Couldn't open ' %s '\n", filename.c_str());
							return false;
						}
		
						// we never get here

						return false;

					}

					// -----------------------------------------------------------------
					// creates hystogram from a text string

					const bool FromString(const std::string &text)
					{
						
						Reset();
						Timer.Init();
						LoadingTime = 0.0f;
						FileName = "string";
						
						if (text.size()!=0)
						{
							
							Source=text;
							LoadingTime = Timer.GetElapsedTime();
							LastError = "No Error";
							
							// tokenize input file

							std::vector < std::string > tokens;
			
							tokens=vml::strings::lexer::Tokenize( text,CharsToLex,CharsToPreserve );
							
							// add tokens

							for ( size_t i=0; i<tokens.size(); ++i )
							{
								std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.find( tokens[i] );
			
								if ( it==WordMap.end() )
								{
									WordMap[ tokens[i] ]= new StringHistogramNode(tokens[i]);
								}
								else
								{
									(*it).second->IncFrequency();
								}
							}

							// Initialize and sort vector with data_t items from link_map    
	
							for ( std::unordered_map<std::string, StringHistogramNode*>::iterator it=WordMap.begin(); it!=WordMap.end(); ++it )
								WordVec.emplace_back( *it ) ;

							// sort according to sort mode

							Sort();
							
							// gets elapsed time

							LexingTime = Timer.GetElapsedTime();
							
						}
						else
						{
							LastError = "Couldn't lex string";
							return false;
						}
						
						// we never get here
						
						return false;

					}

					// ---------------------------------------------------
					// ctor / dtor

					StringHistogram()
					{
						Timer.Init();
						FileName		= "";
						LastError		= "No Error";
						LexingTime		= 0.0f;
						LoadingTime		= 0.0f;
						SortMode		= SORT_BY_WORD_NAME;
						CharsToLex		=" {}()[]<>\\|+-%&@#*$°/:;,._'=^!?\"\n\r\t";
						CharsToPreserve = "{}()[]<>\\|+-%&@#*$°/:;,._'=^!?\"\n\r\t";
					}

					~StringHistogram()
					{
						Reset();
					}

			};

		}  //end of namespace hysto

	}	// end of namespace strings

}	// end of namespace vml

