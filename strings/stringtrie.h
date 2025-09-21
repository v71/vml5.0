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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Trie tree is an ordered tree data structure that is used to store a dynamic set or 
// associative array where the keys are usually strings.Unlike a binary search tree, 
// no node in the tree stores the key associated with that node; instead, its position
// in the tree defines the key with which it is associated.All the descendants of a node
// have a common prefix of the string associated with that node.
// 
//  Usage:
//	
//  Instantiate the trie wiht the constructor :
//
//	vml::strings::trie::CStringTrie trie( flags );
//
// You can construct the trie by calling the constructor class using flags like this
// SORT_BY_STRING_ORDER : orders string based on their lexicographic order ( ascending )
// SORT_BY_STRING_OCCURRENCES : orders string based on their occurrences ( ascending )
// LOWER_CASE_WORDS	 : converts input data strings into lower case strings
// UPPER_CASE_WORDS : converts input data strings into upper case strings
// DEFAULT_CASE_WORDS : preservers input data strings 
// you can or the flags during the construction or after having instantiated
// the trie class ( see functions in the header , they are autoexplicative ) 
//
// Note : that only valid combinations are taken into consideration.
//
// Note : The more you look for words, the more the occurence counter increments itself
//  
//	Example :
//  
//  std::string text;
//
//  vml::strings::trie::CStringTrie p;
//
//	p.LoadDictionary("dictionary.txt");
//
//	p.AddWord("alba");
//	p.AddWord("albo");
//	p.AddWord("arma");
//	p.AddWord("disco");
//	p.AddWord("dado");
//	p.AddWord("dardo");
//	p.AddWord("daino");
//
//	std::vector< vml::strings::trie::CTrieWord > words;
//
//	words = p.FindWords("da");
//	
//	for (size_t i = 0; i<words.size(); ++i)
//		text += vml::strings::convert::Format("%s %d\n", words[i].GetWord().c_str(), words[i].GetOccurrence());
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vml\strings\stringconverter.h>		// string conversion utils

namespace vml
{
	namespace strings
	{
		namespace trie
		{
			/////////////////////////////////////////////////////////////////////////////////////
			//---------------------------------------------------------------------------
			// trie word

			class TrieWord
			{
					// --------------------------------------------------------------------------
					// private data

					std::string		Word;					// word string
					unsigned int	Occurrence;				// number of occurrences obtained from all chars occurrences

				public:

					// ----------------------------------------------------------------------------
					// getters

					const std::string &GetWord() const { return Word; }
					const unsigned int GetOccurrence() const { return Occurrence; }

					// ----------------------------------------------------------------------------
					// used for ordering string in lexicographic ascending order

					bool operator < (const TrieWord& str) const
					{
						return (str.Occurrence < Occurrence);
					}

					//---------------------------------------------------------------------------
					// ctor / dtor

					TrieWord(const std::string &word, unsigned int occurrence) :Word(word), Occurrence(occurrence)
					{
					}

					~TrieWord()
					{}

			};

			/////////////////////////////////////////////////////////////////////////////////////
			// Trie tree is used for storing string in a tree fashion
			// each child is mapped using a map for fast char retrieval

			class StringTrie
			{

				//---------------------------------------------------------------------------
				// trie node class
				// all class members are exposed since the class
				// is private to the trie node class

				class TrieNode
				{

					public:
						
						// ---------------------------------------------------------------------------
						// public data

						char					   Ch;				// character
						bool					   Leaf;			// flag is set when the word is complete
						unsigned int			   Occurrence;		// char occurrence
						TrieNode				  *Parent;			// pointer to parent ( previous prefix char )
						std::map<char, TrieNode*>  Child;			// children ( next suffix char )

						//---------------------------------------------------------------------------
						// ctor / dtor

						TrieNode(const char ch, TrieNode *parent) :Ch(ch), Parent(parent)
						{
							Leaf = false;
							Occurrence = 0;
						}

						~TrieNode()
						{

							// uncomment these lines if you want
							// to trace deallocations for debugging purposes
							//
							//if ( Parent==nullptr )
							//	vml::CMessage::Trace("deleting : root\n" );
							//else
							//	vml::CMessage::Trace("deleting : %c\n", Ch);

						}
				};

