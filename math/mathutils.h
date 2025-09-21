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

///////////////////////////////////////////////////////////////////////////////////////////////
// Disable double to float warning
#pragma warning( disable : 4244 ) 
// Disable warning when a function returns a 
// value inside an __asm block
#pragma warning (disable : 4035)

///////////////////////////////////////////////////////////////////////////////////////////////
// math utilitites

#include <math.h>
#include <vml5.0\math\common\constants.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// glm lib

#define GLM_FORCE_RADIANS
#define GLM_FORCE_INLINE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <vml5.0\libs\glm\glm.hpp>							// glm
#include <vml5.0\libs\glm\gtc\matrix_transform.hpp>			// glm matrix
#include <vml5.0\libs\glm\gtc\type_ptr.hpp>					// glm pointers
#include <vml5.0\libs\glm\gtc\matrix_inverse.hpp>			// for inverting and transposing 
#include <vml5.0\libs\glm\gtx\quaternion.hpp>				// quaternion class for orientation
#include <vml5.0\libs\glm\gtx\perpendicular.hpp>			// quaternion class for orientation


