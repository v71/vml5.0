#pragma once

///////////////////////////////////////////////////////////////////////////////////////
//										Distance Functions
//
//					Point		Ray		Line	Circle		AABB		AOBB		Polygon
//					
//  Point			  X			 X 	 	  X  	  X  	     X  		  X	   
// 
//	Ray				  X 		 X	  	  X   	  X    		 X  
// 
//	Line			  X          X		  X 	  X	  	     X  		    		   			  			 
// 
//	Circle			  X			 X		  X		  X			 X 			  		   		    									  
// 
//	AABB			  X			 X 		  X       X			 X
// 
//	AOBB			  X
// 
// Polygon

///////////////////////////////////////////////////////////////////////////////////////
// squared distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// project point on normalized ray
		
			template <typename T>
			static void ProjectPointOnNormalizedRay(const vml::math::vec2<T>& p,
													const vml::math::vec2<T>& linep, const vml::math::vec2<T>& n,
													vml::math::vec2<T>& r,
													T& mindist)
			{
				mindist = vml::math::Dot(linep - p, n);
				r = p + mindist * n;
			}

			/////////////////////////////////////////////////////////////////////////////
			// project point on line

			template <typename T>
			static [[nodiscard]] uint32_t ProjectPointOnLine(const vml::math::vec2<T>& p,
														     const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
														     vml::math::vec2<T>& r,
															 T& mindist,
															 const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> n = vml::math::Normalize(vml::math::Ortho(lineq - linep));
				mindist = vml::math::Dot(linep - p, n);
				r = p + mindist * n;
				if ((r.x - linep.x) * n.y + (linep.y - r.y) * n.x > -eps && 
					(r.x - lineq.x) * n.y + (lineq.y - r.y) * n.x < -eps)
					return vml::geo2d::Results::DOES_INTERSECT;
				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Distance between points

			template <typename T>
			static [[nodiscard]] T SquaredDistanceBetweenPoints(const vml::math::vec2<T>& p,const vml::math::vec2<T>& q)
			{
				return vml::math::SquaredLength(q - p);
			}

			/////////////////////////////////////////////////////////////////////////////
			// Distance between points

			template <typename T>
			static [[nodiscard]] T DistanceBetweenPoints(const vml::math::vec2<T>& p, const vml::math::vec2<T>& q)
			{
				return vml::math::Length(q - p);
			}

			/////////////////////////////////////////////////////////////////////////////
			// test if 3 2d points are collinear

			template <typename T>
			static [[nodiscard]] bool ArePointsCollinear(const vml::math::vec2<T> &p0, const vml::math::vec2<T> &p1, const vml::math::vec2<T> &p2, const T eps = vml::math::EPSILON)
			{
				return fabs((p0.y - p1.y) * (p0.x - p2.x) - (p0.y - p2.y) * (p0.x - p1.x)) < eps;
			}

			/////////////////////////////////////////////////////////////////////////////
			// test if 2 2d points are whitin a radius

			template <typename T>
			static [[nodiscard]] bool ArePointsInRange(const vml::math::vec2<T> &p, const vml::math::vec2<T> &q, const float radius, const T eps = vml::math::EPSILON)
			{
				return vml::math::SquaredLength(q - p) - radius * radius < eps;
			}

			/////////////////////////////////////////////////////////////////////////////
			// test if 2 2d points are equals whitin an epsilon

			template <typename T>
			static [[nodiscard]] bool ArePointsEquals(const vml::math::vec2<T> &p, const vml::math::vec2<T> &q, const T eps = vml::math::EPSILON)
			{
				return vml::math::SquaredLength(q - p) < eps;
			}

		} // end of distances
	} // end of geo2d namespace
} // enf of vml namespace

///////////////////////////////////////////////////////////////////////////////////////
// signed distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to point

			template <typename T>
			static [[nodiscard]] T SignedDistanceFromCircleToPoint(const vml::math::vec2<T>& c, const T r, const vml::math::vec2<T>& p)
			{
				return vml::math::Length(c - p) - r;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Signed distance from point to  line
			// assumes line normal is normalized

			template <typename T>
			static [[nodiscard]] T SignedDistanceFromPointToRay(const vml::math::vec2<T>& p, const vml::math::vec2<T>& p0, const vml::math::vec2<T>& normal)
			{
				return vml::math::Dot( p0 - p, normal);
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to circlae

			template <typename T>
			static [[nodiscard]] T SignedDistanceFromCircleToCircle(const vml::math::vec2<T>& ca, const T ra, const vml::math::vec2<T>& cb, const T rb)
			{
				return vml::math::Length(cb - ca) - (ra+rb);
			}

		} // end of distances
	} // end of geo2d namespace
} // enf of vml namespace