				//---------------------------------------------------------------------------
				// data

				unsigned int	 Flags;				// bit 'array' for storing flags
				TrieNode		*Root;				// root node

				//--------------------------------------------------------------------
				// recursive dumptree auxiliary function
				// use this function to debug the trie

				void DumpTree(std::string &result, TrieNode *node)
				{
					char ch, pch;

					if (node == Root)
					{
						ch = 'R'; pch = 'N';
					}
					else
					{
						ch = node->Ch;
						if (node->Parent == Root) pch = 'R';
						else
							pch = node->Parent->Ch;
					}

					result += vml::strings::StringFormat::Text("%c %d\n", ch, node->Child.size());

					for (std::map<char, TrieNode*>::iterator it = node->Child.begin(); it != node->Child.end(); ++it)
						DumpTree(result, it->second);
				}

				// --------------------------------------------------------------------------
				// helper function to retrieve word using recursion

				void RecurseStringPath(TrieNode *node, std::vector<TrieNode*> &v, std::vector< TrieWord> &words)
				{
					// if node is a leaf, we have a complete word

					if (node->Leaf)
					{
						node->Occurrence++;

						v.push_back(node);

						std::string suffix, prefix;

						for (size_t i = 0; i < v.size(); ++i)
						{
							suffix.push_back(v[i]->Ch);
						}

						// walk backwards up to the parent node
						// and insert characthers in the front string
						// to create a prefix string 

						TrieNode *prev = v[0];

						prev = prev->Parent;

						while (prev)
						{

							if (prev != Root)
							{
								prefix.insert(0, 1, prev->Ch);
							}

							// step back to parent's node

							prev = prev->Parent;

						}

						// once the requested word has been found
						// it gets stored in the vector

						words.emplace_back(TrieWord(prefix + suffix, node->Occurrence));

					}
					else
					{
						v.push_back(node);
					}

					// recurse all node's children

					for (std::map<char, TrieNode*>::iterator it = node->Child.begin(); it != node->Child.end(); ++it)
					{
						RecurseStringPath((*it).second, v, words);
					}

					// remove last node after exiting recursion

					v.pop_back();

				}

				// -----------------------------------------------------------------------
				// helper function to delete recursively the trie 

				void RecurseDeleteTree(TrieNode* node)
				{
					if (node == nullptr) return;
					for (std::map<char, TrieNode*>::iterator it = node->Child.begin(); it != node->Child.end(); ++it)
						RecurseDeleteTree((*it).second);
					delete(node);
				}

			public:

				//--------------------------------------------------------------------
				// public bit flags
				// don't change this bit positions

				static const unsigned int SORT_BY_STRING_OCCURRENCES = 1;
				static const unsigned int SORT_BY_STRING_ORDER		 = 2;
				static const unsigned int LOWER_CASE_WORDS			 = 4;
				static const unsigned int UPPER_CASE_WORDS			 = 8;
				static const unsigned int DEFAULT_CASE_WORDS		 = 16;

				//--------------------------------------------------------------------
				// dumps tree in a recursive way , note that
				// this is done only for debugging purposes 
				// there is no formatting or fancy tree drawing

				std::string DumpTree()
				{
					std::string output;
					DumpTree(output, Root);
					return output;
				}

				//--------------------------------------------------------------------
				// inserts word in the trie tree

				bool AddWord(const std::string &word)
				{
					// checks if we already have this word

					if (IsPresent(word))
						return false;

					// if not, traverse the tree and insert 
					// every single characther in the tree

					TrieNode *curnode, *node, *newnode;
					std::map<char, TrieNode*>::iterator it;
					char ch;

					curnode = Root;

					for (size_t i = 0; i < word.size(); i++)
					{

						if (((Flags & DEFAULT_CASE_WORDS) != 0)) ch = word[i];
						else if (((Flags & UPPER_CASE_WORDS) != 0))	ch = toupper(word[i]);
						else if (((Flags & LOWER_CASE_WORDS) != 0))	ch = tolower(word[i]);

						node = nullptr;

						it = curnode->Child.find(ch);

						if (it != curnode->Child.end()) node = (*it).second;

						if (!node)
						{
							newnode = new TrieNode(ch, curnode);

							curnode->Child[ch] = newnode;

							curnode = newnode;
						}
						else
						{
							curnode = node;
						}

						// if we reach the last character , we have a complete
						// word, but the node might contain other children 
						// as well, so we flag it as a whole word

						if (i == word.size() - 1) curnode->Leaf = true;
					}

					return true;
				}

