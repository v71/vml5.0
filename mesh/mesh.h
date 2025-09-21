#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include <vml5.0/math/mathutils.h>
#include <vml5.0/math/3d/geometry3d.h>
#include <vml5.0/opengl/shaders/shader.h>

namespace vml
{
	namespace meshes
	{
		
		/////////////////////////////////////////////////////////////////
		// specify if vertex buffers need to be release after creating vbo(s)

		constexpr uint32_t DO_NOT_RELEASE_DATA = 0;

		/////////////////////////////////////////////////////////////////
		// Mesh uploading default parameters

		const vml::utils::Flags DefaultMeshParms = { };

		/////////////////////////////////////////////////////////////////
		// mesh class
		
		class Mesh3d : public vml::utils::SharedResource
		{

			private:
				
				unsigned int			  Surfaces;				// Triangles for this mesh
				unsigned int			  Vertices;				// Vertices for this mesh
				unsigned int			  Indices;				// vbo indices for surfaces
				vml::geo3d::AABBox		  BoundingBox;			// bounding box
				float	 				  Radius;				// radius
				bool					  ReleaseMeshData; 		// flags used to store preferences flags
				GLuint					  VAOid;				// vertex array object id
				GLuint					  IndexBufferObject;	// surface index buffer object
				GLuint					  BufferObjects[8];		// buffer objects
				std::vector<float>		  VertexArray;			// vertex array	
				std::vector<float>		  NormalArray;			// normal array
				std::vector<float>		  UVArray;				// uv array
				std::vector<unsigned int> SurfaceIndices;		// surface indices array

				// ---------------------------------------------------------------
				// Memory clearing :
				// Resets all data to initial values (0) and
				// clears vertex, surface and surface indices arrays
				// releases also vao(s).

				void ReleaseAll();

				// ---------------------------------------------------------------
				// vbo creation
				
				void CreateVBO();

				// ---------------------------------------------------------------
				// load mesh , path is embedded into filename ,no args needed

				bool Load(const std::string& resourcefilename,const float scale);

		public:
				
				// ---------------------------------------------------------------
				//	query functions

				unsigned int				     GetVertexCount()	 const { return Vertices; }							// get mesh Vertex array size
				float						     GetRadius()		 const { return Radius; }							// get mesh bounding sphere radius
				unsigned int				     GetIndicesCount()	 const { return Indices; }							// get mesh indices array
				unsigned int				     GetSurfaceCount()	 const { return Surfaces; }							// gets number of surfaces
				GLuint						     GetVAOId()			 const { return VAOid; }							// get mesh vao id
				bool							 IsValid()			 const { return Vertices> 0 && Surfaces> 0; }		// returns if data is valid
				bool							 IsDataRetained()	 const { return ReleaseMeshData; }
				const vml::geo3d::AABBox		&GetBoundingBox()	 const { return BoundingBox; }
				const std::vector<float>		&GetVertexArray()	 const { return VertexArray; }
				const std::vector<float>		&GetNormalArray()	 const { return NormalArray; }
				const std::vector<float>		&GetUVArray()		 const { return UVArray; }
				const std::vector<unsigned int> &GetSurfaceIndices() const { return SurfaceIndices; }
				
				// ---------------------------------------------------------------
				// this function clear the data buffer once the VAO are created
				// since we might need to preserve the data in some cases 
				// we explicilty delete the buffer when the meshstore 
				// loads this class instance, see the meshstore for more info
				
				void ReleaseVertexBufferData();

				// ---------------------------------------------------------------
				// dumps mesh 

				std::string Dump();

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Mesh3d(const Mesh3d& other) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				Mesh3d& operator=(const Mesh3d& other) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				Mesh3d(Mesh3d&& other) = delete;
				Mesh3d& operator=(Mesh3d&& other) = delete;

				// ---------------------------------------------------------------
				// constructors / destructors

				Mesh3d(const std::string& resourcefilename, const vml::utils::Flags& flags,const float scale=1.0f);
				// destructor

				~Mesh3d();

		};
		
	} // end of meshes namespace

} // end of vml namespace


