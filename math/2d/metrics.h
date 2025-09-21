#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vector metrics

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// --------------------------------------------------------
			// Calculates reflection vector from entering ray direction 'v'
			// and surface normal 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Reflect(const vml::math::vec2<T>& v, 
				                                            const vml::math::vec2<T>& n,
				                                            const T eps=vml::math::EPSILON)
			{
				T d = n.x * n.x + n.y * n.y;
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / sqrtf(d);
				T nx = n.x * d;
				T ny = n.y * d;
				T num = T(2) * (v.x * nx + v.y * ny);
				return vec2<T>(v.x - num * nx, v.y - num * ny);
			}

			// --------------------------------------------------------
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.

			template <typename T>
			static void Orthogonalize(const vml::math::vec2<T>& v, 
				                      const vml::math::vec2<T>& w, 
				                      vml::math::vec2<T>& pv1, 
				                      vml::math::vec2<T>& pv2, 
				                      const T eps = vml::math::EPSILON)
			{
				// v is the basis vector normalized

				pv1 = v;
				T d = pv1.x * pv1.x + pv1.y * pv1.y;
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / sqrtf(d);
				pv1.x *= d;
				pv1.y *= d;

				// compute w-Perpendicular(w,v)

				d = v.x * v.x + v.y * v.y;
				if (d > -eps && d < eps)
					d = eps;
				T num = (v.x * w.x + v.y * w.y) / d;
				pv2 = w - num * v;

				// normalize resulting vector

				d = pv2.x * pv2.x + pv2.y * pv2.y;
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / sqrtf(d);
				pv2.x *= d;
				pv2.y *= d;
			}

			// --------------------------------------------------------
			// Calculates the component of 'v' perpendicular to 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Perpendicular(const vml::math::vec2<T>& v, 
				                                                  const vml::math::vec2<T>& w, 
				                                                  const T eps = vml::math::EPSILON)
			{
				T d = w.x * w.x + w.y * w.y;
				if (d > -eps && d < eps)
					d = eps;
				T num = (v.x * w.x + v.y * w.y) / d;
				return v - num * w;
			}

			// --------------------------------------------------------
			// reciprocal of a vector 

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Reciprocal(const vml::math::vec2<T>& v, 
				                                               const T eps = vml::math::EPSILON)
			{
				if (v.x > -eps && v.x < eps) v.x = eps;
				if (v.y > -eps && v.y < eps) v.y = eps;
				return vec2<T>(T(1) / v.x, T(1) / v.y);
			}

			// --------------------------------------------------------
			// Calculates the projection of 'v' onto 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Project(const vml::math::vec2<T>& v, 
				                                            const vml::math::vec2<T>& w, 
				                                            const T eps = vml::math::EPSILON)
			{
				T d = w.x * w.x + w.y * w.y;
				if (d > -eps && d < eps)
					d = eps;
				T num = (v.x * w.x + v.y * w.y) / d;
				return num * w;
			}

			// --------------------------------------------------------
			// center value

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Mid(const vml::math::vec2<T>& v,
														const vml::math::vec2<T>& w)
			{
				return vec2<T>((v.x + w.x) * T(0.5), (v.y + w.y) * T(0.5));
			}

			// --------------------------------------------------------
			// Linera interpolation

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Lerp(const vml::math::vec2<T>& v,
														 const vml::math::vec2<T>& w,
														 const T s)
			{
				return vec2<T>(v.x + (w.x - v.x) * s, v.y + (w.y - v.y) * s);
			}

			// --------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Saturate(const vml::math::vec2<T>& v, 
				                                             const T eps = vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				if (x > T(1) - eps) x = T(1);
				if (y > T(1) - eps) y = T(1);
				if (x < T(0) + eps) x = T(0);
				if (y < T(0) + eps) y = T(0);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// Saturate to zero

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> SaturateToZero(const vml::math::vec2<T>& v, 
				                                                   const T eps = vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				if (x < T(0) + eps) x = T(0);
				if (y < T(0) + eps) y = T(0);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// Saturate a vector to one

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> SaturateToOne(const vml::math::vec2<T>& v, 
				                                                  const T eps = vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				if (x > T(1) - eps) x = T(1);
				if (y > T(1) - eps) y = T(1);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// floors a vector

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Floor(const vml::math::vec2<T>& v)
			{
				return vec2<T>(floor(v.x), floor(v.y));
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Ceil(const vml::math::vec2<T>& v)
			{
				return vec2<T>(ceil(v.x), ceil(v.y));
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

///////////////////////////////////////////////////////////////////////
// Line metrics

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// compute line length

			template<class T>
			static [[nodiscard]] float LineLength(const vml::math::vec2<T>& p0, 
				                                  const vml::math::vec2<T>& p1, 
				                                  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dir = p1 - p0;
				T denum = dir.x * dir.x + dir.y * dir.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				return sqrtf(denum);
			}

			// ------------------------------------------------------------------------
			// compute squared line length

			template<class T>
			static [[nodiscard]] float SquaredLineLength(const vml::math::vec2<T>& p0, 
				                                         const vml::math::vec2<T>& p1, 
				                                         const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dir = p1 - p0;
				T denum = dir.x * dir.x + dir.y * dir.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				return denum;
			}

			// ------------------------------------------------------------------------
			// compute direction vector given two points

			template<class T>
			static [[nodiscard]] vml::math::vec2<T> LineDirection(const vml::math::vec2<T>& p0, 
				                                                  const vml::math::vec2<T>& p1, 
				                                                  const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dir = p1 - p0;
				T denum = sqrtf(dir.x * dir.x + dir.y * dir.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = T(1) / denum;
				return vml::math::vec2<T>(dir.x * denum, dir.y * denum);
			}

			// ------------------------------------------------------------------------
			// returns normal to a direction, ray input vector must be normalized

			template<class T>
			static [[nodiscard]] vml::math::vec2<T> LineNormal(const vml::math::vec2<T>& p0, 
				                                               const vml::math::vec2<T>& p1, 
				                                               const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> ray = p1 - p0;
				T denum = sqrtf(ray.x * ray.x + ray.y * ray.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = T(1) / denum;
				return vml::math::vec2<T>(-ray.y * denum, ray.x * denum);
			}

			// ------------------------------------------------------------------------
			// returns ray slope

			template<class T>
			static [[nodiscard]] float LineSlope(const vml::math::vec2<T>& p0, 
				                                 const vml::math::vec2<T>& p1, 
				                                 const T eps = vml::math::EPSILON)
			{
				return vml::math::angles::Atan2(p1.x - p0.x, p1.y - p0.y, eps) / vml::math::PI * 180.0f + 180.0f;
			}

			// ------------------------------------------------------------------------
			// line center

			template<class T>
			static [[nodiscard]] vml::math::vec2<T> LineCenter(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax)
			{
				return vml::math::vec2<T>((bmin.x + bmax.x) * 0.5f, (bmin.y + bmax.y) * 0.5f);
			}

			// ------------------------------------------------------------------------
			//  computes the angle in radians swept out between two rays in 2D
			//  a,b,c define the rays
			//  a-b and  c-b which in turn define the
			//  the origin point is a
			//  angle, counterclockwise from  b-a 
			//  the returned value is in radians int the range  0 <= angle < 2 PI
			//  if either ray has zero length, then result is 0

			template<class T>
			static [[nodiscard]] float AngleBetweenLines(const vml::math::vec2<T>& a,
														 const vml::math::vec2<T>& b,
														 const vml::math::vec2<T>& c,
														 const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d1 = a - b;
				vml::math::vec2<T> d2 = c - b;
				T x = d1.x * d2.x + d1.y * d2.y;
				T y = d1.x * d2.y - d1.y * d2.x;
				if (x >= -eps && x <= eps) x = eps;
				return vml::math::angles::Atan2(y, x, eps);
			}

			// ------------------------------------------------------------------------
			// returns whcih side the point is respect a line of p1 and p2 extremes

			template<class T>
			static [[nodiscard]] uint32_t OrientationBetweenLineAndPoint(const vml::math::vec2<T>& p1,
																		 const vml::math::vec2<T>& p2,
																		 const vml::math::vec2<T>& p,
																		 const T eps = vml::math::EPSILON)
			{
				T dp = (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
				if (dp > -eps) return vml::geo2d::Results::LEFT;						// Orientaion is to the left-hand side  
				else if (dp < eps) return vml::geo2d::Results::RIGHT;				// Orientaion is to the right-hand side 
				return vml::geo2d::Results::COLLINEAR;									// Orientaion is neutral aka collinear  
			}

			// ------------------------------------------------------------------------
			// Given a float distance, get point along line

			template<class T>
			static [[nodiscard]] vml::math::vec2<T> PointOnLine(const vml::math::vec2<T>& p0, 
				                                                const vml::math::vec2<T>& p1, 
				                                                const T distance, 
				                                                const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> dir = p1 - p0;
				T denum = sqrtf(dir.x * dir.x + dir.y * dir.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1.0f / denum;
				dir.x *= denum;
				dir.y *= denum;
				return p0 + distance * dir;
			}

			// --------------------------------------------------------
			// returns the distance from two vectors

			template <typename T>
			static [[nodiscard]] float DistanceFrom(const vml::math::vec2<T>& v, const vml::math::vec2<T>& w)
			{
				T dx = v.x - w.x;
				T dy = v.y - w.y;
				return sqrtf(dx * dx + dy * dy);
			}

			// --------------------------------------------------------
			// return the trace of a given vector

			template <typename T>
			static [[nodiscard]] float Trace(const vml::math::vec2<T>& v)
			{
				return v.x + v.y;
			}

			// --------------------------------------------------------
			// transpose a vector

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> Transpose(const vml::math::vec2<T>& v)
			{
				return vml::math::vec2<T>(v.y, v.x);
			}

			// --------------------------------------------------------
			// return the angle between two vectors

			template <typename T>
			static [[nodiscard]] float AngleBetween(const vml::math::vec2<T>& v, 
				                                    const vml::math::vec2<T>& w, 
				                                    const T eps = vml::math::EPSILON)
			{
				T num = v.x * w.x + v.y * w.y;
				T r = num;
				T denum = sqrtf(v.x * v.x + v.y * v.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				r /= denum;
				denum = sqrtf(w.x * w.x + w.y * w.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				r /= denum;
				if (r < -T(1)) r = -T(1);
				if (r >  T(1)) r =  T(1);
				return acos(r);
			}

			// --------------------------------------------------------
			// return the atan ( y/x ) in radians

			template <typename T>
			static [[nodiscard]] float Angle(const vml::math::vec2<T>& v,
				                         const T eps = vml::math::EPSILON)
			{
				if (v.x > -eps && v.x < eps)
					return atan2(v.y, eps);
				return atan2(v.y, v.x);
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T>
			static [[nodiscard]] float GetMax(const vml::math::vec2<T>& v)
			{
				return v.x > v.y ? v.x : v.y;
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			static [[nodiscard]] float GetAbsMax(const vml::math::vec2<T>& v)
			{
				return fabs(v.x) > fabs(v.y) ? fabs(v.x) : fabs(v.y);
			}

			// --------------------------------------------------------
			// gets the minimum value component of a given vector

			template <typename T>
			static [[nodiscard]] float GetMin(const vml::math::vec2<T>& v)
			{
				return v.x < v.y ? v.x : v.y;
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			static [[nodiscard]] float GetAbsMin(const vml::math::vec2<T>& v)
			{
				return fabs(v.x) < fabs(v.y) ? fabs(v.x) : fabs(v.y);
			}

			// --------------------------------------------------------------------
			// check if vector is nearly zero

			template <typename T>
			static [[nodiscard]] bool IsZero(const vml::math::vec2<T>& v,
				                             const T eps = vml::math::EPSILON)
			{
				if ((v.x > -eps && v.x < eps) &&
					(v.y > -eps && v.y < eps))
					return true;
				return false;
			}

			// --------------------------------------------------------------------
			// checks if vector is nearly identticaly to (1,1)

			template <typename T>
			static [[nodiscard]] bool IsIdentity(const vml::math::vec2<T>& v,
				                                 const T eps = vml::math::EPSILON)
			{
				if ((v.x > T(1) -eps && v.x < T(1) + eps) &&
					(v.y > T(1) -eps && v.y < T(1) + eps))
					return true;
				return false;
			}

			// --------------------------------------------------------------------
			// checks if two vectors are nearly equal

			template <typename T>
			static [[nodiscard]] bool IsEqualTo(const vml::math::vec2<T>& v, 
				                                const vml::math::vec2<T>& w, 
				                                const T eps = vml::math::EPSILON)
			{
				return fabs(v.x - w.x) < eps &&	fabs(v.y - w.y) < eps;
			}

			// --------------------------------------------------------------------
			// checks if two vectors are not nearly equal

			template <typename T>
			static [[nodiscard]] bool IsNotEqualTo(const vml::math::vec2<T>& v, 
				                                   const vml::math::vec2<T>& w, 
				                                   const T eps = vml::math::EPSILON)
			{
				return fabs(v.x - w.x) > eps ||	fabs(v.y - w.y) > eps;
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////
// Ray metrics

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// returns normal to a direction, ray input vector must be normalized

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> RayNormal(const vml::math::vec2<T>& p0, 
				                                              const vml::math::vec2<T>& n)
			{
				return vml::math::vec2<T>(-n.y, n.x);
			}
			
			// ------------------------------------------------------------------------
			// returns ray slope

			template <typename T>
			static [[nodiscard]] float RaySlope(const vml::math::vec2<T>& n)
			{
				return vml::math::angles::Atan2(n.x, n.y, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
			}
			
			// ------------------------------------------------------------------------
			// returns whcih side the point is respect a line of p1 and p2 extremes

			template <typename T>
			static [[nodiscard]] uint32_t OrientationBetweenRayAndPoint(const vml::math::vec2<T>& p1,
																	    const vml::math::vec2<T>& n,
																	    const vml::math::vec2<T>& p, 
																        const T eps = vml::math::EPSILON)
			{
				float dp = n.x * (p.y - p1.y) - (p.x - p1.x) * n.y;
				if (dp > -eps) return vml::geo2d::Results::LEFT;			// Orientaion is to the left-hand side  
				else if (dp < eps) return vml::geo2d::Results::RIGHT;	// Orientaion is to the right-hand side 
				return vml::geo2d::Results::COLLINEAR;						// Orientaion is neutral aka collinear  
			}
			
			// ------------------------------------------------------------------------
			//  computes the angle in radians swept out between two rays in 2D
			//  a,b,c define the rays
			//  a-b and  c-b which in turn define the
			//  the origin point is a
			//  angle, counterclockwise from  b-a 
			//  the returned value is in radians int the range  0 <= angle < 2 PI
			//  if either ray has zero length, then result is 0

			template <typename T>
			static [[nodiscard]] float AngleBetweenRays(const vml::math::vec2<T>& a,
													    const vml::math::vec2<T>& b,
													    const vml::math::vec2<T>& c,
													    const T eps = vml::math::EPSILON)
			{
				vml::math::vec2<T> d1 = a - b;
				vml::math::vec2<T> d2 = c - b;
				float x = d1.x * d2.x + d1.y * d2.y;
				float y = d1.x * d2.y - d1.y * d2.x;
				if (x >= -eps && x <= eps) x = eps;
				return vml::math::angles::Atan2(y, x, eps);
			}

			// ------------------------------------------------------------------------
			// Given a float distance, get point along ray, ray normal must be normalized
			
			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> PointOnRay(const vml::math::vec2<T>& p0, const vml::math::vec2<T>& n, const float distance)
			{
				return p0 + distance * n;
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml nam(T)epsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Circle

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// Given a circle pos and radius, compute perimeter

			template <typename T>
			static [[nodiscard]] float GetCirclePerimeter(const vml::math::vec2<T>& p, const float r)
			{
				return 2.0f*vml::math::PI*r;
			}

			// ------------------------------------------------------------------------
			// Given a circle pos and radius, compute area

			template <typename T>
			static [[nodiscard]] float GetCircleArea(const vml::math::vec2<T>& p, const float r)
			{
				return  vml::math::PI * r * r;
			}

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Axis aligned bounding box

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// AABB center

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAABBCenter(const vml::math::vec2<T> &bmin, const vml::math::vec2<T>& bmax )
			{
				return vml::math::vec2<T>( (bmin.x + bmax.x) * 0.5f, (bmin.y + bmax.y) * 0.5f);
			}

			// ------------------------------------------------------------------------
			// AABB extents

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAABBExtents(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax)
			{
				return vml::math::vec2<T>(fabs(bmax.x - bmin.x), fabs(bmax.y - bmin.y));
			}

			// ------------------------------------------------------------------------
			// AABB half extents

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAABBHalfExtents(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax)
			{
				return vml::math::vec2<T>(fabs(bmax.x - bmin.x) * 0.5f, fabs(bmax.y - bmin.y) * 0.5f);
			}

			// ------------------------------------------------------------------------
			// AABB area

			template <typename T>
			static [[nodiscard]] float GetAABBArea(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax)
			{
				return fabs((bmax.x - bmin.x) * (bmax.y - bmin.y));
			}

			// ------------------------------------------------------------------------
			// AABB perimeter

			template <typename T>
			static [[nodiscard]] float GetAABBPerimeter(const vml::math::vec2<T>& bmin, const vml::math::vec2<T>& bmax)
			{
				return 2.0f*fabs((bmax.x - bmin.x) + (bmax.y - bmin.y));
			}

		} // end of namespace metrics
		
	} // end of geo2d namespace

} // end of vml nam(T)epsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// axis oriented bounding box

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{
			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAOBBCenter(const vml::geo2d::AOBB<T>& aobb)
			{
				return vml::math::vec2<T>((aobb.P1.x + aobb.P2.x + aobb.P3.x + aobb.P4.x) * 0.25f, (aobb.P1.y + aobb.P2.y + aobb.P3.y + aobb.P4.y) * 0.25f);
			}
			
			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAOBBExtents(const vml::geo2d::AOBB<T>& aobb)
			{
				return vml::math::vec2<T>( vml::math::Length(aobb.P2- aobb.P1), vml::math::Length(aobb.P4- aobb.P1));
			}
			
			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAOBBHalfExtents(const vml::geo2d::AOBB<T>& aobb)
			{
				return vml::math::vec2<T>( vml::math::Length(aobb.P2- aobb.P1), vml::math::Length(aobb.P4- aobb.P1))*0.5f;
			}

			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] float GetAOBBArea(const vml::geo2d::AOBB<T>& aobb)
			{
				return vml::math::Length(aobb.P2 - aobb.P1) * vml::math::Length(aobb.P4 - aobb.P1);
			}

			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] float GetAOBBPerimeter(const vml::geo2d::AOBB<T>& aobb)
			{
				return (vml::math::Length(aobb.P2 - aobb.P1) + vml::math::Length(aobb.P4 - aobb.P1))*2.0f;
			}

			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] void GetAOBBDirectionVectors(const vml::math::vec2<T>& p0, 
				                                              const vml::math::vec2<T>& p1, 
				                                              const vml::math::vec2<T>& p2, 
				                                              const vml::math::vec2<T>& p3,
															  vml::math::vec2<T>& u, 
				                                              vml::math::vec2<T>& v, 
															  const T eps = vml::math::EPSILON)
			{
				u = p1 - p0;
				float denumu = sqrtf(u.x * u.x + u.y * u.y);
				if (denumu > -eps && denumu < eps)
					denumu = eps;
				denumu = 1.0f / denumu;
				u.x *= denumu;
				u.y *= denumu;

				v = p3 - p0;
				float denumv = sqrtf(v.x * v.x + v.y * v.y);
				if (denumv > -eps && denumv < eps)
					denumv = eps;
				denumv = 1.0f / denumv;
				v.x *= denumv;
				v.y *= denumv;
			}

			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetAABBoxExtremaPointAlongDirection(const vml::math::vec2<T>& min, 
				                                                                        const vml::math::vec2<T>& max, 
				                                                                        const glm::vec3& direction)
			{
				return vml::math::vec2<T>((direction.x >= 0.f ? max.x : min.x), (direction.y >= 0.f ? max.y : min.y));
			}

		} // end of namespace metrics
		
	} // end of geo2d namespace

} // end of vml nam(T)epsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{
			
			// -------------------------------------------------------------------------------------------------
			// area of a triangle

			template <typename T>
			static float [[nodiscard]] GetTriangleArea(const vml::math::vec2<T> &a, 
				                                       const vml::math::vec2<T> &b, 
				                                       const vml::math::vec2<T> &c)
			{
				return fabs((c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x)) *0.5f;
			}

			// -------------------------------------------------------------------------------------------------
			// signed triangel area

			template <typename T>
			static float [[nodiscard]] GetSignedTriangleArea(const vml::math::vec2<T> &a, 
				                                             const vml::math::vec2<T> &b, 
				                                             const vml::math::vec2<T> &c)
			{
				return ((c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x)) *0.5f;
			}

			// ------------------------------------------------------------------------

			template <typename T>
			static float [[nodiscard]] GetTrianglePerimeter(const vml::math::vec2<T>& a, 
				                                            const vml::math::vec2<T>& b, 
				                                            const vml::math::vec2<T>& c)
			{
				return vml::math::Length(b - a) + vml::math::Length(c - b) + vml::math::Length(a - c);
			}

			// -------------------------------------------------------------------------------------------------
			// computes triangle centroid

			template <typename T>
			static [[nodiscard]] vml::math::vec2<T> GetTriangleCentroid(const vml::math::vec2<T> &a, 
				                                                        const vml::math::vec2<T> &b, 
				                                                        const vml::math::vec2<T> &c)
			{
				return vml::math::vec2<T>((a.x + b.x + c.x)*0.33333f, (a.y + b.y + c.y)*0.33333f);
			}

			// -------------------------------------------------------------------------------------------------
			// computes if triangle is ccw

			template <typename T>
			static [[nodiscard]] bool IsTtriangleCCW(const vml::math::vec2<T> &a, 
				                                     const vml::math::vec2<T> &b, 
				                                     const vml::math::vec2<T> &c)
			{
				return (c.y - a.y)*(b.x - a.x) > (b.y - a.y)*(c.x - a.x);
			}
						
		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml nam(T)epsace

/*

	template <typename T>
	constexpr int sgn(T val) { return (T(0) < val) - (val < T(0)); }

	template<typename T>
	struct polygon
	{
		std::vector<olc::v_2d<T>> pos;
	};

	// =========================================================================================================================
	// Closest(shape, point) ===================================================================================================

	// closest(t,p)
	// Returns closest point on triangle to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		olc::utils::geom2d::line<T1> l{ t.pos[0], t.pos[1] };
		auto p0 = closest(l, p);
		auto d0 = (p0 - p).mag2();

		l.end = t.pos[2];
		auto p1 = closest(l, p);
		auto d1 = (p1 - p).mag2();

		l.start = t.pos[1];
		auto p2 = closest(l, p);
		auto d2 = (p2 - p).mag2();

		if ((d0 <= d1) && (d0 <= d2)) {
			return p0;
		}
		else if ((d1 <= d0) && (d1 <= d2)) {
			return p1;
		}
		else {
			return p2;
		}
	}

	// Closest location on [SHAPE] to Line

	// ================================================================================================================
	// POINT ==========================================================================================================

	// contains(l,p)
	// Checks if line contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const olc::v_2d<T2>& p)
	{
		double d = ((p.x - l.start.x) * (l.end.y - l.start.y) - (p.y - l.start.y) * (l.end.x - l.start.x));
		if (std::abs(d) < epsilon)
		{
			// point is on line
			double u = l.vector().dot(p - l.start) / l.vector().mag2();
			return (u >= double(0.0) && u <= double(1.0));
		}

		return false;
	}

	// contains(r,p)
	// Checks if rectangle contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		return !(p.x < r.pos.x || p.y < r.pos.y ||
			p.x > (r.pos.x + r.size.x) || p.y > (r.pos.y + r.size.y));
	}

	// contains(c,p)
	// Checks if circle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		return (c.pos - p).mag2() <= (c.radius * c.radius);
	}

	// contains(t,p)
	// Checks if triangle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		// http://jsfiddle.net/PerroAZUL/zdaY8/1/
		T2 A = T2(0.5) * (-t.pos[1].y * t.pos[2].x + t.pos[0].y * (-t.pos[1].x + t.pos[2].x) + t.pos[0].x * (t.pos[1].y - t.pos[2].y) + t.pos[1].x * t.pos[2].y);
		T2 sign = A < T2(0) ? T2(-1) : T2(1);
		T2 s = (t.pos[0].y * t.pos[2].x - t.pos[0].x * t.pos[2].y + (t.pos[2].y - t.pos[0].y) * p.x + (t.pos[0].x - t.pos[2].x) * p.y) * sign;
		T2 v = (t.pos[0].x * t.pos[1].y - t.pos[0].y * t.pos[1].x + (t.pos[0].y - t.pos[1].y) * p.x + (t.pos[1].x - t.pos[0].x) * p.y) * sign;
		return s >= T2(0) && v >= T2(0) && (s + v) <= T2(2) * A * sign;
	}

	template<typename T1, typename T2>
	inline constexpr bool contains(const ray<T1>& r, const olc::v_2d<T2>& p)
	{
		// Calculate the vector from the ray's origin to point p
		olc::v_2d<T2> op = p - r.origin;

		// Calculate the dot product between op and the ray's direction
		// This checks if p is in the direction of the ray and not behind the origin
		T2 dotProduct = op.dot(r.direction);

		if (dotProduct < 0) {
			// p is behind the ray's origin
			return false;
		}

		// Project op onto the ray's direction (which is already normalized)
		olc::v_2d<T2> projection = { r.direction.x * dotProduct, r.direction.y * dotProduct };

		// Check if the projection of op onto the ray's direction is equivalent to op
		// This is true if p lies on the ray

		T2 distance = std::sqrt((projection.x - op.x) * (projection.x - op.x) + (projection.y - op.y) * (projection.y - op.y));

		// Assuming a small threshold for floating point arithmetic issues
		return distance < epsilon;
	}

	// overlaps(l,p)
	// Checks if line segment overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const olc::v_2d<T2>& p)
	{
		return contains(l, p);
	}

	// overlaps(r,p)
	// Checks if rectangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		return contains(r, p);
	}

	// overlaps(c,p)
	// Checks if circle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		return contains(c, p);
	}

	// overlaps(t,p)
	// Checks if triangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		return contains(t, p);
	}

	// intersects(r,p)
	// Get intersection points where rectangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		for (size_t i = 0; i < r.side_count(); i++)
			if (contains(r.side(i), p))
				return { p };

		return {};
	}

	// intersects(c,p)
	// Get intersection points where circle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		if (std::abs((p - c.pos).mag2() - (c.radius * c.radius)) <= epsilon)
			return { p };

		return {};
	}

	// intersects(t,p)
	// Get intersection points where triangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		for (size_t i = 0; i < t.side_count(); i++)
			if (contains(t.side(i), p))
				return { p };

		return {};

	}

	// ================================================================================================================
	// LINE ===========================================================================================================

	// contains(l,l)
	// Check if line segment contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l1, const line<T2>& l2)
	{
		return overlaps(l1, l2.start) && overlaps(l1, l2.end);
	}

	// contains(r,l)
	// Check if rectangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start) && contains(r, l.end);
	}

	// contains(c,l)
	// Check if circle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const line<T2>& l)
	{
		return contains(c1, l.start) && contains(c1, l.end);
	}

	// contains(t,l)
	// Check if triangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const line<T2>& l)
	{
		return contains(t, l.start) && contains(t, l.end);
	}

	// overlaps(p,l)
	// Check if point overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const line<T2>& l)
	{
		return contains(l, p);
	}

	// overlaps(l,l)
	// Check if line segment overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l1, const line<T2>& l2)
	{
		double D = ((l2.end.y - l2.start.y) * (l1.end.x - l1.start.x) - (l2.end.x - l2.start.x) * (l1.end.y - l1.start.y));
		double uA = ((l2.end.x-l2.start.x)*(l1.start.y-l2.start.y) - (l2.end.y-l2.start.y)*(l1.start.x-l2.start.x)) / D;
		double uB = ((l1.end.x-l1.start.x)*(l1.start.y-l2.start.y) - (l1.end.y-l1.start.y)*(l1.start.x-l2.start.x)) / D;
		return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
	}

	// overlaps(r,l)
	// Check if rectangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start)
			|| overlaps(r.top(), l)
			|| overlaps(r.bottom(), l)
			|| overlaps(r.left(), l)
			|| overlaps(r.right(), l);
	}

	// overlaps(c,l)
	// Check if circle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const line<T2>& l)
	{
		auto vClosest = closest(l, c.pos);
		return ((c.pos - vClosest).mag2() <= (c.radius * c.radius));
	}

	// overlaps(t,l)
	// Check if triangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const line<T2>& l)
	{
		return overlaps(t, l.start) || overlaps(t.side(0), l) || overlaps(t.side(1), l) || overlaps(t.side(2), l);
	}

	// intersects(p,l)
	// Get intersection points where point intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const line<T2>& l)
	{
		return intersects(l, p);
	}

	// intersects(l,l)
	// Get intersection points where line segment intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l1, const line<T2>& l2, bool infinite = false)
	{
		if (infinite)
		{
			float rd = l1.vector().cross(l2.vector());
			if (rd == 0) return {}; // Parallel or Colinear TODO: Return two points

			//Inverse rd product
			rd = 1.f / rd;

			//Cross products:
			//rn = (b1b2 x b1a1)
			float rn = ((l2.end.x - l2.start.x) * (l1.start.y - l2.start.y) - (l2.end.y - l2.start.y) * (l1.start.x - l2.start.x)) * rd;
			//sn = (a1a2 x b1a1)
			float sn = ((l1.end.x - l1.start.x) * (l1.start.y - l2.start.y) - (l1.end.y - l1.start.y) * (l1.start.x - l2.start.x)) * rd;

			//Return the intersection depth
			//if (d) *d = rn;

			return { l1.start + rn * l1.vector() };
		}
		else
		{
			float rd = l1.vector().cross(l2.vector());
			if (rd == 0) return {}; // Parallel or Colinear TODO: Return two points

			//Inverse rd product
			rd = 1.f / rd;

			//Cross products:
			//rn = (b1b2 x b1a1)
			float rn = ((l2.end.x - l2.start.x) * (l1.start.y - l2.start.y) - (l2.end.y - l2.start.y) * (l1.start.x - l2.start.x)) * rd;
			//sn = (a1a2 x b1a1)
			float sn = ((l1.end.x - l1.start.x) * (l1.start.y - l2.start.y) - (l1.end.y - l1.start.y) * (l1.start.x - l2.start.x)) * rd;

			//Return the intersection depth
			//if (d) *d = rn;

			if (rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)
				return {}; // Intersection not within line segment

			return { l1.start + rn * l1.vector() };
		}
	}

	// intersects(r,l)
	// Get intersection points where rectangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const line<T2>& l)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(r.side(i), l);
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(c,l)
	// Get intersection points where circle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const line<T2>& l)
	{
		const auto closestPointToSegment = closest(l, c.pos);
		if (!overlaps(c, closestPointToSegment))
		{
			// Circle is too far away
			return {};
		}

		// Compute point closest to the circle on the line
		const auto d = l.vector();
		const auto uLine = d.dot(c.pos - l.start) / d.mag2();
		const auto closestPointToLine = l.start + uLine * d;
		const auto distToLine = (c.pos - closestPointToLine).mag2();

		if (std::abs(distToLine - c.radius * c.radius) < epsilon)
		{
			// Circle "kisses" the line
			return { closestPointToLine };
		}

		// Circle intersects the line
		const auto length = std::sqrt(c.radius * c.radius - distToLine);
		const auto p1 = closestPointToLine + l.vector().norm() * length;
		const auto p2 = closestPointToLine - l.vector().norm() * length;

		std::vector<olc::v_2d<T2>> intersections;
		intersections.reserve(2);

		if ((p1 - closest(l, p1)).mag2() < epsilon * epsilon)
			intersections.push_back(p1);
		if ((p2 - closest(l, p2)).mag2() < epsilon * epsilon)
			intersections.push_back(p2);

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(t,l)
	// Get intersection points where triangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const line<T2>& l)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(t.side(i), l);
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// ================================================================================================================
	// RECTANGLE ======================================================================================================


	// contains(r,r)
	// Check if rectangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r2.pos.x >= r1.pos.x) && (r2.pos.x + r2.size.x <= r1.pos.x + r1.size.x) &&
			   (r2.pos.y >= r1.pos.y) && (r2.pos.y + r2.size.y <= r1.pos.y + r1.size.y);
	}

	// contains(c,r)
	// Check if circle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const rect<T2>& r)
	{
		return contains(c, r.pos)
			&& contains(c, olc::v_2d<T2>{ r.pos.x + r.size.x, r.pos.y })
			&& contains(c, olc::v_2d<T2>{ r.pos.x, r.pos.y + r.size.y })
			&& contains(c, r.pos + r.size);
	}

	// contains(t,r)
	// Check if triangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const rect<T2>& r)
	{
		return contains(t, r.pos)
			&& contains(t, r.pos + r.size)
			&& contains(t, olc::v_2d<T2>{ r.pos.x + r.size.x,r.pos.y })
			&& contains(t, olc::v_2d<T2>{ r.pos.x, r.pos.y + r.size.y });
	}

	// overlaps(p,r)
	// Check if point overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const rect<T2>& r)
	{
		return overlaps(r, p);
	}

	// overlaps(l,r)
	// Check if line segment overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const rect<T2>& r)
	{
		return overlaps(r, l);
	}

	// overlaps(r,r)
	// Check if rectangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r1.pos.x <= r2.pos.x + r2.size.x && r1.pos.x + r1.size.x >= r2.pos.x &&
			r1.pos.y <= r2.pos.y + r2.size.y && r1.pos.y + r1.size.y >= r2.pos.y);
	}

	// overlaps(c,r)
	// Check if circle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const rect<T2>& r)
	{
		// Inspired by this (very clever btw)
		// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
		// But modified to work :P
		double overlap = (olc::v_2d<T2>{ std::clamp(c.pos.x, r.pos.x, r.pos.x + r.size.x), std::clamp(c.pos.y, r.pos.y, r.pos.y + r.size.y) } - c.pos).mag2();
		if (std::isnan(overlap)) overlap = 0;
		return (overlap - (c.radius * c.radius)) < 0;
	}

	// overlaps(t,r)
	// Check if triangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const rect<T2>& r)
	{
		return overlaps(t, r.top())
			|| overlaps(t, r.bottom())
			|| overlaps(t, r.left())
			|| overlaps(t, r.right())
			|| contains(r, t.pos[0]);
	}

	// intersects(p,r)
	// Get intersection points where point intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const rect<T2>& r)
	{
		return intersects(r, p);
	}

	// intersects(l,r)
	// Get intersection points where line segment intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const rect<T2>& r)
	{
		return intersects(r,l);
	}

	// intersects(r,r)
	// Get intersection points where rectangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r1, const rect<T2>& r2)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r2.side_count(); i++) {
			auto v = intersects(r1, r2.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(c,r)
	// Get intersection points where circle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(c, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(t,r)
	// Get intersection points where triangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++) {
			auto v = intersects(t, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// ================================================================================================================
	// CIRCLE =========================================================================================================

	// contains(p,c)
	// Check if point contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const olc::v_2d<T1>& p, [[maybe_unused]] const circle<T2>& c)
	{
		return false; // It can't!
	}

	// contains(l,c)
	// Check if line segment contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const line<T1>& l, [[maybe_unused]] const circle<T2>& c)
	{
		return false; // It can't!
	}

	// contains(r,c)
	// Check if rectangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const circle<T2>& c)
	{
		return r.pos.x + c.radius <= c.pos.x
			&& c.pos.x <= r.pos.x + r.size.x - c.radius
			&& r.pos.y + c.radius <= c.pos.y
			&& c.pos.y <= r.pos.y + r.size.y - c.radius;
	}

	// contains(c,c)
	// Check if circle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (std::sqrt(std::pow(c2.pos.x - c1.pos.x, 2) + std::pow(c2.pos.y - c1.pos.y, 2)) + c2.radius) <= c1.radius;

	}

	// contains(t,c)
	// Check if triangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const circle<T2>& c)
	{
		return contains(t, c.pos) && (c.pos - closest(t, c.pos)).mag2() >= c.radius * c.radius;
	}

	// overlaps(p,c)
	// Check if point overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const circle<T2>& c)
	{
		return overlaps(c, p);
	}

	// overlaps(l,c)
	// Check if line segment overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const circle<T2>& c)
	{
		return overlaps(c, l);
	}

	// overlaps(r,c)
	// Check if rectangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const circle<T2>& c)
	{
		return overlaps(c, r);
	}

	// overlaps(c,c)
	// Check if circle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (c1.pos - c2.pos).mag2() <= (c1.radius + c2.radius) * (c1.radius + c2.radius);
	}

	// overlaps(t,c)
	// Check if triangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const circle<T2>& c)
	{
		return contains(t, c.pos) || (c.pos - closest(t, c.pos)).mag2() <= c.radius * c.radius;
	}

	// intersects(p,c)
	// Get intersection points where point intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const circle<T2>& c)
	{
		return intersects(c, p);
	}

	// intersects(l,c)
	// Get intersection points where line segment intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const circle<T2>& c)
	{
		return intersects(c, l);
	}

	// intersects(r,c)
	// Get intersection points where rectangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const circle<T2>& c)
	{
		return intersects(c, r);
	}

	// intersects(c,c)
	// Get intersection points where circle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c1, const circle<T2>& c2)
	{
		if (c1.pos == c2.pos) return {}; // circles are either within one another so cannot intersect, or are
									 // identical so share all points which there's no good way to represent in return value.
		v_2d<T1> between = c2.pos - c1.pos;
		float dist2 = between.mag2();
		float radiusSum = c1.radius + c2.radius;
		if (dist2 > radiusSum*radiusSum) return {}; // circles are too far apart to be touching.
		if (contains(c1, c2) || contains(c2, c1)) return {}; // one circle is inside of the other, they can't be intersecting.
		if (dist2 == radiusSum) return {c1.pos + between.norm() * c1.radius}; // circles are touching at exactly 1 point
		// otherwise they're touching at 2 points.
	  //
	  //                ______     ________
	  //             .''      ''.X'        ''.      X = intersections
	  //            /          / |\           \     O = chordCenter
	  //           |          |  | |           |    In order to find the intersections we first find O.
	  //           (      c1--(--O-)--c2       )    To do this, we find the distance c1->O by solving for the
	  //           |          |  | |           |    two right triangles formed by the chord and c1->c2 (L).
	  //            \          \ |/           /           .                Pythagorean theorem:
	  //             '.        .'X          ,'          .'|\               (L-x)^2 + h^2 = c1.r^2
	  //               ''----''   ''------''      c1.r /  | \ c2.radius        x^2 + h^2 = c2.r^2
	  //                                             .'  h|  \             Subtract 1 equation from the other and solve:
	  //                                            /_____|___\            (L-x)^2 + h^2 - (x^2 + h^2) = c1.r^2 - c2.r^2
	  //                                               x   L-x             (L-x)^2 - x^2 = c1.r^2 - c2.r^2
	  //                                                                   L^2 - 2Lx = c1.r^2 - c2.r^2
	  //                                                                   2Lx - L^2 = c2.r^2 - c1.r^2
	  //       v------------<-----------------<-----------------<-----.    x = (L^2 + c2.r^2 - c1.r^2)/2 -.
	  //       Next, we have to solve for the height h, and move       '-------<--------------------<-----'
	  //       that distance from O, perpendicular to c1->c2.
	  //
	  //       Pythagorean theorem: x^2 + h^2 = c1.r^2  ->  c1.r^2 - x^2 = h^2  ->  h = sqrt(c1.r^2 - x^2)
	  //
	  //       x is ccDist and h is halfChord.
	  //
		float dist = std::sqrt(dist2);
		float ccDist = (dist2 + c1.radius * c1.radius - c2.radius * c2.radius)/(2*dist);
		v_2d<T1> chordCenter = c1.pos + between.norm() * ccDist;
		v_2d<T1> halfChord = between.norm().perp() * std::sqrt(c1.radius * c1.radius - ccDist * ccDist);
		return {chordCenter + halfChord, chordCenter - halfChord};
	}

	// intersects(t,c)
	// Get intersection points where triangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const circle<T2>& c)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++) {
			auto v = intersects(c, t.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// ================================================================================================================
	// TRIANGLE =======================================================================================================

	// contains(r,t)
	// Check if rectangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const triangle<T2>& t)
	{
		return contains(r, t.side(0))
			&& contains(r, t.side(1))
			&& contains(r, t.side(2));
	}

	// contains(c,t)
	// Check if circle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const triangle<T2>& t)
	{
		return contains(c, t.pos[0])
			&& contains(c, t.pos[1])
			&& contains(c, t.pos[2]);
	}

	// contains(t,t)
	// Check if triangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		return contains(t1, t2.pos[0])
			&& contains(t1, t2.pos[1])
			&& contains(t1, t2.pos[2]);
	}

	// overlaps(p,t)
	// Check if point overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const triangle<T2>& t)
	{
		return overlaps(t, p);
	}

	// overlaps(l,t)
	// Check if line segment overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const triangle<T2>& t)
	{
		return overlaps(t, l);
	}

	// overlaps(r,t)
	// Check if rectangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const triangle<T2>& t)
	{
		return overlaps(t, r);
	}

	// overlaps(c,t)
	// Check if circle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const triangle<T2>& t)
	{
		return overlaps(t, c);
	}

	// overlaps(t,t)
	// Check if triangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		return overlaps(t1, t2.side(0))
			|| overlaps(t1, t2.side(1))
			|| overlaps(t1, t2.side(2))
			|| overlaps(t2, t1.pos[0]);
	}

	// intersects(p,t)
	// Get intersection points where point intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const triangle<T2>& t)
	{
		return intersects(t, p);
	}

	// intersects(l,t)
	// Get intersection points where line segment intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const triangle<T2>& t)
	{
		return intersects(t, l);
	}

	// intersects(r,t)
	// Get intersection points where rectangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const triangle<T2>& t)
	{
		return intersects(t, r);
	}

	// intersects(c,t)
	// Get intersection points where circle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const triangle<T2>& t)
	{
		return intersects(t, c);
	}

	// intersects(t,t)
	// Get intersection points where triangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t2.side_count(); i++) {
			auto v = intersects(t1, t2.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}


	// envelope_c(c)
	// Return circle that fully encapsulates a point
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const olc::v_2d<T1>& p)
	{
		return circle<T1>(p, 0);
	}

	// envelope_c(l)
	// Return circle that fully encapsulates a line
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const line<T1>& l)
	{
		return {l.upoint(0.5),l.vector().mag()/2};
	}

	// envelope_c(r)
	// Return circle that fully encapsulates a rectangle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const rect<T1>& r)
	{
		// Construct the circle from the rectangle's diagonal
		return envelope_c(line<T1>(r.pos, r.pos + r.size));
	}

	// envelope_c(c)
	// Return circle that fully encapsulates a circle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const circle<T1>& c)
	{
		return c;
	}

	// envelope_c(t)
	// Return circle that fully encapsulates a triangle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const triangle<T1>& t)
	{
		olc::v_2d<T1> circumcenter;

		double D = 2 * (t.pos[0].x * (t.pos[1].y - t.pos[2].y) + t.pos[1].x * (t.pos[2].y - t.pos[0].y) + t.pos[2].x * (t.pos[0].y - t.pos[1].y));
		circumcenter.x = T1(double(
			(t.pos[0].x * t.pos[0].x + t.pos[0].y * t.pos[0].y) * (t.pos[1].y - t.pos[2].y) +
			(t.pos[1].x * t.pos[1].x + t.pos[1].y * t.pos[1].y) * (t.pos[2].y - t.pos[0].y) +
			(t.pos[2].x * t.pos[2].x + t.pos[2].y * t.pos[2].y) * (t.pos[0].y - t.pos[1].y)
		) / D);
		circumcenter.y = T1(double(
			(t.pos[0].x * t.pos[0].x + t.pos[0].y * t.pos[0].y) * (t.pos[2].x - t.pos[1].x) +
			(t.pos[1].x * t.pos[1].x + t.pos[1].y * t.pos[1].y) * (t.pos[0].x - t.pos[2].x) +
			(t.pos[2].x * t.pos[2].x + t.pos[2].y * t.pos[2].y) * (t.pos[1].x - t.pos[0].x)
		) / D);

		double r = 0;
		for (auto& point : t.pos) {
			r = std::max(r, double(std::hypot(circumcenter.x - point.x, circumcenter.y - point.y)));
		}

		return {circumcenter, T1(r)};
	}

	// envelope_r(p)
	// Return rectangle that fully encapsulates a point
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const olc::v_2d<T1>& p)
	{
		return rect<T1>(p, { 0,0 });
	}

	// envelope_r(l)
	// Return rectangle that fully encapsulates a line
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const line<T1>& l)
	{
		T1 min_x=std::min(l.start.x,l.end.x);
		T1 min_y=std::min(l.start.y,l.end.y);
		T1 size_x=std::abs(l.start.x-l.end.x);
		T1 size_y=std::abs(l.start.y-l.end.y);
		return {{min_x,min_y},{size_x,size_y}};
	}

	// envelope_r(r)
	// Return rectangle that fully encapsulates a rectangle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const rect<T1>& r)
	{
		return r;
	}

	// envelope_r(c)
	// Return rectangle that fully encapsulates a circle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const circle<T1>& c)
	{
		return rect<T1>(c.pos - v_2d<T1>{c.radius, c.radius}, { c.radius * 2, c.radius * 2 });
	}

	// envelope_r(t)
	// Return rectangle that fully encapsulates a triangle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const triangle<T1>& t)
	{
		auto vMin = t.pos[0].min(t.pos[1].min(t.pos[2]));
		auto vMax = t.pos[0].max(t.pos[1].max(t.pos[2]));
		return rect<T1>(vMin, vMax - vMin);
	}

	template<typename T>
	inline constexpr auto bounding_box(T&& t)
	{
		return envelope_r(std::forward<T>(t));
	}

	template<typename T>
	inline constexpr auto bounding_circle(T&& t)
	{
		return envelope_c(std::forward<T>(t));
	}

	// PROJECTIONS ==========================================================================================================

	// project(c,p)
	// project a circle, onto a point, via a ray (i.e. how far along the ray can the circle travel until it contacts the point?)
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T1>> project(const circle<T1>& c, const olc::v_2d<T2>& p, const ray<T3>& q)
	{
		return project(c, circle<T2>(p, 0), q);
	}

	// project(c,c)
	// project a circle, onto a circle, via a ray (i.e. how far along the ray can the circle travel until it contacts the other circle?)
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T1>> project(const circle<T1>& c1, const circle<T2>& c2, const ray<T3>& q)
	{
		// Inspired by https://math.stackexchange.com/a/929240

		double A = q.direction.mag2();
		double B = 2.0 * (q.origin.dot(q.direction) - c2.pos.dot(q.direction));
		double C = c2.pos.mag2() + q.origin.mag2() - (2.0 * c2.pos.x * q.origin.x) - (2.0 * c2.pos.y * q.origin.y) - ((c1.radius + c2.radius) * (c1.radius + c2.radius));
		double D = B * B - 4.0 * A * C;

		if (D < 0.0)
			return std::nullopt;
		else
		{
			const auto sD = std::sqrt(D);
			const auto s1 = (-B + sD) / (2.0 * A);
			const auto s2 = (-B - sD) / (2.0 * A);

			if (s1 < 0 && s2 < 0)
				return std::nullopt;
			if (s1 < 0)
				return q.origin + q.direction * s2;
			if (s2 < 0)
				return q.origin + q.direction * s1;

			return q.origin + q.direction * std::min(s1, s2);
		}
	}

	// project(c,l)
	// project a circle, onto a line segment, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const line<T2>& l, const ray<T3>& q)
	{
		// Treat line segment as capsule with radius that of the circle
		// and treat the circle as a point

		// First do we hit ends of line segment, inflated to be circles
		const auto vHitsStartCirc = intersects(q, circle<T2>(l.start, c.radius));
		const auto vHitsEndCirc = intersects(q, circle<T2>(l.end, c.radius));

		// Now create two line segments in parallel to the original, that join
		// up the end circles to form the sides of the capsule
		const auto displace = l.vector().norm().perp() * c.radius;
		const auto vHitsSide1 = intersects(q, line<T2>(l.start + displace, l.end + displace));
		const auto vHitsSide2 = intersects(q, line<T2>(l.start - displace, l.end - displace));

		// Bring the multitude of points to one place
		std::vector<olc::v_2d<T2>> vAllIntersections;
		vAllIntersections.insert(vAllIntersections.end(), vHitsStartCirc.begin(), vHitsStartCirc.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsEndCirc.begin(), vHitsEndCirc.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsSide1.begin(), vHitsSide1.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsSide2.begin(), vHitsSide2.end());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}

	// project(c,r)
	// project a circle, onto a rectangle, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const rect<T2>& r, const ray<T3>& q)
	{
		const auto s1 = project(c, r.top(), q);
		const auto s2 = project(c, r.bottom(), q);
		const auto s3 = project(c, r.left(), q);
		const auto s4 = project(c, r.right(), q);

		std::vector<olc::v_2d<T2>> vAllIntersections;
		if (s1.has_value()) vAllIntersections.push_back(s1.value());
		if (s2.has_value()) vAllIntersections.push_back(s2.value());
		if (s3.has_value()) vAllIntersections.push_back(s3.value());
		if (s4.has_value()) vAllIntersections.push_back(s4.value());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}

	// project(c,t)
	// project a circle, onto a triangle, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const triangle<T2>& t, const ray<T3>& q)
	{
		const auto s1 = project(c, t.side(0), q);
		const auto s2 = project(c, t.side(1), q);
		const auto s3 = project(c, t.side(2), q);

		std::vector<olc::v_2d<T2>> vAllIntersections;
		if (s1.has_value()) vAllIntersections.push_back(s1.value());
		if (s2.has_value()) vAllIntersections.push_back(s2.value());
		if (s3.has_value()) vAllIntersections.push_back(s3.value());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}


	// RAYS =================================================================================================================

	// intersects(q,q)
	// return intersection point (if it exists) of a ray and a ray
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q1, const ray<T2>& q2)
	{
		const auto origin_diff = q2.origin - q1.origin;
		const auto cp1 = q1.direction.cross(q2.direction);
		const auto cp2 = origin_diff.cross(q2.direction);

		if (cp1 == 0) // Early rejection
		{
			if (cp2 == 0)
				return { q1.origin }; // co-linear
			else
				return {}; // parallel
		}

		const auto cp3 = origin_diff.cross(q1.direction);
		const auto t1 = cp2 / cp1; // distance along q1 to intersection
		const auto t2 = cp3 / cp1; // distance along q2 to intersection

		if (t1 >= 0 && t2 >= 0)
			return { q1.origin + q1.direction * t1 }; // Intersection, both rays positive
		else
			return {}; // Intersection, but behind a rays origin, so not really an intersection in context
	}

	// intersects(q,p)
	// return intersection point (if it exists) of a ray and a point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const v_2d<T2>& p)
	{
		const line<T1> l = { q.origin, q.origin + q.direction };

		if (std::abs(l.side(p)) < epsilon )
			return { p }; // Intersection
		else
			return {};
	}

	// intersects(q,l)
	// return intersection point (if it exists) of a ray and a line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const line<T2>& l)
	{
		const auto line_direction = l.vector();
		const auto origin_diff = l.start - q.origin;
		const auto cp1 = q.direction.cross(line_direction);
		const auto cp2 = origin_diff.cross(line_direction);

		if (cp1 == 0) // Early rejection
		{
			if (cp2 == 0)
				return { q.origin }; // co-linear
			else
				return {}; // parallel
		}

		const auto cp3 = origin_diff.cross(q.direction);
		const auto t1 = cp2 / cp1; // distance along ray to intersection
		const auto t2 = cp3 / cp1; // distance along line segment to intersection

		if (t1 >= 0 && t2 >= 0 && t2 <= 1)
			return { q.origin + q.direction * t1 }; // Intersection, both rays positive
		else
			return {};	// Intersection, but behind a rays origin, or outside line segment bounds.
						// so not really an intersection in context
	}

	// collision(q,l)
	// optionally returns collision point and collision normal of ray and a line segment, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T2>, olc::v_2d<T2>>> collision(const ray<T1>& q, const line<T2>& l)
	{
		const auto vIntersection = intersects(q, l);
		if (vIntersection.size() > 0)
		{
			return { {vIntersection[0], l.vector().perp().norm() * l.side(q.origin)} };
		}

		return std::nullopt;
	}

	// reflect(q,l)
	// optionally returns a ray reflected off a line segement if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const line<T2>& l)
	{
		const auto vCollision = collision(q, l);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// reflect(q,p)
	// optionally returns a ray reflected off a point if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const olc::v_2d<T2>& p)
	{
		// TODO:
		return std::nullopt;
	}

	// collision(q,r)
	// optionally returns collision point and collision normal of ray and a line segment, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T1>, olc::v_2d<T1>>> collision(const ray<T1>& q, const rect<T2>& r)
	{
		olc::v_2d<T1> vClosestIntersection;
		olc::v_2d<T1> vIntersectionNormal;
		double dClosestDistance2 = std::numeric_limits<double>::max();
		bool bCollide = false;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(q, r.side(i));
			if (v.size() > 0)
			{
				bCollide = true;
				double d = (v[0] - q.origin).mag2();
				if (d < dClosestDistance2)
				{
					dClosestDistance2 = d;
					vClosestIntersection = v[0];
					vIntersectionNormal = r.side(i).vector().perp().norm();
				}
			}
		}

		if (bCollide)
		{
			return { {vClosestIntersection, vIntersectionNormal} };
		}

		return std::nullopt;
	}

	// reflect(q,r)
	// optionally returns a ray reflected off a rectangle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const rect<T2>& r)
	{
		const auto vCollision = collision(q, r);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// collision(q,c)
	// optionally returns collision point and collision normal of ray and a circle, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T2>, olc::v_2d<T2>>> collision(const ray<T1>& q, const circle<T2>& c)
	{
		const auto vIntersection = intersects(q, c);
		if (vIntersection.size() > 0)
		{
			return { {vIntersection[0], (vIntersection[0] - c.pos).norm()}};
		}

		return std::nullopt;
	}

	// reflect(q,c)
	// optionally returns a ray reflected off a circle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const circle<T2>& c)
	{
		const auto vCollision = collision(q, c);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// collision(q,r)
	// optionally returns collision point and collision normal of ray and a triangle, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T1>, olc::v_2d<T1>>> collision(const ray<T1>& q, const triangle<T2>& t)
	{
		olc::v_2d<T1> vClosestIntersection;
		olc::v_2d<T1> vIntersectionNormal;
		double dClosestDistance2 = std::numeric_limits<double>::max();
		bool bCollide = false;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(q, t.side(i));
			if (v.size() > 0)
			{
				bCollide = true;
				double d = (v[0] - q.origin).mag2();
				if (d < dClosestDistance2)
				{
					dClosestDistance2 = d;
					vClosestIntersection = v[0];
					vIntersectionNormal = t.side(i).vector().perp().norm();
				}
			}
		}

		if (bCollide)
		{
			return { {vClosestIntersection, vIntersectionNormal} };
		}

		return std::nullopt;
	}

	// reflect(q,t)
	// optionally returns a ray reflected off a triangle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const triangle<T2>& t)
	{
		const auto vCollision = collision(q, t);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// reflect(q,r)
	// can't reflect a ray of a ray
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q1, const ray<T2>& q2)
	{
		// Can't reflect!
		return std::nullopt;
	}


	// intersects(q,c)
	// Get intersection points where a ray intersects a circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const circle<T2>& c)
	{
		// Look familiar?
		double A = q.direction.mag2();
		double B = 2.0 * (q.origin.dot(q.direction) - c.pos.dot(q.direction));
		double C = c.pos.mag2() + q.origin.mag2() - (2.0 * c.pos.x * q.origin.x) - (2.0 * c.pos.y * q.origin.y) - (c.radius * c.radius);
		double D = B * B - 4.0 * A * C;

		if (D < 0.0)
			return {};
		else
		{
			const auto sD = std::sqrt(D);
			const auto s1 = (-B + sD) / (2.0 * A);
			const auto s2 = (-B - sD) / (2.0 * A);

			if (s1 < 0 && s2 < 0)
				return {};
			if (s1 < 0)
				return { q.origin + q.direction * s2 };
			if (s2 < 0)
				return { q.origin + q.direction * s1 };

			const auto [min_s, max_s] = std::minmax(s1, s2);
			return { q.origin + q.direction * min_s, q.origin + q.direction * max_s };
		}
	}

	// intersects(q,r)
	// Get intersection points where a ray intersects a rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(q, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(q,t)
	// Get intersection points where a ray intersects a triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const triangle<T2>& t)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(q, t.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}
}
*/