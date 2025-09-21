#pragma once

#include <vml5.0/libs/tinyobjloader/tiny_obj_loader.h>
#include <vml5.0/libs/meshoptimizer/meshoptimizer.h>

///////////////////////////////////////////////////////////////////////
// tinyobjloader Importer

namespace vml
{
	namespace importer
	{
		/////////////////////////////////////////////////////////////////////
		// vertex struct
		
		struct Vertex
		{
			glm::vec3 Position{ 0,0,0 };
			glm::vec3 Normal{ 0,0,0 };
			glm::vec2 UV{ 0,0 };

			Vertex(const glm::vec3 &p , const glm::vec3 &n, const glm::vec2 &s)
			{
				Position = p;
				Normal   = n;
				UV		 = s;
			}

			Vertex()
			{
			}
		};
		
		//////////////////////////////////////////////////////////////////////////
		// importer

		class TinyObjImporter
		{
	
			private:

				std::string ImportError;

				// ---------------------------------------------------------------------------------
				// transform mesh

				void TransformMesh(std::vector<Vertex>& Vertices,
								   glm::vec3& boundingboxmin, glm::vec3& boundingboxmax, float& radius,
								   const glm::vec3& rot, const glm::vec3& scale)
				{
					// subtract center from every vertex, thus  
					// centering the mesh at (0,0,0)

					glm::vec3 center(0, 0, 0);

					size_t n = Vertices.size();
					
					for (size_t i = 0; i < n; ++i)
						center += Vertices[i].Position;

					center /= (float)n;

					for (size_t i = 0; i < n; ++i)
						Vertices[i].Position -= center;

					// transform vertices

					float sx = sin(rot.x * vml::math::DEGTORAD);
					float sy = sin(rot.y * vml::math::DEGTORAD);
					float sz = sin(rot.z * vml::math::DEGTORAD);
					float cx = cos(rot.x * vml::math::DEGTORAD);
					float cy = cos(rot.y * vml::math::DEGTORAD);
					float cz = cos(rot.z * vml::math::DEGTORAD);

					// get parent matrix pointer

					float M[16] = { 0 };

					// compute model matrix

					M[ 0] = (cy * cz) * scale.x;
					M[ 1] = (cy * sz) * scale.x;
					M[ 2] = (-sy) * scale.x;
					M[ 3] = 0;
					M[ 4] = (sx * sy * cz - cx * sz) * scale.y;
					M[ 5] = (sx * sy * sz + cx * cz) * scale.y;
					M[ 6] = (sx * cy) * scale.y;
					M[ 7] = 0;
					M[ 8] = (cx * sy * cz + sx * sz) * scale.z;
					M[ 9] = (cx * sy * sz - sx * cz) * scale.z;
					M[10] = (cx * cy) * scale.z;
					M[11] = 0;
					M[12] = 0;
					M[13] = 0;
					M[14] = 0;
					M[15] = 1;

					std::vector<Vertex>	 destVertices;

					for (size_t i = 0; i < n; ++i)
					{
						glm::vec3 p(Vertices[i].Position.x * M[0] + Vertices[i].Position.y * M[4] + Vertices[i].Position.z * M[8] + M[12],
									Vertices[i].Position.x * M[1] + Vertices[i].Position.y * M[5] + Vertices[i].Position.z * M[9] + M[13],
									Vertices[i].Position.x * M[2] + Vertices[i].Position.y * M[6] + Vertices[i].Position.z * M[10] + M[14]);

						glm::vec3 n(Vertices[i].Normal.x * M[0] + Vertices[i].Normal.y * M[4] + Vertices[i].Normal.z * M[8] + M[12],
									Vertices[i].Normal.x * M[1] + Vertices[i].Normal.y * M[5] + Vertices[i].Normal.z * M[9] + M[13],
									Vertices[i].Normal.x * M[2] + Vertices[i].Normal.y * M[6] + Vertices[i].Normal.z * M[10] + M[14]);

						glm::vec2 s(Vertices[i].UV.x, Vertices[i].UV.y);

						destVertices.emplace_back(Vertex(p, n, s));
					}
				
					// copy remapped vertice sinto proper mesh buffer

					for (size_t i = 0; i < destVertices.size(); ++i)
					{
						Vertices[i].Position = destVertices[i].Position;
						Vertices[i].Normal = destVertices[i].Normal;
						Vertices[i].UV = destVertices[i].UV;
					}

					// compute bounding box

					boundingboxmin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					boundingboxmax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < Vertices.size(); ++i)
					{
						float px = Vertices[i].Position.x;
						float py = Vertices[i].Position.y;
						float pz = Vertices[i].Position.z;

						if (px <= boundingboxmin.x) boundingboxmin.x = px;
						if (py <= boundingboxmin.y) boundingboxmin.y = py;
						if (pz <= boundingboxmin.z) boundingboxmin.z = pz;
						if (px >= boundingboxmax.x) boundingboxmax.x = px;
						if (py >= boundingboxmax.y) boundingboxmax.y = py;
						if (pz >= boundingboxmax.z) boundingboxmax.z = pz;
					}

