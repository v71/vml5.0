#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////
// TO DO :
// axis aligned bounding box against oriented bounding box

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxes

			static const unsigned int AABBOX_DISJOINT			 = 0;
			static const unsigned int AABBOX_FIRST_INSIDE_SECOND = 1;
			static const unsigned int AABBOX_SECOND_INSIDE_FIRST = 2;
			static const unsigned int AABBOX_INTERSECTED		 = 3;

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxe - sphere

			static const unsigned int SPHERE_OUTSIDE_AABBOX	   = 0;
			static const unsigned int SPHERE_INSIDE_AABBOX	   = 1;
			static const unsigned int SPHERE_INTERSECTS_AABBOX = 2;
			static const unsigned int AABBOX_INSIDE_SPHERE	   = 3;

			/////////////////////////////////////////////////////////////////////////////
			// checks if ray intersect an axis aligned bounding box

			static const unsigned int AABBOX_RAY_OUTSIDE	  = 0;
			static const unsigned int AABBOX_RAY_INTERSECTS	  = 1;
			static const unsigned int AABBOX_RAY_INSIDEAABBOX = 2;

			/////////////////////////////////////////////////////////////////////////////
			// checks if ray intersect an axis aligned bounding box

			static const unsigned int AOBBOX_RAY_OUTSIDE      = 0;
			static const unsigned int AOBBOX_RAY_INTERSECTS   = 1;
						
			/////////////////////////////////////////////////////////////////////////////
			// bounding boxe intersection tests 

			static unsigned int AABBoxVsAABBox(const glm::vec3& bmin1, const glm::vec3& bmax1, const glm::vec3& bmin2, const glm::vec3& bmax2)
			{
				// check if the first bounding box is entirely inside the second bounding box

				if ((bmin1.x >= bmin2.x && bmax1.x <= bmax2.x) &&
					(bmin1.y >= bmin2.y && bmax1.y <= bmax2.y) &&
					(bmin1.z >= bmin2.z && bmax1.z <= bmax2.z))
					return AABBOX_FIRST_INSIDE_SECOND;

				// check if the second bounding box is entirely inside the first bounding box

				if ((bmin2.x >= bmin1.x && bmax2.x <= bmax1.x) &&
					(bmin2.y >= bmin1.y && bmax2.y <= bmax1.y) &&
					(bmin2.z >= bmin1.z && bmax2.z <= bmax1.z))
					return AABBOX_SECOND_INSIDE_FIRST;

				// check if bounding box is out of second bounding box

				if ((bmax2.x < bmin1.x || bmin2.x > bmax1.x) ||
					(bmax2.y < bmin1.y || bmin2.y > bmax1.y) ||
					(bmax2.z < bmin1.z || bmin2.z > bmax1.z))
					return AABBOX_DISJOINT;

				// if we get here bounding boxes are intersecting each ohter

				return AABBOX_INTERSECTED;
			}
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection between sphere and bounding box
			
			static int AABBoxVsSphere(const glm::vec3& bmin, const glm::vec3& bmax, const glm::vec3& p0, const float radius)
			{
				// cache values

				float uax = p0.x - bmin.x;
				float uay = p0.y - bmin.y;
				float uaz = p0.z - bmin.z;
				float ubx = p0.x - bmax.x;
				float uby = p0.y - bmax.y;
				float ubz = p0.z - bmax.z;

				// checks if sphere is totally inside the bounding box

				if (uax + radius >= 0 && ubx + radius <= 0)
				if (uax - radius >= 0 && ubx - radius <= 0)
				if (uay + radius >= 0 && uby + radius <= 0)
				if (uay - radius >= 0 && uby - radius <= 0)
				if (uaz + radius >= 0 && ubz + radius <= 0)
				if (uaz + radius >= 0 && ubz - radius <= 0)
					return SPHERE_INSIDE_AABBOX;

				// check if bounding box is totally outside the sphere

				glm::vec3 v0(0, 0, 0);

				float dax = uax * uax;
				float day = uay * uay;
				float daz = uaz * uaz;
				float dbx = ubx * ubx;
				float dby = uby * uby;
				float dbz = ubz * ubz;

				v0.x = bmin.x;
				v0.y = bmin.y;
				v0.z = bmin.z;

				if (dbx >= dax) { v0.x = bmax.x; dax = dbx; }
				if (dby >= day) { v0.y = bmax.y; day = dby; }
				if (dbz >= daz) { v0.z = bmax.z; daz = dbz; }

				if (dax + day + daz < radius * radius)
					return SPHERE_INTERSECTS_AABBOX;

				// check if sphere and bounding box are intersecting
				// note that uax, uay,uaz,ubx,uby,ubz squared values
				// must be recomputed since we come from the other
				// step and values may get overwritten by the 
				// previous checks

				float dmin = 0.0f;

				if (uax < 0) dmin += uax * uax;
				else if (ubx > 0) dmin += ubx * ubx;

				if (uay < 0) dmin += uay * uay;
				else if (uby > 0) dmin += uby * uby;

				if (uaz < 0) dmin += uaz * uaz;
				else if (ubz > 0) dmin += ubz * ubz;

				if (dmin < radius * radius)
					return AABBOX_INSIDE_SPHERE;

				return SPHERE_OUTSIDE_AABBOX;
			}
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection between infinit ray and bouding box
			// min is bounding box min
			// max is bounding box max
			// a is firt line point
			// b is sencond line point
			// p and q are intersection points

			static unsigned int AABBoxVsRay(const glm::vec3& min, const glm::vec3& max,
											const glm::vec3& a, const glm::vec3& b,
											glm::vec3& p, glm::vec3& q,
											float& tmin, float& tmax,
											const float eps = vml::math::EPSILON)
			{
				glm::vec3 d = b - a;

				if (d.x > -eps && d.x < eps)
					d.x = eps;

				float txmin = (min.x - a.x) / d.x;
				float txmax = (max.x - a.x) / d.x;

				if (txmin > txmax)
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}

				if (d.y > -eps && d.y < eps)
					d.y = eps;

				float tymin = (min.y - a.y) / d.y;
				float tymax = (max.y - a.y) / d.y;

				if (tymin > tymax)
				{
					float t = tymin;
					tymin = tymax;
					tymax = t;
				}

				if ((txmin > tymax) || (tymin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tymin > txmin)
					txmin = tymin;

				if (tymax < txmax)
					txmax = tymax;

				if (d.z > -eps && d.z < eps)
					d.z = eps;

				float tzmin = (min.z - a.z) / d.z;
				float tzmax = (max.z - a.z) / d.z;

				if (tzmin > tzmax)
				{
					float t = tzmin;
					tzmin = tzmax;
					tzmax = t;
				}

				if ((txmin > tzmax) || (tzmin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tzmin > txmin)
					txmin = tzmin;

				if (tzmax < txmax)
					txmax = tzmax;

				tmin = txmin;
				tmax = txmax;

				p = a + tmin * d;
				q = a + tmax * d;

				return AABBOX_RAY_INTERSECTS;
			}
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection between infinit ray and bouding box
			// min is bounding box min
			// max is bounding box max
			// a is firt line point
			// b is sencond line point
			// p and q are intersection points

			static unsigned int AABBoxVsLine(const glm::vec3& min, const glm::vec3& max,
											 const glm::vec3& a, const glm::vec3& b,
											 glm::vec3& p, glm::vec3& q,
											 float& txmin, float& txmax,
											 const float eps = vml::math::EPSILON)
			{
				// line bounding box

				glm::vec3 rmin(0, 0, 0);
				glm::vec3 rmax(0, 0, 0);

				if (a.x < b.x) { rmin.x = a.x; rmax.x = b.x; }
				else { rmin.x = b.x; rmax.x = a.x; }
				if (a.y < b.y) { rmin.y = a.y; rmax.y = b.y; }
				else { rmin.y = b.y; rmax.y = a.y; }
				if (a.z < b.z) { rmin.z = a.z; rmax.z = b.z; }
				else { rmin.z = b.z; rmax.z = a.z; }

				// chck if line is contained inside bbounding box

				if ((rmin.x >= min.x && rmax.x <= max.x) &&
					(rmin.y >= min.y && rmax.y <= max.y) &&
					(rmin.z >= min.z && rmax.z <= max.z))
				{
					p = a;
					q = b;
					return AABBOX_RAY_INSIDEAABBOX;
				}

				// check if bounding box is out of second bounding box

				if ((max.x < rmin.x || min.x > rmax.x) ||
					(max.y < rmin.y || min.y > rmax.y) ||
					(max.z < rmin.z || min.z > rmax.z))
					return AABBOX_RAY_OUTSIDE;

				// check for intersection

				glm::vec3 d = b - a;

				if (d.x > -eps && d.x < eps)
					d.x = eps;

				txmin = (min.x - a.x) / d.x;
				txmax = (max.x - a.x) / d.x;

				if (txmin > txmax)
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}

				if (d.y > -eps && d.y < eps)
					d.y = eps;

				float tymin = (min.y - a.y) / d.y;
				float tymax = (max.y - a.y) / d.y;

				if (tymin > tymax)
				{
					float t = tymin;
					tymin = tymax;
					tymax = t;
				}

				if ((txmin > tymax) || (tymin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tymin > txmin)
					txmin = tymin;

				if (tymax < txmax)
					txmax = tymax;

				if (d.z > -eps && d.z < eps)
					d.z = eps;

				float tzmin = (min.z - a.z) / d.z;
				float tzmax = (max.z - a.z) / d.z;

				if (tzmin > tzmax)
				{
					float t = tzmin;
					tzmin = tzmax;
					tzmax = t;
				}

				if ((txmin > tzmax) || (tzmin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tzmin > txmin)
					txmin = tzmin;

				if (tzmax < txmax)
					txmax = tzmax;

				if (txmin > txmax)
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}

				if (txmin < -eps)     txmin = 0.0f;
				if (txmax > 1.0f + eps) txmax = 1.0f;

				p = a + txmin * d;
				q = a + txmax * d;

				return AABBOX_RAY_INTERSECTS;
			}
			
			/////////////////////////////////////////////////////////////////////////////
			// Ray origin, in world space
			// Ray direction (NOT target position!), in world space. Must be normalize()'d.
			// Minimum X,Y,Z coords of the mesh when not transformed at all.
			// Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
			// Transformation applied to the mesh (which will thus be also applied to its bounding box)
			// Output : distance between ray_origin and the intersection with the OBB

			static unsigned int AOBBoxVsRay(const glm::vec3& start,
											const glm::vec3& end,
											const glm::vec3& aabb_min,
											const glm::vec3& aabb_max,
											const glm::mat4& ModelMatrix,
											glm::vec3& p, glm::vec3& q,
											float& tmin, float& tmax,
											const float eps = vml::math::EPSILON)
			{
				tmin = -FLT_MAX;
				tmax = FLT_MAX;

				glm::vec3 dir = end - start;

				glm::vec3 obbpos(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

				glm::vec3 delta = obbpos - start;

				// Test intersection with the 2 planes perpendicular to the OBB's X axis

				{
					glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);

					float e = glm::dot(xaxis, delta);
					float f = glm::dot(dir, xaxis);

					if (fabs(f) > eps)
					{
						// Standard case

						float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
						float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane

						// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

						// We want t1 to represent the nearest intersection, 
						// so if it's not the case, invert t1 and t2

						if (t1 > t2)
						{
							// swap t1 and t2

							float w = t1;
							t1 = t2;
							t2 = w;
						}

						// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)

						if (t2 < tmax)
							tmax = t2;
						// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)

						if (t1 > tmin)
							tmin = t1;

						// And here's the trick :
						// If "far" is closer than "near", then there is NO intersection.
						// See the images in the tutorials for the visual explanation.

						if (tmax < tmin)
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{
						// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"

						if (-e + aabb_min.x > -eps || -e + aabb_max.x < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				// Test intersection with the 2 planes perpendicular to the OBB's Y axis
				// Exactly the same thing than above.

				{
					glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);

					float e = glm::dot(yaxis, delta);
					float f = glm::dot(dir, yaxis);

					if (fabs(f) > eps)
					{

						float t1 = (e + aabb_min.y) / f;
						float t2 = (e + aabb_max.y) / f;

						if (t1 > t2)
						{
							// swap t1 and t2

							float w = t1;
							t1 = t2;
							t2 = w;
						}

						if (t2 < tmax)
							tmax = t2;

						if (t1 > tmin)
							tmin = t1;

						if (tmin > tmax)
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{
						if (-e + aabb_min.y > -eps || -e + aabb_max.y < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				// Test intersection with the 2 planes perpendicular to the OBB's Z axis
				// Exactly the same thing than above.

				{

					glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);

					float e = glm::dot(zaxis, delta);
					float f = glm::dot(dir, zaxis);

					if (fabs(f) > eps)
					{

						float t1 = (e + aabb_min.z) / f;
						float t2 = (e + aabb_max.z) / f;

						if (t1 > t2)
						{
							// swap t1 and t2

							float w = t1;
							t1 = t2;
							t2 = w;
						}

						if (t2 < tmax)
							tmax = t2;

						if (t1 > tmin)
							tmin = t1;

						if (tmin > tmax)
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{

						if (-e + aabb_min.z > -eps || -e + aabb_max.z < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				p = start + tmin * dir;
				q = start + tmax * dir;

				return AOBBOX_RAY_INTERSECTS;
			}
			
		} // end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace


/////////////////////////////////////////////////////////////////////////////
// checks if lines intersect each other

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			
			static const unsigned int LINE_DO_NOT_INTERSECTS_LINE = 0;
			static const unsigned int LINE_INTERSECTS_LINE        = 1;

			///////////////////////////////////////////////////////////////////////////////////////////
			// intersection between 3d lines

			static int LineLineIntersect(const glm::vec3 &p0, const glm::vec3 &p1,
										 const glm::vec3 &q0, const glm::vec3 &q1,
										 glm::vec3 &pa, glm::vec3 &pb,
										 float eps = vml::math::EPSILON)
			{

				glm::vec3 u = p1 - p0;
				glm::vec3 v = q1 - q0;
				glm::vec3 w = p0 - q0;

				// computing cross component of u X v

				float det1 = u.y*v.z - u.z*v.y;
				float det2 = u.x*v.z - u.z*v.x;
				float det3 = u.x*v.y - u.y*v.x;

				// if cross is zero , then the vectors are parallel

				if (det1 > -eps && det1 < eps)
				if (det2 > -eps && det2 < eps)
				if (det3 > -eps && det3 < eps)
					return LINE_DO_NOT_INTERSECTS_LINE;

				// compute cofatctrs

				float a = u.x*u.x + u.y*u.y + u.z*u.z;
				float b = u.x*v.x + u.y*v.y + u.z*v.z;
				float c = v.x*v.x + v.y*v.y + v.z*v.z;
				float d = u.x*w.x + u.y*w.y + u.z*w.z;
				float e = v.x*w.x + v.y*w.y + v.z*w.z;

				// check if intersection lies within 0 , 1 range

				float denum = 1.0f / (a * c - b * b);

				float mua = (b*e - c * d) * denum;
				if (mua< -eps || mua >1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_LINE;

				float mub = (a*e - b * d) * denum;
				if (mub< -eps || mub >1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_LINE;

				// compute intersection points

				pa.x = p0.x + mua * u.x;
				pa.y = p0.y + mua * u.y;
				pa.z = p0.z + mua * u.z;
				pb.x = q0.x + mub * v.x;
				pb.y = q0.y + mub * v.y;
				pb.z = q0.z + mub * v.z;

				return LINE_INTERSECTS_LINE;
			}
			
		}	// end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace


namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			
			/////////////////////////////////////////////////////////////////////////////
			// intersection return codes

			static const unsigned int SPHERE_DISJOINT			 = 0;
			static const unsigned int SPHERE_FIRST_INSIDE_SECOND = 1;
			static const unsigned int SPHERE_SECOND_INSIDE_FIRST = 2;
			static const unsigned int SPHERE_INTERSECTED		 = 3;

			static const unsigned int DO_NOT_INTERSECTS			 = 0;
			static const unsigned int DO_INTERSECTS				 = 1;

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding spheres

			static int SphereVsSphere(const glm::vec3& pos1, float radius1, const glm::vec3& pos2, float radius2)
			{
				glm::vec3 d = pos2 - pos1;
				float r = d.x * d.x + d.y * d.y + d.z * d.z;
				float r1sr2s = radius1 * radius1 + radius2 * radius2;
				float r1r2 = 2.0f * radius1 * radius2;

				// check if the first sphere is entirely inside the second sphere or viceversa

				if (radius1 > radius2)
				{
					if (r < r1sr2s - r1r2) return SPHERE_FIRST_INSIDE_SECOND;
				}
				else
				{
					if (r < r1sr2s - r1r2) return SPHERE_SECOND_INSIDE_FIRST;
				}

				// check if spheres are disjoint from each other

				if (r > r1sr2s + r1r2) return SPHERE_DISJOINT;

				// if we get here spheres are intersecting each other

				return SPHERE_INTERSECTED;

			}

			/////////////////////////////////////////////////
			// tests if a ray intersects a sphere
			// uses a start and end point as ray description
			// and returns points of intersection

			static int SphereVsRay(const glm::vec3& center, float radius,
								   const glm::vec3& start, const glm::vec3& end,
								   glm::vec3& p,
								   glm::vec3& q,
								   float& tmin,
								   float& tmax)
			{

				glm::vec3 d = end - start;
				glm::vec3 r = start - center;
				float a = d.x * d.x + d.y * d.y + d.z * d.z;
				float b = glm::dot(r, d);
				float c = glm::dot(r, r) - radius * radius;
				float delta = b * b - a * c;
				if (delta < 0.0f)
					return DO_NOT_INTERSECTS;
				delta = sqrtf(delta);
				float inva = 1.0f / a;
				tmin = (-b + delta) * inva;
				tmax = (-b - delta) * inva;
				if (tmax < tmin)
				{
					float temp = tmin;
					tmin = tmax;
					tmax = temp;
				}
				p = start + tmin * d;
				q = start + tmax * d;

				return DO_INTERSECTS;
			}

			//////////////////////////////////////////////////////////////////////////
			// ellipsoid ray intersection

			static int EllipsoidVsRay(const glm::vec3& pos,
									  const glm::vec3& right,
									  const glm::vec3& up,
									  const glm::vec3& fwd,
									  const glm::vec3& ellipsoidradius,
									  const glm::vec3& a,
									  const glm::vec3& b,
									  float& t1,
									  float& t2,
									  glm::vec3& u1,
									  glm::vec3& u2,
									  const float eps = vml::math::EPSILON)
			{

				// extract rotation matrix
				// we need only the rotational part
				// thus the matrix is multiplied by the inverse
				// scaling factors, also the matrix is 
				// scaled by the ellipsoid half sizes

				float halfxradius = 0.5f * ellipsoidradius.x;
				float halfyradius = 0.5f * ellipsoidradius.y;
				float halfzradius = 0.5f * ellipsoidradius.z;

				// scale parent matrix

				float M00 = right.x * halfxradius;
				float M01 = right.y * halfxradius;
				float M02 = right.z * halfxradius;

				float M10 = up.x * halfyradius;
				float M11 = up.y * halfyradius;
				float M12 = up.z * halfyradius;

				float M20 = fwd.x * halfzradius;
				float M21 = fwd.y * halfzradius;
				float M22 = fwd.z * halfzradius;

				// ellipsodi extremal points

				// glm::vec3 qa = glm::vec3( M00 + mptr[12], M01 + mptr[13], M02 + mptr[14]);
				// glm::vec3 qb = glm::vec3( M10 + mptr[12], M11 + mptr[13], M12 + mptr[14]);
				// glm::vec3 qc = glm::vec3( M20 + mptr[12], M21 + mptr[13], M22 + mptr[14]);

				// invert rotational matrix 
				// so the ellipsoid is now a 1 radius sphere

				float det = M00 * M11 * M22 - M00 * M21 * M12 +
					M10 * M21 * M02 - M10 * M01 * M22 +
					M20 * M01 * M12 - M20 * M11 * M02;

				if (det > -eps && det < eps)
					det = eps;

				det = 1.0f / det;

				float Invmm00 = (M11 * M22 - M21 * M12) * det;
				float Invmm01 = (M20 * M12 - M10 * M22) * det;
				float Invmm02 = (M10 * M21 - M20 * M11) * det;

				float Invmm10 = (M21 * M02 - M01 * M22) * det;
				float Invmm11 = (M00 * M22 - M20 * M02) * det;
				float Invmm12 = (M20 * M01 - M00 * M21) * det;

				float Invmm20 = (M01 * M12 - M02 * M11) * det;
				float Invmm21 = (M10 * M02 - M00 * M12) * det;
				float Invmm22 = (M00 * M11 - M10 * M01) * det;

				// map ray vertices to ellipsoid space

				glm::vec3 ta((a.x - pos.x) * Invmm00 + (a.y - pos.y) * Invmm01 + (a.z - pos.z) * Invmm02,
							 (a.x - pos.x) * Invmm10 + (a.y - pos.y) * Invmm11 + (a.z - pos.z) * Invmm12,
							 (a.x - pos.x) * Invmm20 + (a.y - pos.y) * Invmm21 + (a.z - pos.z) * Invmm22);

				glm::vec3 tb((b.x - pos.x) * Invmm00 + (b.y - pos.y) * Invmm01 + (b.z - pos.z) * Invmm02,
							 (b.x - pos.x) * Invmm10 + (b.y - pos.y) * Invmm11 + (b.z - pos.z) * Invmm12,
							 (b.x - pos.x) * Invmm20 + (b.y - pos.y) * Invmm21 + (b.z - pos.z) * Invmm22);

				// collision detection between ray and sphere

				glm::vec3 d = tb - ta;

				float k1 = d.x * d.x + d.y * d.y + d.z * d.z;
				float k2 = d.x * ta.x + d.y * ta.y + d.z * ta.z;
				float k3 = ta.x * ta.x + ta.y * ta.y + ta.z * ta.z - 1;

				float delta = k2 * k2 - k1 * k3;

				if (delta > eps)
				{

					delta = sqrtf(delta);

					if (k1 > -eps && k1 < eps)
						k1 = eps;

					float invk1 = 1.0f / k1;

					t1 = (-k2 + delta) * invk1;
					t2 = (-k2 - delta) * invk1;

					if (t2 < t1)
					{
						float temp = t1;
						t1 = t2;
						t2 = temp;
					}

					glm::vec3 p1 = ta + t1 * d;
					glm::vec3 p2 = ta + t2 * d;

					// unmap point to world space

					u1.x = p1.x * M00 + p1.y * M10 + p1.z * M20 + pos.x;
					u1.y = p1.x * M01 + p1.y * M11 + p1.z * M21 + pos.y;
					u1.z = p1.x * M02 + p1.y * M12 + p1.z * M22 + pos.z;

					u2.x = p2.x * M00 + p2.y * M10 + p2.z * M20 + pos.x;
					u2.y = p2.x * M01 + p2.y * M11 + p2.z * M21 + pos.y;
					u2.z = p2.x * M02 + p2.y * M12 + p2.z * M22 + pos.z;

					return DO_INTERSECTS;

				}

				return DO_NOT_INTERSECTS;

			}
			
		} // end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{

			//////////////////////////////////////////////////////////////
			// ray - quad intgersection test

			static const unsigned int QUAD_DO_NOT_INTERSECTS_RAY = 0;
			static const unsigned int QUAD_INTERSECTS_RAY		 = 1;

			/////////////////////////////////////////////////
			// intersection beetween triangle and line
			// returns point of intersection
			// computes normal

			static bool QuadVsRay(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
								  const glm::vec3& ra, const glm::vec3& rb, glm::vec3& p,
								  float& t,
								  const float eps = vml::math::EPSILON)
			{
				glm::vec3 v, n, d;
				float denum;

				// compute normal

				n = cross(p1 - p0, p2 - p0);
				n = glm::normalize(n);

				// compute intersection with 
				// infinite plane

				d = rb - ra;
				t = dot(p0 - ra, n);
				denum = dot(n, d);

				if (fabs(denum) < eps)
					denum = eps;

				t /= denum;

				// store intersection point

				p = ra + t * d;

				// check which side the point lies on
				// respect segment p0-p1
				// computes the side tanget using
				// cross product between segmetn a-b
				// and normal

				v = cross(p1 - p0, n);

				if (dot(v, p0 - p) < 0)
					return QUAD_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment p2-p1
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				v = cross(p2 - p1, n);

				if (dot(v, p1 - p) < 0)
					return QUAD_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment p3-p2
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				v = cross(p3 - p2, n);

				if (dot(v, p2 - p) < 0)
					return QUAD_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment p0-p3
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				v = cross(p0 - p3, n);

				if (dot(v, p3 - p) < 0)
					return QUAD_DO_NOT_INTERSECTS_RAY;

				return QUAD_INTERSECTS_RAY;
			}

		} // end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			
			//////////////////////////////////////////////////////////////
			// ray-triangle axis aligned bounding box flags

			static const unsigned int TRIANGLE_DO_NOT_INTERSECTS_AABBOX = 0;
			static const unsigned int TRIANGLE_INTERSECTS_AABBOX        = 1;
	
			//////////////////////////////////////////////////////
			//    use separating axis theorem to test overlap 
			//    between triangle and box 
			//    need to test for overlap in these directions: 
			//    1) the {x,y,z}-directions (actually, since we use 
			//       the AABB of the triangle 
			//       we do not even need to test these) 
			//    2) normal of the triangle 
			//    3) crossproduct(edge from tri, {x,y,z}-directin) 
			//       this gives 3x3=9 more tests

			static int TriangleVsAABB(const glm::vec3& boxcenter,
									  const glm::vec3& boxhalfsize,
									  const glm::vec3& trivert0,
									  const glm::vec3& trivert1,
									  const glm::vec3& trivert2,
									  const glm::vec3& normal)
			{

				float min, max;
				float p0, p1, p2;
				float d;
				float fex, fey, fez;
				float e0[3] = { 0 };
				float e1[3] = { 0 };
				float e2[3] = { 0 };
				float v0[3] = { 0 };
				float v1[3] = { 0 };
				float v2[3] = { 0 };
				float vmin[3] = { 0 };
				float vmax[3] = { 0 };

				// center triangle at box center

				v0[0] = trivert0.x - boxcenter.x;
				v0[1] = trivert0.y - boxcenter.y;
				v0[2] = trivert0.z - boxcenter.z;

				v1[0] = trivert1.x - boxcenter.x;
				v1[1] = trivert1.y - boxcenter.y;
				v1[2] = trivert1.z - boxcenter.z;

				v2[0] = trivert2.x - boxcenter.x;
				v2[1] = trivert2.y - boxcenter.y;
				v2[2] = trivert2.z - boxcenter.z;

				// compute triangle edges 

				e0[0] = v1[0] - v0[0];
				e0[1] = v1[1] - v0[1];
				e0[2] = v1[2] - v0[2];

				e1[0] = v2[0] - v1[0];
				e1[1] = v2[1] - v1[1];
				e1[2] = v2[2] - v1[2];

				e2[0] = v0[0] - v2[0];
				e2[1] = v0[1] - v2[1];
				e2[2] = v0[2] - v2[2];

				//  test the 9 tests for axis overlapping 

				// axis test x01

				fex = fabs(e0[0]);
				fey = fabs(e0[1]);
				fez = fabs(e0[2]);

				p0 = e0[2] * v0[1] - e0[1] * v0[2];
				p2 = e0[2] * v2[1] - e0[1] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y02

				p0 = -e0[2] * v0[0] + e0[0] * v0[2];
				p2 = -e0[2] * v2[0] + e0[0] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2) { min = p0; max = p2; }

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z12

				p1 = e0[1] * v1[0] - e0[0] * v1[1];
				p2 = e0[1] * v2[0] - e0[0] * v2[1];

				min = p1;
				max = p2;

				if (p2 < p1)
				{
					min = p2;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				fex = fabs(e1[0]);
				fey = fabs(e1[1]);
				fez = fabs(e1[2]);

				// axis test x01

				p0 = e1[2] * v0[1] - e1[1] * v0[2];
				p2 = e1[2] * v2[1] - e1[1] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y02

				p0 = -e1[2] * v0[0] + e1[0] * v0[2];
				p2 = -e1[2] * v2[0] + e1[0] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z0

				p0 = e1[1] * v0[0] - e1[0] * v0[1];
				p1 = e1[1] * v1[0] - e1[0] * v1[1];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				fex = fabs(e2[0]);
				fey = fabs(e2[1]);
				fez = fabs(e2[2]);

				// axis test x02

				p0 = e2[2] * v0[1] - e2[1] * v0[2];
				p1 = e2[2] * v1[1] - e2[1] * v1[2];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y01

				p0 = -e2[2] * v0[0] + e2[0] * v0[2];
				p1 = -e2[2] * v1[0] + e2[0] * v1[2];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z12 

				p1 = e2[1] * v1[0] - e2[0] * v1[1];
				p2 = e2[1] * v2[0] - e2[0] * v2[1];

				min = p1;
				max = p2;

				if (p2 < p1)
				{
					min = p2;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				//  first test overlap in the {x,y,z}-directions 
				//  find min, max of the triangle each direction, 
				//  and test for overlap in 
				//  that direction -- this is equivalent to testing 
				//  a minimal AABB around 
				//  the triangle against the AABB 

				// test in X-direction 

				min = v0[0];
				max = v0[0];

				if (v1[0] < min) min = v1[0];
				if (v1[0] > max) max = v1[0];
				if (v2[0] < min) min = v2[0];
				if (v2[0] > max) max = v2[0];

				if (min > boxhalfsize[0] || max < -boxhalfsize[0])
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// test in Y-direction 

				min = v0[1];
				max = v0[1];

				if (v1[1] < min) min = v1[1];
				if (v1[1] > max) max = v1[1];
				if (v2[1] < min) min = v2[1];
				if (v2[1] > max) max = v2[1];

				if (min > boxhalfsize[1] || max < -boxhalfsize[1])
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// test in Z-direction 

				min = v0[2];
				max = v0[2];

				if (v1[2] < min) min = v1[2];
				if (v1[2] > max) max = v1[2];
				if (v2[2] < min) min = v2[2];
				if (v2[2] > max) max = v2[2];

				if (min > boxhalfsize[2] || max < -boxhalfsize[2])
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				//  test if the box intersects the 
				//  plane of the triangle 

				// test x

				if (normal[0] > 0.0f)
				{
					vmin[0] = -boxhalfsize[0];
					vmax[0] = boxhalfsize[0];
				}
				else
				{
					vmin[0] = boxhalfsize[0];
					vmax[0] = -boxhalfsize[0];
				}

				// test y

				if (normal[1] > 0.0f)
				{
					vmin[1] = -boxhalfsize[1];
					vmax[1] = boxhalfsize[1];
				}
				else
				{
					vmin[1] = boxhalfsize[1];
					vmax[1] = -boxhalfsize[1];
				}

				// test z

				if (normal[2] > 0.0f)
				{
					vmin[2] = -boxhalfsize[2];
					vmax[2] = boxhalfsize[2];
				}
				else
				{
					vmin[2] = boxhalfsize[2];
					vmax[2] = -boxhalfsize[2];
				}

				// test max and min 

				d = -(normal[0] * v0[0] + normal[1] * v0[1] + normal[2] * v0[2]);

				if (normal[0] * vmin[0] +
					normal[1] * vmin[1] +
					normal[2] * vmin[2] + d > 0.0f)
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				if (normal[0] * vmax[0] +
					normal[1] * vmax[1] +
					normal[2] * vmax[2] + d > 0.0f)
					return TRIANGLE_INTERSECTS_AABBOX;

				// box and triangle don't overlap 

				return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;
			}
			

		} // end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			//////////////////////////////////////////////////////////////
			// ray-triangle intersections flags

			static const unsigned int TRIANGLE_DO_NOT_INTERSECTS_RAY = 0;
			static const unsigned int TRIANGLE_INTERSECTS_RAY		 = 1;

			/////////////////////////////////////////////////
			// intersection beetween triangle and line
			// returns point of intersection
			// computes normal

			static bool TriangleVsRay(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
									  const glm::vec3& ra, const glm::vec3& rb, glm::vec3& p,
									  float& t,
									  const float eps = vml::math::EPSILON)
			{
				glm::vec3 v, n, d;
				float denum;

				// compute normal

				n = cross(b - a, c - a);
				n = glm::normalize(n);

				// compute intersection with 
				// infinite plane

				d = rb - ra;
				t = dot(a - ra, n);
				denum = dot(n, d);

				if (fabs(denum) < eps)
					denum = eps;

				t /= denum;

				// store intersection point

				p = ra + t * d;

				// check which side the point lies on
				// respect segment a-b
				// computes the side tanget using
				// cross product between segmetn a-b
				// and normal

				v = cross(b - a, n);

				if (dot(v, a - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment c-b
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				v = cross(c - b, n);

				if (dot(v, b - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment a-c
				// computes the side tanget using
				// cross product between segmetn a-c
				// and normal

				v = cross(a - c, n);

				if (dot(v, c - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				return TRIANGLE_INTERSECTS_RAY;
			}
			
			/////////////////////////////////////////////////
			// intersection beetween triangle and line segment
			// returns point of intersection
			// computes normal

			static bool TriangleVSLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
									   const glm::vec3& ra, const glm::vec3& rb,
									   glm::vec3& p,
									   float& t,
									   const float eps = vml::math::EPSILON)
			{
				glm::vec3 V, N, D;
				float denum;

				// compute normal

				N = glm::cross(b - a, c - a);

				N = glm::normalize(N);

				// compute intersection with 
				// infinite plane

				D = rb - ra;

				t = dot(a - ra, N);

				denum = dot(N, D);

				if (fabs(denum) < eps)
					denum = eps;

				t /= denum;

				if (t < -eps || t > 1.0f + eps)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// store intersection point

				p = ra + t * D;

				// check which side the point lies on
				// respect segment a-b
				// computes the side tanget using
				// cross product between segmetn a-b
				// and normal

				V = cross(b - a, N);

				if (dot(V, a - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment c-b
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				V = cross(c - b, N);

				if (dot(V, b - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment a-c
				// computes the side tanget using
				// cross product between segmetn a-c
				// and normal

				V = cross(a - c, N);

				if (dot(V, c - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				return TRIANGLE_INTERSECTS_RAY;
			}
			
		} // end of intersections namespace
	}	// end of geo3d namespace
}	// end of vml namespace

