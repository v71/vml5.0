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
// easing function

namespace vml
{
	namespace math
	{
		namespace easing
		{
			// static constants

			static const unsigned int EASEINSINE       =  0;
			static const unsigned int EASEOUTSINE      =  1;
			static const unsigned int EASEINOUTSINE    =  2;
			static const unsigned int EASEINQUAD       =  3;
			static const unsigned int EASEOUTQUAD      =  4;
			static const unsigned int EASEINOUTQUAD    =  5;
			static const unsigned int EASEINCUBIC      =  6;
			static const unsigned int EASEOUTCUBIC     =  7;
			static const unsigned int EASEINOUTCUBIC   =  8;
			static const unsigned int EASEINQUART      =  9;
			static const unsigned int EASEOUTQUART     = 10;
			static const unsigned int EASEINOUTQUART   = 11;
			static const unsigned int EASEINQUINT      = 12;
			static const unsigned int EASEOUTQUINT     = 13;
			static const unsigned int EASEINOUTQUINT   = 14;
			static const unsigned int EASEINEXPO       = 15;
			static const unsigned int EASEOUTEXPO      = 16;
			static const unsigned int EASEINOUTEXPO    = 17;
			static const unsigned int EASEINCIRC       = 18;
			static const unsigned int EASEOUTCIRC      = 19;
			static const unsigned int EASEINOUTCIRC    = 20;
			static const unsigned int EASEINBACK       = 21;
			static const unsigned int EASEOUTBACK      = 22;
			static const unsigned int EASEINOUTBACK    = 23;
			static const unsigned int EASEINELASTIC    = 24;
			static const unsigned int EASEOUTELASTIC   = 25;
			static const unsigned int EASEINOUTELASTIC = 26;
			static const unsigned int EASEINBOUNCE     = 27;
			static const unsigned int EASEOUTBOUNCE    = 28;
			static const unsigned int EASEINOUTBOUNCE  = 29;

			// number of functions

			static const size_t EasingFunctionsCount = 30;

			// static easing functions

			static float easeInSine(float t)
			{
				return sin(1.5707963 * t);
			}

			static float easeOutSine(float t)
			{
				return 1 + sin(1.5707963 * (--t));
			}

			static float easeInOutSine(float t)
			{
				return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
			}

			static float easeInQuad(float t)
			{
				return t * t;
			}

			static float easeOutQuad(float t)
			{
				return t * (2 - t);
			}

			static float easeInOutQuad(float t)
			{
				return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
			}

			static float easeInCubic(float t)
			{
				return t * t * t;
			}

			static float easeOutCubic(float t)
			{
				return 1 + (--t) * t * t;
			}

			static float easeInOutCubic(float t)
			{
				return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
			}

			static float easeInQuart(float t)
			{
				t *= t;
				return t * t;
			}

			static float easeOutQuart(float t)
			{
				t = (--t) * t;
				return 1 - t * t;
			}

			static float easeInOutQuart(float t)
			{
				if (t < 0.5)
				{
					t *= t;
					return 8 * t * t;
				}
				else
				{
					t = (--t) * t;
					return 1 - 8 * t * t;
				}
			}

			static float easeInQuint(float t)
			{
				float t2 = t * t;

				return t * t2 * t2;
			}

			static float easeOutQuint(float t)
			{
				float t2 = (--t) * t;

				return 1 + t * t2 * t2;
			}

			static float easeInOutQuint(float t)
			{
				float t2;

				if (t < 0.5)
				{
					t2 = t * t;
					return 16 * t * t2 * t2;
				}
				else
				{
					t2 = (--t) * t;
					return 1 + 16 * t * t2 * t2;
				}
			}

			static float easeInExpo(float t)
			{
				return (pow(2, 8 * t) - 1) / 255;
			}

			static float easeOutExpo(float t)
			{
				return 1 - pow(2, -8 * t);
			}

			static float easeInOutExpo(float t)
			{
				if (t < 0.5)
				{
					return (pow(2, 16 * t) - 1) / 510;
				}
				else
				{
					return 1 - 0.5 * pow(2, -16 * (t - 0.5));
				}
			}

			static float easeInCirc(float t)
			{
				return 1 - sqrt(1 - t);
			}

			static float easeOutCirc(float t)
			{
				return sqrt(t);
			}

			static float easeInOutCirc(float t)
			{
				if (t < 0.5)
				{
					return (1 - sqrt(1 - 2 * t)) * 0.5;
				}
				else
				{
					return (1 + sqrt(2 * t - 1)) * 0.5;
				}
			}

			static float easeInBack(float t)
			{
				return t * t * (2.70158 * t - 1.70158);
			}

			static float easeOutBack(float t)
			{
				return 1 + (--t) * t * (2.70158 * t + 1.70158);
			}

			static float easeInOutBack(float t)
			{
				if (t < 0.5)
				{
					return t * t * (7 * t - 2.5) * 2;
				}
				else
				{
					return 1 + (--t) * t * 2 * (7 * t + 2.5);
				}
			}

			static float easeInElastic(float t)
			{
				float t2 = t * t;
				return t2 * t2 * sin(t * vml::math::PI * 4.5);
			}

			static float easeOutElastic(float t)
			{
				float t2 = (t - 1) * (t - 1);
				return 1 - t2 * t2 * cos(t * vml::math::PI * 4.5);
			}

			static float easeInOutElastic(float t)
			{
				float t2;

				if (t < 0.45)
				{
					t2 = t * t;
					return 8 * t2 * t2 * sin(t * vml::math::PI * 9);
				}
				else if (t < 0.55)
				{
					return 0.5 + 0.75 * sin(t * vml::math::PI * 4);
				}
				else
				{
					t2 = (t - 1) * (t - 1);
					return 1 - 8 * t2 * t2 * sin(t * vml::math::PI * 9);
				}
			}

			static float easeInBounce(float t)
			{
				return pow(2, 6 * (t - 1)) * abs(sin(t * vml::math::PI * 3.5));
			}

			static float easeOutBounce(float t)
			{
				return 1 - pow(2, -6 * t) * abs(cos(t * vml::math::PI * 3.5));
			}

			static float easeInOutBounce(float t)
			{
				if (t < 0.5)
				{
					return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * vml::math::PI * 7));
				}
				else
				{
					return 1 - 8 * pow(2, -8 * t) * abs(sin(t * vml::math::PI * 7));
				}
			}

			// function pointers

			typedef float (*funptr)(float);

			static funptr EasingFns[EasingFunctionsCount] = {
																easeInSine,
																easeOutSine,
																easeInOutSine,
																easeInQuad,
																easeOutQuad,
																easeInOutQuad,
																easeInCubic,
																easeOutCubic,
																easeInOutCubic,
																easeInQuart,
																easeOutQuart,
																easeInOutQuart,
																easeInQuint,
																easeOutQuint,
																easeInOutQuint,
																easeInExpo,
																easeOutExpo,
																easeInOutExpo,
																easeInCirc,
																easeOutCirc,
																easeInOutCirc,
																easeInBack,
																easeOutBack,
																easeInOutBack,
																easeInElastic,
																easeOutElastic,
																easeInOutElastic,
																easeInBounce,
																easeOutBounce,
																easeInOutBounce
			};

		
		}	// end of namespace easing
	}	// end of namespace math
}	// end of namespace vml
