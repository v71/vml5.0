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
			static void DistanceFromPointToRay(const vml::math::vec2<T>& point,
											   const vml::math::vec2<T>& q, const vml::math::vec2<T>& dir,
											   vml::math::vec2<T>& r,
											   T& mindist)
			{
				mindist = -(q.x - point.x) * dir.y + (q.y - point.y) * dir.x;
				r.x = point.x - mindist * dir.y;
				r.y = point.y + mindist * dir.x;
			}

			/////////////////////////////////////////////////////////////////////////////
			// project point on line

			template <typename T>
			static [[nodiscard]] uint32_t DistanceFromPointToLine(const vml::math::vec2<T>& p,
																  const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
															      vml::math::vec2<T>& r,
																  T& mindist,
																  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2f n;
				n.x = -lineq.y + linep.y;
				n.y =  lineq.x - linep.x;
				T denum = n.x * n.x + n.y * n.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = (T)1 / sqrtf(denum);
				n.x *= denum;
				n.y *= denum;
				mindist = (linep.x - p.x) * n.x + (linep.y - p.y) * n.y;
				r.x = p.x + mindist * n.x;
				r.y = p.y + mindist * n.y;
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
				vml::math::vec2<T> d=q-p;
				return d.x * d.x + d.y * d.y;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Distance between points

			template <typename T>
			static [[nodiscard]] T DistanceBetweenPoints(const vml::math::vec2<T>& p, const vml::math::vec2<T>& q)
			{
				vml::math::vec2<T> d = q - p;
				return sqrtf(d.x * d.x + d.y * d.y);
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
			static [[nodiscard]] bool ArePointsInRange(const vml::math::vec2<T> &p, const vml::math::vec2<T> &q, const T radius, const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = q - p;
				return d.x * d.x + d.y * d.y - radius * radius < eps;
			}

			/////////////////////////////////////////////////////////////////////////////
			// test if 2 2d points are equals whitin an epsilon

			template <typename T>
			static [[nodiscard]] bool ArePointsEquals(const vml::math::vec2<T> &p, const vml::math::vec2<T> &q, const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = q - p;
				return d.x * d.x + d.y * d.y < eps;
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
				vml::math::vec2<T> d = c - p;
				return sqrtf(d.x * d.x + d.y * d.y) - r;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Signed distance from point to  line
			// assumes line normal is normalized

			template <typename T>
			static [[nodiscard]] T SignedDistanceFromPointToRay(const vml::math::vec2<T>& p, const vml::math::vec2<T>& p0, const vml::math::vec2<T>& normal)
			{
				return (p0.x - p.x) * normal.x + (p0.y - p.y) * normal.y;
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to circlae

			template <typename T>
			static [[nodiscard]] T SignedDistanceFromCircleToCircle(const vml::math::vec2<T>& ca, const T ra, const vml::math::vec2<T>& cb, const T rb)
			{
				vml::math::vec2<T> d = cb - ca;
				return sqrtf( d.x * d.x + d.y * d.y) - (ra+rb);
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

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenLines(const vml::math::vec2<T>& p1, const vml::math::vec2<T>& q1,
																   const vml::math::vec2<T>& p2, const vml::math::vec2<T>& q2,
																   vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																   T& mindist,
																   const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> ba = q1 - p1;
				vml::math::vec2<T> da = q2 - p1;
				vml::math::vec2<T> cd = p2 - q2;

				T denom = cd.y * ba.x - cd.x * ba.y;
				T numa  = cd.y * da.x - cd.x * da.y;
				T numb  = ba.y * da.x - ba.x * da.y;
				
				// compute intersection point

				denom = (T)1.0 / denom;

				T ua = numa * denom;
				T ub = numb * denom;

				if (ua >= -eps && ua <= (T)1.0 + eps && ub >= -eps && ub <= (T)1.0 + eps)
				{
					// Get the intersection point.

					closestp = p1 + ua * ba;
					closestq = closestp;
					mindist = (T)0;

					return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
				}

				// proejct segment vertices on both segments

				T dmin,t;

				vml::math::vec2<T> cb = p2 - q1;
				vml::math::vec2<T> ca = p2 - p1;

				denom = (T) 1.0 / (ba.x * ba.x + ba.y * ba.y);

				// project c onto a and b
				
				t = (ca.x * ba.x + ca.y * ba.y) * denom;
				dmin = (T)1.0;
				if (t < dmin) dmin = t;
				t = (T)0.0;
				if (dmin > (T)0.0) t = dmin;
				vml::math::vec2<T> r1 = p1 + t * ba;

				// project d onto a and b

				t = (da.x * ba.x + da.y * ba.y) * denom;
				dmin = (T)1.0;
				if (t < dmin) dmin = t;
				t = (T)0.0;
				if (dmin > (T)0.0) t = dmin;
				vml::math::vec2<T> r2 = p1 + t * ba;

				// project c onto d and a

				denom = (T)1.0f / (cd.x * cd.x + cd.y * cd.y);

				t = (ca.x * cd.x + ca.y * cd.y) * denom;
				dmin = (T)1.0;
				if (t < dmin) dmin = t;
				t = (T)0.0;
				if (dmin > (T)0.0) t = dmin;
				vml::math::vec2<T> r3 = p2 - t * cd;

				// project c onto d and b

				t = (cb.x * cd.x + cb.y * cd.y) * denom;
				dmin = (T)1.0;
				if (t < dmin) dmin = t;
				t = (T)0.0;
				if (dmin > (T)0.0) t = dmin;
				vml::math::vec2<T> r4 = p2 - t * cd;

				// sort distanfes
				
				vml::math::vec2<T> dist;

				dist = p2 - r1;
				T d0 = dist.x * dist.x + dist.y * dist.y;

				dist = q2 - r2;
				T d1 = dist.x * dist.x + dist.y * dist.y;

				dist = r3 - p1;
				T d2 = dist.x * dist.x + dist.y * dist.y;

				dist = r4 - q1;
				T d3 = dist.x * dist.x + dist.y * dist.y;

				mindist = FLT_MAX;

				if (d0 < mindist) { mindist = d0; closestp = r1; closestq = p2; }
				if (d1 < mindist) { mindist = d1; closestp = r2; closestq = q2; }
				if (d2 < mindist) { mindist = d2; closestp = p1; closestq = r3; }
				if (d3 < mindist) { mindist = d3; closestp = q1; closestq = r4; }

				return vml::geo2d::Results::DOES_NOT_INTERSECT;

			}

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between rays

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenRays(const vml::math::vec2<T>& p, const vml::math::vec2<T>& v,
																  const vml::math::vec2<T>& q, const vml::math::vec2<T>& u,
																  vml::math::vec2<T>& closestp,
																  vml::math::vec2<T>& closestq,
																  T& mindist,
																  const T eps = vml::math::EPSILON)
			{
				T dx = q.x - p.x;
				T dy = q.y - p.y;
				T denum = v.x * u.y - v.y * u.x;
				T num = dx * v.y - dy * v.x;

				// if rays are parallel compute minimun distance

				if (denum > -eps && denum < eps)
				{
					T denum = v.x * v.x + v.y * v.y;
					if (denum > -eps && denum < eps)
						denum = eps;
					T t = num / denum;
					closestp.x = q.x - t * v.y;
					closestp.y = q.y + t * v.x;
					closestq = q;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrtf(d.x * d.x + d.y * d.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// if rays are not paralle coumpute intersection
				// point , thus the mindist is null

				closestp = p + (dx * u.y - dy * u.x) * v / denum;
				closestq = closestp;
				mindist = 0;

				return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between ray and line

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointBetweenRayAndLine(const vml::math::vec2<T>& q, const vml::math::vec2<T>& dir,
																		const vml::math::vec2<T>& a, const vml::math::vec2<T>& b,
																		vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																		T& mindist,
																		const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> r0, r1;
				T num, denum;
				T mindist0, mindist1;
				T dp0, dp1;
				T dx, dy;

				// sets initial distance as maximum

				mindist = FLT_MAX;

				// project a line point on ray

				num = (a.x - q.x) * dir.x + (a.y - q.y) * dir.y;
				r0.x = q.x + num * dir.x;
				r0.y = q.y + num * dir.y;
				dx = r0.x - a.x;
				dy = r0.y - a.y;
				mindist0 = dx * dx + dy * dy;
				dp0 = dy * dir.x - dx * dir.y;

				if (mindist0 < mindist)
				{
					mindist = mindist0;
					closestp = r0;
					closestq = a;
				}

				num = (b.x - q.x) * dir.x + (b.y - q.y) * dir.y;
				r1.x = q.x + num * dir.x;
				r1.y = q.y + num * dir.y;
				dx = r1.x - b.x;
				dy = r1.y - b.y;
				mindist1 = dx * dx + dy * dy;
				dp1 = dy * dir.x - dx * dir.y;

				if (mindist1 < mindist)
				{
					mindist = mindist1;
					closestp = r1;
					closestq = b;
				}

				mindist = sqrtf(mindist);

				// check if line crosses the ray

				if ((dp0 > -eps || dp1 < eps) && (dp0 < eps || dp1 > -eps))
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				// compute intersection

				dx = b.x - a.x;
				dy = b.y - a.y;
				num = (a.y - q.y) * dx  +( q.x - a.x) * dy;
				denum = dx * dir.y -dir.x * dy;
				if (denum > -eps && denum < eps)
					denum = eps;
				T t = num / denum;
				closestp.x = q.x + t * dir.x;
				closestp.y = q.y + t * dir.y;
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

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromPointToLine(const vml::math::vec2<T>& p,
																	  const vml::math::vec2<T>& linep, const vml::math::vec2<T>& lineq,
																	  vml::math::vec2<T>& closestp,
																	  T& mindist,
																	  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dist = lineq - linep;
				T denum = dist.x * dist.x + dist.y * dist.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1.0f / sqrtf(denum);

				vml::math::vec2<T> n(-dist.y * denum, dist.x * denum);

				// Project c onto ab, computing parameterized position d(t) = a + t*(b – a)

				T t = (linep.x - p.x) * n.x + (linep.y - p.y) * n.y;

				// intersection point

				closestp = p + t * n;

				// find if point stays on line

				T dp0 = -(linep.x - closestp.x) * n.y + (linep.y - closestp.y) * n.x;
				T dp1 = -(closestp.x - lineq.x) * n.y + (closestp.y - lineq.y) * n.x;

				// point is inside line

				if (dp0 > -eps && dp1 > -eps)
				{
					dist = p - closestp;
					mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
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
			// closest point on line from point

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromPointToRay(const vml::math::vec2<T>& p,
																	 const vml::math::vec2<T>& q, const vml::math::vec2<T>& dir,
																	 vml::math::vec2<T>& closestp,
																	 T& mindist,
																	 const T eps = vml::math::EPSILON)
			{
				// Project p onto q,dir, computing parameterized position d(t) = a + t*(b – a)

				mindist = -(q.x - p.x) * dir.y + (q.y - p.y) * dir.x;
				closestp.x = p.x - mindist * dir.y;
				closestp.y = p.y + mindist * dir.x;
				vml::math::vec2<T> dist = p - closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q in Circle b which is closest to p

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromPointToCircle(const vml::math::vec2<T>& p,
																		const vml::math::vec2<T>& c, const T r,
																		vml::math::vec2<T>& closestp,
																		T& mindist,
																	    const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = c - p;
				T a = d.x * d.x + d.y * d.y;
				T t = (a - r * sqrtf(a)) / a;
				closestp = p + t * d;
				mindist = sqrtf(t * a);
				if ( a-r*r < eps)
					return vml::geo2d::Results::INSIDE; 
			
				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q in Circle b which is closest to p

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromCircleToCircle(const vml::math::vec2<T>& c0, const T r0,
																	     const vml::math::vec2<T>& c1, const T r1,
																		 vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																		 T& mindist,
																		 const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d = c1 - c0;
				T denum = sqrt( d.y*d.y + d.x*d.x );

				// Check for solvability. 
				
				if (denum > r0 + r1)
				{
					// circle do not intersects

					vml::math::vec2<T> dir = d/denum;
					closestp = c0 + r0 * dir;
					closestq = c1 - r1 * dir;
					vml::math::vec2<T> d = closestq-closestp;
					mindist = sqrt(d.y * d.y + d.x * d.x);

					return vml::geo2d::Results::OUTSIDE;
				}

				if (denum < fabs(r0 - r1))
				{
					// circles are one inside another

					vml::math::vec2<T> dir = d / denum;
					closestp = c0 + r0 * dir;
					closestq = c1 + r1 * dir;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrt(d.y * d.y + d.x * d.x);
					T diff = denum - (r0 - r1);
					if (diff > -eps)
						return vml::geo2d::Results::FIRST_INSIDE_SECOND;
					else if (diff < eps)
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
				vml::math::vec2<T> ap = p - linep;

				T a = pq.x * pq.x + pq.y * pq.y;
				if (a > -eps && a < eps)
					a = eps;
				T b = ap.x * pq.x + ap.y * pq.y;
				T c = ap.x * ap.x + ap.y * ap.y - r * r;
				T delta = b * b - a * c;
				T t = b / a;

				if (delta > 0)
				{
					delta = sqrtf(delta) / a;
					T u = t + delta;
					T v = t - delta;
					bool c0 = u > -eps && u < (T)1 + eps;
					bool c1 = v > -eps && v < (T)1 + eps;

					if (c0 && !c1)
					{
						closestp = linep + u * pq;
						mindist = 0;
						return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT_P;
					}

					if (!c0 && c1)
					{
						closestq = linep + v * pq;
						mindist = 0;
						return vml::geo2d::Results::DOES_INTERSECT_ONE_POINT_Q;
					}

					if (c0 && c1)
					{
						closestp = linep + u * pq;
						closestq = linep + v * pq;
						vml::math::vec2<T> d = closestq - closestp;
						mindist = sqrtf(d.x * d.x + d.y * d.y);
						return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
					}
				}

				// line doe snot intersect circle, compute nearest point

				closestp.x = linep.x + t * pq.x;
				closestp.y = linep.y + t * pq.y;

				if (t > -eps && t < (T)1 + eps)
				{
					pq.x = p.x - closestp.x;
					pq.y = p.y - closestp.y;
					T denum = pq.x * pq.x + pq.y * pq.y;
					if (denum > -eps && denum < eps)
						denum = eps;
					denum = (T)1 / sqrtf(denum);
					pq.x *= denum;
					pq.y *= denum;
					closestq.x = p.x - r * pq.x;
					closestq.y = p.y - r * pq.y;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrtf(d.x * d.x + d.y * d.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				if (t < -eps && t < (T)1 + eps)
				{
					pq.x = p.x - linep.x;
					pq.y = p.y - linep.y;
					T denum = pq.x * pq.x + pq.y * pq.y;
					if (denum > -eps && denum < eps)
						denum = eps;
					denum = (T)1 / sqrtf(denum);
					pq.x *= denum;
					pq.y *= denum;
					closestq.x = p.x - r * pq.x;
					closestq.y = p.y - r * pq.y;
					closestp = linep;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrtf(d.x * d.x + d.y * d.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				if (t > -eps && t > (T)1 + eps)
				{
					pq.x = p.x - lineq.x;
					pq.y = p.y - lineq.y;
					T denum = pq.x * pq.x + pq.y * pq.y;
					if (denum > -eps && denum < eps)
						denum = eps;
					denum = (T)1 / sqrtf(denum);
					pq.x *= denum;
					pq.y *= denum;
					closestq.x = p.x - r * pq.x;
					closestq.y = p.y - r * pq.y;
					closestp = lineq;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrtf(d.x * d.x + d.y * d.y);
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// we never get here

				return vml::geo2d::Results::FAIL;
			}

			/////////////////////////////////////////////////////////////////////////////
		    // Given a ray with linep and direction dir, and a cricle p with radius r
		    // computes the closest point from circle to line
			// direction must be normalized

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromRayToCircle(const vml::math::vec2<T>& linep, const vml::math::vec2<T>& dir,
																	  const vml::math::vec2<T>& p, const T r,
																	  vml::math::vec2<T>& closestp, vml::math::vec2<T>& closestq,
																	  T& mindist,
																	  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> ap = p - linep;

				T a = dir.x * dir.x + dir.y * dir.y;
				if (a > -eps && a < eps)
					a = eps;
				T b = ap.x * dir.x + ap.y * dir.y;
				T c = ap.x * ap.x + ap.y * ap.y - r * r;
				T delta = b * b - a * c;
				T t = b / a;

				if (delta > 0)
				{
					delta = sqrtf(delta) / a;
					T u = t + delta;
					T v = t - delta;
					closestp = linep + u * dir;
					closestq = linep + v * dir;
					vml::math::vec2<T> d = closestq - closestp;
					mindist = sqrtf(d.x * d.x + d.y * d.y);

					return vml::geo2d::Results::DOES_INTERSECT_TWO_POINT;
				}

				// compute intersection point

				mindist = -(linep.x - p.x) * dir.y + (linep.y - p.y) * dir.x;

				if (mindist < -eps)
				{
					closestp.x = p.x - mindist * dir.y;
					closestp.y = p.y + mindist * dir.x;
					T denum = (T)1 / sqrtf(a);
					closestq.x = p.x + r * dir.y * denum;
					closestq.y = p.y - r * dir.x * denum;

					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				}
				else if (mindist > eps)
				{
					closestp.x = p.x - mindist * dir.y;
					closestp.y = p.y + mindist * dir.x;
					T denum = (T)1 / sqrtf(a);
					closestq.x = p.x - r * dir.y * denum;
					closestq.y = p.y + r * dir.x * denum;

					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// we never get here

				return vml::geo2d::Results::FAIL;
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
				if (result != vml::geo2d::Results::DOES_NOT_INTERSECT) {
					mindist = (T)0;
					return result;
				}

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

				vml::geo2d::distances::ClosestPointBetweenLines(linep, lineq, b0, b1, p0, q0, d0, eps);
				vml::geo2d::distances::ClosestPointBetweenLines(linep, lineq, b1, b2, p1, q1, d1, eps);
				vml::geo2d::distances::ClosestPointBetweenLines(linep, lineq, b2, b3, p2, q2, d2, eps);
				vml::geo2d::distances::ClosestPointBetweenLines(linep, lineq, b3, b0, p3, q3, d3, eps);

				// find the closest point

				T dmax = FLT_MAX;

				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }

				vml::math::vec2<T> d = closestq - closestp;
				mindist = sqrtf(d.x * d.x + d.y * d.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
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
				if (result != vml::geo2d::Results::DOES_NOT_INTERSECT){
					mindist = (T)0;
					return result;
				}
		
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

				ClosestPointBetweenRayAndLine(linep, dir, b0, b1, p0, q0, d0, eps);
				ClosestPointBetweenRayAndLine(linep, dir, b1, b2, p1, q1, d1, eps);
				ClosestPointBetweenRayAndLine(linep, dir, b2, b3, p2, q2, d2, eps);
				ClosestPointBetweenRayAndLine(linep, dir, b3, b0, p3, q3, d3, eps);

				// find the closest point

				T dmax = FLT_MAX;

				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }

				vml::math::vec2<T> d = closestq - closestp;
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
					if (closestp.x <= bmin.x) closestp.x = bmin.x;
					if (closestp.x >= bmax.x) closestp.x = bmax.x;
					if (closestp.y <= bmin.y) closestp.y = bmin.y;
					if (closestp.y >= bmax.y) closestp.y = bmax.y;
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
																		 const vml::math::vec2<T>& p, const T r,
																		 std::vector <vml::math::vec2<T>>& points,
																		 T& mindist,
																		 const T eps = vml::math::EPSILON)
			{
				// clear points array


				T dx, dy;
				T cx, cy;
				T a, b, c;
				T delta;
				T t0, t1;
				size_t numpoints;

				points.clear();
				mindist = (T)0.0;

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

				// there are intersection points

				if (numpoints != 0)
					return vml::geo2d::Results::DOES_INTERSECT;

				// if we get here, there is no intersection
				// but we meed to know if circle is entirely 
				// inside the rectangle

				// check if the second bounding box is entirely inside the first bounding box

				if ((p.x - r >= bmin.x && p.x + r <= bmax.x) &&
					(p.y - r >= bmin.y && p.y + r <= bmax.y))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				if ((bmin.x >= p.x - r && bmax.x <= p.x + r) &&
					(bmin.y >= p.y - r && bmax.y <= p.y + r))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				// if cirlce isn't contained in the rectangle, 
				// then compute closest points

				vml::math::vec2<T> closestp;
				vml::math::vec2<T> closestq;

				closestp = p;

				// compute closest point on box from circle center

				if (closestp.x <= bmin.x) closestp.x = bmin.x;
				if (closestp.x >= bmax.x) closestp.x = bmax.x;
				if (closestp.y <= bmin.y) closestp.y = bmin.y;
				if (closestp.y >= bmax.y) closestp.y = bmax.y;
				vml::math::vec2<T> dist = closestp - p;
				T denum = sqrtf(dist.x * dist.x + dist.y * dist.y);
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
				T t = r * denum;
				closestq = p + t * dist;

				// compute minimum distance

				points.emplace_back(closestp);
				points.emplace_back(closestq);
				dist = closestq - closestp;
				mindist = sqrtf(dist.x * dist.x + dist.y * dist.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between an aabbox and another aabbx

			template <typename T>
			static [[nodiscard]] uint32_t ClosestPointFromAABBToAABB(const vml::math::vec2<T>& amin, const vml::math::vec2<T>& amax,
																	const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax,
																	std::vector<vml::math::vec2<T>>& points,
																	T& mindist,
																	const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> pa(amin.x, amin.y);
				vml::math::vec2<T> pb(amax.x, amin.y);
				vml::math::vec2<T> pc(amax.x, amax.y);
				vml::math::vec2<T> pd(amin.x, amax.y);

				vml::math::vec2<T> qa(bmin.x, bmin.y);
				vml::math::vec2<T> qb(bmax.x, bmin.y);
				vml::math::vec2<T> qc(bmax.x, bmax.y);
				vml::math::vec2<T> qd(bmin.x, bmax.y);

				// clear points array

				points.clear();
				mindist = (T)0.0;

				// clip against box B vertical lines

				if ((qb.x - pa.x > -eps && qb.x - pb.x < eps) &&
					(pa.y - qa.y > -eps && pa.y - qd.y < eps))
				{
					points.emplace_back(vml::math::vec2<T>(qb.x, pa.y));
				}

				if ((qb.x - pd.x > -eps && qb.x - pc.x < eps) &&
					(pd.y - qa.y > -eps && pd.y - qd.y < eps))
				{
					points.emplace_back(vml::math::vec2<T>(qb.x, pd.y));
				}

				if ((qa.x - pa.x > -eps && qa.x - pb.x < eps) &&
					(pa.y - qa.y > -eps && pa.y - qd.y < eps))
				{
					points.emplace_back(vml::math::vec2<T>(qa.x, pa.y));
				}

				if ((qa.x - pd.x > -eps && qa.x - pc.x < eps) &&
					(pd.y - qa.y > -eps && pd.y - qd.y < eps))
				{
					points.emplace_back(vml::math::vec2<T>(qa.x, pd.y));
				}

				// clip against box B horizonthal lines

				if ((pc.y - qa.y > -eps && pb.y - qa.y < eps) &&
					(pc.x - qa.x > -eps && pc.x - qb.x < eps))
				{
					points.emplace_back(vml::math::vec2<T>(pc.x, qa.y));
				}

				if ((pd.y - qa.y > -eps && pa.y - qa.y < eps) &&
					(pd.x - qa.x > -eps && pd.x - qb.x < eps))
				{
					points.emplace_back(vml::math::vec2<T>(pd.x, qa.y));
				}

				if ((pc.y - qd.y > -eps && pb.y - qd.y < eps) &&
					(pc.x - qa.x > -eps && pc.x - qb.x < eps))
				{
					points.emplace_back(vml::math::vec2<T>(pc.x, qd.y));
				}

				if ((pd.y - qd.y > -eps && pa.y - qd.y < eps) &&
					(pd.x - qa.x > -eps && pd.x - qb.x < eps))
				{
					points.emplace_back(vml::math::vec2<T>(pd.x, qd.y));
				}

				// there are intersection points

				if (points.size() != 0)
					return vml::geo2d::Results::DOES_INTERSECT;

				// if we get here, there is no intersection
				// but we meed to know if first rectangle is entirely 
				// inside the other rectangle

				// check if the first bounding box is entirely inside the second bounding box

				if ((pa.x >= qa.x && pc.x <= qc.x) &&
					(pa.y >= qa.y && pc.y <= qc.y))
					return vml::geo2d::Results::FIRST_INSIDE_SECOND;

				// check if the second bounding box is entirely inside the first bounding box

				if ((qa.x >= pa.x && qc.x <= pc.x) &&
					(qa.y >= pa.y && qc.y <= pc.y))
					return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				// compute closest points

				vml::math::vec2<T> closestp, closestq;

				// for X

				if (pc.x < qa.x)
				{
					// A is left of B

					closestp.x = pc.x;
					closestq.x = qa.x;
				}
				else if (qc.x < pa.x)
				{
					// B is left of A

					closestp.x = pa.x;
					closestq.x = qc.x;
				}
				else
				{
					// Overlapping in x, choose clamped value
					// find minimum x

					T xOverlap = pa.x;
					if (qa.x > xOverlap) xOverlap = qa.x;

					// clamp to x axis a bounding box extents

					T minx = xOverlap;
					if (pc.x < minx) minx = xOverlap;
					T maxx = pa.x;
					if (minx > maxx) maxx = minx;

					closestp.x = maxx;
					closestq.x = maxx;
				}

				// for Y

				if (pc.y < qa.y)
				{
					// A is below B

					closestp.y = pc.y;
					closestq.y = qa.y;
				}
				else if (qc.y < pa.y)
				{
					// B is below A

					closestp.y = pa.y;
					closestq.y = qc.y;
				}
				else
				{
					// Overlapping in y, choose clamped value
					// find minimum y

					T yOverlap = pa.y;
					if (qa.y > yOverlap) yOverlap = qa.y;

					// clamp to y axis a bounding box extents

					T miny = yOverlap;
					if (pc.y < miny) miny = yOverlap;
					T maxy = pa.y;
					if (miny > maxy) maxy = miny;

					closestp.y = maxy;
					closestq.y = maxy;
				}

				// add closest points to the points array

				points.emplace_back(closestp);
				points.emplace_back(closestq);

				// compute minimum distance

				vml::math::vec2<T> d = closestq - closestp;
				mindist = sqrtf(d.x * d.x + d.y * d.y);

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/*
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

				// computer inverse matrix
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
			*/
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

