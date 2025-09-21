#pragma once

//////////////////////////////////////////////////////////////////
// vertex class holding vertex attributes

namespace vml
{
	namespace geo3d
	{

		class Vertex
		{
			public:

				glm::vec4 Pos;		// homogeneous point coordinates
				glm::vec3 Normal;	// normal coordinates
				glm::vec2 UV;		// texture coordinates

				// ------------------------------------------
				// ctor / dtor

				Vertex(const glm::vec3& pos,
					   const glm::vec3& normal,
					   const glm::vec2& uv) : Pos(glm::vec4(pos.x, pos.y, pos.z, 1)), Normal(normal), UV(uv)
				{}

				Vertex() :Pos(glm::vec4(0, 0, 0, 0)), Normal(glm::vec3(0, 0, 0)), UV(glm::vec2(0, 0))
				{}

				~Vertex()
				{}

		};

	}
}

//////////////////////////////////////////////////////////////
//	indexed triangle class holding indices to a vertex buffer

namespace vml
{
	namespace geo3d
	{

		//////////////////////////////////////////////////////////////////
		//	surface class

		class IndexedTriangle
		{
			private:

				int	Id;					// surface id

			public:

				int		  I0;			// i0 vertex index
				int		  I1;			// i1 vertex index
				int		  I2;			// i2 vertex index
				glm::vec3 Center;		// center
				glm::vec3 Normal;		// normal
				glm::vec3 EdgeNormal0;	// edge normal 
				glm::vec3 EdgeNormal1;	// edge normal
				glm::vec3 EdgeNormal2;	// edge normal
				float	  Area;

			public:

				// ------------------------------------------------------------------
				//

				void Set(int id, int i0, int i1, int i2, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					Id = id;
					I0 = i0;
					I1 = i1;
					I2 = i2;
					Center = (p0 + p1 + p2) * 0.333f;									// compute center
					Normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));				// compute normal
					EdgeNormal0 = glm::normalize(glm::cross(Normal, p2 - p0));				// compute first edge normal
					EdgeNormal1 = glm::normalize(glm::cross(Normal, p0 - p1));				// compute second edge normal
					EdgeNormal2 = glm::normalize(glm::cross(p1 - p2 + Normal, p1 - p2));	// compute third edge normal
					Area = fabs(glm::length(Normal) * 0.5f);
				}

				// ------------------------------------------

				const glm::vec3& GetCenter() const { return Center; }
				const glm::vec3& GetNormal() const { return Normal; }
				const glm::vec3& GetEdgeNromal0() const { return EdgeNormal0; }
				const glm::vec3& GetEdgeNromal1() const { return EdgeNormal1; }
				const glm::vec3& GetEdgeNromal2() const { return EdgeNormal2; }
				float GetArea() const { return Area; }

				// ------------------------------------------
				// ctro / dtro

				IndexedTriangle(int id, int i0, int i1, int i2, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					Set(id, i0, i1, i2, p0, p1, p2);
				}

				IndexedTriangle(int id, int i0, int i1, int i2)
				{
					Set(id, i0, i1, i2, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
				}

				IndexedTriangle()
				{
					Set(-1, -1, -1, -1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
				}

				~IndexedTriangle()
				{
				}
		};
	}

}
