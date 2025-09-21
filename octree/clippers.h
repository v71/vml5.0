#pragma once

#include <vector>
#include <vml5.0/math/common/ranges.h>
#include <vml5.0/octree/octreetypes.h>

namespace vml
{
	namespace geo3d
	{
		namespace clippers
		{

			class AABBoxTriangleClipper
			{
				
				private:
					
					// ----------------------------------------------------------------------------------
					// clipped triangles

					vml::geo3d::Vertex VertexArray[16];		// 12 maximum intersection point with a cube, but i have aligned the struct to 16
					size_t			   VertexCount;
					
					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is ( -1,0,0 )
					// polygon is defined as an array
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstLeftPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																							const glm::vec3& p0,
																							const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> points;

						size_t sourcepts = pverts.size();

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = p0.x - pverts[s1].Pos.x;
							c2 = p0.x - pverts[s2].Pos.x;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s1].Pos.x - pverts[s2].Pos.x;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = p0.x - pverts[s1].Pos.x;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pverts[s1]);

								points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s1].Pos.x - pverts[s2].Pos.x;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = p0.x - pverts[s1].Pos.x;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));


								points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								points.emplace_back(pverts[s1]);
							}

						}

						return points;
					}
					
					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is ( 0,-1,0 )
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstBottomPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																							  const glm::vec3& p0,
																							  const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> Points;

						size_t sourcepts = pverts.size();

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = p0.y - pverts[s1].Pos.y;
							c2 = p0.y - pverts[s2].Pos.y;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s1].Pos.y - pverts[s2].Pos.y;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = p0.y - pverts[s1].Pos.y;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								Points.emplace_back(pverts[s1]);

								Points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s1].Pos.y - pverts[s2].Pos.y;

								if (denom > -eps && denom < eps)
									denom = vml::math::EPSILON;

								num = p0.y - pverts[s1].Pos.y;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								Points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								Points.emplace_back(pverts[s1]);
							}

						}

						return Points;
					}

					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is ( 0,0,-1)
					// polygon is defined as an array
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstBackPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																							const glm::vec3& p0,
																							const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> points;

						size_t sourcepts = pverts.size();

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = p0.z - pverts[s1].Pos.z;
							c2 = p0.z - pverts[s2].Pos.z;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s1].Pos.z - pverts[s2].Pos.z;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = p0.z - pverts[s1].Pos.z;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pverts[s1]);

								points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s1].Pos.z - pverts[s2].Pos.z;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = p0.z - pverts[s1].Pos.z;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								points.emplace_back(pverts[s1]);
							}

						}

						return points;
					}

					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is (1,0,0)
					// polygon is defined as an array
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise
					// this functions clips also texture coords

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstRightPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																							 const glm::vec3& p0,
																							 const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> points;

						size_t sourcepts = pverts.size();

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = pverts[s1].Pos.x - p0.x;
							c2 = pverts[s2].Pos.x - p0.x;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s2].Pos.x - pverts[s1].Pos.x;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.x - p0.x;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pverts[s1]);

								points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s2].Pos.x - pverts[s1].Pos.x;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.x - p0.x;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								points.emplace_back(pverts[s1]);
							}

						}

						return points;
					}

					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is ( 0,1,0)
					// polygon is defined as an array
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstTopPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																						   const glm::vec3& p0,
																						   const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> points;

						size_t sourcepts = pverts.size();

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = pverts[s1].Pos.y - p0.y;
							c2 = pverts[s2].Pos.y - p0.y;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s2].Pos.y - pverts[s1].Pos.y;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.y - p0.y;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pverts[s1]);

								points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s2].Pos.y - pverts[s1].Pos.y;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.y - p0.y;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								points.emplace_back(pverts[s1]);
							}

						}

						return points;
					}

					// ----------------------------------------------------------------------------------
					// Clips a convex polygon against a plane, whose normal is ( 0,0,1)
					// polygon is defined as an array
					// of sequential vertices in clockwise order
					// if polygon is not convex, incorrect results might arise

					const std::vector<vml::geo3d::Vertex> ClipConvexPolygonAgainstFrontPlane(const std::vector<vml::geo3d::Vertex>& pverts,
																							 const glm::vec3& p0,
																							 const float& eps = vml::math::EPSILON)
					{
						size_t s1, s2;
						float c1, c2;
						unsigned int flag;
						float u, num, denom;
						std::vector<vml::geo3d::Vertex> points;

						size_t sourcepts = pverts.size();

						// TO DO : check if polygon is at zero distance from the plane

						for (s1 = 0; s1 < sourcepts; s1++)
						{
							s2 = (s1 + 1) % sourcepts;

							c1 = pverts[s1].Pos.z - p0.z;
							c2 = pverts[s2].Pos.z - p0.z;

							flag = (c1 < 0) << 0 | (c2 < 0) << 1;

							// line is out

							if (flag == 0x0)
							{
								// do nothing
							}

							// line is intersecting coming in

							if (flag == 0x1)
							{

								// computes intersection point

								denom = pverts[s2].Pos.z - pverts[s1].Pos.z;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.z - p0.z;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pverts[s1]);

								points.emplace_back(pi);

							}

							// line is intersecting coming out

							if (flag == 0x2)
							{
								// computes intersection point

								denom = pverts[s2].Pos.z - pverts[s1].Pos.z;

								if (denom > -eps && denom < eps)
									denom = eps;

								num = pverts[s1].Pos.z - p0.z;

								u = -num / denom;

								// Get the intersection point.

								vml::geo3d::Vertex pi(pverts[s1].Pos + u * (pverts[s2].Pos - pverts[s1].Pos),
													  pverts[s1].Normal + u * (pverts[s2].Normal - pverts[s1].Normal),
													  pverts[s1].UV + u * (pverts[s2].UV - pverts[s1].UV));

								points.emplace_back(pi);

							}

							// line is in

							if (flag == 0x3)
							{
								points.emplace_back(pverts[s1]);
							}

						}

						return points;
					}
				
				public:
					
					// ----------------------------------------------------------------------------------
					// clips a triangle against an axis aligned bounding box

					const unsigned int ClipTriangleAgainstAABB(const vml::geo3d::Vertex& p0, const vml::geo3d::Vertex& p1, const vml::geo3d::Vertex& p2,
															   const glm::vec3& bmin, const glm::vec3& bmax)
					{
						VertexCount = 0;

						// compute triangle bounding box

						glm::vec3 tbmin(vml::math::ranges::Min(p0.Pos.x, p1.Pos.x, p2.Pos.x),
										vml::math::ranges::Min(p0.Pos.y, p1.Pos.y, p2.Pos.y),
										vml::math::ranges::Min(p0.Pos.z, p1.Pos.z, p2.Pos.z));

						glm::vec3 tbmax(vml::math::ranges::Max(p0.Pos.x, p1.Pos.x, p2.Pos.x),
										vml::math::ranges::Max(p0.Pos.y, p1.Pos.y, p2.Pos.y),
										vml::math::ranges::Max(p0.Pos.z, p1.Pos.z, p2.Pos.z));

						// triangle is out, no verts are generated

						if ((tbmax.x < bmin.x || tbmin.x > bmax.x) ||
							(tbmax.y < bmin.y || tbmin.y > bmax.y) ||
							(tbmax.z < bmin.z || tbmin.z > bmax.z))
						{
							return vml::geo3d::Results::OUTSIDE;
						}

						// triangle is contained in bounding box

						if ((tbmin.x >= bmin.x && tbmax.x <= bmax.x) &&
							(tbmin.y >= bmin.y && tbmax.y <= bmax.y) &&
							(tbmin.z >= bmin.z && tbmax.z <= bmax.z))
						{
							VertexArray[0] = p0;
							VertexArray[1] = p1;
							VertexArray[2] = p2;
							VertexCount = 3;

							return vml::geo3d::Results::INSIDE;
						}

						// if its not outisde or inside, check for intersection

						std::vector<vml::geo3d::Vertex> SourcePoints = { p0,p1,p2 };

						// clip triangle cyclically until all planes have been intersected

						std::vector<vml::geo3d::Vertex> verts;

						verts = ClipConvexPolygonAgainstLeftPlane(SourcePoints, bmin);
						SourcePoints = verts;
						verts = ClipConvexPolygonAgainstBottomPlane(SourcePoints, bmin);
						SourcePoints = verts;
						verts = ClipConvexPolygonAgainstBackPlane(SourcePoints, bmin);
						SourcePoints = verts;
						verts = ClipConvexPolygonAgainstRightPlane(SourcePoints, bmax);
						SourcePoints = verts;
						verts = ClipConvexPolygonAgainstTopPlane(SourcePoints, bmax);
						SourcePoints = verts;
						verts = ClipConvexPolygonAgainstFrontPlane(SourcePoints, bmax);

						// return the destination vertices in clockwise order

						if (verts.size() > 0)
						{
							VertexCount = 0;

							// decompose convex polygon vertices computed from the clipped triangle
							// into triangles

							for (size_t j = 1, k = 2; k < verts.size(); ++k, ++j)
							{
								VertexArray[VertexCount++] = verts[0];
								VertexArray[VertexCount++] = verts[j];
								VertexArray[VertexCount++] = verts[k];
							}

							return vml::geo3d::Results::DOES_INTERSECT;
						}

						return vml::geo3d::Results::OUTSIDE;

					}
					
					// ----------------------------------------------------
					// getters

					size_t GetTrianglesCount() const { return VertexCount / 3; }

					const vml::geo3d::Vertex& GetTriangleV0(size_t i) const { return VertexArray[i * 3]; }
					const vml::geo3d::Vertex& GetTriangleV1(size_t i) const { return VertexArray[i * 3 + 1]; }
					const vml::geo3d::Vertex& GetTriangleV2(size_t i) const { return VertexArray[i * 3 + 2]; }
					
					// ----------------------------------------------------
					// ctro / dtor

					AABBoxTriangleClipper()
					{
						VertexCount = 0;
					}

					~AABBoxTriangleClipper()
					{}

			};
		}
	}
}