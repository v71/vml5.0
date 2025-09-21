#pragma once

///////////////////////////////////////////////////////////////////////////////////////
//										Intersections Functions
//
//					Point		Ray		Line	Circle		AABB		AOBB		Polygon
//					
//  Point			  -			 X	 					     X			  
//	 
//	Ray				  X	  		  				  X	   		 X
//	
//	Line			  				       					 X			  			 
// 
//	Circle			  			 X		  X 		   		 X							  
// 
//	AABB			  X			 X		  X					 X	  			 		  
// 
//	AOBB			  
// 
// Polygon

/////////////////////////////////////////////////////////////////////
// intersections

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding box and infinite ray

			template<typename T>
			static [[nodiscard]] uint32_t AABBOXVsRay(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
													  const vml::math::vec2<T>& r0, const vml::math::vec2<T>& dir,
													  vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
													  const float eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> tMin = vml::math::vec2<T>((bmin.x - r0.x) / dir.x, (bmin.y - r0.y) / dir.y);
				vml::math::vec2<T> tMax = vml::math::vec2<T>((bmax.x - r0.x) / dir.x, (bmax.y - r0.y) / dir.y);
				vml::math::vec2<T> t1 = vml::math::vec2<T>(std::min(tMin.x, tMax.x), std::min(tMin.y, tMax.y));
				vml::math::vec2<T> t2 = vml::math::vec2<T>(std::max(tMin.x, tMax.x), std::max(tMin.y, tMax.y));

				float tNear = std::max(t1.x, t1.y);
				float tFar = std::min(t2.x, t2.y);

				// compute intersection points

				vml::math::vec2<T> p = vml::math::vec2<T>(r0.x + tFar * dir.x, r0.y + tFar * dir.y);
				vml::math::vec2<T> q = vml::math::vec2<T>(r0.x + tNear * dir.x, r0.y + tNear * dir.y);

				// compute scaled box to prevent
				// inclusion test for points to 
				// fail due to floating points accuracy

				float neps = 1.0f + vml::math::EPSILON;
				vml::math::vec2<T> c = (bmin + bmax) * 0.5f;
				vml::math::vec2<T> scaled_bmin = (bmin - c) * neps + c;
				vml::math::vec2<T> scaled_bmax = (bmax - c) * neps + c;

				vml::math::vec2<T> dp;
				vml::math::vec2<T> dq;
				vml::math::vec2<T> points[2];

				size_t numpoints = 0;
				float denum = dir.x * dir.x + dir.y * dir.y;

				// check if points p ins inside scaled rectangle

				if ((p.x >= scaled_bmin.x && p.y >= scaled_bmin.y) &&
					(p.x <= scaled_bmax.x && p.y <= scaled_bmax.y))
				{
					points[numpoints++] = p;
				}

				// check if points q ins inside scaled rectangle

				if ((q.x >= scaled_bmin.x && q.y >= scaled_bmin.y) &&
					(q.x <= scaled_bmax.x && q.y <= scaled_bmax.y))
				{
					points[numpoints++] = q;
				}

				if (numpoints == 0)
				{
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				if (numpoints == 1)
				{
					closestp = points[0];
					return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
				}

				if (numpoints == 2)
				{
					closestp = points[0];
					closestq = points[1];
					return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
				}

				// we never get here

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding box and a line

			template<class T>
			static [[nodiscard]] uint32_t AABBOXVsLine(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
													   const vml::math::vec2<T>& r0, const vml::math::vec2<T>& r1,
													   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
													   const float eps = vml::math::EPSILON)
			{
				// check if line is entirely inside bounding box

				if (r0.x - bmin.x > eps)
				if (r0.x - bmax.x < eps)
				if (r0.y - bmin.y > eps)
				if (r0.y - bmax.y < eps)
				if (r1.x - bmin.x > eps)
				if (r1.x - bmax.x < eps)
				if (r1.y - bmin.y > eps)
				if (r1.y - bmax.y < eps)
					return vml::geo2d::Results::INSIDE;

				// slab test

				vml::math::vec2<T> dir = r1 - r0;
				vml::math::vec2<T> tMin = vml::math::vec2<T>((bmin.x - r0.x) / dir.x, (bmin.y - r0.y) / dir.y);
				vml::math::vec2<T> tMax = vml::math::vec2<T>((bmax.x - r0.x) / dir.x, (bmax.y - r0.y) / dir.y);
				vml::math::vec2<T> t1 = vml::math::vec2<T>(std::min(tMin.x, tMax.x), std::min(tMin.y, tMax.y));
				vml::math::vec2<T> t2 = vml::math::vec2<T>(std::max(tMin.x, tMax.x), std::max(tMin.y, tMax.y));

				float tNear = std::max(t1.x, t1.y);
				float tFar = std::min(t2.x, t2.y);

				// compute intersection points

				vml::math::vec2<T> p = r0 + tFar * dir;
				vml::math::vec2<T> q = r0 + tNear * dir;

				// compute scaled box to prevent
				// inclusion test for points to 
				// fail due to floating points accuracy

				float neps = 1.0f + vml::math::EPSILON;
				vml::math::vec2<T> c = (bmin + bmax) * 0.5f;
				vml::math::vec2<T> scaled_bmin = (bmin - c) * neps + c;
				vml::math::vec2<T> scaled_bmax = (bmax - c) * neps + c;

				vml::math::vec2<T> dp;
				vml::math::vec2<T> dq;
				float denum;
				vml::math::vec2<T> points[2];
				size_t numpoints;

				numpoints = 0;
				denum = dir.x * dir.x + dir.y * dir.y;

				// check if points p ins inside scaled rectangle

				if ((p.x >= scaled_bmin.x && p.y >= scaled_bmin.y) &&
					(p.x <= scaled_bmax.x && p.y <= scaled_bmax.y))
				{
					dp = p - r0;
					dq = p - r1;

					// check if point p is on line

					if (dp.x * dp.x + dp.y * dp.y < denum &&
						dq.x * dq.x + dq.y * dq.y < denum)
					{
						points[numpoints++] = p;
					}
				}

				// check if points q ins inside scaled rectangle

				if ((q.x >= scaled_bmin.x && q.y >= scaled_bmin.y) &&
					(q.x <= scaled_bmax.x && q.y <= scaled_bmax.y))
				{
					dp = q - r0;
					dq = q - r1;

					// check if point q is on line

					if (dp.x * dp.x + dp.y * dp.y < denum &&
						dq.x * dq.x + dq.y * dq.y < denum)
					{
						points[numpoints++] = q;
					}
				}

				if (numpoints == 0)
				{
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				if (numpoints == 1)
				{
					closestp = closestq=points[0];
					return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
				}

				if (numpoints == 2)
				{
					closestp = points[0];
					closestq = points[1];
					return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
				}
				
				// we never get here

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxes tests

			template<class T>
			static [[nodiscard]] uint32_t AABBoxVsAABBox(const vml::math::vec2<T> &bmin1,
													     const vml::math::vec2<T> &bmax1,
													     const vml::math::vec2<T> &bmin2,
													     const vml::math::vec2<T> &bmax2)
			{
				// check if the first bounding box is entirely inside the second bounding box

				if ((bmin1.x >= bmin2.x && bmax1.x <= bmax2.x) &&
					(bmin1.y >= bmin2.y && bmax1.y <= bmax2.y))
						return vml::geo2d::Results::FIRST_INSIDE_SECOND;

				// check if the second bounding box is entirely inside the first bounding box

				if ((bmin2.x >= bmin1.x && bmax2.x <= bmax1.x) &&
					(bmin2.y >= bmin1.y && bmax2.y <= bmax1.y))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				// check if bounding box is out of second bounding box

				if ((bmax2.x <= bmin1.x || bmin2.x >= bmax1.x) ||
					(bmax2.y <= bmin1.y || bmin2.y >= bmax1.y))
						return vml::geo2d::Results::DOES_NOT_INTERSECT;

				// if we get here bounding boxes are intersecting each ohter

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// checks if a point is inside an axis aligned / axis oriented bounding box

			template<class T>
			static [[nodiscard]] uint32_t IsPointInAABBox(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
														  const vml::math::vec2<T>& p,
														  const float eps = vml::math::EPSILON)
			{
				if (p.x - bmin.x > eps)
				if (p.x - bmax.x < eps)
				if (p.y - bmin.y > eps)
				if (p.y - bmax.y < eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}

			/*

			/////////////////////////////////////////////////////////////////////////////
			// test if a point is inside a triangle

			static unsigned int IsPointInTriangle(const vml::math::vec2<T>& p,
												  const vml::math::vec2<T>& v1, const vml::math::vec2<T>& v2, const vml::math::vec2<T>& v3,
												  const float eps = vml::math::EPSILON)
			{
				if ((p.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (p.y - v2.y) > eps)
				if ((p.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (p.y - v3.y) > eps)
				if ((p.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (p.y - v1.y) > eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to circlae
			
			static unsigned int IsPointInCircle(const vml::math::vec2<T>& a, const float ra,
												const vml::math::vec2<T>& b, const float rb,
												const float eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> c = b - a;
				float r0 = ra + rb;
				if (c.x * c.x + c.y * c.y - r0 * r0 < eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}
			*/
		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{
			/*
			///////////////////////////////////////////////////////////////////////////////////
			// test if two ray intersects 

			static unsigned int RayVsRay(const vml::math::vec2<T>& a, const vml::math::vec2<T>& b,
										 const vml::math::vec2<T>& c, const vml::math::vec2<T>& d,
										 vml::math::vec2<T> &q,
										 const float eps = vml::math::EPSILON)
			{
				float denom = (c.y - d.y)*(b.x - a.x) - (c.x - d.x)*(b.y - a.y);
				float numa  = (c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x);
				float numb  = (b.x - a.x)*(a.y - d.y) - (b.y - a.y)*(a.x - d.x);

				if (denom > -eps && denom < eps)
				{
					// lines are collinear if num a is equal to num 
					// added an epsilon range check

					if ((-eps < numa && numa < eps) && (-eps < numb && numb < eps))
						return vml::geo2d::Results::COLLINEAR;

					// else they are parallel

					return vml::geo2d::Results::PARALLEL;
				}

				// compute intersection point

				denom = 1.0f / denom;

				float ua = numa * denom;
				float ub = numb * denom;

				// Get the intersection point.

				q = a + ua * (b - a);

				return vml::geo2d::Results::DOES_INTERSECT;
			}
			*/
			/*
			/////////////////////////////////////////////////////////////////////
			// check if point s on line

			static unsigned int PointVsRay(const vml::math::vec2<T> &p0, const vml::math::vec2<T> &p1,
										   const vml::math::vec2<T> &q,
										   const float eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> u = p1 - p0;
				vml::math::vec2<T> v = p1 - q;
				float dp = -v.x*u.y + v.y*u.x;
				if (dp > -eps && dp < eps)
					return vml::geo2d::Results::DOES_INTERSECT;
				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}
			*/
			/////////////////////////////////////////////////////////////////////
			// check if ray intersects circle

			template<class T>
			static [[nodiscard]] uint32_t RayVsCircle(const vml::math::vec2<T>& c0, const float r,
													  const vml::math::vec2<T>& p0, const vml::math::vec2<T>& dir,
													  vml::math::vec2<T>& q0, vml::math::vec2<T>& q1,
													  const float eps = vml::math::EPSILON)
			{
				float sx = p0.x - c0.x;
				float sy = p0.y - c0.y;
				float a = dir.x * dir.x + dir.y * dir.y;
				float b = dir.x * sx + dir.y * sy;
				float c = sx * sx + sy * sy - r * r;
				float delta = b * b - a * c;

				if (delta < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				// create a ray starting from circle center with a direction
				// orthogonal do ray direction , to find the intersetcion point

				float t = b / a;
				vml::math::vec2<T> q = vml::math::vec2<T>(p0.x - t * dir.x, p0.y - t * dir.y);

				// from the intersection point, inside the circle , shot a ray with
				// a direction parallel to ray direction

				sx = q.x - c0.x;
				sy = q.y - c0.y;
				b = dir.x * sx + dir.y * sy;
				c = sx * sx + sy * sy - r * r;
				delta = b * b - a * c;
				delta = sqrtf(delta);
				a = 1.0f / a;
				t = (-b + delta) * a;

				q0 = q + t * dir;
				q1 = q - t * dir;

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			///////////////////////////////////////////////////////////////////////////////////
			// test if line and cricle intersects , point of intersection is computed

			template<class T>
			static [[nodiscard]] uint32_t LineVsCircle(const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
													   const vml::math::vec2<T>& p, const float r,
													   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
													   const float eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> pq = lineq - linep;
				vml::math::vec2<T> ap = p - linep;
				float a = pq.x * pq.x + pq.y * pq.y;
				if (a > -eps && a < eps)
					a = eps;
				float b = pq.x * ap.x + pq.y * ap.y;
				float t = b / a;

				// check if closest point is at line extremes

				if (t < -eps) closestp = linep;
				if (t > 1 + eps) closestp = lineq;
				if (t > -eps && t < 1 + eps) closestp = linep + t * pq;
				vml::math::vec2<T> dist = closestp - p;
				float denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;

				// if circle is intersectiong the line
				// compute intersection points

				if (denum < r)
				{
					float c = ap.x * ap.x + ap.y * ap.y - r * r;
					float delta = sqrtf(b * b - a * c) / a;
					float u = t + delta;
					float v = t - delta;
					closestp = linep + u * pq;
					closestq = linep + v * pq;
					float numa = ((closestp.x - lineq.x) * pq.x + (closestp.y - lineq.y) * pq.y);
					float numb = ((closestq.x - linep.x) * pq.x + (closestq.y - linep.y) * pq.y);

					// border conditions 

					if (numa < 0 && numb < 0)
					{
						// border conditions for left side of the line
						closestp = closestp;
						return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
					}
					else if (numa > 0 && numb > 0)
					{
						// border conditions for right side of the line

						closestp = closestq;
						return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
					}
					else
					{
						return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
					}
				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

		}  // end of rays namespace

	} // end of geo2d namespace

} // end of vml namepsace


///////////////////////////////////////////////////////////////////////////////////////
// line intersections

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{
			
			///////////////////////////////////////////////////////////////////////////////////
			// test if two lines overlap 

			template<class T>
			static [[nodiscard]] uint32_t LineVsLineOverlapTest(const vml::math::vec2<T> &p0, const vml::math::vec2<T> &p1,
															    const vml::math::vec2<T> &q0, const vml::math::vec2<T> &q1,
															    const float eps = vml::math::EPSILON)
			{

				vml::math::vec2<T> u, v, w;
				float	  d0, d1;
				bool	  c0, c1;

				u = p1 - p0;
				v = q0 - p0;
				w = q1 - p0;

				d0 = u.x * v.y - v.x * u.y;
				d1 = u.x * w.y - w.x * u.y;

				c0 = d0 > -eps && d0 < eps;
				c1 = d1 > -eps && d1 < eps;

				// if lines are coplanar 

				if (c0 && c1)
				{

					d0 = u.x*u.x + u.y*u.y;

					if (d0 > -eps && d0 < eps)
						d0 = eps;

					d0 = 1.0f / d0;

					vml::math::vec2<T> d = q1 - q0;

					// check if vectors are pointing towrds same direction

					float dir = u.x*d.x + u.y*d.y;

					// case in which vector points away form each other

					if (dir > 0)
					{

						vml::math::vec2<T> a = p0 - q0;
						vml::math::vec2<T> b = p0 - q1;

						float a2 = (u.x*a.x + u.y*a.y) * d0;
						float b2 = (u.x*b.x + u.y*b.y) * d0;

						if (a2 > -1 && b2 < 0)
							return true;

						return false;

					}

					// case in which vecotr points against each other

					if (dir < 0)
					{

						vml::math::vec2<T> a = p1 - q0;
						vml::math::vec2<T> b = p1 - q1;

						float a2 = (u.x*a.x + u.y*a.y) * d0;
						float b2 = (u.x*b.x + u.y*b.y) * d0;

						if (a2 < 1 && b2 > 0)
							return true;

						return false;

					}

				}

				return false;
			}
			
			///////////////////////////////////////////////////////////////////////////////////
			// test if two lines intersects , point of intersection is computed

			template<class T>
			static [[nodiscard]] uint32_t LineVsLine(const vml::math::vec2<T>& a, const vml::math::vec2<T>& b,
											         const vml::math::vec2<T>& c, const vml::math::vec2<T>& d,
												     vml::math::vec2<T> &q,
												     const float eps = vml::math::EPSILON)
			{

				float denom = (c.y - d.y)*(b.x - a.x) - (c.x - d.x)*(b.y - a.y);
				float numa  = (c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x);
				float numb  = (b.x - a.x)*(a.y - d.y) - (b.y - a.y)*(a.x - d.x);

				if (denom > -eps && denom < eps)
				{
					// lines are collinear if num a is equal to num b
					// added an epsilon range check

					if ((-eps < numa && numa < eps) && (-eps < numb && numb < eps))
					{
						// lines may be collinear but still intersecting

						if (LineVsLineOverlapTest(a, b,c,d,eps))
							return vml::geo2d::Results::DOES_INTERSECT;

						return vml::geo2d::Results::COLLINEAR;
					}

					// else they are parallel

					return vml::geo2d::Results::PARALLEL;
				}

				// compute intersection point

				denom = 1.0f / denom;

				float ua = numa * denom;
				float ub = numb * denom;

				if (ua >= -eps && ua <= 1.0f + eps && ub >= -eps && ub <= 1.0f + eps)
				{
					// Get the intersection point.

					q = a + ua * (b - a);

					return vml::geo2d::Results::DOES_INTERSECT;
				}

				// else ther eis no intersection

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/*
			/////////////////////////////////////////////////////////////////////////////
			// checks if an aligned bounding box intersects a circle
			// returns points of intersections

			static uint32_t AABBOXVsCircle(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
										   const vml::math::vec2<T>& c0, const float radius,
										   std::vector<vml::math::vec2<T>>& points,
										   const float eps = vml::math::EPSILON)
			{
				float cx = c0.x;
				float cy = c0.y;
				float rx = bmin.x;
				float ry = bmin.y;
				float rw = bmax.x - bmin.x;
				float rh = bmax.y - bmin.y;

				// temporary variables to set edges for testing

				float testX = cx;
				float testY = cy;

				// which edge is closest?

				if (cx < rx)           testX = rx;        // test left edge
				else if (cx > rx + rw) testX = rx + rw;   // right edge
				if (cy < ry)           testY = ry;        // top edge
				else if (cy > ry + rh) testY = ry + rh;   // bottom edge

				// get distance from closest edges

				float distX = cx - testX;
				float distY = cy - testY;
				float distance = ((distX * distX) + (distY * distY));

				// if the distance is less than the radius, collision!

				if (distance <= radius * radius)
				{

					vml::math::vec2<T> p0, p1;
					vml::math::vec2<T> q0, q1;
					bool pointq0isinside;
					bool pointq1isinside;

					// top box line intersection trest

					p0 = vml::math::vec2<T>(bmin.x, bmin.y);
					p1 = vml::math::vec2<T>(bmax.x, bmin.y);

					if (vml::geo2d::intersections::LineVsCircle(c0, radius, p0, p1, q0, q1, pointq0isinside, pointq1isinside) == vml::geo2d::Results::DOES_INTERSECT)
					{
						if (pointq0isinside) points.emplace_back(q0);
						if (pointq1isinside) points.emplace_back(q1);
					}

					// right box line intersection trest

					p0 = vml::math::vec2<T>(bmax.x, bmin.y);
					p1 = vml::math::vec2<T>(bmax.x, bmax.y);

					if (vml::geo2d::intersections::LineVsCircle(c0, radius, p0, p1, q0, q1, pointq0isinside, pointq1isinside) == vml::geo2d::Results::DOES_INTERSECT)
					{
						if (pointq0isinside) points.emplace_back(q0);
						if (pointq1isinside) points.emplace_back(q1);
					}

					// bottom box line intersection trest

					p0 = vml::math::vec2<T>(bmax.x, bmax.y);
					p1 = vml::math::vec2<T>(bmin.x, bmax.y);

					if (vml::geo2d::intersections::LineVsCircle(c0, radius, p0, p1, q0, q1, pointq0isinside, pointq1isinside) == vml::geo2d::Results::DOES_INTERSECT)
					{
						if (pointq0isinside) points.emplace_back(q0);
						if (pointq1isinside) points.emplace_back(q1);
					}

					// left box line intersection trest

					p0 = vml::math::vec2<T>(bmin.x, bmax.y);
					p1 = vml::math::vec2<T>(bmin.x, bmin.y);

					if (vml::geo2d::intersections::LineVsCircle(c0, radius, p0, p1, q0, q1, pointq0isinside, pointq1isinside) == vml::geo2d::Results::DOES_INTERSECT)
					{
						if (pointq0isinside) points.emplace_back(q0);
						if (pointq1isinside) points.emplace_back(q1);
					}

					if (points.size() != 0)
						return vml::geo2d::Results::DOES_INTERSECT;

				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/*
			///////////////////////////////////////////////////////////////////////////////////
			// test if two circles intersects , point(s) of intersection is computed

			static int CircleVsCircle(const vml::math::vec2<T>& c0, float r0,
									  const vml::math::vec2<T>& c1, float r1,
									  vml::math::vec2<T>& q1, vml::math::vec2<T>& q2,
									  const float eps = vml::math::EPSILON)
			{
				float a, dx, dy, d, h, rx, ry;
				float x2, y2;

				// dx and dy are the vertical and horizontal distances between
				// the circle centers.

				dx = c1.x - c0.x;
				dy = c1.y - c0.y;

				// Determine the straight-line distance between the centers. 

				d = sqrt((dy * dy) + (dx * dx));

				//d = hypot(dx, dy); // Suggested by Keith Briggs

				// Check for solvability. 

				if (d > (r0 + r1))
				{
					// no solution. circles do not intersect. 
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}
				if (d < fabs(r0 - r1))
				{
					// no solution. one circle is contained in the other 
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// 'point 2' is the point where the line through the circle
				//  intersection points crosses the line between the circle
				//  centers.

				// Determine the distance from point 0 to point 2. 

				a = ((r0 * r0) - (r1 * r1) + (d * d)) / (2.0 * d);

				// Determine the coordinates of point 2. 

				x2 = c0.x + (dx * a / d);
				y2 = c0.y + (dy * a / d);

				// Determine the distance from point 2 to either of the
				// intersection points.

				h = sqrt((r0 * r0) - (a * a));

				// Now determine the offsets of the intersection points from point 2.

				rx = -dy * (h / d);
				ry = dx * (h / d);

				// Determine the absolute intersection points. 

				q1.x = x2 + rx;
				q2.x = x2 - rx;

				q1.y = y2 + ry;
				q2.y = y2 - ry;

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			////////////////////////////////////////////////////////////////////
			// intersection between line and polygon

			static unsigned int LineVsPolygon(const vml::geo2d::Polygon &polygon, 
				                              const vml::geo2d::Line &line, 
				                              std::vector<vml::math::vec2<T>> &rs,
											  const float eps = vml::math::EPSILON)
			{

				rs.clear();

				int j = (int)polygon.GetVertexCount() - 1;

				for (int i = 0; i < polygon.GetVertexCount(); ++i)
				{

					vml::math::vec2<T> r;

					if (vml::geo2d::intersections::LineVsLine(polygon.GetVertex(i), polygon.GetVertex(j), line.P, line.Q, r) == vml::geo2d::Results::DOES_INTERSECT)
					{
						rs.emplace_back(r);
					}

					j = i;

				}

				if (rs.size() == 0)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				return vml::geo2d::Results::DOES_INTERSECT;

			}
			*/
		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace


///////////////////////////////////////////////////////////////////////////////////////
// triangle intersections class

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{
			/*
			//////////////////////////////////////////////////////////////////////////////
			// Moller triangle vs triangle intersection test

			class TriangleTriangleIntersection
			{

				private:

					// ---------------------------------------------------------------
					// compute ccw or anticcw direction of a triangle formaed by a,b,c points

					static float Orient(const vml::math::vec2<T> &a, const vml::math::vec2<T> &b, const vml::math::vec2<T> &c)
					{
						return (a[0] - c[0]) * (b[1] - c[1]) - (a[1] - c[1]) * (b[0] - c[0]);
					}

					// ---------------------------------------------------------------
					// Checks if vertex is inside triangle

					static unsigned int IntersectionTestVertex(const vml::math::vec2<T> &P1, const vml::math::vec2<T> &Q1, const vml::math::vec2<T> &R1, 
											                   const vml::math::vec2<T> &P2, const vml::math::vec2<T> &Q2, const vml::math::vec2<T> &R2)
					{
						if (Orient(R2, P2, Q1) >= 0.0f)
						{
							if (Orient(R2, Q2, Q1) <= 0.0f)
							{
								if (Orient(P1, P2, Q1) > 0.0f)
								{
									if (Orient(P1, Q2, Q1) <= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									if (Orient(P1, P2, R1) >= 0.0f)
									{
										if (Orient(Q1, R1, P2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
							}
							else
							{
								if (Orient(P1, Q2, Q1) <= 0.0f)
								{
									if (Orient(R2, Q2, R1) <= 0.0f)
									{
										if (Orient(Q1, R1, Q2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
						}
						else
						{
							if (Orient(R2, P2, R1) >= 0.0f)
							{
								if (Orient(Q1, R1, R2) >= 0.0f)
								{
									if (Orient(P1, P2, R1) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									if (Orient(Q1, R1, Q2) >= 0.0f)
									{
										if (Orient(R2, R1, Q2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
							}
							else
							{
								return vml::geo2d::Results::DOES_NOT_INTERSECT;
							}
						}
					}

					// ---------------------------------------------------------------
					// Check for edges intersection

					static unsigned int IntersectionTestEdge(const vml::math::vec2<T> &P1, const vml::math::vec2<T> &Q1, const vml::math::vec2<T> &R1, 
											                 const vml::math::vec2<T> &P2, const vml::math::vec2<T> &Q2, const vml::math::vec2<T> &R2)
					{
						if (Orient(R2, P2, Q1) >= 0.0f)
						{
							if (Orient(P1, P2, Q1) >= 0.0f)
							{
								if (Orient(P1, Q1, R2) >= 0.0f)
								{
									return vml::geo2d::Results::DOES_INTERSECT;
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
							else
							{
								if (Orient(Q1, R1, P2) >= 0.0f)
								{
									if (Orient(R1, P1, P2) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									return 0;
								}
							}
						}
						else
						{
							if (Orient(R2, P2, R1) >= 0.0f)
							{
								if (Orient(P1, P2, R1) >= 0.0f)
								{
									if (Orient(P1, R1, R2) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										if (Orient(Q1, R1, R2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
							else
							{
								return vml::geo2d::Results::DOES_NOT_INTERSECT;
							}
						}
					}

					// ---------------------------------------------------------------
					// 

					static unsigned int CCWTriTriIntersection(const vml::math::vec2<T> &p1, const vml::math::vec2<T> &q1, const vml::math::vec2<T> &r1,
													 const vml::math::vec2<T> &p2, const vml::math::vec2<T> &q2, const vml::math::vec2<T> &r2)
					{
						if (Orient(p2, q2, p1) >= 0.0f)
						{
							if (Orient(q2, r2, p1) >= 0.0f)
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return vml::geo2d::Results::DOES_INTERSECT;
								}
								else
								{
									return IntersectionTestEdge(p1, q1, r1, p2, q2, r2);
								}
							}
							else
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return IntersectionTestEdge(p1, q1, r1, r2, p2, q2);
								}
								else
								{
									return IntersectionTestVertex(p1, q1, r1, p2, q2, r2);
								}
							}
						}
						else
						{
							if (Orient(q2, r2, p1) >= 0.0f)
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return IntersectionTestEdge(p1, q1, r1, q2, r2, p2);
								}
								else
								{
									return IntersectionTestVertex(p1, q1, r1, q2, r2, p2);
								}
							}
							else
							{
								return IntersectionTestVertex(p1, q1, r1, r2, p2, q2);
							}
						}
					};

				public:

					// -------------------------------------------------------------------------
					// 

					static unsigned int Test(const vml::math::vec2<T> &p1, const vml::math::vec2<T> &q1, const vml::math::vec2<T> &r1,
											 const vml::math::vec2<T> &p2, const vml::math::vec2<T> &q2, const vml::math::vec2<T> &r2)
					{
						if (Orient(p1, q1, r1) < 0.0f)
						{
							if (Orient(p2, q2, r2) < 0.0f)
							{
								return CCWTriTriIntersection(p1, r1, q1, p2, r2, q2);
							}
							else
							{
								return CCWTriTriIntersection(p1, r1, q1, p2, q2, r2);
							}
						}
						else
						{
							if (Orient(p2, q2, r2) < 0.0f)
							{
								return CCWTriTriIntersection(p1, q1, r1, p2, r2, q2);
							}
							else
							{
								return CCWTriTriIntersection(p1, q1, r1, p2, q2, r2);
							}
						}
					}

					// ----------------------------------------------------------
					// ctro / dtor

					TriangleTriangleIntersection()
					{}

					~TriangleTriangleIntersection()
					{}

			};
			*/
		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo2d
	{
		namespace clipping
		{

			/*
			//////////////////////////////////////////////////////////////////
			// clip ab aann against another aabb
			// amin is target aabb
			// dmin is clipping aabb

			static unsigned int ClipAABBToAABB(const vml::math::vec2<T>& amin, const vml::math::vec2<T>& amax,
											   const vml::math::vec2<T>& dmin, const vml::math::vec2<T>& dmax,
											   float& aabb3Minx, float& aabb3Miny, float& aabb3Maxx, float& aabb3Maxy,
											   const float eps = vml::math::EPSILON)
			{
				// check if quad is out of aabb clipping box

				if ((dmax.x <= amin.x || dmin.x >= amax.x) ||
					(dmax.y <= amin.y || dmin.y >= amax.y))
				{
					aabb3Minx = amin.x;
					aabb3Miny = amin.y;
					aabb3Maxx = amax.x;
					aabb3Maxy = amax.y;
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// check if quad is inside of aabb clipping box

				if ((amin.x >= dmin.x && amax.x <= dmax.x) &&
					(amin.y >= dmin.y && amax.y <= dmax.y))
				{
					aabb3Minx = amin.x;
					aabb3Miny = amin.y;
					aabb3Maxx = amax.x;
					aabb3Maxy = amax.y;
					return vml::geo2d::Results::FIRST_INSIDE_SECOND;

				}

				// if we get here , we need to clip the target aabb
				// against the clipping aabb

				float aabb0Minx = 0;
				float aabb0Miny = 0;
				float aabb0Maxx = 0;
				float aabb0Maxy = 0;

				float aabb1Minx = 0;
				float aabb1Miny = 0;
				float aabb1Maxx = 0;
				float aabb1Maxy = 0;

				float aabb2Minx = 0;
				float aabb2Miny = 0;
				float aabb2Maxx = 0;
				float aabb2Maxy = 0;

				// clip dest square against top window

				if (amin.y - dmin.y < eps && amax.y - dmin.y > eps)
				{
					aabb0Minx = amin.x;
					aabb0Miny = dmin.y;
					aabb0Maxx = amax.x;
					aabb0Maxy = amax.y;
				}

				if (amin.y - dmin.y > eps)
				{
					aabb0Minx = amin.x;
					aabb0Miny = amin.y;
					aabb0Maxx = amax.x;
					aabb0Maxy = amax.y;
				}

				// clip dest square against bottom window

				if (aabb0Miny - dmax.y < eps && aabb0Maxy - dmax.y > eps)
				{
					aabb1Minx = aabb0Minx;
					aabb1Miny = aabb0Miny;
					aabb1Maxx = aabb0Maxx;
					aabb1Maxy = dmax.y;
				}

				if (aabb0Maxy - dmax.y < eps)
				{
					aabb1Minx = aabb0Minx;
					aabb1Miny = aabb0Miny;
					aabb1Maxx = aabb0Maxx;
					aabb1Maxy = aabb0Maxy;
				}

				// clip dest square against left window

				if (aabb1Minx - dmin.x < eps && aabb1Maxx - dmin.x > eps)
				{
					aabb2Minx = dmin.x;
					aabb2Miny = aabb1Miny;
					aabb2Maxx = aabb1Maxx;
					aabb2Maxy = aabb1Maxy;
				}

				if (aabb1Minx - dmin.x > eps)
				{
					aabb2Minx = aabb1Minx;
					aabb2Miny = aabb1Miny;
					aabb2Maxx = aabb1Maxx;
					aabb2Maxy = aabb1Maxy;
				}

				// clip dest square against right window

				if (aabb2Minx - dmax.x < eps && aabb2Maxx - dmax.x > eps)
				{
					aabb3Minx = aabb2Minx;
					aabb3Miny = aabb2Miny;
					aabb3Maxx = dmax.x;
					aabb3Maxy = aabb2Maxy;
				}

				if (aabb2Maxx - dmax.x < eps)
				{
					aabb3Minx = aabb2Minx;
					aabb3Miny = aabb2Miny;
					aabb3Maxx = aabb2Maxx;
					aabb3Maxy = aabb2Maxy;
				}

				// if we get here bounding boxes are intersecting each ohter

				return vml::geo2d::Results::DOES_INTERSECT;
			}
			*/
			/////////////////////////////////////////////////////////////////////////////
			// Implementing Cohen - Sutherland algorithm
			// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)

			static [[nodiscard]] uint32_t CohenSutherlandClip(int x1, int y1, int x2, int y2,
															  int XMIN, int YMIN, int XMAX, int YMAX,
															  int& xp, int& yp, int& xq, int& yq)
			{
				// Defining region codes

				constexpr unsigned int CS_INSIDE = 0;	// 0000 
				constexpr unsigned int CS_LEFT	 = 1;	// 0001
				constexpr unsigned int CS_RIGHT  = 2;	// 0010
				constexpr unsigned int CS_BOTTOM = 4;	// 0100
				constexpr unsigned int CS_TOP	 = 8;	// 1000

				// init with c1 as 'inside' code

				int c1 = CS_INSIDE;

				if (x1 < XMIN)				// to the left of rectangle
					c1 |= CS_LEFT;
				else if (x1 > XMAX)			// to the right of rectangle
					c1 |= CS_RIGHT;

				if (y1 < YMIN)				// below the rectangle
					c1 |= CS_BOTTOM;
				else if (y1 > YMAX)			// above the rectangle
					c1 |= CS_TOP;

				int c2 = CS_INSIDE;

				if (x2 < XMIN)				// to the left of rectangle
					c2 |= CS_LEFT;
				else if (x2 > XMAX)			// to the right of rectangle
					c2 |= CS_RIGHT;

				if (y2 < YMIN)				// below the rectangle
					c2 |= CS_BOTTOM;
				else if (y2 > YMAX)			// above the rectangle
					c2 |= CS_TOP;

				xp = -INT_MAX;
				yp = -INT_MAX;
				xq = INT_MAX;
				yq = INT_MAX;

				// Initialize line as outside the rectangular window

				while (true)
				{

					if ((c1 == 0) && (c2 == 0))
					{
						// If both endpoints lie within rectangle

						xp = x1;
						yp = y1;
						xq = x2;
						yq = y2;

						return vml::geo2d::Results::DOES_INTERSECT;

					}
					else if (c1 & c2)
					{
						// If both endpoints are outside rectangle,
						// in same region

						return vml::geo2d::Results::DOES_NOT_INTERSECT;
					}
					else
					{
						// Some segment of line lies within the
						// rectangle

						int c;
						int x, y;

						// At least one endpoint is outside the
						// rectangle, pick it.

						if (c1 != 0)
							c = c1;
						else
							c = c2;

						// Find intersection point;
						// using formulas y = y1 + slope * (x - x1),
						// x = x1 + (1 / slope) * (y - y1)

						if (c & CS_TOP)
						{
							// point is above the clip rectangle

							x = x1 + (x2 - x1) * (YMAX - y1) / (y2 - y1);
							y = YMAX;
						}
						else if (c & CS_BOTTOM)
						{
							// point is below the rectangle

							x = x1 + (x2 - x1) * (YMIN - y1) / (y2 - y1);
							y = YMIN;
						}
						else if (c & CS_RIGHT)
						{
							// point is to the right of rectangle

							y = y1 + (y2 - y1) * (XMAX - x1) / (x2 - x1);
							x = XMAX;
						}
						else if (c & CS_LEFT)
						{
							// point is to the left of rectangle

							y = y1 + (y2 - y1) * (XMIN - x1) / (x2 - x1);
							x = XMIN;
						}

						// Now intersection point x, y is found
						// We replace point outside rectangle
						// by intersection point

						if (c == c1)
						{
							x1 = x;
							y1 = y;

							c1 = CS_INSIDE;

							if (x1 < XMIN)				// to the left of rectangle
								c1 |= CS_LEFT;
							else if (x1 > XMAX)			// to the right of rectangle
								c1 |= CS_RIGHT;

							if (y1 < YMIN)				// below the rectangle
								c1 |= CS_BOTTOM;
							else if (y1 > YMAX)			// above the rectangle
								c1 |= CS_TOP;

						}
						else
						{
							x2 = x;
							y2 = y;

							c2 = CS_INSIDE;

							if (x2 < XMIN)				// to the left of rectangle
								c2 |= CS_LEFT;
							else if (x2 > XMAX)			// to the right of rectangle
								c2 |= CS_RIGHT;

							if (y2 < YMIN)				// below the rectangle
								c2 |= CS_BOTTOM;
							else if (y2 > YMAX)			// above the rectangle
								c2 |= CS_TOP;

						}

					}

				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}
			
		}	// end of intersections namespace

	}	// end of geo3d namespace

}	// end of vml namespace