				//--------------------------------------------------------------------
				// delete a word

				bool DeleteWord(const std::string &word)
				{
					if (word.empty()) return false;

					TrieNode *curnode, *node, *parent;
					std::map<char, TrieNode*>::iterator it;
					char ch;

					curnode = Root;

					for (size_t i = 0; i < word.size(); ++i)
					{

						node = nullptr;

						if (((Flags & DEFAULT_CASE_WORDS) != 0)) ch = word[i];
						else if (((Flags & UPPER_CASE_WORDS) != 0))	ch = toupper(word[i]);
						else if (((Flags & LOWER_CASE_WORDS) != 0))	ch = tolower(word[i]);

						it = curnode->Child.find(ch);

						if (it != curnode->Child.end()) node = (*it).second;

						// no such word is present in the trie

						if (!node) return false;

						curnode = node;

					}

					// this always happens, if the word
					// is present in the trie , we always reach
					// a leaf , it remains to be seen if its
					// a whole word ( leafy node ) or if it
					// is prefix of another word

					if (curnode->Leaf)
					{

						if (curnode->Child.size() != 0)
						{
							curnode->Leaf = false;
						}
						else
						{

							// its a suffix of a prefix we need to 
							// detach each single node and remove the child
							// node from the node's parent itself

							bool done = false;

							while (!done)
							{

								parent = curnode->Parent;

								it = parent->Child.find(curnode->Ch);

								// delete link to current node
								// from parent node
								// deleting reduces the number of children
								// thus when we have a node with zero children
								// it can be removed

								parent->Child.erase(it);

								delete(curnode);

								curnode = parent;

								if (curnode->Child.size() != 0 || curnode->Leaf || curnode == Root) done = true;

							}

						}

					}

					return true;

				}

				//--------------------------------------------------------------------
				// check if word has been inserted into the trie 

				bool IsPresent(const std::string &word)
				{
					TrieNode *curnode, *node;
					std::map<char, TrieNode*>::iterator it;
					char ch;

					curnode = Root;

					for (size_t i = 0; i < word.size(); ++i)
					{

						node = nullptr;

						if (((Flags & DEFAULT_CASE_WORDS) != 0)) ch = word[i];
						else if (((Flags & UPPER_CASE_WORDS) != 0))	ch = toupper(word[i]);
						else if (((Flags & LOWER_CASE_WORDS) != 0))	ch = tolower(word[i]);

						it = curnode->Child.find(ch);

						if (it != curnode->Child.end()) node = (*it).second;

						if (!node) return false;

						curnode = node;

					}

					return curnode->Leaf;
				}

				//--------------------------------------------------------------------
				// finds all words starting with a prefix

				const std::vector< TrieWord > FindWords(const std::string &word)
				{

					TrieNode *curnode, *next;
					std::map<char, TrieNode*>::iterator it;
					bool found;
					char ch;
					std::vector<TrieWord> words;

					curnode = Root;

					found = true;

					for (size_t i = 0; i < word.size() && found; ++i)
					{
						next = nullptr;

						if (((Flags & DEFAULT_CASE_WORDS) != 0)) ch = word[i];
						else if (((Flags & UPPER_CASE_WORDS) != 0))	ch = toupper(word[i]);
						else if (((Flags & LOWER_CASE_WORDS) != 0))	ch = tolower(word[i]);

						it = curnode->Child.find(ch);

						if (it != curnode->Child.end()) next = (*it).second;

						if (!next) found = false;

						curnode = next;
					}

					if (found)
					{
						std::vector<TrieNode*> v;

						RecurseStringPath(curnode, v, words);
					}

					// sort words according to occurrences

					if ((Flags & SORT_BY_STRING_OCCURRENCES) != 0)
					{
						std::sort(words.begin(), words.end());
					}

					return words;
				}

