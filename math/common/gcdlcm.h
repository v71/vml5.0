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

//////////////////////////////////////////////////////////////////////////////////////
//  gcdlcm functiuons

namespace vml
{
	namespace math
	{
		namespace gcdlcm
		{
				///////////////////////////////////////////////////////////////
				// Find the greatest common divisor of 2 numbers  
				// See http://en.wikipedia.org/wiki/Greatest_common_divisor  
 
				static unsigned int GCD( unsigned int a, unsigned int b )  
				{  
					 unsigned int c;  
					 while ( a != 0 )  
					 {  
						c = a;  
						a = b%a;  
						b = c;  
					 }  
					 return b;  
				 }  

				///////////////////////////////////////////////////////////////
				// Find the least common multiple of 2 numbers  
				// See http://en.wikipedia.org/wiki/Least_common_multiple  
  
				 static unsigned int LCM(unsigned int a, unsigned b)  
				 {  
				   return ( b / GCD(a, b) ) * a;  
				 }  
   
				 ///////////////////////////////////////////////////////////////
				 // Find the greatest common divisor of an array of numbers  
				 // See http://en.wikipedia.org/wiki/Greatest_common_divisor  

				 static unsigned int GCD( const std::vector<unsigned int> &n )  
				 {  
					  unsigned int last_gcd, i;  
      
					  if( n.size() < 2) return 0;  
      
					  last_gcd = GCD(n[0], n[1]);  
      
					  for(i=2; i < n.size(); i++)  
						   last_gcd = GCD(last_gcd, n[i]);  
      
					  return last_gcd;  
				 }  

				 ///////////////////////////////////////////////////////////////
				 // Find the least common multiple of an array of numbers  
				 // See http://en.wikipedia.org/wiki/Least_common_multiple  
  
				 static unsigned int LCM( const std::vector<unsigned int > &n )  
				 {  
					  unsigned int last_lcm, i;  
      
					  if(n.size() < 2) return 0;  
      
					  last_lcm = LCM(n[0], n[1]);  
    
					  for(i=2; i < n.size(); i++)  
						   last_lcm = LCM(last_lcm, n[i]);  
      
					  return last_lcm;  
				 }  

		} // end of gcdlcm namespace

	} // end of math namespace

} // end of vml namespace
