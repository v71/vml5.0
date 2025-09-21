
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
////////////////////////////////////////////////////////////////////////////////////

#include <vml5.0/mesh/mesh.h>

namespace vml
{
	namespace meshes
	{

		// ---------------------------------------------------------------
		// Memory clearing :
		// Resets all data to initial values (0) and
		// clears vertex, surface and surface indices arrays
		// releases also vao(s).

		void Mesh3d::ReleaseAll()
		{
			// delete buffers
			VertexArray.clear();
			NormalArray.clear();
			UVArray.clear();
			SurfaceIndices.clear();

			// delete vbo
			if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
			if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
			if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
			if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
			if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
			if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
			if (BufferObjects[4]) { glDeleteBuffers(1, &BufferObjects[4]); BufferObjects[4] = 0; }
			if (BufferObjects[5]) { glDeleteBuffers(1, &BufferObjects[5]); BufferObjects[5] = 0; }
			if (BufferObjects[6]) { glDeleteBuffers(1, &BufferObjects[6]); BufferObjects[6] = 0; }
			if (BufferObjects[7]) { glDeleteBuffers(1, &BufferObjects[7]); BufferObjects[7] = 0; }
		}

		// ---------------------------------------------------------------
		// vbo creation

		void Mesh3d::CreateVBO()
		{
			// Create the vertex array object for the mesh.
			//
			// The buffer objects that are to be used by the vertex stage of the GL
			// are collected together to form a vertex array object. All state related
			// to the definition of data used by the vertex processor is encapsulated
			// in a vertex array object.
			//
			// Vertex array objects maintain state. Any required vertex and index
			// buffer objects are created and bound to the vertex array object. Then
			// any vertex shader generic shader input variables are mapped to the
			// vertex data stored in the vertex buffer objects bound to the vertex
			// array object. All of this only needs to be done once when using vertex
			// array objects.

			// Create the VAO

			// Map the generic vertex attributes used by the mesh's shader program to
			// the mesh's vertex data stored in the vertex buffer object.

			GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
			GLuint AttributeNormal = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
			GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;

			glGenVertexArrays(1, &VAOid);
			glBindVertexArray(VAOid);

			// Create the Vertex Buffer Object 

			glGenBuffers(1, &BufferObjects[0]);
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
			glBufferData(GL_ARRAY_BUFFER, (VertexArray.size()) * sizeof(float), &VertexArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(AttributePosition);
			glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glGenBuffers(1, &BufferObjects[1]);
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
			glBufferData(GL_ARRAY_BUFFER, (NormalArray.size()) * sizeof(float), &NormalArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(AttributeNormal);
			glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glGenBuffers(1, &BufferObjects[2]);
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
			glBufferData(GL_ARRAY_BUFFER, (UVArray.size()) * sizeof(float), &UVArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(AttributeTexCoord);
			glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

			// Create the Index Buffer Object

			glGenBuffers(1, &IndexBufferObject);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (SurfaceIndices.size()) * sizeof(unsigned int), &SurfaceIndices[0], GL_STATIC_DRAW);

			// unbinds buffers

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (ReleaseMeshData)
			{
		//		vml::logger::Logger2::GetInstance()->Info({ "Mesh",GetResourceFileName() + " is set to release Data" });

				// if mesh is set to release data, clear all buffers
				// *do not set indices to zero or nothing will be drawn* 

				VertexArray.clear();
				NormalArray.clear();
				UVArray.clear();
				SurfaceIndices.clear();
				Vertices = 0;
				Surfaces = 0;
			}
			else
			{
		//		vml::logger::Logger2::GetInstance()->Info({ "Mesh",GetResourceFileName() + " is set to retain Data" });
			}
		}
		
		// ---------------------------------------------------------------
		// load mesh , path is embedded into filename ,no args needed

		bool Mesh3d::Load(const std::string& resourcefilename,const float scale)
		{
			// validate extension

			if (!resourcefilename.ends_with(".3df"))
				vml::os::Message::Error("Mesh3d : ", "Wrong extension ' ", resourcefilename.c_str(), " '");

			//	std::cout << "loading : " << resourcefilename << std::endl;

			FILE* stream;

			// models are loaded according to the model's path

			errno_t err = fopen_s(&stream, resourcefilename.c_str(), "rb");

			if (err == 0)
			{

				unsigned int vertices;
				unsigned int indices;
				float radius;
				float bmin[3];
				float bmax[3];

				// clears everything before loading

				ReleaseAll();

				// read number of vertices

				fread(&vertices, sizeof(unsigned int), 1, stream);

				//	std::cout << "Vertices : " << vertices << std::endl;

				VertexArray.resize((size_t)vertices * 4);
				NormalArray.resize((size_t)vertices * 3);
				UVArray.resize((size_t)vertices * 2);

				// read data

				for (size_t i = 0; i < vertices; i++)
				{

					float p[3];
					float n[3];
					float t[2];

					// _Post_ _Notnull_ suppress warning c6387

					fread(&p, sizeof(float), 3, stream);
					fread(&n, sizeof(float), 3, stream);
					fread(&t, sizeof(float), 2, stream);

					size_t offset;

					offset = i * 4;

					VertexArray[offset    ] = p[0]*scale;
					VertexArray[offset + 1] = p[1]*scale;
					VertexArray[offset + 2] = p[2]*scale;
					VertexArray[offset + 3] = 1.0f;

					//	std::cout << i << " : " << p[0] << " " << p[1] << " " << p[2] << std::endl;
					offset = i * 3;

					NormalArray[offset    ] = n[0];
					NormalArray[offset + 1] = n[1];
					NormalArray[offset + 2] = n[2];

					offset = i * 2;

					UVArray[offset    ] = t[0];
					UVArray[offset + 1] = t[1];

					//	std::cout << i << " : " << t[0] << " " << t[1] << std::endl;

					// increase vertex counter

					Vertices++;
				}

				// read surface indices for vbo indexing
				// _Post_ _Notnull_ suppress warning c6387

				fread(&indices, sizeof(unsigned int), 1, stream);

				Indices = indices;

				//	std::cout << "Indices : " << indices << std::endl;

				Surfaces = Indices / 3;

				// read surfaces

				SurfaceIndices.resize(indices);

				fread(&SurfaceIndices[0], sizeof(unsigned int), Indices, stream);

				// read metrics

				fread(&bmin, sizeof(float), 3, stream);
				fread(&bmax, sizeof(float), 3, stream);
				fread(&radius, sizeof(float), 1, stream);

				//	std::cout << "Bmin : " << bmin[0] << " " << bmin[1] << " " << bmin[2] << std::endl;
				//	std::cout << "Bmax : " << bmax[0] << " " << bmax[1] << " " << bmax[2] << std::endl;
				//	std::cout << "Radius : " << radius << std::endl;

				// fill metrics data

				BoundingBox.Set(glm::vec3(bmin[0], bmin[1], bmin[2])*scale, glm::vec3(bmax[0], bmax[1], bmax[2])*scale);
				Radius = radius*scale;

				// create vbos

				CreateVBO();

				// once the data is uploaded into the gpu , it should be rleased
				// see the meshsotre class, here we *DO NOT* release the data
				// since the data itslef might be needed for further computation
				// see the meshsotre clss where the data is explicilty releasde

				// close stream

				if (fclose(stream) != 0)
					vml::os::Message::Error("Mesh3d : ", "Cannot close ' ", resourcefilename.c_str(), " '");

				return true;
			}
			else
			{
				vml::os::Message::Error("Mesh3d : ", "Cannot load mesh ' ", resourcefilename.c_str(), " '", " ( error code : ", err, " )");
			}

			return false;
		}

		// ---------------------------------------------------------------
		// this function clear the data buffer once the VAO are created
		// since we might need to preserve the data in some cases 
		// we explicilty delete the buffer when the meshstore 
		// loads this class instance, see the meshstore for more info

		void Mesh3d::ReleaseVertexBufferData()
		{
			if (!VAOid)
				vml::os::Message::Error("Mesh3d : ", "Cannot delete buffers if VAO has not been created yet");

			//* do not reset retaindata flag or mesh
			// might become invalid if data is needed 
			// to be read *

			VertexArray.clear();
			NormalArray.clear();
			UVArray.clear();
			SurfaceIndices.clear();
			Vertices = 0;
			Surfaces = 0;
			Indices = 0;
		}
		
		// ---------------------------------------------------------------
		// dumps mesh 

		std::string Mesh3d::Dump()
		{
			std::string text = "FileName             : " + GetResourceFileName() + "\n";
			text += std::format("Vertices             : {}\n", Vertices);
			text += std::format("Surfaces             : {}\n", Surfaces);
			text += std::format("Indices              : {}\n", Indices);
			text += std::format("Bounding box minimum : {} , {} , {}\n", BoundingBox.GetMin().x, BoundingBox.GetMin().y, BoundingBox.GetMin().z);
			text += std::format("Bounding box maximum : {} , {} , {}\n", BoundingBox.GetMax().x, BoundingBox.GetMax().y, BoundingBox.GetMax().z);
			text += std::format("Bounding box extents : {} , {} , {}\n", BoundingBox.GetExtents().x, BoundingBox.GetExtents().y, BoundingBox.GetExtents().z);
			text += std::format("Radius               : {}\n", Radius);
			if (ReleaseMeshData)
				text += "Mesh Data            : Released\n";
			else if (!ReleaseMeshData)
				text += "Mesh Data            : Retained\n";
			if (!ReleaseMeshData)
			{
				text += std::format("Vertices             : {}\n", Vertices);
				text += std::format("Surfaces             : {}\n", Indices / 3);
				text += std::format("Indices              : {}\n", Indices);
				text += "inline static float VertexArray[] =\n";
				text += "{\n";
				for (size_t i = 0; i < (size_t)Vertices * 4; i += 4)
					text += std::format("{} , {} , {} , {} ,\n", VertexArray[i], VertexArray[i + 1], VertexArray[i + 2], VertexArray[i + 3]);
				text += "};\n";
				text += "inline static float NormalArray[] =\n";
				text += "{\n";
				for (size_t i = 0; i < (size_t)Vertices * 3; i += 3)
					text += std::format("{} , {} , {} ,\n", NormalArray[i], NormalArray[i + 1], NormalArray[i + 2]);
				text += "};\n";
				text += "inline static float UVArray[] =\n";
				text += "{\n";
				for (size_t i = 0; i < (size_t)Vertices * 2; i += 2)
					text += std::format("{} , {} ,\n", UVArray[i], UVArray[i + 1]);
				text += "};\n";
				text += "inline static unsigned int TrianglesArray[] =\n";
				text += "{\n";
				for (size_t i = 0; i < (size_t)Indices / 3; ++i)
				{
					size_t offset = i * 3;
					text += std::format("{} , {} , {} ,\n", SurfaceIndices[offset], SurfaceIndices[offset + 1], SurfaceIndices[offset + 2]);
				}
				text += "};\n";
			}

			return text;
		}
		
		// ---------------------------------------------------------------
		// constructors / destructors

		Mesh3d::Mesh3d(const std::string& resourcefilename, const vml::utils::Flags& flags,const float scale) : vml::utils::SharedResource(resourcefilename, flags)
		{
			Surfaces = 0;
			Vertices = 0;
			Indices = 0;
			Radius = 0;
			BoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
			VAOid = 0;
			IndexBufferObject = 0;
			BufferObjects[0] = 0;
			BufferObjects[1] = 0;
			BufferObjects[2] = 0;
			BufferObjects[3] = 0;
			BufferObjects[4] = 0;
			BufferObjects[5] = 0;
			BufferObjects[6] = 0;
			BufferObjects[7] = 0;
			ReleaseMeshData = true;

			// boolean flags to enable reuqested features
			if (flags.Get(DO_NOT_RELEASE_DATA)) ReleaseMeshData = false;

			// cleans everything and uses raii paradigm to load mesh resource
			Load(resourcefilename,scale);

		}
		
		// destructor

		Mesh3d::~Mesh3d()
		{
			ReleaseAll();
		}
		
	} // end of meshes namespace

} // end of vml namespace


