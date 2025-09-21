#pragma once

////////////////////////////////////////////////////////////////////////////////////
// 3d predicates

////////////////////////////////////////////////////////////////////////////////////
// Line

namespace vml
{
	namespace geo3d
	{
		template<typename T>
		struct Line
		{
			vml::math::vec3<T> P;
			vml::math::vec3<T> Q;

			// -----------------------------------------------------
			// ctor / dtor

			Line() : P(vml::math::vec3<T>(0, 0)), Q(vml::math::vec3<T>(0, 0)) {}
			Line(const vml::math::vec3<T>& p, const vml::math::vec3<T>& q) : P(p), Q(q) {}
			Line(const T px, const T py, const T qx, const T qy) : P(px, py), Q(qx, qy) {}
		};
		
	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////
//  Ray

namespace vml
{
	namespace geo3d
	{
		template<typename T>
		struct Ray
		{
			vml::math::vec3<T> P;	// position
			vml::math::vec3<T> Dir;	// unit direction

			// -----------------------------------------------------
			// ctor / dtor

			Ray() : P(vml::math::vec3<T>(0, 0)), Dir(vml::math::vec3<T>(0, 0)) {}
			Ray(const vml::math::vec3<T>& p, const vml::math::vec3<T>& dir) : P(p), Dir(dir) {}
			Ray(const T px, const T py, const T dx, const T dy) : P(px, py), Dir(dx, dy) {}
		};

	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{

		template<typename T>
		struct Triangle
		{
			vml::math::vec3<T> P0;	// first vertex
			vml::math::vec3<T> P1;	// second vertex
			vml::math::vec3<T> P2;	// third vertex
			vml::math::vec3<T> Normal;

			void Set(const vml::math::vec3<T>& p0, const vml::math::vec3<T>& p1, const vml::math::vec3<T>& p2)
			{
				P0 = p0;
				P1 = p1;
				P2 = p2;
				vml::math::vec3<T> u = p0 - p1;
				vml::math::vec3<T> v = p0 - p2;
				Normal = vml::math::Normalize(vml::math::Cross(u, v));
			}

			// -----------------------------------------------------
			// ctor / dtor

			Triangle() :P0(0, 0, 0), P1(0, 0, 0), P2(0, 0, 0), Normal(0, 0, 0)
			{
			}

			Triangle(const vml::math::vec3<T>& p0, const vml::math::vec3<T>& p1, const vml::math::vec3<T>& p2)
			{
				Set(P0, P1, P2);
			}

			Triangle(const T p0x, const T p0y, const T p0z, 
				     const T p1x, const T p1y, const T p1z, 
					 const T p2x, const T p2y, const T p2z)
			{
				Set(vml::math::vec3<T>(p0x, p0y, p0z), vml::math::vec3<T>(p1x, p1y, p1z), vml::math::vec3<T>(p2x, p2y, p2z));
			}

		};

	}	// end of geo2d namespace

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// sphere class

namespace vml
{
	namespace geo2d
	{
		template<typename T>
		struct Sphere
		{
			vml::math::vec3<T>P;	// position
			T R;					// radius

			// -----------------------------------------------------
			// ctor / dtor

			Sphere() : P(vml::math::vec3<T>(0, 0)), R(0) {}
			Sphere(const vml::math::vec3<T>& p, const float radius) : P(vml::math::vec3<T>(p)), R(radius) {}
			Sphere(const T px, const T py, const T radius) : P(px, py), R(radius) {}
		};

	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////
// plane class

namespace vml
{
	namespace geo3d
	{
		template<typename T>
		class Plane
		{

			private:

				// ------------------------------------------------------------------
				// private data 

				vml::math::vec3<T> P0;
				vml::math::vec3<T> Normal;
				T D;

			public:

				// ------------------------------------------------------------------
				// public function

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const vml::math::vec3<T>& p0, const vml::math::vec3<T>& p1, const vml::math::vec3<T>& p2)
				{
					P0 = p0;
					vml::math::vec3<T> u = p0 - p1;
					vml::math::vec3<T> v = p0 - p2;
					Normal = vml::math::Normalize(vml::math::Cross(u, v));
					D = -vml::math::Dot(P0, Normal);
				}

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetP0() const { return P0; }
				const glm::vec3& GetNormal() const { return Normal; }
				T GetD() const { return D; }

				// ------------------------------------------------------------------
				// ctor / dtor

				Plane()
				{
					P0 = vml::math::vec3<T>(0, 0, 0);
					Normal = vml::math::vec3<T>(0, 0, 0);
					D = (T)0.0f;
				}