///////////////////////////////////////////////////////////////////////////////////////
// distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between lines
			// TO DO : check when two lines intersects

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenLines(const vml::math::vec2<T>& p1, const vml::math::vec2<T>& q1,
																   const vml::math::vec2<T>& p2, const vml::math::vec2<T>& q2,
																   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																   T& mindist,
																   const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d1 = q1 - p1; // Direction vector of segment S1
				vml::math::vec2<T> d2 = q2 - p2; // Direction vector of segment S2
				vml::math::vec2<T> r = p1 - p2;

				T a = d1.x * d1.x + d1.y * d1.y; // Squared length of segment S1, always nonnegative
				T e = d2.x * d2.x + d2.y * d2.y; // Squared length of segment S2, always nonnegative
				T f = d2.x * r.x + d2.y * r.y;

				T s, t;

				// Check if either or both segments degenerate into points

				if (a < eps && e < eps)
				{
					// Both segments degenerate into points
					closestp = p1;
					closestq = p2;
					vml::math::vec2<T> dist = closestq - closestp;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
					return vml::geo2d::Results::FAIL;
				}

				if (a < eps)
				{
					// First segment degenerates into a point

					s = 0;
					t = f / e; // s = 0 => t = (b*s + f) / e = f / e
					if (t < 0) t = 0;
					if (t > 1) t = 1;
				}
				else
				{
					float c = d1.x*r.x + d1.y * r.y;

					if (e < eps)
					{
						// Second segment degenerates into a point

						t = 0;
						s = -c / a; // t = 0 => s = (b*t - c) / a = -c / a
						if (s < 0) s = 0;
						if (s > 1) s = 1;
					}
					else
					{
						// The general nondegenerate case starts here

						float b = d1.x * d2.x + d1.y * d2.y;
						float denom = a * e - b * b; // Always nonnegative

						// If segments not parallel, compute closest point on L1 to L2 and
						// clamp to segment S1. Else pick arbitrary s (here 0)

						// if (denom != 0.0f)
						if (denom < -eps || denom > eps)
						{
							s = (b * f - c * e) / denom;
							if (s < 0) s = 0;
							if (s > 1) s = 1;
						}
						else
						{
							s = 0;
						}

						// Compute point on L2 closest to S1(s) using
						// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e

						t = (b * s + f) / e;

						//If t in [0,1] done. Else clamp t, recompute s for the new value
						// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
						// and clamp s to [0, 1]

						if (t < 0)
						{
							t = 0;
							s = -c / a;

							if (s < 0) s = 0;
							if (s > 1) s = 1;
						}
						else if (t > (T)1)
						{
							t = 1;
							s = (b - c) / a;

							if (s < 0) s = 0;
							if (s > 1) s = 1;
						}
					}
				}

				// closest points
				closestp = p1 + d1 * s;
				closestq = p2 + d2 * t;
				// minimum distance
				vml::math::vec2<T> dist = closestq - closestp;
				mindist = dist.x * dist.x + dist.y * dist.y;
				if (mindist > eps) {
					mindist = sqrtf(mindist);
					return vml::geo2d::Results::DOES_NOT_INTERSECT; }
				mindist = 0;
				return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT; 
			}

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between rays

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenRays(const vml::math::vec2<T>& p, const vml::math::vec2<T>& v,
																  const vml::math::vec2<T>& q, const vml::math::vec2<T>& u,
																  vml::math::vec2<T>& closestp,
																  T& mindist,
																  const T eps = vml::math::EPSILON)
			{
				T denum = v.x * u.y - v.y * u.x;
				T num = (q.x - p.x) * v.y - (q.y - p.y) * v.x;

				// if rays are parallel compute minimun distance

				if (denum > -eps && denum < eps)
				{
					denum = v.x * v.x + v.y * v.y;
					if (denum > -eps && denum < eps)
						denum = eps;
					denum = 1 / denum;
					float t = num * denum;
					closestp.x = q.x - t * v.y;
					closestp.y = q.y + t * v.x;
					vml::math::vec2<T> d = closestp - p;
					mindist = sqrtf(d.x * d.x + d.y * d.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// if rays are not paralle coumpute intersection
				// point , thus the mindist is null
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1 / denum;
				float t = num * denum;
				closestp.x = q.x + t * u.x;
				closestp.y = q.y + t * u.y;
				mindist = 0;
				return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between ray and line

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenRayAndLine(const vml::math::vec2<T>& q, const vml::math::vec2<T>& u,
																		const vml::math::vec2<T>& a, const vml::math::vec2<T>& b,
																		vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																		T& mindist,
																		const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> r0, r1;
				T num, denum;
				T mindist0, mindist1;
				T t, dp0, dp1;
				T dx, dy;

				// sets initial distance as maximum

				mindist = FLT_MAX;

				// compute line directions vector

				dx = b.x - a.x;
				dy = b.y - a.y;
				denum = dx * dx + dy * dy;
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1 / sqrtf(denum);

				vml::math::vec2<T> v(-dy * denum, dx * denum);

				// the lines above equals to this line
				// vml::math::vec2<T> v = vml::math::Normalize(vml::math::Ortho(b - a));

				// comput denum for ray

				denum = u.x * u.x + u.y * u.y;
				if (denum > -eps && denum < eps)
					denum = eps;

				// project a line point on ray

				num = (a.x - q.x) * u.y + (q.y - a.y) * u.x;
				t = num / denum;
				r0.x = a.x - t * u.y;
				r0.y = a.y + t * u.x;
				dx = r0.x - a.x;
				dy = r0.y - a.y;
				mindist0 = dx * dx + dy * dy;
				dp0 = dy * u.x - dx * u.y;
				if (mindist0 < mindist)
				{
					mindist = mindist0;
					closestp = r0;
					closestq = a;
				}

				// project b line point on ray

				num = (q.x - b.x) * (-u.y) + (q.y - b.y) * (u.x);
				t = num / denum;
				r1.x = b.x - t * u.y;
				r1.y = b.y + t * u.x;
				dx = r1.x - b.x;
				dy = r1.y - b.y;
				mindist1 = dx * dx + dy * dy;
				dp1 = dy * u.x - dx * u.y;
				if (mindist1 < mindist)
				{
					mindist = mindist1;
					closestp = r1;
					closestq = b;
				}
				// check if line crosses the ray

				if ((dp0 > -eps || dp1 < eps) && (dp0 < eps || dp1 > -eps))
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				num = (a.x - q.x) * u.y - (a.y - q.y) * u.x;
				denum = u.x * v.x + u.y * v.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				t = num / denum;
				closestp.x = a.x - t * v.y;
				closestp.y = a.y + t * v.x;
				mindist = 0;
				return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
			}

		} // end of distances
	} // end of geo2d namespace
} // enf of vml namespace