					// compute radius
					radius = -FLT_MAX;

					for (size_t i = 0; i < Vertices.size(); ++i)
					{
						float px = Vertices[i].Position.x;
						float py = Vertices[i].Position.y;
						float pz = Vertices[i].Position.z;
						float d = px * px + py * py + pz * pz;
						if (d >= radius)
							radius = d;
					}
					radius = sqrtf(radius);
				}

			public:

			// -----------------------------------------------------------------------
			//  Get Error string

			const std::string& GetError() const { return ImportError; }

			// -----------------------------------------------------------------------
			// Import mesh 

			bool Import(const std::string& sourcepath,
						const std::string& sourcefilename,
						const std::string& destpath,
						const glm::vec3& rot=glm::vec3(0,0,0),
						const glm::vec3& scale=glm::vec3(1,1,1))
			{
				std::vector<tinyobj::material_t> materials;
				std::map<std::string, uint32_t>  textures;
				std::vector<tinyobj::shape_t>    inshapes;
				tinyobj::attrib_t				 inattrib;
				std::string						 warn;
				std::string						 err;

				// start importing

				ImportError = "NoError";

				std::string fullpath = sourcepath + sourcefilename;

				std::cout << "starting importing " << fullpath << std::endl;

				if (fullpath.empty())
				{
					ImportError = "Fatal : No input file";
					return false;
				}

				if (destpath.empty())
				{
					ImportError = "Fatal : No dest path";
					return false;
				}
				
				bool ret = tinyobj::LoadObj(&inattrib, &inshapes, &materials, &warn, &err, &fullpath[0], &sourcepath[0]);

				// check if file has been properly imported

				if (!warn.empty())
				{
					ImportError = "Warn : " + warn;
					//return false;	
				}

				if (!err.empty())
				{
					ImportError = "Error : " + err;
					return false;
				}

				if (!ret)
				{
					ImportError = "Failed to load : " + sourcefilename;
					return false;
				}

				std::cout << "# of vertices  = " << (int)(inattrib.vertices.size()) / 3 << std::endl;
				std::cout << "# of normals   = " << (int)(inattrib.normals.size()) / 3 << std::endl;
				std::cout << "# of texcoords = " << (int)(inattrib.texcoords.size()) / 2 << std::endl;
				std::cout << "# of materials = " << (int)materials.size() << std::endl;
				std::cout << "# of shapes    = " << (int)inshapes.size() << std::endl;

				std::string meshname = vml::strings::SplitPath::GetTitle(sourcefilename);
				std::string destfullpath = destpath + meshname + "_imp/";
				std::string meshespath = destfullpath + "3dfs/";
				std::string texturespath = destfullpath + "textures/";

				std::cout << meshname << std::endl;
				std::cout << destfullpath << std::endl;
				std::cout << meshespath << std::endl;
				std::cout << texturespath << std::endl;

				std::filesystem::create_directories(destfullpath);
				std::filesystem::create_directories(meshespath);
				std::filesystem::create_directories(texturespath);

				// import meshes

				for (const auto& shape : inshapes)
				{
					
					const tinyobj::mesh_t& mesh = shape.mesh;

					int icount = 0;

					std::vector<Vertex> vertices;
					std::vector<unsigned int> indices;

					for (const auto& index : mesh.indices)
					{
						
						tinyobj::index_t i = index;

						size_t ivindex = (size_t)i.vertex_index * 3;
						size_t inindex = (size_t)i.normal_index * 3;
						size_t itindex = (size_t)i.texcoord_index * 2;
						
						glm::vec3 pos(inattrib.vertices[ivindex], inattrib.vertices[ivindex + 1], inattrib.vertices[ivindex + 2]);
						glm::vec3 normal(0, 0, 0);
						glm::vec2 uv(0, 0);
						
						if (inattrib.normals.size() > 0)
							normal = glm::vec3(inattrib.normals[inindex], inattrib.normals[inindex + 1], inattrib.normals[inindex + 2]);

						if (inattrib.texcoords.size() > 0)
							uv = glm::vec2(inattrib.texcoords[itindex], inattrib.texcoords[itindex + 1]);
						
						// add this vertex struct

						vertices.emplace_back(Vertex(pos, normal, uv));

						// add indices

						indices.emplace_back(icount);

						icount++;
						
					}

					// remap vertices
					std::vector<unsigned int> remap(indices.size());
					const size_t vertexCount = meshopt_generateVertexRemap(remap.data(), indices.data(), indices.size(), vertices.data(), indices.size(), sizeof(Vertex));

					// create anew mesh using remapped vertices
					std::vector<unsigned int> remappedIndices(indices.size());

					std::cout << "VertexCount : " << vertexCount << " , Indices : " << indices.size() << std::endl;

					std::vector<Vertex> remappedVertices(vertexCount);
					meshopt_remapIndexBuffer(remappedIndices.data(), indices.data(), indices.size(), remap.data());
					meshopt_remapVertexBuffer(remappedVertices.data(), vertices.data(), vertices.size(), sizeof(Vertex), remap.data());

					// create mesh 

					glm::vec3 boundingboxmin(-FLT_MAX, -FLT_MAX, -FLT_MAX);
					glm::vec3 boundingboxmax(FLT_MAX, FLT_MAX, FLT_MAX);
					float radius = 0.0f;

					// transform mesh 

					TransformMesh(remappedVertices, boundingboxmin, boundingboxmax, radius, rot, scale);

					// save mesh

					std::string fullpathfilename = meshespath + shape.name + ".3df";

					std::cout << "saving : " << shape.name << std::endl;
					std::cout << "filename :" << shape.name + ".3df" << std::endl;
					std::cout << "saving : " << fullpathfilename << std::endl;
					std::cout << "vertices : " << remappedVertices.size() << std::endl;
					std::cout << "indices : " << remappedIndices.size() << std::endl;
					
					FILE* stream;

					errno_t err = fopen_s(&stream, fullpathfilename.c_str(), "wb");

					// save vertices count
					if(stream)
					{
						
						unsigned int verticescount = (unsigned int)remappedVertices.size();

						fwrite(&verticescount, sizeof(unsigned int), 1, stream);
						
						// save vertices

						for (size_t i = 0; i < remappedVertices.size(); ++i)
						{
							fwrite(&remappedVertices[i].Position, sizeof(float), 3, stream);
							fwrite(&remappedVertices[i].Normal, sizeof(float), 3, stream);
							fwrite(&remappedVertices[i].UV, sizeof(float), 2, stream);
						}
						
						// save surfaces indices count

						unsigned int indicescount = (unsigned int)remappedIndices.size();

						fwrite(&indicescount, sizeof(unsigned int), 1, stream);

						// save surfaces indices
						
						fwrite(&remappedIndices[0], sizeof(unsigned int), indicescount, stream);

						// save metrics

						fwrite(&boundingboxmin, sizeof(float), 3, stream);
						fwrite(&boundingboxmax, sizeof(float), 3, stream);
						fwrite(&radius, sizeof(float), 1, stream);
						
						std::cout << "Bmin : " << boundingboxmin.x << " " << boundingboxmin.y << " " << boundingboxmin.z << std::endl;
						std::cout << "Bmax : " << boundingboxmax.x << " " << boundingboxmax.y << " " << boundingboxmax.z << std::endl;
						std::cout << "Radius : " << radius << std::endl;
						
						// Close stream if it is not NULL

						if (fclose(stream) != 0)
						{
							ImportError = "Fatal : couldn't save " + fullpathfilename;
							return false;
						}
					}
					
				}

				return true;
			}
		
			// -----------------------------------------------------------------------
			// ctor / dtor

			TinyObjImporter()
			{
				ImportError = "Ok";
			}

			~TinyObjImporter()
			{}

		};

	}
}