				Plane(const vml::math::vec3<T>& p0, const vml::math::vec3 < T>& p1, const vml::math::vec3<T>& p2, const vml::math::vec3<T>& p3)
				{
					Set(p0, p1, p2, p3);
				}

				Plane(const T p0x, const T p0y, const T p0z,
					  const T p1x, const T p1y, const T p1z,
					  const T p2x, const T p2y, const T p2z)
				{
					Set(vml::math::vec3<T>(p0x,p0y,p0z),vml::math::vec3<T>(p1x,p1y,p1z),vml::math::vec3<T>(p2x,p2y,p2z));
				}

				~Plane()
				{
				}

		};

	} // end of namespace goe3d

}	// end of vml namespace


////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box class

namespace vml
{
	namespace geo3d
	{

		class AABBox
		{

			private:

				glm::vec3 Min;
				glm::vec3 Max;
				glm::vec3 Center;
				glm::vec3 Extents;
				glm::vec3 HalfExtents;

			public:

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetMin()		  const { return Min; }
				const glm::vec3& GetMax()		  const { return Max; }
				const glm::vec3& GetCenter()	  const { return Center; }
				const glm::vec3& GetHalfExtents() const { return HalfExtents; }
				const glm::vec3& GetExtents()	  const { return Extents; }

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min			= bmin;
					Max			= bmax;
					Extents	    = Max - Min;
					Center	    = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				AABBox()
				{
					Min			= glm::vec3(0, 0, 0);
					Max		    = glm::vec3(0, 0, 0);
					Extents	    = glm::vec3(0, 0, 0);
					Center	    = glm::vec3(0, 0, 0);
					HalfExtents = glm::vec3(0, 0, 0);
				}

				AABBox(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min			= bmin;
					Max			= bmax;
					Extents		= Max - Min;
					Center	    = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				~AABBox()
				{}

		};

	}	// end of geo3d namespace

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box class

namespace vml
{
	namespace geo3d
	{

		class AOBBox
		{

			private:

				glm::vec3 Min;
				glm::vec3 Max;
				glm::vec3 Center;
				glm::vec3 Extents;						// Extents
				glm::vec3 HalfExtents;					// Half Extents
				glm::vec3 OrientedBoundingBox[8];		// Oriented bounding box vertices

