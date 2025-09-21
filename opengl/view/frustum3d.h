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
//	The above copyright notice and this permission notice shall be included in-
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
	namespace views
	{
		///////////////////////////////////////////////////////////////
		// frustum tests class

		namespace frustum
		{
			
			// ---------------------------------------------------------------
			// enums for node frustum test
				
			static const unsigned int OUTSIDE     = 0;
			static const unsigned int INSIDE      = 1;
			static const unsigned int INTERSECTED = 2;
				
			////////////////////////////////////////////////////////////////////////////
			// tests if a point is inside the view frustum, integer return value is used
			// to mantain consistency with the other test functions
				
			static int TestPoint(const glm::vec4 *planes, const glm::vec4 &point)
			{
				// cache vertex position

				float x = point.x;
				float y = point.y;
				float z = point.z;

				// performs frustum planes test

				if (planes[0].x * x + planes[0].y * y + planes[0].z * z + planes[0].w >= 0.0f)
				if (planes[1].x * x + planes[1].y * y + planes[1].z * z + planes[1].w >= 0.0f)
				if (planes[2].x * x + planes[2].y * y + planes[2].z * z + planes[2].w >= 0.0f)
				if (planes[3].x * x + planes[3].y * y + planes[3].z * z + planes[3].w >= 0.0f)
				if (planes[4].x * x + planes[4].y * y + planes[4].z * z + planes[4].w >= 0.0f)
				if (planes[5].x * x + planes[5].y * y + planes[5].z * z + planes[5].w >= 0.0f)
					return INSIDE;

				return OUTSIDE;
			}

			////////////////////////////////////////////////////////////////////////////
			// test a model's sphere against frustum

			static int TestSphere(const glm::vec4* planes, const glm::vec3& center, const float radius)
			{
				float centerx = center.x;
				float centery = center.y;
				float centerz = center.z;
				int result = INSIDE;

				float d;

				d = planes[0].x * centerx + planes[0].y * centery + planes[0].z * centerz + planes[0].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				d = planes[1].x * centerx + planes[1].y * centery + planes[1].z * centerz + planes[1].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				d = planes[2].x * centerx + planes[2].y * centery + planes[2].z * centerz + planes[2].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				d = planes[3].x * centerx + planes[3].y * centery + planes[3].z * centerz + planes[3].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				d = planes[4].x * centerx + planes[4].y * centery + planes[4].z * centerz + planes[4].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				d = planes[5].x * centerx + planes[5].y * centery + planes[5].z * centerz + planes[5].w;

				if (d + radius < 0.0f) return OUTSIDE;
				if (d - radius < 0.0f) result = INTERSECTED;

				return result;
			}

			///////////////////////////////////////////////////////////////////////////////////
			// test an axis aligned bounding box against the view frustum
			// 0 = out, 1 = in , 2 = intersected

			static int TestAABBox(const glm::vec4 *planes, const glm::vec3 &center,const glm::vec3 &halfextents)
			{
				// cache center position

				float centerx = center.x;
				float centery = center.y;
				float centerz = center.z;

				// cache half extensions

				float halfextentsx = halfextents.x;
				float halfextentsy = halfextents.y;
				float halfextentsz = halfextents.z;

				// performs frustum planes test

				float d, r;

				int result = INSIDE;

				// left plane

				d = centerx * planes[0].x + centery * planes[0].y + centerz * planes[0].z + planes[0].w;
				r = halfextentsx * abs(planes[0].x) + halfextentsy * abs(planes[0].y) + halfextentsz * abs(planes[0].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				// right plane

				d = centerx * planes[1].x + centery * planes[1].y + centerz * planes[1].z + planes[1].w;
				r = halfextentsx * abs(planes[1].x) + halfextentsy * abs(planes[1].y) + halfextentsz * abs(planes[1].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				// top plane

				d = centerx * planes[2].x + centery * planes[2].y + centerz * planes[2].z + planes[2].w;
				r = halfextentsx * abs(planes[2].x) + halfextentsy * abs(planes[2].y) + halfextentsz * abs(planes[2].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				// bottom plane

				d = centerx * planes[3].x + centery * planes[3].y + centerz * planes[3].z + planes[3].w;
				r = halfextentsx * abs(planes[3].x) + halfextentsy * abs(planes[3].y) + halfextentsz * abs(planes[3].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				// near plane

				d = centerx * planes[4].x + centery * planes[4].y + centerz * planes[4].z + planes[4].w;
				r = halfextentsx * abs(planes[4].x) + halfextentsy * abs(planes[4].y) + halfextentsz * abs(planes[4].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				// far plane

				d = centerx * planes[5].x + centery * planes[5].y + centerz * planes[5].z + planes[5].w;
				r = halfextentsx * abs(planes[5].x) + halfextentsy * abs(planes[5].y) + halfextentsz * abs(planes[5].z);

				if (d + r < 0)  return OUTSIDE;			// out
				if (d - r < 0)	result = INTERSECTED;	// intersection

				return result;

			}
				
		};
	}
}


