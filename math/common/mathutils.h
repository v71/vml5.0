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

///////////////////////////////////////////////////////////////////////////////////////////////
// Disable double to float warning
#pragma warning( disable : 4244 ) 
// Disable warning when a function returns a 
// value inside an __asm block
#pragma warning (disable : 4035)

///////////////////////////////////////////////////////////////////////////////////////////////
// math utilitites

#include <math.h>
#include <vml2.0\math\constants.h>
#include <vml2.0\math\ranges.h>
#include <vml2.0\math\trigonometrics\angles.h>
//#include <vml2.0\math\logarithms.h>
//#include <vml2.0\math\complex\complex.h>
//#include <vml2.0\math\equationsolvers.h>
//#include <vml2.0\math\gcdlcm.h>
//#include <vml2.0\math\interpolators.h>
//#include <vml2.0\math\integrators.h>
//#include <vml2.0\math\primes.h>
//#include <vml2.0\math\sort.h>
//#include <vml2.0\math\swap.h>
//#include <vml2.0\math\stats.h>
//#include <vml2.0\math\sqrts.h>
//#include <vml2.0\math\sequences.h>
//#include <vml2.0\math\random.h>
//#include <vml2.0\math\conversion.h>
//#include <vml2.0\math\numerics.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// 2x2 matrix operations

//#include <vml2.0\math\vec2\vec2.h>
//#include <vml2.0\math\matrix2x2\matrix2x2.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// 3x3 matrix operations

//#include <vml2.0\math\vec3\vec3.h>
//#include <vml2.0\math\matrix3x3\matrix3x3.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// 4x4 matrix operations

//#include <vml2.0\math\vec4\vec4.h>
//#include <vml2.0\math\matrix4x4\matrix4x4.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// nxn matrix operations

//#include <vml2.0/math/vecn/vecn.h>
//#include <vml2.0/math/matrixNxN/matrixNxN.h>
//#include <vml2.0/math/matrixNxN/matrixNxNsolvers.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// glm for matrix functions

#define GLM_FORCE_RADIANS			
#define GLM_FORCE_INLINE
#include <c:\vml2.0\libs\glm\glm.hpp>							// glm
#include <c:\vml2.0\libs\glm\gtc\matrix_transform.hpp>			// glm matrix
#include <c:\vml2.0\libs\glm\gtc\type_ptr.hpp>					// glm pointers
#include <c:\vml2.0\libs\glm\gtc\matrix_inverse.hpp>			// for inverting and transposing 
#include <c:\vml2.0\libs\glm\gtx\quaternion.hpp>				// quaternion class for orientation

//#include <c:\vml2.0\math\trigs.h>