			public:

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetMin()		  const { return Min; }
				const glm::vec3& GetMax()		  const { return Max; }
				const glm::vec3& GetCenter()	  const { return Center; }
				const glm::vec3& GetHalfExtents() const { return HalfExtents; }
				const glm::vec3& GetExtents()	  const { return Extents; }

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& bmin, const glm::vec3& bmax, const float* m)
				{

					// cache transfomred vertices

					glm::vec3 minx(bmin.x * m[0], bmin.x * m[1], bmin.x * m[2]);
					glm::vec3 miny(bmin.y * m[4], bmin.y * m[5], bmin.y * m[6]);
					glm::vec3 minz(bmin.z * m[8], bmin.z * m[9], bmin.z * m[10]);

					glm::vec3 maxx(bmax.x * m[0], bmax.x * m[1], bmax.x * m[2]);
					glm::vec3 maxy(bmax.y * m[4], bmax.y * m[5], bmax.y * m[6]);
					glm::vec3 maxz(bmax.z * m[8], bmax.z * m[9], bmax.z * m[10]);

					// fill models 's axis oriented bounding box
					// the bounding box is composed of 8 vertices
					// organized in a 32 float array

					OrientedBoundingBox[0].x = minx[0] + miny[0] + minz[0] + m[12];
					OrientedBoundingBox[0].y = minx[1] + miny[1] + minz[1] + m[13];
					OrientedBoundingBox[0].z = minx[2] + miny[2] + minz[2] + m[14];

					OrientedBoundingBox[1].x = maxx[0] + miny[0] + minz[0] + m[12];
					OrientedBoundingBox[1].y = maxx[1] + miny[1] + minz[1] + m[13];
					OrientedBoundingBox[1].z = maxx[2] + miny[2] + minz[2] + m[14];

					OrientedBoundingBox[2].x = maxx[0] + maxy[0] + minz[0] + m[12];
					OrientedBoundingBox[2].y = maxx[1] + maxy[1] + minz[1] + m[13];
					OrientedBoundingBox[2].z = maxx[2] + maxy[2] + minz[2] + m[14];

					OrientedBoundingBox[3].x = minx[0] + maxy[0] + minz[0] + m[12];
					OrientedBoundingBox[3].y = minx[1] + maxy[1] + minz[1] + m[13];
					OrientedBoundingBox[3].z = minx[2] + maxy[2] + minz[2] + m[14];

					OrientedBoundingBox[4].x = minx[0] + miny[0] + maxz[0] + m[12];
					OrientedBoundingBox[4].y = minx[1] + miny[1] + maxz[1] + m[13];
					OrientedBoundingBox[4].z = minx[2] + miny[2] + maxz[2] + m[14];

					OrientedBoundingBox[5].x = maxx[0] + miny[0] + maxz[0] + m[12];
					OrientedBoundingBox[5].y = maxx[1] + miny[1] + maxz[1] + m[13];
					OrientedBoundingBox[5].z = maxx[2] + miny[2] + maxz[2] + m[14];

					OrientedBoundingBox[6].x = maxx[0] + maxy[0] + maxz[0] + m[12];
					OrientedBoundingBox[6].y = maxx[1] + maxy[1] + maxz[1] + m[13];
					OrientedBoundingBox[6].z = maxx[2] + maxy[2] + maxz[2] + m[14];

					OrientedBoundingBox[7].x = minx[0] + maxy[0] + maxz[0] + m[12];
					OrientedBoundingBox[7].y = minx[1] + maxy[1] + maxz[1] + m[13];
					OrientedBoundingBox[7].z = minx[2] + maxy[2] + maxz[2] + m[14];

					// compute axis aligned bounding box
					// the bounding box is computed searching
					// for the nearest and farthest vertices
					// in the oriented bounding box
					// accordint to vertex ordering, axis are :
					// 0-1 x axis
					// 0-3 y axis
					// 0-4 z axis

					// sets large values for lowest vertex

					Min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					Max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					// check for lowest x

					if (OrientedBoundingBox[0].x < Min.x) Min.x = OrientedBoundingBox[0].x;
					if (OrientedBoundingBox[1].x < Min.x) Min.x = OrientedBoundingBox[1].x;
					if (OrientedBoundingBox[2].x < Min.x) Min.x = OrientedBoundingBox[2].x;
					if (OrientedBoundingBox[3].x < Min.x) Min.x = OrientedBoundingBox[3].x;
					if (OrientedBoundingBox[4].x < Min.x) Min.x = OrientedBoundingBox[4].x;
					if (OrientedBoundingBox[5].x < Min.x) Min.x = OrientedBoundingBox[5].x;
					if (OrientedBoundingBox[6].x < Min.x) Min.x = OrientedBoundingBox[6].x;
					if (OrientedBoundingBox[7].x < Min.x) Min.x = OrientedBoundingBox[7].x;

					// check for lowest y

					if (OrientedBoundingBox[0].y < Min.y) Min.y = OrientedBoundingBox[0].y;
					if (OrientedBoundingBox[1].y < Min.y) Min.y = OrientedBoundingBox[1].y;
					if (OrientedBoundingBox[2].y < Min.y) Min.y = OrientedBoundingBox[2].y;
					if (OrientedBoundingBox[3].y < Min.y) Min.y = OrientedBoundingBox[3].y;
					if (OrientedBoundingBox[4].y < Min.y) Min.y = OrientedBoundingBox[4].y;
					if (OrientedBoundingBox[5].y < Min.y) Min.y = OrientedBoundingBox[5].y;
					if (OrientedBoundingBox[6].y < Min.y) Min.y = OrientedBoundingBox[6].y;
					if (OrientedBoundingBox[7].y < Min.y) Min.y = OrientedBoundingBox[7].y;

					// check for lowest z

					if (OrientedBoundingBox[0].z < Min.z) Min.z = OrientedBoundingBox[0].z;
					if (OrientedBoundingBox[1].z < Min.z) Min.z = OrientedBoundingBox[1].z;
					if (OrientedBoundingBox[2].z < Min.z) Min.z = OrientedBoundingBox[2].z;
					if (OrientedBoundingBox[3].z < Min.z) Min.z = OrientedBoundingBox[3].z;
					if (OrientedBoundingBox[4].z < Min.z) Min.z = OrientedBoundingBox[4].z;
					if (OrientedBoundingBox[5].z < Min.z) Min.z = OrientedBoundingBox[5].z;
					if (OrientedBoundingBox[6].z < Min.z) Min.z = OrientedBoundingBox[6].z;
					if (OrientedBoundingBox[7].z < Min.z) Min.z = OrientedBoundingBox[7].z;

					// check for highest x

					if (OrientedBoundingBox[0].x > Max.x) Max.x = OrientedBoundingBox[0].x;
					if (OrientedBoundingBox[1].x > Max.x) Max.x = OrientedBoundingBox[1].x;
					if (OrientedBoundingBox[2].x > Max.x) Max.x = OrientedBoundingBox[2].x;
					if (OrientedBoundingBox[3].x > Max.x) Max.x = OrientedBoundingBox[3].x;
					if (OrientedBoundingBox[4].x > Max.x) Max.x = OrientedBoundingBox[4].x;
					if (OrientedBoundingBox[5].x > Max.x) Max.x = OrientedBoundingBox[5].x;
					if (OrientedBoundingBox[6].x > Max.x) Max.x = OrientedBoundingBox[6].x;
					if (OrientedBoundingBox[7].x > Max.x) Max.x = OrientedBoundingBox[7].x;

					// check for highest y

					if (OrientedBoundingBox[0].y > Max.y) Max.y = OrientedBoundingBox[0].y;
					if (OrientedBoundingBox[1].y > Max.y) Max.y = OrientedBoundingBox[1].y;
					if (OrientedBoundingBox[2].y > Max.y) Max.y = OrientedBoundingBox[2].y;
					if (OrientedBoundingBox[3].y > Max.y) Max.y = OrientedBoundingBox[3].y;
					if (OrientedBoundingBox[4].y > Max.y) Max.y = OrientedBoundingBox[4].y;
					if (OrientedBoundingBox[5].y > Max.y) Max.y = OrientedBoundingBox[5].y;
					if (OrientedBoundingBox[6].y > Max.y) Max.y = OrientedBoundingBox[6].y;
					if (OrientedBoundingBox[7].y > Max.y) Max.y = OrientedBoundingBox[7].y;

					// check for highest z

					if (OrientedBoundingBox[0].z > Max.z) Max.z = OrientedBoundingBox[0].z;
					if (OrientedBoundingBox[1].z > Max.z) Max.z = OrientedBoundingBox[1].z;
					if (OrientedBoundingBox[2].z > Max.z) Max.z = OrientedBoundingBox[2].z;
					if (OrientedBoundingBox[3].z > Max.z) Max.z = OrientedBoundingBox[3].z;
					if (OrientedBoundingBox[4].z > Max.z) Max.z = OrientedBoundingBox[4].z;
					if (OrientedBoundingBox[5].z > Max.z) Max.z = OrientedBoundingBox[5].z;
					if (OrientedBoundingBox[6].z > Max.z) Max.z = OrientedBoundingBox[6].z;
					if (OrientedBoundingBox[7].z > Max.z) Max.z = OrientedBoundingBox[7].z;

					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

				}

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
						 const glm::vec3& p4, const glm::vec3& p5, const glm::vec3& p6, const glm::vec3& p7)

