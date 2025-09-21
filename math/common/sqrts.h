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

/////////////////////////////////////////////////////////////////////////////
// square roots

namespace vml
{
	namespace math
	{
		namespace sqrts
		{

			static float ApproxInvSqrt1(float x)
			{
				float xcopy = x;
				float xhalf = 0.5f * xcopy;
				int i = *(long *)&xcopy;
				i = 0x5f3759df - (i >> 1);
				xcopy = *(float *)&i;
				xcopy *= (1.5f - xhalf * xcopy * xcopy);
				return xcopy;
			}

			static float ApproxInvSqrt2(float x)
			{
				float xcopy = x;
				float xhalf = 0.5f * xcopy;
				int i = *(long *)&xcopy;
				i = 0x5f3759df - (i >> 1);
				xcopy = *(float *)&i;
				xcopy *= (1.5f - xhalf * xcopy * xcopy);
				xcopy *= (1.5f - xhalf * xcopy * xcopy);
				return xcopy;
			}

			static double FastSqrt(double fX)
			{
				double tmp = fX;
				double fHalf = 0.5*fX;
				float  fXf = static_cast<float>(fX);
				int i = *reinterpret_cast<int*>(&fXf);
				i = 0x5f3759df - (i >> 1); // This line hides a LOT of math!
				fX = *reinterpret_cast<float*>(&i);
				// repeat this statement for a better approximation
				fX = fX * (1.5 - fHalf * fX*fX);
				fX = fX * (1.5 - fHalf * fX*fX);
				return tmp * fX;
			}

			static double FastInvSqrt(double fX)
			{
				double fHalf = 0.5 * fX;
				float fXf = static_cast<float>(fX);
				int i = *reinterpret_cast<int*>(&fXf);
				i = 0x5f3759df - (i >> 1); // This line hides a LOT of math!
				fX = *reinterpret_cast<float*>(&i);
				// repeat this statement for a better approximation
				fX = fX * (1.5 - fHalf * fX*fX);
				fX = fX * (1.5 - fHalf * fX*fX);
				return fX;
			}

		}  // end of primefactors namespace

	}  // end of math namespace

} // end of vml namespace