///////////////////////////////////////////////////////////////////////////////////////
// closest points

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// closest point on line from point
			// distance is signed

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromPointToLine(const vml::math::vec2<T>& p,
																	  const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
																	  vml::math::vec2<T>& closestp,
																	  T& mindist,
																	  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dist = lineq - linep;
				T denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1 / denum;
				
				vml::math::vec2<T> n(-dist.y * denum, dist.x * denum);

				// Project c onto ab, computing parameterized position d(t) = a + t*(b – a)

				T t = (linep.x - p.x) * n.x + (linep.y - p.y) * n.y;

				// intersc tion point

				closestp = p + t * n;

				// find if point stays on line

				T dp0 = -(linep.x - closestp.x) * n.y + (linep.y - closestp.y) * n.x;
				T dp1 = -(closestp.x - lineq.x) * n.y + (closestp.y - lineq.y) * n.x;

				// point is on line
				
				if (dp0 > -eps && dp1 > -eps)
				{
					mindist = 0;
					return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
				}

				// check if point is past the first line vertex

				if (dp0 < -eps)
				{
					closestp = linep;
					dist = p - closestp;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
				}

				// check if point is past the second line vertex

				if (dp1 < -eps)
				{
					closestp = lineq;
					dist = p - closestp;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q in Circle b which is closest to p

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromPointToCircle(const vml::math::vec2<T>& p,
																		const vml::math::vec2<T>& c, const float r,
																		vml::math::vec2<T>& closestp,
																		T& mindist,
																	    const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = c - p;
				T a = d.x * d.x + d.y * d.y;
				if (a > -eps && a < eps)
				{	closestp = c;
					mindist = 0;
					return vml::geo2d::Results::INSIDE; }
				T t = (a - r * sqrtf(a)) / a;
				closestp = p + t * d;
				mindist = sqrtf(t*a);
				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q in Circle b which is closest to p

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromCircleToCircle(const vml::math::vec2<T>& c0, const float r0,
																	     const vml::math::vec2<T>& c1, const float r1,
																		 vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																		 T& mindist,
																		 const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = c1 - c0;
				T denum = sqrt( d.y*d.y + d.x*d.x );

				// Check for solvability. 
				
				if (denum > r0 + r1)
				{
					// no solution. circles do not intersect. 
					vml::math::vec2<T> dir = d/denum;
					closestp = c0 + r0 * dir;
					closestq = c1 - r1 * dir;
					mindist = 0;
					return vml::geo2d::Results::OUTSIDE;
				}

				if (denum < fabs(r0 - r1))
				{
					// no solution. one circle is contained in the other 
					mindist = 0;
					float diff = denum - (r0 - r1);
					if (diff > 0)
						return vml::geo2d::Results::FIRST_INSIDE_SECOND;
					else if ( diff <0)
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;
				}

				// 'point 2' is the point where the line through the circle
				// intersection points crosses the line between the circle centers.

				// Determine the distance from point 0 to point 2. 

				T a = ( r0 * r0 - r1 * r1 + denum * denum ) / (2 * denum);

				// Determine the coordinates of point 2. 
				
				T a1 = a / denum;
				T x2 = c0.x + (d.x * a1);
				T y2 = c0.y + (d.y * a1);

				// Determine the distance from point 2 to either of the intersection points.
				 
				T h = sqrtf( r0 * r0 - a * a );

				// Now determine the offsets of the intersection points from point 2.
				 
				T h1 = h / denum;
				T rx = -d.y * h1;
				T ry =  d.x * h1;

				// Determine the absolute intersection points. 

				closestp.x = x2 + rx;
				closestp.y = y2 + ry;
				closestq.x = x2 - rx;
				closestq.y = y2 - ry;

				vml::math::vec2<T> dist = closestq - closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given a line with p1 and p2 points, and a cricle p with radius r
			// computes the closest point from circle to line

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromLineToCircle(const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
																	   const vml::math::vec2<T>& p, const T r,
																	   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																	   T& mindist,
																	   const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> pq = lineq - linep;
				vml::math::vec2<T> ap = p  - linep;
				T a = pq.x * pq.x + pq.y * pq.y;
				if (a > -eps && a < eps)
					a = eps;
				T b = pq.x * ap.x + pq.y * ap.y;
				T t = b / a;
				
				// check if closest point is at line extremes

				if (t < -eps) closestp = linep;
				if (t > 1 + eps) closestp = lineq;
				if (t > -eps && t < 1 + eps) closestp = linep + t * pq;
				vml::math::vec2<T> dist = closestp - p;
				T denum=sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				
				// if circle is intersectiong the line
				// compute intersection points

				if (denum < r)
				{
					T c = ap.x * ap.x + ap.y * ap.y - r * r;
					T delta= sqrtf(b * b - a * c) / a;
					T u = t + delta;
					T v = t - delta;
					closestp = linep + u * pq;
					closestq = linep + v * pq;
					mindist = 0.0f;
					T numa = ( (closestp.x - lineq.x) * pq.x + (closestp.y - lineq.y) * pq.y);
					T numb = ( (closestq.x - linep.x) * pq.x + (closestq.y - linep.y) * pq.y);
					
					// border conditions 

					if (numa < 0 && numb < 0)
					{
						// border conditions for left side of the line
						//closestp = closestp;
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

				// if circle does not intersects the line, then compute the closest point

				t = r / denum;
				closestq = p + t * dist;
				dist=closestq-closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}
		
			/////////////////////////////////////////////////////////////////////////////
		    // Given a ray with p1 and p2 points, and a cricle p with radius r
		    // computes the closest point from circle to line
			// normal 'n' must be normalized

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromRayToCircle(const vml::math::vec2<T>& linep, const vml::math::vec2<T>& n,
																	  const vml::math::vec2<T>& p, const float r,
																	  vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																      T &mindist,
																	  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> pq = vml::math::vec2<T>(-n.y,n.x);
				vml::math::vec2<T> ap = p - linep;
				T b = pq.x * ap.x + pq.y * ap.y;
				closestp = linep + b * pq;
				vml::math::vec2<T> dist = closestp - p;
				T denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;

				// if circle is intersectiong the line
				// compute intersection points

				if (denum < r)
				{
					T c = ap.x * ap.x + ap.y * ap.y - r * r;
					T delta = sqrtf(b * b - c);
					T u = b + delta;
					T v = b - delta;
					closestp = linep + u * pq;
					closestq = linep + v * pq;
					mindist = 0;

					return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
				}

				// if circle does not intersects the line, then compute the closest point

				T t = r / denum;
				closestq = p + t * dist;
				dist = closestq - closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between a line and an axis alinged bounding box

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAABBToLine(const vml::math::vec2<T>& aabbMin, const vml::math::vec2<T>& aabbMax, 
																     const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
																     vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																     T& mindist,
																	 const T eps = vml::math::EPSILON)
			{
				// check if box and line intersect
				uint32_t result = vml::geo2d::intersections::AABBOXVsLine(aabbMin, aabbMax, linep, lineq, closestp, closestq);
				if (result != vml::geo2d::Results::DOES_NOT_INTERSECT)
					return result;
				// if not, compute closest point from line to each side of the box
				// cache box vertices
				vml::math::vec2<T> b0 = vml::math::vec2<T>(aabbMin.x, aabbMin.y);
				vml::math::vec2<T> b1 = vml::math::vec2<T>(aabbMax.x, aabbMin.y);
				vml::math::vec2<T> b2 = vml::math::vec2<T>(aabbMax.x, aabbMax.y);
				vml::math::vec2<T> b3 = vml::math::vec2<T>(aabbMin.x, aabbMax.y);
				vml::math::vec2<T> p0, q0, p1, q1, p2, q2,p3, q3;
				T d0 = FLT_MAX;
				T d1 = FLT_MAX;
				T d2 = FLT_MAX;
				T d3 = FLT_MAX;
				// compute distance from each side of the box to line
				ClosestPointBetweenLines(linep, lineq, b0, b1, p0, q0, d0,eps);
				ClosestPointBetweenLines(linep, lineq, b1, b2, p1, q1, d1,eps);
				ClosestPointBetweenLines(linep, lineq, b2, b3, p2, q2, d2,eps);
				ClosestPointBetweenLines(linep, lineq, b3, b0, p3, q3, d3,eps);
				// find the closest point
				T dmax = FLT_MAX;
				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }
				vml::math::vec2<T> d = closestq-closestp;
				mindist = sqrtf(d.x * d.x + d.y * d.y);
				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q on or in AABB b whichis closest to p
			// For each coordinate axis, if the point coordinate value is
			// outside box, clamp it to the box, else keep it as is

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAABBoxToPoint(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
																	    const vml::math::vec2<T>& p,
																	    vml::math::vec2<T>& closestp,
																		T& mindist)
			{
				if ((bmin.x >= p.x || p.x >= bmax.x) || (bmin.y >= p.y || p.y >= bmax.y))
				{
					closestp = p;
					if (closestp.x < bmin.x) closestp.x = bmin.x;
					if (closestp.x > bmax.x) closestp.x = bmax.x;
					if (closestp.y < bmin.y) closestp.y = bmin.y;
					if (closestp.y > bmax.y) closestp.y = bmax.y;
					// compute minimum distance
					vml::math::vec2<T> dist = p - closestp;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
					return vml::geo2d::Results::OUTSIDE;
				}
				mindist = 0;
				return vml::geo2d::Results::INSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q on or in AABB b whichis closest to p
			// For each coordinate axis, if the point coordinate value is
			// outside box, clamp it to the box, else keep it as is

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAABBoxToCircle(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
																		 const vml::math::vec2<T>& p, const float r,
																		 std::vector <vml::math::vec2<T>>& points,
																		 T& mindist,
																		 const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> closestp;
				vml::math::vec2<T> closestq;

				T dx, dy;
				T cx, cy;
				T a, b, c;
				T delta;
				T t0, t1;
				size_t numpoints;

				mindist = 0;
				dx = bmax.x - bmin.x;
				dy = bmax.y - bmin.y;

				// test intersection between circle and top rectangle segment

				cx = p.x - bmin.x;
				cy = p.y - bmin.y;
				a = dx * dx;
				b = cx * dx;
				c = cx * cx + cy * cy - r * r;
				delta = b * b - a * c;
				a = 1 / a;

				numpoints = 0;

				if (delta > 0)
				{
					delta = sqrtf(delta);
					t0 = -b * a + delta * a;
					t1 = -b * a - delta * a;

					vml::math::vec2<T> r0 = vml::math::vec2<T>(bmin.x - t0 * dx, bmin.y);
					vml::math::vec2<T> r1 = vml::math::vec2<T>(bmin.x - t1 * dx, bmin.y);

					if (r0.x >= bmin.x && r0.x <= bmax.x)
					{
						numpoints++;
						points.emplace_back(r0);
					}

					if (r1.x >= bmin.x && r1.x <= bmax.x)
					{
						numpoints++;
						points.emplace_back(r1);
					}
				}

				// test intersection between circle and bottom rectangle segment

				cx = p.x - bmin.x;
				cy = p.y - bmax.y;
				a = dx * dx;
				b = cx * dx;
				c = cx * cx + cy * cy - r * r;
				delta = b * b - a * c;
				a = 1 / a;

				if (delta > 0)
				{
					delta = sqrtf(delta);
					t0 = -b * a + delta * a;
					t1 = -b * a - delta * a;

					vml::math::vec2<T> r0 = vml::math::vec2<T>(bmin.x - t0 * dx, bmax.y);
					vml::math::vec2<T> r1 = vml::math::vec2<T>(bmin.x - t1 * dx, bmax.y);

					if (r0.x >= bmin.x && r0.x <= bmax.x)
					{
						numpoints++;
						points.emplace_back(r0);
					}

					if (r1.x >= bmin.x && r1.x <= bmax.x)
					{
						numpoints++;
						points.emplace_back(r1);
					}
				}

				// test intersection between circle and left rectangle segment

				cx = p.x - bmin.x;
				cy = p.y - bmin.y;
				a = dy * dy;
				b = cy * dy;
				c = cx * cx + cy * cy - r * r;
				delta = b * b - a * c;
				a = 1 / a;

				if (delta > 0)
				{
					delta = sqrtf(delta);
					t0 = -b * a + delta * a;
					t1 = -b * a - delta * a;
					vml::math::vec2<T> r0 = vml::math::vec2<T>(bmin.x, bmin.y - t0 * dy);
					vml::math::vec2<T> r1 = vml::math::vec2<T>(bmin.x, bmin.y - t1 * dy);

					if (r0.y >= bmin.y && r0.y <= bmax.y)
					{
						numpoints++;
						points.emplace_back(r0);
					}

					if (r1.y >= bmin.y && r1.y <= bmax.y)
					{
						numpoints++;
						points.emplace_back(r1);
					}

				}

				// test intersection between circle and right rectangle segment

				cx = p.x - bmax.x;
				cy = p.y - bmax.y;
				a = dy * dy;
				b = cy * dy;
				c = cx * cx + cy * cy - r * r;
				delta = b * b - a * c;
				a = 1 / a;

				if (delta > 0)
				{
					delta = sqrtf(delta);
					t0 = -b * a + delta * a;
					t1 = -b * a - delta * a;
					vml::math::vec2<T> r0 = vml::math::vec2<T>(bmax.x, bmax.y - t0 * dy);
					vml::math::vec2<T> r1 = vml::math::vec2<T>(bmax.x, bmax.y - t1 * dy);

					if (r0.y >= bmin.y && r0.y <= bmax.y)
					{
						numpoints++;
						points.emplace_back(r0);
					}

					if (r1.y >= bmin.y && r1.y <= bmax.y)
					{
						numpoints++;
						points.emplace_back(r1);
					}
				}

				// if we get here, there is no intersection
				// but we meed to know if circle is entirely 
				// inside the rectangle

				if (numpoints != 0)
					return vml::geo2d::Results::DOES_INTERSECT;

				// check if the second bounding box is entirely inside the first bounding box

				if ((p.x - r >= bmin.x && p.x + r <= bmax.x) &&
					(p.y - r >= bmin.y && p.y + r <= bmax.y))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				if ((bmin.x >= p.x - r && bmax.x <= p.x + r) &&
					(bmin.y >= p.y - r && bmax.y <= p.y + r))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				// if cirlce isn't contained in the rectangle, 
				// then compute closest points

				closestp = p;

				// compute closest point on box from circle center

				if (closestp.x < bmin.x) closestp.x = bmin.x;
				if (closestp.x > bmax.x) closestp.x = bmax.x;
				if (closestp.y < bmin.y) closestp.y = bmin.y;
				if (closestp.y > bmax.y) closestp.y = bmax.y;
				vml::math::vec2<T> dist = closestp - p;
				float denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				dist.x *= denum;
				dist.y *= denum;

				// compute intersection from closest 
				// point on box to circle center

				dist = r * dist;
				denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				denum = 1.0f / denum;
				float t = r * denum;
				closestq = p + t * dist;

				// compute minimum distance

				points.emplace_back(closestp);
				points.emplace_back(closestq);
				dist = closestq - closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between a line and an axis alinged bounding box

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAABBToRay(const vml::math::vec2<T>& aabbMin, const vml::math::vec2<T>& aabbMax,
													  			    const vml::math::vec2<T>& linep, const vml::math::vec2<T>& dir,
																    vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																    T& mindist,
																	const T eps = vml::math::EPSILON)
			{
				// check if box and line intersect
				uint32_t result = vml::geo2d::intersections::AABBOXVsRay(aabbMin, aabbMax, linep, dir, closestp, closestq);
				if (result != vml::geo2d::Results::DOES_NOT_INTERSECT)
					return result;
				// if not, compute closest point from line to each side of the box
				// cache box vertices
				vml::math::vec2<T> b0 = vml::math::vec2<T>(aabbMin.x, aabbMin.y);
				vml::math::vec2<T> b1 = vml::math::vec2<T>(aabbMax.x, aabbMin.y);
				vml::math::vec2<T> b2 = vml::math::vec2<T>(aabbMax.x, aabbMax.y);
				vml::math::vec2<T> b3 = vml::math::vec2<T>(aabbMin.x, aabbMax.y);
				vml::math::vec2<T> p0, q0, p1, q1, p2, q2, p3, q3;
				T d0 = FLT_MAX;
				T d1 = FLT_MAX;
				T d2 = FLT_MAX;
				T d3 = FLT_MAX;
				// compute distance from each side of the box to line
				ClosestPointBetweenRayAndLine(linep, dir, b0, b1, p0, q0 ,d0 ,eps);
				ClosestPointBetweenRayAndLine(linep, dir, b1, b2, p1, q1, d1, eps);
				ClosestPointBetweenRayAndLine(linep, dir, b2, b3, p2, q2, d2, eps);
				ClosestPointBetweenRayAndLine(linep, dir, b3, b0, p3, q3, d3, eps);
				// find the closest point
				float dmax = FLT_MAX;
				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }
				vml::math::vec2<T> d = closestq - closestp;
				mindist = sqrtf(d.x * d.x + d.y * d.y);
				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between an aabbox and another aabbx

			template <typename T>
			static void ClosestPointFromAABBToAABB(const vml::math::vec2<T>& amin, const vml::math::vec2<T>& amax,
												   const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
												   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
												   T& mindist)
			{
				// if the following conditions are met, the distance is zero

				mindist = 0;

				// check if the first bounding box is entirely inside the second bounding box

				if ((amin.x >= bmin.x && amax.x <= bmax.x) &&
					(amin.y >= bmin.y && amax.y <= bmax.y))
					return;

				// check if the second bounding box is entirely inside the first bounding box

				if ((bmin.x >= amin.x && bmax.x <= amax.x) &&
					(bmin.y >= amin.y && bmax.y <= amax.y))
					return;

				// check if bounding box intersects each other

				if ((bmax.x >= amin.x && bmin.x <= amax.x) && (bmax.y >= amin.y && bmin.y <= amax.y))
					return;

				vml::math::vec2<T> c0, c1, c2, c3;
				T dist[4] = { 0 };
				vml::math::vec2<T> closest_a_start, closest_a_end;
				vml::math::vec2<T> closest_b_start, closest_b_end;

				// cache aabbox vertices

				vml::math::vec2<T> pa0 = vml::math::vec2<T>(amin.x, amin.y);
				vml::math::vec2<T> pa1 = vml::math::vec2<T>(amax.x, amin.y);
				vml::math::vec2<T> pa2 = vml::math::vec2<T>(amax.x, amax.y);
				vml::math::vec2<T> pa3 = vml::math::vec2<T>(amin.x, amax.y);

				vml::math::vec2<T> pb0 = vml::math::vec2<T>(bmin.x, bmin.y);
				vml::math::vec2<T> pb1 = vml::math::vec2<T>(bmax.x, bmin.y);
				vml::math::vec2<T> pb2 = vml::math::vec2<T>(bmax.x, bmax.y);
				vml::math::vec2<T> pb3 = vml::math::vec2<T>(bmin.x, bmax.y);

				// compute closest point and minimum distance from each 
				// vertex of first rect to second rect

				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pb0, pb2, pa0, c0, dist[0]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pb0, pb2, pa1, c1, dist[1]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pb0, pb2, pa2, c2, dist[2]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pb0, pb2, pa3, c3, dist[3]);

				// find shortest distance 

				closest_a_start = pa0;
				closest_a_end = c0;
				if (dist[1] < dist[0]) { closest_a_start = pa1; closest_a_end = c1; }
				if (dist[2] < dist[1]) { closest_a_start = pa2; closest_a_end = c2; }
				if (dist[3] < dist[2]) { closest_a_start = pa3; closest_a_end = c3; }

				// compute closest point and minimum distance from each 
				// vertex of second rect to first rect

				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pa0, pa2, pb0, c0, dist[0]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pa0, pa2, pb1, c1, dist[1]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pa0, pa2, pb2, c2, dist[2]);
				vml::geo2d::distances::ClosestPointFromAABBoxToPoint(pa0, pa2, pb3, c3, dist[3]);

				// find shortest distance 

				closest_b_start = pb0;
				closest_b_end = c0;
				if (dist[1] < dist[0]) { closest_b_start = pb1; closest_b_end = c1; }
				if (dist[2] < dist[1]) { closest_b_start = pb2; closest_b_end = c2; }
				if (dist[3] < dist[2]) { closest_b_start = pb3; closest_b_end = c3; }

				// find shortest vector

				float adx = closest_a_end.x - closest_a_start.x;
				float ady = closest_a_end.y - closest_a_start.y;
				float bdx = closest_b_end.x - closest_b_start.x;
				float bdy = closest_b_end.y - closest_b_start.y;
				float ad0 = adx * adx + ady * ady;
				float bd0 = bdx * bdx + bdy * bdy;

				closestp = closest_a_start;
				closestq = closest_a_end;
				mindist = ad0;
				if (bd0 < ad0)
				{
					closestp = closest_b_start;
					closestq = closest_b_end;
					mindist = bd0;
				}
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q on or in AOBB b whichis closest to p

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAOBBoxToPoint(const vml::math::vec2<T>& p1, const vml::math::vec2<T>& p2,
																		const vml::math::vec2<T>& p3, const vml::math::vec2<T>& p4,
																		const vml::math::vec2<T>& o,
																		const vml::math::vec2<T>& u, const vml::math::vec2<T>& v,
																		const vml::math::vec2<T>& p,
																		vml::math::vec2<T>& closestp,
																		T &mindist,
																		const T eps = vml::math::EPSILON)
			{
				// extract bounding box reference system
				// u and v are respectively the axes center 
				// in the counding box center

				// computer inverse matricx
				// so that the bounding box is centered at orgin
				// with the rotational part aligned to an
				// orthonormal base so that the oriented 
				// bounding boixs is transformed into an
				// axis aligned bounding box

				T det = v.y * u.x - u.y * v.x;
				if (det > -eps && det < eps)
					det = eps;
				det = 1 / det;

				T N0 = -v.x * det;
				T N1 = -u.x * det;
				T N2 = v.y * det;
				T N3 = u.y * det;

				// compute transformed point
				// so we can test the trasnfored point against
				// the axis aligned bounding box

				T dx = p.x - o.x;
				T dy = p.y - o.y;

				vml::math::vec2<T> tp,tq;

				tp.x = dx * N0 + dy * N1;
				tp.y = dx * N2 + dy * N3;

				T t1x = p1.x - o.x;
				T t1y = p1.y - o.y;
				T t2x = p2.x - o.x;
				T t2y = p2.y - o.y;
				T t3x = p3.x - o.x;
				T t3y = p3.y - o.y;
				T t4x = p4.x - o.x;
				T t4y = p4.y - o.y;

				vml::math::vec2<T> bmin(t1x * N0 + t1y * N1, t1x * N2 + t1y * N3);
				vml::math::vec2<T> bmax(t3x * N0 + t3y * N1, t3x * N2 + t3y * N3);

				// compute the transformed point closest point to the
				// axis alinged bounding box

				mindist = 0;

				if (vml::geo2d::distances::ClosestPointFromAABBoxToPoint(bmin, bmax, tp, tq, mindist) == vml::geo2d::Results::OUTSIDE)
				{
					// transform point into original axis oriented bounding box reference system
					closestp = vml::math::vec2<T>(tq.x * N0 + tq.y * N2 + o.x, tq.x * N1 + tq.y * N3 + o.y);
					vml::math::vec2<T> dist = closestp - p;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
					return vml::geo2d::Results::OUTSIDE;
				}
				
				return vml::geo2d::Results::INSIDE;
			}
			/*
			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between a line and an axis alinged bounding box

			static void ClosestPointFromAOBBToLine(const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
												   const vml::math::vec2<T>& b0, const vml::math::vec2<T>& b1, const vml::math::vec2<T>& b2, const vml::math::vec2<T>& b3,
												   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq)

			{
			

			}
			*/
		}  // end of closest distances namespace
	} // end of geo2d namespace
} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// polygon distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{
			/*
			/////////////////////////////////////////////////////////////////
			// tests if a point is inside a concave or convex polygon,
			// not that this function might give you false positive if the 
			// point lies excatly on the border since multiple intersections
			// might be found. 
			
			static unsigned int IsPointInPolygon(const vml::geo2d::Polygon &polygon, const vml::math::vec2<T> &point)
			{
				bool oddnodes = false;

				int j = int(polygon.GetVertexCount()) - 1;

				for (int i = 0; i < polygon.GetVertexCount(); ++i)
				{

					vml::math::vec2<T> p1 = polygon.GetVertex(i);

					vml::math::vec2<T> p2 = polygon.GetVertex(j);

					if ((p1.y < point.y && p2.y >= point.y || p2.y < point.y && p1.y >= point.y) && (p1.x <= point.x || p2.x <= point.x))
					{
						oddnodes ^= (p1.x + (point.y - p1.y) / (p2.y - p1.y)*(p2.x - p1.x) <= point.x);
					}

					j = i;

				}

				if (!oddnodes)
					return vml::geo2d::Results::OUTSIDE;

				return vml::geo2d::Results::INSIDE;
			}
			*/
		} // end of distances
	} // end of geo2d namespace
} // enf of vml namespace