				{

					Min         = p0;
					Max         = p6;
					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

					OrientedBoundingBox[0] = p0;
					OrientedBoundingBox[1] = p1;
					OrientedBoundingBox[2] = p2;
					OrientedBoundingBox[3] = p3;
					OrientedBoundingBox[4] = p4;
					OrientedBoundingBox[5] = p5;
					OrientedBoundingBox[6] = p6;
					OrientedBoundingBox[7] = p7;

				}

				// ------------------------------------------------------------------

				const glm::vec3& operator[](size_t pos) const 
				{
					return OrientedBoundingBox[pos]; 
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				AOBBox()
				{
					Min         = glm::vec3(0, 0, 0);
					Max         = glm::vec3(0, 0, 0);
					Extents     = glm::vec3(0, 0, 0);
					Center      = glm::vec3(0, 0, 0);
					HalfExtents = glm::vec3(0, 0, 0);

					OrientedBoundingBox[0] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[1] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[2] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[3] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[4] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[5] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[6] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[7] = glm::vec3(0, 0, 0);
				}

				AOBBox(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min         = bmin;
					Max         = bmax;
					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

					OrientedBoundingBox[0] = glm::vec3(bmin.x, bmin.y, bmin.z);
					OrientedBoundingBox[1] = glm::vec3(bmax.x, bmin.y, bmin.z);
					OrientedBoundingBox[2] = glm::vec3(bmax.x, bmax.y, bmin.z);
					OrientedBoundingBox[3] = glm::vec3(bmin.x, bmax.y, bmin.z);
					OrientedBoundingBox[4] = glm::vec3(bmin.x, bmin.y, bmax.z);
					OrientedBoundingBox[5] = glm::vec3(bmax.x, bmin.y, bmax.z);
					OrientedBoundingBox[6] = glm::vec3(bmax.x, bmax.y, bmax.z);
					OrientedBoundingBox[7] = glm::vec3(bmin.x, bmax.y, bmax.z);

				}

				~AOBBox()
				{}

		};

	}	// end of geo3d namespace

}	// end of vml namespace

