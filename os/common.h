#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
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

//////////////////////////////////////////////////////////////////////////////
//	MSVC will spit out all sorts of useless warnings if
//	you create vectors of strings, this pragma gets rid of them.
//	comment if you feel safer

#pragma warning(disable: 4786)  // identifier was truncated to '255' characters in the debug information.
#pragma warning(disable: 4244)  // Disable double to float warning
#pragma warning(disable: 4035)  // Disable warning when a function returns a value inside an __asm block
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning(disable: 4710)  // 'function' : function not inlined

/////////////////////////////////////////////////////////////////////////////////////
// disable deprecatred warnings

#if (defined(_WIN32) || defined(_WIN64)) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS 1
#endif

/////////////////////////////////////////////////////////////////////////////////////
// avoid clash with min / max stl functions

#define NOMINMAX   
#define WIN32_LEAN_AND_MEAN

/////////////////////////////////////////////////////////////////////////////////////
//	include libs used by other components

#include <conio.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>

//#include <stdexcept>
//#include <cstdlib>
//#include <string>				
//#include <sstream>	
#include <chrono>

//#include <unordered_set>
#include <unordered_map>
//#include <iomanip>
#include <random>
//#include <cstdarg>
//#include <time.h>
//#include <vector>
//#include <math.h>
#include <regex>
#include <format>
#include <queue>
#include <mutex>
#include <filesystem>
#include <stack>
#include <unordered_set>

/////////////////////////////////////////////////////////////////////
// safe deleting of pointers

namespace vml
{
	namespace os
	{
		template< class T >
		void SafeDelete(T*& pVal)
		{
			if (pVal)
			{
				delete pVal;
				pVal = nullptr;
			}
		}

		template< class T >
		void SafeDeleteArray(T*& pVal)
		{
			if (pVal)
			{
				delete[] pVal;
				pVal = nullptr;
			}
		}
	}
}