				// -------------------------------------------------------
				// Release all memory and reallocate root

				void Empty()
				{
					RecurseDeleteTree(Root);
					Root = new TrieNode(0, nullptr);
				}

				// -------------------------------------------------------
				// load a dictinoary from a file

				bool LoadDictionary(const std::string &filename)
				{
					std::string vocabulary;
					std::vector< std::string > words;

				//	vml::CMessage::Trace(L"Loading %s\n", vml::strings::convert::ToWString(filename).c_str());

					vocabulary = vml::strings::StringUtils::LoadText(filename);

					words = vml::strings::lexer::Tokenize(vocabulary, "\n", "");

					RecurseDeleteTree(Root);

					Root = new TrieNode(0, nullptr);

				//	vml::CMessage::Trace(L"Loaded %d words\nAdding words to trie...\n", words.size());

					for (size_t i = 0; i < words.size(); ++i)
						AddWord(words[i]);

					vml::CMessage::Trace(L"done\n");

					return true;
				}

				// -------------------------------------------------------
				// getters

				const unsigned int GetFlags() const { return Flags; }
				const bool IsSortedByStringOccurrences() { return ((Flags & SORT_BY_STRING_OCCURRENCES) != 0); }
				const bool IsSortedByStringOrder() { return ((Flags & SORT_BY_STRING_ORDER) != 0); }
				const bool AreWordsDefaulted() { return ((Flags & DEFAULT_CASE_WORDS) != 0); }
				const bool AreWordsUpperCased() { return ((Flags & UPPER_CASE_WORDS) != 0); }
				const bool AreWordsLowerCased() { return ((Flags & LOWER_CASE_WORDS) != 0); }

				// -------------------------------------------------------
				// setters

				void SetFlags(unsigned int flags) { Flags = flags; }
				void SortByStringOccurrences() { Flags |= SORT_BY_STRING_OCCURRENCES; Flags &= ~SORT_BY_STRING_ORDER; }
				void SortByStringOrder() { Flags |= SORT_BY_STRING_ORDER; Flags &= ~SORT_BY_STRING_OCCURRENCES; }
				void SetDefaultWords() { Flags |= DEFAULT_CASE_WORDS; Flags &= ~UPPER_CASE_WORDS; Flags &= ~LOWER_CASE_WORDS; }
				void SetLowerCaseWords() { Flags |= LOWER_CASE_WORDS; Flags &= ~UPPER_CASE_WORDS; Flags &= ~DEFAULT_CASE_WORDS; }
				void SetUpperCaseWords() { Flags |= UPPER_CASE_WORDS; Flags &= ~LOWER_CASE_WORDS; Flags &= ~DEFAULT_CASE_WORDS; }

				// -------------------------------------------------------
				// ctor / dtor

				StringTrie(unsigned int flags = SORT_BY_STRING_ORDER | LOWER_CASE_WORDS)
				{
					Root = new TrieNode(0, nullptr);
					Flags = flags;

					// validate flags 
					// some flags must be set if sorting type
					// has not been set, the program will fall back
					// to the default settings which are 
					// sort by string occurencies ( frequency of search )
					// and default words ( no upper or lower case conversion )

					// validate string sorting method

					if (!((IsSortedByStringOccurrences() && !IsSortedByStringOrder()) ||
						(!IsSortedByStringOccurrences() && IsSortedByStringOrder()))) SortByStringOccurrences();

					// validate word case

					if (!((AreWordsDefaulted() && !AreWordsUpperCased() && !AreWordsLowerCased()) ||
						(!AreWordsDefaulted() && AreWordsUpperCased() && !AreWordsLowerCased()) ||
						(!AreWordsDefaulted() && !AreWordsUpperCased() && AreWordsLowerCased()))) SetDefaultWords();

				}

				~StringTrie()
				{
					RecurseDeleteTree(Root);
				}

			};
		}
	}
}
