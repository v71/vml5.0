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

	namespace utils
	{

		///////////////////////////////////////////////////////////
		// 32 bit set class , this class is to be used as
		// a kind of static class.

		namespace bits32
		{

				///////////////////////////////////////////////
				// this array block is a 9*256 raw memory block
				// which contains in the first position the
				// number of activer bits making up the 
				// corresponding value , the other 8 values
				// represent the corresponding active bits
				// example value = 8
				// memory block : 1, 0, 0, 0, 1, 0, 0, 0, 0
				// number of active bits is the first 
				// value in the memory block, which is 1
				// starting from the first value at the forth 
				// position we have a 1 which is 2 raised 
				// to the power of 3 , thus we have the value of 8 

				static const unsigned char PopData[] =
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 1, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 1, 0, 0, 0, 0, 0, 0,
					2, 1, 1, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 1, 0, 0, 0, 0, 0,
					2, 1, 0, 1, 0, 0, 0, 0, 0,
					2, 0, 1, 1, 0, 0, 0, 0, 0,
					3, 1, 1, 1, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 1, 0, 0, 0, 0,
					2, 1, 0, 0, 1, 0, 0, 0, 0,
					2, 0, 1, 0, 1, 0, 0, 0, 0,
					3, 1, 1, 0, 1, 0, 0, 0, 0,
					2, 0, 0, 1, 1, 0, 0, 0, 0,
					3, 1, 0, 1, 1, 0, 0, 0, 0,
					3, 0, 1, 1, 1, 0, 0, 0, 0,
					4, 1, 1, 1, 1, 0, 0, 0, 0,
					1, 0, 0, 0, 0, 1, 0, 0, 0,
					2, 1, 0, 0, 0, 1, 0, 0, 0,
					2, 0, 1, 0, 0, 1, 0, 0, 0,
					3, 1, 1, 0, 0, 1, 0, 0, 0,
					2, 0, 0, 1, 0, 1, 0, 0, 0,
					3, 1, 0, 1, 0, 1, 0, 0, 0,
					3, 0, 1, 1, 0, 1, 0, 0, 0,
					4, 1, 1, 1, 0, 1, 0, 0, 0,
					2, 0, 0, 0, 1, 1, 0, 0, 0,
					3, 1, 0, 0, 1, 1, 0, 0, 0,
					3, 0, 1, 0, 1, 1, 0, 0, 0,
					4, 1, 1, 0, 1, 1, 0, 0, 0,
					3, 0, 0, 1, 1, 1, 0, 0, 0,
					4, 1, 0, 1, 1, 1, 0, 0, 0,
					4, 0, 1, 1, 1, 1, 0, 0, 0,
					5, 1, 1, 1, 1, 1, 0, 0, 0,
					1, 0, 0, 0, 0, 0, 1, 0, 0,
					2, 1, 0, 0, 0, 0, 1, 0, 0,
					2, 0, 1, 0, 0, 0, 1, 0, 0,
					3, 1, 1, 0, 0, 0, 1, 0, 0,
					2, 0, 0, 1, 0, 0, 1, 0, 0,
					3, 1, 0, 1, 0, 0, 1, 0, 0,
					3, 0, 1, 1, 0, 0, 1, 0, 0,
					4, 1, 1, 1, 0, 0, 1, 0, 0,
					2, 0, 0, 0, 1, 0, 1, 0, 0,
					3, 1, 0, 0, 1, 0, 1, 0, 0,
					3, 0, 1, 0, 1, 0, 1, 0, 0,
					4, 1, 1, 0, 1, 0, 1, 0, 0,
					3, 0, 0, 1, 1, 0, 1, 0, 0,
					4, 1, 0, 1, 1, 0, 1, 0, 0,
					4, 0, 1, 1, 1, 0, 1, 0, 0,
					5, 1, 1, 1, 1, 0, 1, 0, 0,
					2, 0, 0, 0, 0, 1, 1, 0, 0,
					3, 1, 0, 0, 0, 1, 1, 0, 0,
					3, 0, 1, 0, 0, 1, 1, 0, 0,
					4, 1, 1, 0, 0, 1, 1, 0, 0,
					3, 0, 0, 1, 0, 1, 1, 0, 0,
					4, 1, 0, 1, 0, 1, 1, 0, 0,
					4, 0, 1, 1, 0, 1, 1, 0, 0,
					5, 1, 1, 1, 0, 1, 1, 0, 0,
					3, 0, 0, 0, 1, 1, 1, 0, 0,
					4, 1, 0, 0, 1, 1, 1, 0, 0,
					4, 0, 1, 0, 1, 1, 1, 0, 0,
					5, 1, 1, 0, 1, 1, 1, 0, 0,
					4, 0, 0, 1, 1, 1, 1, 0, 0,
					5, 1, 0, 1, 1, 1, 1, 0, 0,
					5, 0, 1, 1, 1, 1, 1, 0, 0,
					6, 1, 1, 1, 1, 1, 1, 0, 0,
					1, 0, 0, 0, 0, 0, 0, 1, 0,
					2, 1, 0, 0, 0, 0, 0, 1, 0,
					2, 0, 1, 0, 0, 0, 0, 1, 0,
					3, 1, 1, 0, 0, 0, 0, 1, 0,
					2, 0, 0, 1, 0, 0, 0, 1, 0,
					3, 1, 0, 1, 0, 0, 0, 1, 0,
					3, 0, 1, 1, 0, 0, 0, 1, 0,
					4, 1, 1, 1, 0, 0, 0, 1, 0,
					2, 0, 0, 0, 1, 0, 0, 1, 0,
					3, 1, 0, 0, 1, 0, 0, 1, 0,
					3, 0, 1, 0, 1, 0, 0, 1, 0,
					4, 1, 1, 0, 1, 0, 0, 1, 0,
					3, 0, 0, 1, 1, 0, 0, 1, 0,
					4, 1, 0, 1, 1, 0, 0, 1, 0,
					4, 0, 1, 1, 1, 0, 0, 1, 0,
					5, 1, 1, 1, 1, 0, 0, 1, 0,
					2, 0, 0, 0, 0, 1, 0, 1, 0,
					3, 1, 0, 0, 0, 1, 0, 1, 0,
					3, 0, 1, 0, 0, 1, 0, 1, 0,
					4, 1, 1, 0, 0, 1, 0, 1, 0,
					3, 0, 0, 1, 0, 1, 0, 1, 0,
					4, 1, 0, 1, 0, 1, 0, 1, 0,
					4, 0, 1, 1, 0, 1, 0, 1, 0,
					5, 1, 1, 1, 0, 1, 0, 1, 0,
					3, 0, 0, 0, 1, 1, 0, 1, 0,
					4, 1, 0, 0, 1, 1, 0, 1, 0,
					4, 0, 1, 0, 1, 1, 0, 1, 0,
					5, 1, 1, 0, 1, 1, 0, 1, 0,
					4, 0, 0, 1, 1, 1, 0, 1, 0,
					5, 1, 0, 1, 1, 1, 0, 1, 0,
					5, 0, 1, 1, 1, 1, 0, 1, 0,
					6, 1, 1, 1, 1, 1, 0, 1, 0,
					2, 0, 0, 0, 0, 0, 1, 1, 0,
					3, 1, 0, 0, 0, 0, 1, 1, 0,
					3, 0, 1, 0, 0, 0, 1, 1, 0,
					4, 1, 1, 0, 0, 0, 1, 1, 0,
					3, 0, 0, 1, 0, 0, 1, 1, 0,
					4, 1, 0, 1, 0, 0, 1, 1, 0,
					4, 0, 1, 1, 0, 0, 1, 1, 0,
					5, 1, 1, 1, 0, 0, 1, 1, 0,
					3, 0, 0, 0, 1, 0, 1, 1, 0,
					4, 1, 0, 0, 1, 0, 1, 1, 0,
					4, 0, 1, 0, 1, 0, 1, 1, 0,
					5, 1, 1, 0, 1, 0, 1, 1, 0,
					4, 0, 0, 1, 1, 0, 1, 1, 0,
					5, 1, 0, 1, 1, 0, 1, 1, 0,
					5, 0, 1, 1, 1, 0, 1, 1, 0,
					6, 1, 1, 1, 1, 0, 1, 1, 0,
					3, 0, 0, 0, 0, 1, 1, 1, 0,
					4, 1, 0, 0, 0, 1, 1, 1, 0,
					4, 0, 1, 0, 0, 1, 1, 1, 0,
					5, 1, 1, 0, 0, 1, 1, 1, 0,
					4, 0, 0, 1, 0, 1, 1, 1, 0,
					5, 1, 0, 1, 0, 1, 1, 1, 0,
					5, 0, 1, 1, 0, 1, 1, 1, 0,
					6, 1, 1, 1, 0, 1, 1, 1, 0,
					4, 0, 0, 0, 1, 1, 1, 1, 0,
					5, 1, 0, 0, 1, 1, 1, 1, 0,
					5, 0, 1, 0, 1, 1, 1, 1, 0,
					6, 1, 1, 0, 1, 1, 1, 1, 0,
					5, 0, 0, 1, 1, 1, 1, 1, 0,
					6, 1, 0, 1, 1, 1, 1, 1, 0,
					6, 0, 1, 1, 1, 1, 1, 1, 0,
					7, 1, 1, 1, 1, 1, 1, 1, 0,
					1, 0, 0, 0, 0, 0, 0, 0, 1,
					2, 1, 0, 0, 0, 0, 0, 0, 1,
					2, 0, 1, 0, 0, 0, 0, 0, 1,
					3, 1, 1, 0, 0, 0, 0, 0, 1,
					2, 0, 0, 1, 0, 0, 0, 0, 1,
					3, 1, 0, 1, 0, 0, 0, 0, 1,
					3, 0, 1, 1, 0, 0, 0, 0, 1,
					4, 1, 1, 1, 0, 0, 0, 0, 1,
					2, 0, 0, 0, 1, 0, 0, 0, 1,
					3, 1, 0, 0, 1, 0, 0, 0, 1,
					3, 0, 1, 0, 1, 0, 0, 0, 1,
					4, 1, 1, 0, 1, 0, 0, 0, 1,
					3, 0, 0, 1, 1, 0, 0, 0, 1,
					4, 1, 0, 1, 1, 0, 0, 0, 1,
					4, 0, 1, 1, 1, 0, 0, 0, 1,
					5, 1, 1, 1, 1, 0, 0, 0, 1,
					2, 0, 0, 0, 0, 1, 0, 0, 1,
					3, 1, 0, 0, 0, 1, 0, 0, 1,
					3, 0, 1, 0, 0, 1, 0, 0, 1,
					4, 1, 1, 0, 0, 1, 0, 0, 1,
					3, 0, 0, 1, 0, 1, 0, 0, 1,
					4, 1, 0, 1, 0, 1, 0, 0, 1,
					4, 0, 1, 1, 0, 1, 0, 0, 1,
					5, 1, 1, 1, 0, 1, 0, 0, 1,
					3, 0, 0, 0, 1, 1, 0, 0, 1,
					4, 1, 0, 0, 1, 1, 0, 0, 1,
					4, 0, 1, 0, 1, 1, 0, 0, 1,
					5, 1, 1, 0, 1, 1, 0, 0, 1,
					4, 0, 0, 1, 1, 1, 0, 0, 1,
					5, 1, 0, 1, 1, 1, 0, 0, 1,
					5, 0, 1, 1, 1, 1, 0, 0, 1,
					6, 1, 1, 1, 1, 1, 0, 0, 1,
					2, 0, 0, 0, 0, 0, 1, 0, 1,
					3, 1, 0, 0, 0, 0, 1, 0, 1,
					3, 0, 1, 0, 0, 0, 1, 0, 1,
					4, 1, 1, 0, 0, 0, 1, 0, 1,
					3, 0, 0, 1, 0, 0, 1, 0, 1,
					4, 1, 0, 1, 0, 0, 1, 0, 1,
					4, 0, 1, 1, 0, 0, 1, 0, 1,
					5, 1, 1, 1, 0, 0, 1, 0, 1,
					3, 0, 0, 0, 1, 0, 1, 0, 1,
					4, 1, 0, 0, 1, 0, 1, 0, 1,
					4, 0, 1, 0, 1, 0, 1, 0, 1,
					5, 1, 1, 0, 1, 0, 1, 0, 1,
					4, 0, 0, 1, 1, 0, 1, 0, 1,
					5, 1, 0, 1, 1, 0, 1, 0, 1,
					5, 0, 1, 1, 1, 0, 1, 0, 1,
					6, 1, 1, 1, 1, 0, 1, 0, 1,
					3, 0, 0, 0, 0, 1, 1, 0, 1,
					4, 1, 0, 0, 0, 1, 1, 0, 1,
					4, 0, 1, 0, 0, 1, 1, 0, 1,
					5, 1, 1, 0, 0, 1, 1, 0, 1,
					4, 0, 0, 1, 0, 1, 1, 0, 1,
					5, 1, 0, 1, 0, 1, 1, 0, 1,
					5, 0, 1, 1, 0, 1, 1, 0, 1,
					6, 1, 1, 1, 0, 1, 1, 0, 1,
					4, 0, 0, 0, 1, 1, 1, 0, 1,
					5, 1, 0, 0, 1, 1, 1, 0, 1,
					5, 0, 1, 0, 1, 1, 1, 0, 1,
					6, 1, 1, 0, 1, 1, 1, 0, 1,
					5, 0, 0, 1, 1, 1, 1, 0, 1,
					6, 1, 0, 1, 1, 1, 1, 0, 1,
					6, 0, 1, 1, 1, 1, 1, 0, 1,
					7, 1, 1, 1, 1, 1, 1, 0, 1,
					2, 0, 0, 0, 0, 0, 0, 1, 1,
					3, 1, 0, 0, 0, 0, 0, 1, 1,
					3, 0, 1, 0, 0, 0, 0, 1, 1,
					4, 1, 1, 0, 0, 0, 0, 1, 1,
					3, 0, 0, 1, 0, 0, 0, 1, 1,
					4, 1, 0, 1, 0, 0, 0, 1, 1,
					4, 0, 1, 1, 0, 0, 0, 1, 1,
					5, 1, 1, 1, 0, 0, 0, 1, 1,
					3, 0, 0, 0, 1, 0, 0, 1, 1,
					4, 1, 0, 0, 1, 0, 0, 1, 1,
					4, 0, 1, 0, 1, 0, 0, 1, 1,
					5, 1, 1, 0, 1, 0, 0, 1, 1,
					4, 0, 0, 1, 1, 0, 0, 1, 1,
					5, 1, 0, 1, 1, 0, 0, 1, 1,
					5, 0, 1, 1, 1, 0, 0, 1, 1,
					6, 1, 1, 1, 1, 0, 0, 1, 1,
					3, 0, 0, 0, 0, 1, 0, 1, 1,
					4, 1, 0, 0, 0, 1, 0, 1, 1,
					4, 0, 1, 0, 0, 1, 0, 1, 1,
					5, 1, 1, 0, 0, 1, 0, 1, 1,
					4, 0, 0, 1, 0, 1, 0, 1, 1,
					5, 1, 0, 1, 0, 1, 0, 1, 1,
					5, 0, 1, 1, 0, 1, 0, 1, 1,
					6, 1, 1, 1, 0, 1, 0, 1, 1,
					4, 0, 0, 0, 1, 1, 0, 1, 1,
					5, 1, 0, 0, 1, 1, 0, 1, 1,
					5, 0, 1, 0, 1, 1, 0, 1, 1,
					6, 1, 1, 0, 1, 1, 0, 1, 1,
					5, 0, 0, 1, 1, 1, 0, 1, 1,
					6, 1, 0, 1, 1, 1, 0, 1, 1,
					6, 0, 1, 1, 1, 1, 0, 1, 1,
					7, 1, 1, 1, 1, 1, 0, 1, 1,
					3, 0, 0, 0, 0, 0, 1, 1, 1,
					4, 1, 0, 0, 0, 0, 1, 1, 1,
					4, 0, 1, 0, 0, 0, 1, 1, 1,
					5, 1, 1, 0, 0, 0, 1, 1, 1,
					4, 0, 0, 1, 0, 0, 1, 1, 1,
					5, 1, 0, 1, 0, 0, 1, 1, 1,
					5, 0, 1, 1, 0, 0, 1, 1, 1,
					6, 1, 1, 1, 0, 0, 1, 1, 1,
					4, 0, 0, 0, 1, 0, 1, 1, 1,
					5, 1, 0, 0, 1, 0, 1, 1, 1,
					5, 0, 1, 0, 1, 0, 1, 1, 1,
					6, 1, 1, 0, 1, 0, 1, 1, 1,
					5, 0, 0, 1, 1, 0, 1, 1, 1,
					6, 1, 0, 1, 1, 0, 1, 1, 1,
					6, 0, 1, 1, 1, 0, 1, 1, 1,
					7, 1, 1, 1, 1, 0, 1, 1, 1,
					4, 0, 0, 0, 0, 1, 1, 1, 1,
					5, 1, 0, 0, 0, 1, 1, 1, 1,
					5, 0, 1, 0, 0, 1, 1, 1, 1,
					6, 1, 1, 0, 0, 1, 1, 1, 1,
					5, 0, 0, 1, 0, 1, 1, 1, 1,
					6, 1, 0, 1, 0, 1, 1, 1, 1,
					6, 0, 1, 1, 0, 1, 1, 1, 1,
					7, 1, 1, 1, 0, 1, 1, 1, 1,
					5, 0, 0, 0, 1, 1, 1, 1, 1,
					6, 1, 0, 0, 1, 1, 1, 1, 1,
					6, 0, 1, 0, 1, 1, 1, 1, 1,
					7, 1, 1, 0, 1, 1, 1, 1, 1,
					6, 0, 0, 1, 1, 1, 1, 1, 1,
					7, 1, 0, 1, 1, 1, 1, 1, 1,
					7, 0, 1, 1, 1, 1, 1, 1, 1,
					8, 1, 1, 1, 1, 1, 1, 1, 1,
				};

				//////////////////////////////////////////////////////
				// bits positions

				const static unsigned int BIT0  =          1;
				const static unsigned int BIT1  =		   2;
				const static unsigned int BIT2  =		   4;
				const static unsigned int BIT3  =		   8;
				const static unsigned int BIT4  =	 	  16;
				const static unsigned int BIT5  =		  32;
				const static unsigned int BIT6  =		  64;
				const static unsigned int BIT7  =        128;
				const static unsigned int BIT8  =        256;
				const static unsigned int BIT9  =	     512;
				const static unsigned int BIT10 =       1024;
				const static unsigned int BIT11 =	    2048;
				const static unsigned int BIT12 =       4096;
				const static unsigned int BIT13 =       8192;
				const static unsigned int BIT14 =      16384;
				const static unsigned int BIT15 =	   32768;
				const static unsigned int BIT16 =      65536;
				const static unsigned int BIT17 =	  131072;
				const static unsigned int BIT18 =     262144;
				const static unsigned int BIT19 =	  524288;
				const static unsigned int BIT20 =    1048576;
				const static unsigned int BIT21 =    2097152;
				const static unsigned int BIT22 =    4194304;
				const static unsigned int BIT23 =    8388608;
				const static unsigned int BIT24 =	16777216;
				const static unsigned int BIT25 =   33554432;
				const static unsigned int BIT26 =	67108864;
				const static unsigned int BIT27 =  134217728;
				const static unsigned int BIT28 =  268435456;
				const static unsigned int BIT29 =  536870912;
				const static unsigned int BIT30 = 1073741824;
				const static unsigned int BIT31 = 2147483648;
		
				/////////////////////////////////////////////////////////////////
				// clear all bit flags

				static void Clear( unsigned int &bitflag )
				{
					bitflag=0;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to true 

				static void SetToTrue( unsigned int &bitflag,unsigned int bit )
				{
					bitflag |=  bit;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to false

				static void SetToFalse( unsigned int &bitflag,unsigned int bit )
				{
					bitflag &= ~bit;
				}

				/////////////////////////////////////////////////////////////////
				// xor a single bit

				static void Toggle( unsigned int &bitflag,unsigned int bit )
				{
					bitflag ^= bit;
				}

				/////////////////////////////////////////////////////////////////
				// gets value of single bit

				static bool Get( const unsigned int &bitflag,unsigned int bit ) 
				{
					return ( ( bitflag & bit )!=0 );
				}

				/////////////////////////////////////////////////////////////////
				// sets value of single bit

				static void SetMask( unsigned int &bitflag,unsigned int mask )
				{
					bitflag=mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Bit functions based on bitmask

				static unsigned int SetBitsExcept( unsigned int &bitflag, unsigned int mask )
				{
					return bitflag | ~mask;
				}

				static unsigned int ClearBits( unsigned int &bitflag, unsigned int mask )
				{
					return bitflag & ~mask;
				}

				static unsigned int ClearBitsExcept(unsigned int &bitflag, unsigned int mask)
				{
					return bitflag & mask;
				}

				static int PosFromBitValue(int bitvalue)
				{
					switch (bitvalue)
					{
						case BIT0  : return  0; break;
						case BIT1  : return  1; break;
						case BIT2  : return  2; break;
						case BIT3  : return  3; break;
						case BIT4  : return  4; break;
						case BIT5  : return  5; break;
						case BIT6  : return  6; break;
						case BIT7  : return  7; break;
						case BIT8  : return  8; break;
						case BIT9  : return  9; break;
						case BIT10 : return 10; break;
						case BIT11 : return 11; break;
						case BIT12 : return 12; break;
						case BIT13 : return 13; break;
						case BIT14 : return 14; break;
						case BIT15 : return 15; break;
						case BIT16 : return 16; break;
						case BIT17 : return 17; break;
						case BIT18 : return 18; break;
						case BIT19 : return 19; break;
						case BIT20 : return 20; break;
						case BIT21 : return 21; break;
						case BIT22 : return 22; break;
						case BIT23 : return 23; break;
						case BIT24 : return 24; break;
						case BIT25 : return 25; break;
						case BIT26 : return 26; break;
						case BIT27 : return 27; break;
						case BIT28 : return 28; break;
						case BIT29 : return 29; break;
						case BIT30 : return 30; break;
						case BIT31 : return 31; break;

					}

					return - 1;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if any bit is set based on the bitmask

				static bool IsAnyBitSet( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask ) != 0;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bits are 1 based on the bitmask

				static bool AreAllBitsSet( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask) == mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bit are 0 based on the bitmask

				static bool AreAllBitsClear( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask ) == 0;
				}

				/////////////////////////////////////////////////////////////////////////////
				/// Returns the number of 1's set in the given value.

				static unsigned int PopCount(unsigned int &bitflag)
				{
					unsigned int bits = 0;
					if (bitflag &  BIT0) bits++;
					if (bitflag &  BIT1) bits++;
					if (bitflag &  BIT2) bits++;
					if (bitflag &  BIT3) bits++;
					if (bitflag &  BIT4) bits++;
					if (bitflag &  BIT5) bits++;
					if (bitflag &  BIT6) bits++;
					if (bitflag &  BIT7) bits++;
					if (bitflag &  BIT8) bits++;
					if (bitflag &  BIT9) bits++;
					if (bitflag & BIT10) bits++;
					if (bitflag & BIT11) bits++;
					if (bitflag & BIT12) bits++;
					if (bitflag & BIT13) bits++;
					if (bitflag & BIT14) bits++;
					if (bitflag & BIT15) bits++;
					if (bitflag & BIT16) bits++;
					if (bitflag & BIT17) bits++;
					if (bitflag & BIT18) bits++;
					if (bitflag & BIT19) bits++;
					if (bitflag & BIT20) bits++;
					if (bitflag & BIT21) bits++;
					if (bitflag & BIT22) bits++;
					if (bitflag & BIT23) bits++;
					if (bitflag & BIT24) bits++;
					if (bitflag & BIT25) bits++;
					if (bitflag & BIT26) bits++;
					if (bitflag & BIT27) bits++;
					if (bitflag & BIT28) bits++;
					if (bitflag & BIT29) bits++;
					if (bitflag & BIT30) bits++;
					if (bitflag & BIT31) bits++;
					return bits;
				}

				/////////////////////////////////////////////////////////////////////////////
				/// Returns the number of 0's set in the given value.

				static unsigned int ZeroPopCount(unsigned int &bitflag)
				{
					unsigned int bits = 32;
					if (bitflag &  BIT0) bits--;
					if (bitflag &  BIT1) bits--;
					if (bitflag &  BIT2) bits--;
					if (bitflag &  BIT3) bits--;
					if (bitflag &  BIT4) bits--;
					if (bitflag &  BIT5) bits--;
					if (bitflag &  BIT6) bits--;
					if (bitflag &  BIT7) bits--;
					if (bitflag &  BIT8) bits--;
					if (bitflag &  BIT9) bits--;
					if (bitflag & BIT10) bits--;
					if (bitflag & BIT11) bits--;
					if (bitflag & BIT12) bits--;
					if (bitflag & BIT13) bits--;
					if (bitflag & BIT14) bits--;
					if (bitflag & BIT15) bits--;
					if (bitflag & BIT16) bits--;
					if (bitflag & BIT17) bits--;
					if (bitflag & BIT18) bits--;
					if (bitflag & BIT19) bits--;
					if (bitflag & BIT20) bits--;
					if (bitflag & BIT21) bits--;
					if (bitflag & BIT22) bits--;
					if (bitflag & BIT23) bits--;
					if (bitflag & BIT24) bits--;
					if (bitflag & BIT25) bits--;
					if (bitflag & BIT26) bits--;
					if (bitflag & BIT27) bits--;
					if (bitflag & BIT28) bits--;
					if (bitflag & BIT29) bits--;
					if (bitflag & BIT30) bits--;
					if (bitflag & BIT31) bits--;
					return bits;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Clears the LSB bit and returns the zero-based index of where the bit was set.

				static int ExtractLSB(unsigned int *bitflag)
				{
					if ((*bitflag & (1<< 0)) != 0) {*bitflag &= ~(1<< 0); return  0; }
					if ((*bitflag & (1<< 1)) != 0) {*bitflag &= ~(1<< 1); return  1; }
					if ((*bitflag & (1<< 2)) != 0) {*bitflag &= ~(1<< 2); return  2; }
					if ((*bitflag & (1<< 3)) != 0) {*bitflag &= ~(1<< 3); return  3; }
					if ((*bitflag & (1<< 4)) != 0) {*bitflag &= ~(1<< 4); return  4; }
					if ((*bitflag & (1<< 5)) != 0) {*bitflag &= ~(1<< 5); return  5; }
					if ((*bitflag & (1<< 6)) != 0) {*bitflag &= ~(1<< 6); return  6; }
					if ((*bitflag & (1<< 7)) != 0) {*bitflag &= ~(1<< 7); return  7; }
					if ((*bitflag & (1<< 8)) != 0) {*bitflag &= ~(1<< 8); return  8; }
					if ((*bitflag & (1<< 9)) != 0) {*bitflag &= ~(1<< 9); return  9; }
					if ((*bitflag & (1<<10)) != 0) {*bitflag &= ~(1<<10); return 10; }
					if ((*bitflag & (1<<11)) != 0) {*bitflag &= ~(1<<11); return 11; }
					if ((*bitflag & (1<<12)) != 0) {*bitflag &= ~(1<<12); return 12; }
					if ((*bitflag & (1<<13)) != 0) {*bitflag &= ~(1<<13); return 13; }
					if ((*bitflag & (1<<14)) != 0) {*bitflag &= ~(1<<14); return 14; }
					if ((*bitflag & (1<<15)) != 0) {*bitflag &= ~(1<<15); return 15; }
					if ((*bitflag & (1<<16)) != 0) {*bitflag &= ~(1<<16); return 16; }
					if ((*bitflag & (1<<17)) != 0) {*bitflag &= ~(1<<17); return 17; }
					if ((*bitflag & (1<<18)) != 0) {*bitflag &= ~(1<<18); return 18; }
					if ((*bitflag & (1<<19)) != 0) {*bitflag &= ~(1<<19); return 19; }
					if ((*bitflag & (1<<20)) != 0) {*bitflag &= ~(1<<20); return 20; }
					if ((*bitflag & (1<<21)) != 0) {*bitflag &= ~(1<<21); return 21; }
					if ((*bitflag & (1<<22)) != 0) {*bitflag &= ~(1<<22); return 22; }
					if ((*bitflag & (1<<23)) != 0) {*bitflag &= ~(1<<23); return 23; }
					if ((*bitflag & (1<<24)) != 0) {*bitflag &= ~(1<<24); return 24; }
					if ((*bitflag & (1<<25)) != 0) {*bitflag &= ~(1<<25); return 25; }
					if ((*bitflag & (1<<26)) != 0) {*bitflag &= ~(1<<26); return 26; }
					if ((*bitflag & (1<<27)) != 0) {*bitflag &= ~(1<<27); return 27; }
					if ((*bitflag & (1<<28)) != 0) {*bitflag &= ~(1<<28); return 28; }
					if ((*bitflag & (1<<29)) != 0) {*bitflag &= ~(1<<29); return 29; }
					if ((*bitflag & (1<<30)) != 0) {*bitflag &= ~(1<<30); return 30; }
					if ((*bitflag & (1<<31)) != 0) {*bitflag &= ~(1<<31); return 31; }
					return -1;
				}

				/////////////////////////////////////////////////////////////////
				// convert bitarray into a string 
			
				static unsigned int ConvertFromString( const std::string &bitstring )
				{
					if ( bitstring.size()!=32 )	
						vml::os::Message::Error("bitarray32 : " ,"size must be 32 characthers");
					unsigned int flag = 0;
					if (bitstring[ 0] == '1') flag |= BIT0;
					if (bitstring[ 1] == '1') flag |= BIT1;
					if (bitstring[ 2] == '1') flag |= BIT2;
					if (bitstring[ 3] == '1') flag |= BIT3;
					if (bitstring[ 4] == '1') flag |= BIT4;
					if (bitstring[ 5] == '1') flag |= BIT5;
					if (bitstring[ 6] == '1') flag |= BIT6;
					if (bitstring[ 7] == '1') flag |= BIT7;
					if (bitstring[ 8] == '1') flag |= BIT8;
					if (bitstring[ 9] == '1') flag |= BIT9;
					if (bitstring[10] == '1') flag |= BIT10;
					if (bitstring[11] == '1') flag |= BIT11;
					if (bitstring[12] == '1') flag |= BIT12;
					if (bitstring[13] == '1') flag |= BIT13;
					if (bitstring[14] == '1') flag |= BIT14;
					if (bitstring[15] == '1') flag |= BIT15;
					if (bitstring[16] == '1') flag |= BIT16;
					if (bitstring[17] == '1') flag |= BIT17;
					if (bitstring[18] == '1') flag |= BIT18;
					if (bitstring[19] == '1') flag |= BIT19;
					if (bitstring[20] == '1') flag |= BIT20;
					if (bitstring[21] == '1') flag |= BIT21;
					if (bitstring[22] == '1') flag |= BIT22;
					if (bitstring[23] == '1') flag |= BIT23;
					if (bitstring[24] == '1') flag |= BIT24;
					if (bitstring[25] == '1') flag |= BIT25;
					if (bitstring[26] == '1') flag |= BIT26;
					if (bitstring[27] == '1') flag |= BIT27;
					if (bitstring[28] == '1') flag |= BIT28;
					if (bitstring[29] == '1') flag |= BIT29;
					if (bitstring[30] == '1') flag |= BIT30;
					if (bitstring[31] == '1') flag |= BIT31;
					return flag;
				}
				
				/////////////////////////////////////////////////////////////////
				// dumps bit contents into a string

				static std::string ConvertToString(const unsigned int &bitflag)
				{
					std::string bitstring = "00000000000000000000000000000000";
					if (bitflag & BIT0 ) bitstring[ 0] = '1'; else bitstring[ 0] = '0';
					if (bitflag & BIT1 ) bitstring[ 1] = '1'; else bitstring[ 1] = '0';
					if (bitflag & BIT2 ) bitstring[ 2] = '1'; else bitstring[ 2] = '0';
					if (bitflag & BIT3 ) bitstring[ 3] = '1'; else bitstring[ 3] = '0';
					if (bitflag & BIT4 ) bitstring[ 4] = '1'; else bitstring[ 4] = '0';
					if (bitflag & BIT5 ) bitstring[ 5] = '1'; else bitstring[ 5] = '0';
					if (bitflag & BIT6 ) bitstring[ 6] = '1'; else bitstring[ 6] = '0';
					if (bitflag & BIT7 ) bitstring[ 7] = '1'; else bitstring[ 7] = '0';
					if (bitflag & BIT8 ) bitstring[ 8] = '1'; else bitstring[ 8] = '0';
					if (bitflag & BIT9 ) bitstring[ 9] = '1'; else bitstring[ 9] = '0';
					if (bitflag & BIT10) bitstring[10] = '1'; else bitstring[10] = '0';
					if (bitflag & BIT11) bitstring[11] = '1'; else bitstring[11] = '0';
					if (bitflag & BIT12) bitstring[12] = '1'; else bitstring[12] = '0';
					if (bitflag & BIT13) bitstring[13] = '1'; else bitstring[13] = '0';
					if (bitflag & BIT14) bitstring[14] = '1'; else bitstring[14] = '0';
					if (bitflag & BIT15) bitstring[15] = '1'; else bitstring[15] = '0';
					if (bitflag & BIT16) bitstring[16] = '1'; else bitstring[16] = '0';
					if (bitflag & BIT17) bitstring[17] = '1'; else bitstring[17] = '0';
					if (bitflag & BIT18) bitstring[18] = '1'; else bitstring[18] = '0';
					if (bitflag & BIT19) bitstring[19] = '1'; else bitstring[19] = '0';
					if (bitflag & BIT20) bitstring[20] = '1'; else bitstring[20] = '0';
					if (bitflag & BIT21) bitstring[21] = '1'; else bitstring[21] = '0';
					if (bitflag & BIT22) bitstring[22] = '1'; else bitstring[22] = '0';
					if (bitflag & BIT23) bitstring[23] = '1'; else bitstring[23] = '0';
					if (bitflag & BIT24) bitstring[24] = '1'; else bitstring[24] = '0';
					if (bitflag & BIT25) bitstring[25] = '1'; else bitstring[25] = '0';
					if (bitflag & BIT26) bitstring[26] = '1'; else bitstring[26] = '0';
					if (bitflag & BIT27) bitstring[27] = '1'; else bitstring[27] = '0';
					if (bitflag & BIT28) bitstring[28] = '1'; else bitstring[28] = '0';
					if (bitflag & BIT29) bitstring[29] = '1'; else bitstring[29] = '0';
					if (bitflag & BIT30) bitstring[30] = '1'; else bitstring[30] = '0';
					if (bitflag & BIT31) bitstring[31] = '1'; else bitstring[31] = '0';
					return bitstring;
				}

				//////////////////////////////////////////////////////////////////////
				// counts bits population using the memory data block
				// the offset is computed as :
				// bitflag*9 = bitflag*8+bitflag = (bitflag <<3 ) +bitflag 
				// unsigned *v is an 32 bit array
				// bits contains the active bits ( bits set to 1 )
				// the 32 bit unsigned int is decomposed into 4 bytes

				static void PopList(unsigned long int bitflag, unsigned char *v, unsigned int &bits)
				{
					size_t b1, b2, b3, b4;
					size_t offset;

					b1 = (bitflag & 0xff000000UL) >> 24;
					b2 = (bitflag & 0x00ff0000UL) >> 16;
					b3 = (bitflag & 0x0000ff00UL) >> 8;
					b4 = (bitflag & 0x000000ffUL);

					// split unsigned int into first byte

					offset = (b1 << 3) + b1;
					bits = PopData[offset];

					v[0] = PopData[offset + 1];
					v[1] = PopData[offset + 2];
					v[2] = PopData[offset + 3];
					v[3] = PopData[offset + 4];
					v[4] = PopData[offset + 5];
					v[5] = PopData[offset + 6];
					v[6] = PopData[offset + 7];
					v[7] = PopData[offset + 8];

					// split unsigned int into second byte

					offset = (b2 << 3) + b2;
					bits += PopData[offset];

					v[ 8] = PopData[offset + 1];
					v[ 9] = PopData[offset + 2];
					v[10] = PopData[offset + 3];
					v[11] = PopData[offset + 4];
					v[12] = PopData[offset + 5];
					v[13] = PopData[offset + 6];
					v[14] = PopData[offset + 7];
					v[15] = PopData[offset + 8];

					// split unsigned int into third byte

					offset = (b3 << 3) + b3;
					bits += PopData[offset];

					v[16] = PopData[offset + 1];
					v[17] = PopData[offset + 2];
					v[18] = PopData[offset + 3];
					v[19] = PopData[offset + 4];
					v[20] = PopData[offset + 5];
					v[21] = PopData[offset + 6];
					v[22] = PopData[offset + 7];
					v[23] = PopData[offset + 8];

					// split unsigned int into forth byte

					offset = (b4 << 3) + b4;
					bits += PopData[offset];

					v[24] = PopData[offset + 1];
					v[25] = PopData[offset + 2];
					v[26] = PopData[offset + 3];
					v[27] = PopData[offset + 4];
					v[28] = PopData[offset + 5];
					v[29] = PopData[offset + 6];
					v[30] = PopData[offset + 7];
					v[31] = PopData[offset + 8];

				}

		};
	
	} // end oif namespace utils

} // end of vml namespace

