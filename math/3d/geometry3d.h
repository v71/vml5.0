#pragma once

#include <vml5.0/math/mathutils.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////
// Operation results

namespace vml
{
	namespace geo3d
	{

		struct Results
		{

			// true / false flags

			static const uint32_t OK   = 0;
			static const uint32_t FAIL = 1;

			// intersections flags

			static const uint32_t DOES_INTERSECT	  = 1;
			static const uint32_t DOES_NOT_INTERSECT  = 2;
			static const uint32_t COLLINEAR			  = 4;
			static const uint32_t PARALLEL			  = 8;

			// containment flags

			static const uint32_t INSIDE			  = 16;
			static const uint32_t OUTSIDE			  = 32;
			static const uint32_t SECOND_INSIDE_FIRST = 64;
			static const uint32_t FIRST_INSIDE_SECOND = 128;
		
			// get bit desciptors

			static const std::string ToString(const unsigned int bitvalue)
			{
				switch (bitvalue)
				{
					case DOES_NOT_INTERSECT  : return "Do Not Intersect";	 break;
					case DOES_INTERSECT      : return "Do Intersect";		 break;
					case COLLINEAR		     : return "Collinear";			 break;
					case PARALLEL		     : return "Parallel";			 break;
					case INSIDE			     : return "Inside";				 break;
					case OUTSIDE			 : return "Outside";			 break;
					case SECOND_INSIDE_FIRST : return "Second Inside First"; break;
					case FIRST_INSIDE_SECOND : return "First Inside Second"; break;
				}

				return "Undefined BitValue";
			}

		};

	} // end of geo2d namespace

} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// 3d geometry predicsates

#include <vml5.0/math/3d/vec3.h>
#include <vml5.0/math/3d/predicates3d.h>
#include <vml5.0/math/3d/intersections.h>

/*
#include <vml4.0/math/3d/metrics.h>
#include <vml4.0/math/3d/distances.h>
*/


