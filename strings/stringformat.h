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

namespace vml
{
	namespace strings
	{

		class StringFormat
		{
			private:

				class ArgBase
				{
					public:

						ArgBase() {}
						virtual ~ArgBase() {}
						virtual void Format(std::ostringstream &ss, const std::string& fmt) = 0;
				};

				template <class T>
				class Arg : public ArgBase
				{
					public:

						Arg(T arg) : m_arg(arg) {}
						virtual ~Arg() {}
						virtual void Format(std::ostringstream &ss, const std::string& fmt)
						{
							ss << m_arg;
						}

					private:

						T m_arg;
				};

				class ArgArray : public std::vector < ArgBase* >
				{
					public:

						ArgArray() {}
						~ArgArray()
						{
							std::for_each(begin(), end(), [](ArgBase* p) { delete p; });
						}
				};

				// -----------------------------------------------------------------------------

				static void FormatItem(std::ostringstream& ss, const std::string& item, const ArgArray& args)
				{
					int index = 0;
					int alignment = 0;
					std::string fmt;

					char* endptr = nullptr;

					index = strtol(&item[0], &endptr, 10);

					if (index < 0 || index >= int(args.size()))
					{
						return;
					}

					if (*endptr == ',')
					{
						alignment = strtol(endptr + 1, &endptr, 10);

						if (alignment > 0)
						{
							ss << std::right << std::setw(alignment);
						}
						else if (alignment < 0)
						{
							ss << std::left << std::setw(-alignment);
						}
					}

					if (*endptr == ':')
					{
						fmt = endptr + 1;
					}

					args[index]->Format(ss, fmt);

					return;
				}

				template <class T>
				static void Transfer(ArgArray& argArray, T t)
				{
					argArray.push_back(new Arg<T>(t));
				}

				template <class T, typename... Args>
				static void Transfer(ArgArray& argArray, T t, Args&&... args)
				{
					Transfer(argArray, t);
					Transfer(argArray, args...);
				}

		public:

				//////////////////////////////////////////////////////////////////////
				// formats text with varying parameters

				template <typename... Args>
				static std::string Text(const std::string& format, Args&&... args)
				{
					if (sizeof...(args) == 0)
						return format;

					ArgArray argArray;
					Transfer(argArray, args...);
					size_t start = 0;
					size_t pos = 0;

					std::ostringstream ss;

					while (true)
					{
						pos = format.find('{', start);

						if (pos == std::string::npos)
						{
							ss << format.substr(start);
							break;
						}

						ss << format.substr(start, pos - start);

						if (format[pos + 1] == '{')
						{
							ss << '{';
							start = pos + 2;
							continue;
						}

						start = pos + 1;

						pos = format.find('}', start);

						if (pos == std::string::npos)
						{
							ss << format.substr(start - 1);
							break;
						}

						FormatItem(ss, format.substr(start, pos - start), argArray);

						start = pos + 1;
					}

					return ss.str();
				}

				///////////////////////////////////////////////////////////////////////////
				// text word wrapping

				static void WordWrap(const std::string& inputString,
									 std::vector<std::string> &outputString,
									 unsigned int lineLength)
				{
					std::istringstream iss(inputString);

					std::string line;

					do
					{
						std::string word;
						iss >> word;

						if (line.length() + word.length() > lineLength)
						{
							outputString.emplace_back(line);
							line.clear();
						}

						line += word + " ";

					} while (iss);

					if (!line.empty())
					{
						outputString.emplace_back(line);
					}
				}

				// ------------------------------------------
				// cotr / dtor

				StringFormat()
				{}
				
				~StringFormat()
				{}

		};
		
	} // end of string namespace

} // end of vml namespace
