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

#include <string>
#include <sstream>	

/////////////////////////////////////////////////
// error message report class
// compile using multi bytes character set 
// in visual studio :
// project properties ->general->character set 
// specific to windows platform

#ifdef _WIN32 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#endif	

namespace vml
{
	namespace os
	{
		/////////////////////////////////////////////////////////////////////////////////////

		class Message
		{
			private:
				
				template <typename T>
				static void printargument(std::stringstream& ss, T t)
				{
					ss << t << " ";
				}

				template <typename... t>
				static std::string WArgs(t&&... args)
				{
					std::stringstream ss;
					int dummy[] = { 0, ((void)printargument(ss,std::forward<t>(args)),0)... };
					return ss.str();
				}

			public:
			
				template <typename... Args>
				static void Trace(Args&&... args)
				{
					std::string buffer = WArgs(args...);
					size_t len = buffer.size();
					if (len == 0) throw;
					std::wstring to(&buffer[0], &buffer[len - 1]);
					to += L"\n";
					OutputDebugStringW(&to[0]);
				}

				template <typename... Args>
				static void Error(Args&&... args)
				{
					std::string buffer = WArgs(args...);
					size_t len = buffer.size();
					if (len == 0) throw;
					std::wstring to(&buffer[0], &buffer[len - 1]);
					MessageBoxW(0, &to[0], L"Fatal error Message", MB_OK);
					exit(EXIT_FAILURE);
				}
			
				template <typename... Args>
				static void Warning(Args&&... args)
				{
					std::string buffer = WArgs(args...);
					size_t len = buffer.size();
					if (len == 0) throw;
					std::wstring to(&buffer[0], &buffer[len - 1]);
					MessageBoxW(0, &to[0], L"Warning Message", MB_OK);
					exit(EXIT_FAILURE);
				}
			
		};

	}

}

