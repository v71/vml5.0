#pragma once

#include <vml5.0/math/mathutils.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////
// Operation results

namespace vml
{
	namespace geo2d
	{
		struct Results
		{
			
			// true / false flags

			static const uint32_t OK					    =  0;
			static const uint32_t FAIL					    =  1;

			// intersections flags

			static const uint32_t DOES_INTERSECT 		    =   2;
			static const uint32_t DOES_INTERSECT_ONE_POINT  =   3;
			static const uint32_t DOES_INTERSECT_TWO_POINT  =   4;
			static const uint32_t DOES_NOT_INTERSECT	    =   5;
			static const uint32_t COLLINEAR				    =   6;
			static const uint32_t PARALLEL				    =   7;
			static const uint32_t DOES_INTERSECT_ONE_POINT_P =  8;
			static const uint32_t DOES_INTERSECT_ONE_POINT_Q =  9;

			// containment flags

			static const uint32_t INSIDE					= 16;
			static const uint32_t OUTSIDE					= 17;
			static const uint32_t SECOND_INSIDE_FIRST		= 18;
			static const uint32_t FIRST_INSIDE_SECOND		= 19;

			// position flags

			static const uint32_t TOP						= 32;
			static const uint32_t RIGHT						= 33;
			static const uint32_t BOTTOM					= 34;
			static const uint32_t LEFT						= 35;

			// get bit desciptors

			static const std::string ToString(const unsigned int value)
			{
				switch (value)
				{
					case OK							   : return "Success";						break;
					case FAIL						   : return "Fail";							break;

					case DOES_INTERSECT				   : return "Do Intersect";					break;
					case DOES_INTERSECT_ONE_POINT	   : return "Do Intersect in One Point";	break;
					case DOES_INTERSECT_TWO_POINT	   : return "Do Intersect in two Point";	break;
					case DOES_NOT_INTERSECT		       : return "Do Not Intersect";				break;
					case COLLINEAR				       : return "Collinear";					break;
					case PARALLEL					   : return "Parallel";						break;
					
					case INSIDE						   : return "Inside";						break;
					case OUTSIDE					   : return "Outside";						break;
					case SECOND_INSIDE_FIRST		   : return "Second Inside First";			break;
					case FIRST_INSIDE_SECOND		   : return "First Inside Second";		    break;
					
					case TOP						   : return "Top";							break;
					case RIGHT						   : return "Right";					    break;
					case BOTTOM						   : return "Bottom";					    break;
					case LEFT						   : return "Left";						    break;
			
				}

				return "Undefined Value";
			}
			
		};

	} // end of geo2d namespace

} 

////////////////////////////////////////////////////////////////////////////////////
// 2d geometry predicates

#include <vml5.0/math/2d/vec2.h>
#include <vml5.0/math/common/angles.h>
#include <vml5.0/math/2d/matrix2x2.h>
#include <vml5.0/math/2d/predicates2d.h>
#include <vml5.0/math/2d/metrics.h>
#include <vml5.0/math/2d/transform.h>
#include <vml5.0/math/2d/intersections.h>
#include <vml5.0/math/2d/distances.h>

/*
////////////////////////////////////////////////////////////////////////////////////
// end of vml namepsace
#include <vml2.0\math\2d\polygon.h>

////////////////////////////////////////////////////////////////////////////////////
// 2d geometry algorithms

#include <vml2.0\math\2d\vertexarray.h>

*/