#pragma once

#include <vml5.0/octree/octreetypes.h>
#include <vml5.0/octree/removeduplicates.h>
#include <vml5.0/octree/clippers.h>

namespace vml
{
	namespace octree
	{
		
				////////////////////////////////////////////////////////////////////////////
				// octree node

				class OctTreeNode
				{
					private:

						// ----------------------------------------------------------------

						OctTreeNode		   *Parent;					// parent node
						OctTreeNode		   *Child[8];				// child nodes
						vml::geo3d::AABBox	BoundingBox;			// bounding box
						unsigned int		Visible;				// visibility flag
						int					Id;						// node id
						bool				Leaf;					// node is a leaf if it contains rendering data
						GLuint				VAOid;					// vertex array object id
						GLuint				IndexBufferObject;		// surface index buffer object
						GLuint				BufferObjects[4];		// buffer objects
						GLuint				Indices;				// Indices for the vao buffers
						GLuint				Vertices;				// vertices count
						GLuint				Surfaces;				// surfaces count
						glm::mat4			M;
						glm::mat4			MV;
						glm::mat4			MVP;
						glm::mat3			NV;

					public:
						
						// ---------------------------------------------------------------
						// enums for node frustum test

						static const unsigned int OUTSIDE	  = 0;
						static const unsigned int INSIDE	  = 1;
						static const unsigned int INTERSECTED = 2;
						
						// ----------------------------------------------------------------
						// getters

						const vml::geo3d::AABBox &GetBoundingBox()	   const { return BoundingBox; }
						OctTreeNode*			  GetParent()		   const { return Parent; }
						OctTreeNode*			  GetChild(size_t pos) const { return Child[pos]; }
						int						  IsVisible()          const { return Visible == INSIDE; }
						int						  IsOut()		       const { return Visible == OUTSIDE; }
						int						  IsIntersected()	   const { return Visible == INTERSECTED; }
						int						  IsInFrustum()		   const { return Visible != OUTSIDE; }
						bool					  IsLeaf()			   const { return Leaf; }
						bool					  IsRoot()			   const { return Parent == nullptr; }
						int						  GetId()			   const { return Id; }
						size_t					  GetVertexCount()     const { return Vertices; }
						size_t					  GetSurfacesCount()   const { return Surfaces; }
						size_t					  GetIndicesCount()    const { return Indices; }
						GLuint					  GetVAOId()		   const { return VAOid; }
						const					  glm::mat4& GetM()    const { return M; }
						const					  glm::mat4& GetMV()   const { return MV; }
						const					  glm::mat4& GetMVP()  const { return MVP; }
						const					  glm::mat3& GetNV()   const { return NV; }

						// ----------------------------------------------------------------
						// setters

						void SetChild(int pos, OctTreeNode* child) { Child[pos] = child; }
						void SetParent(OctTreeNode* parent)		   { Parent = parent; Leaf = false; }
						void SetLeaf()							   { Leaf = true; }
						void ResetVisibleFlag()					   { Visible = OUTSIDE; }
						
						// ------------------------------------------------------------------------------
						// test an object's axis aligned bounding box against the view frustum
						// 0 = out, 1 = in , 2 = intersected

						int TestAABBox(const glm::vec4* Planes)
						{
							// cache center position
							
							float centerx = BoundingBox.GetCenter().x;
							float centery = BoundingBox.GetCenter().y;
							float centerz = BoundingBox.GetCenter().z;

							// cache half extensions
							
							float extentsx = BoundingBox.GetHalfExtents().x;
							float extentsy = BoundingBox.GetHalfExtents().y;
							float extentsz = BoundingBox.GetHalfExtents().z;

							// performs frustum planes test

							float d, r;

							Visible = INSIDE;
							
							// left plane

							d = centerx * Planes[0].x + centery * Planes[0].y + centerz * Planes[0].z + Planes[0].w;
							r = extentsx * abs(Planes[0].x) + extentsy * abs(Planes[0].y) + extentsz * abs(Planes[0].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection

							// right plane

							d = centerx * Planes[1].x + centery * Planes[1].y + centerz * Planes[1].z + Planes[1].w;
							r = extentsx * abs(Planes[1].x) + extentsy * abs(Planes[1].y) + extentsz * abs(Planes[1].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection

							// top plane

							d = centerx * Planes[2].x + centery * Planes[2].y + centerz * Planes[2].z + Planes[2].w;
							r = extentsx * abs(Planes[2].x) + extentsy * abs(Planes[2].y) + extentsz * abs(Planes[2].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection

							// bottom plane

							d = centerx * Planes[3].x + centery * Planes[3].y + centerz * Planes[3].z + Planes[3].w;
							r = extentsx * abs(Planes[3].x) + extentsy * abs(Planes[3].y) + extentsz * abs(Planes[3].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection

							// near plane

							d = centerx * Planes[4].x + centery * Planes[4].y + centerz * Planes[4].z + Planes[4].w;
							r = extentsx * abs(Planes[4].x) + extentsy * abs(Planes[4].y) + extentsz * abs(Planes[4].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection

							// far plane

							d = centerx * Planes[5].x + centery * Planes[5].y + centerz * Planes[5].z + Planes[5].w;
							r = extentsx * abs(Planes[5].x) + extentsy * abs(Planes[5].y) + extentsz * abs(Planes[5].z);

							if (d + r < 0) { Visible = OUTSIDE;  return Visible; }		// out
							if (d - r < 0)	Visible = INTERSECTED;						// intersection
							
							return Visible;
						}
												
						// ---------------------------------------------------------------
						// vbo creation

						void CreateVAO(const std::vector<vml::geo3d::Vertex> &srcvertexarray,const std::vector<vml::geo3d::IndexedTriangle>&srcsurfacearray)
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

							std::vector<unsigned int> surfaceindices;
							std::vector<float>	      uvArray;
							std::vector<float>		  vertexarray;
							std::vector<float>		  normalarray;

							for (int i = 0; i < srcvertexarray.size(); ++i)
							{
								vertexarray.emplace_back(srcvertexarray[i].Pos.x);
								vertexarray.emplace_back(srcvertexarray[i].Pos.y);
								vertexarray.emplace_back(srcvertexarray[i].Pos.z);
								vertexarray.emplace_back(1);

								normalarray.emplace_back(srcvertexarray[i].Normal.x);
								normalarray.emplace_back(srcvertexarray[i].Normal.y);
								normalarray.emplace_back(srcvertexarray[i].Normal.z);

								// * FOR THIS GAME ,  WE DO NOT NEED UV ARRAYS *
								 
							//	uvArray.emplace_back(srcvertexarray[i].UV.x);
							//	uvArray.emplace_back(srcvertexarray[i].UV.y);
							}

							for (int i = 0; i < srcsurfacearray.size(); ++i)
							{
								surfaceindices.emplace_back(srcsurfacearray[i].I0);
								surfaceindices.emplace_back(srcsurfacearray[i].I1);
								surfaceindices.emplace_back(srcsurfacearray[i].I2);
							}

							// get vertex, surfaces and indices count for node metrics

							Vertices = (unsigned int)srcvertexarray.size();
							Surfaces = (unsigned int)srcsurfacearray.size(); 
							Indices  = (unsigned int)surfaceindices.size();

							// clear opengl vao data buffers

							if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
							if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
							if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
							if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
							if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
							if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
							
							// Create the VAO

							// Map the generic vertex attributes used by the mesh's shader program to
							// the mesh's vertex data stored in the vertex buffer object.

							GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
							GLuint AttributeNormal   = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
							GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
							GLuint AttributeColor    = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

							glGenVertexArrays(1, &VAOid);
							glBindVertexArray(VAOid);

							// Create the Vertex Buffer Object 

							glGenBuffers(1, &BufferObjects[0]);
							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
							glBufferData(GL_ARRAY_BUFFER, ((unsigned int)vertexarray.size()) * sizeof(float), &vertexarray[0], GL_STATIC_DRAW);
							glEnableVertexAttribArray(AttributePosition);
							glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

							glGenBuffers(1, &BufferObjects[1]);
							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
							glBufferData(GL_ARRAY_BUFFER, ((unsigned int)normalarray.size()) * sizeof(float), &normalarray[0], GL_STATIC_DRAW);
							glEnableVertexAttribArray(AttributeNormal);
							glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

							// *FOR THE NEXT GAME , BIND UV ARRAY HERE*
						
							// Create the Index Buffer Object

							glGenBuffers(1, &IndexBufferObject);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
							glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(unsigned int), &surfaceindices[0], GL_STATIC_DRAW);

							// unbinds buffers

							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						}
					
						// -----------------------------------------------------------------------------------
						// trasnfor node to view

						void TransformToView(vml::views::View* view)
						{
							if (view)
							{

								// model matrix

								const glm::mat4& V = view->GetView();
								const glm::mat4& P = view->GetProjection();

								MV  = M * V;
								MVP = P * MV;

								// get normal matrix pointer

								float* NormalViewMatrix = glm::value_ptr(NV);

								// get model * view matrix

								float* ModelViewMatrix = glm::value_ptr(MV);

								// normal matrix is computed as the inverse transpose
								// of the model view matrix, this causes the
								// first 3x3 order for this matrix is to be divided
								// by each scaling factor along correspondig axes, so
								// we need to unskew normal matrix

								float Determinant = + ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
													- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
													+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

								if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
								else Determinant = 1.0f / Determinant;

								NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
								NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
								NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
								NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
								NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
								NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
								NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[ 6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
								NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[ 6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
								NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[ 5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;
								
							}
						
						}

						// ----------------------------------------------------------------
						// ctor / dtor

						OctTreeNode(OctTreeNode *parent, const int id, const glm::vec3 &bmin, const glm::vec3 &bmax)
						{
							Parent             =  parent;
							BoundingBox		   =  vml::geo3d::AABBox(bmin, bmax);
							Child[0]		   =  nullptr;
							Child[1]		   =  nullptr;
							Child[2]		   =  nullptr;
							Child[3]		   =  nullptr;
							Child[4]		   =  nullptr;
							Child[5]		   =  nullptr;
							Child[6]		   =  nullptr;
							Child[7]		   =  nullptr;
							Visible			   =  false;
							Leaf			   =  false;
							Id				   =  id;
							VAOid			   =  0;
							Indices			   =  0;
							Surfaces		   =  0;
							Vertices		   =  0;
							IndexBufferObject  =  0;
							BufferObjects[0]   =  0;
							BufferObjects[1]   =  0;
							BufferObjects[2]   =  0;
							BufferObjects[3]   =  0;
							M = glm::mat4(1, 0, 0, 0,
							 		      0, 1, 0, 0,
									      0, 0, 1, 0,
									      0, 0, 0, 1);
							MV  = glm::mat4(0);
							MVP = glm::mat4(0);
							NV  = glm::mat3(0);
						}

						~OctTreeNode()
						{
							// delete vbo

							if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
							if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
							if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
							if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
							if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
							if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
						}

				};
					
				////////////////////////////////////////////////////////////////////////////
				// Octree generator

				class OctTree
				{

					private:
						
						// ----------------------------------------------------------------
						// private data

						std::string								   ResourceFileName;
						int										   MaxSurfaces;					// max number of surfaces per octree node
						int										   LeafNodes;
						int										   CurrentNode;
						OctTreeNode								  *Root;
						float									   BoundingBoxEps;
						std::vector<vml::geo3d::Vertex>			   VertexArray;
						std::vector<OctTreeNode*>				   OctTreeNodes;				// octree nodes
						std::vector<vml::geo3d::IndexedTriangle>   SurfaceIndices;
						OctTreeNode								 **Stack;
						OctTreeNode							     **RenderedNodes;
						int 								       StackCounter;				// stack counter
						size_t									   RenderedNodesCount;			// Rendered nodes count
						float									   RenderedNodeRatio;
						unsigned int							   Flags;
						std::vector<OctTreeNode*>				   QueryOctTreeNodes;			// octree nodes vector for fast searches
						int										   QueryOctTreeNodesCount;

						// ----------------------------------------------------------------
						// octree node position identifiers

						static const unsigned int FAR_BOTTOM_LEFT_NODE   = 0;
						static const unsigned int FAR_BOTTOM_RIGHT_NODE  = 1;
						static const unsigned int FAR_TOP_RIGHT_NODE     = 2;
						static const unsigned int FAR_TOP_LEFT_NODE      = 3;
						static const unsigned int NEAR_BOTTOM_LEFT_NODE  = 4;
						static const unsigned int NEAR_BOTTOM_RIGHT_NODE = 5;
						static const unsigned int NEAR_TOP_RIGHT_NODE    = 6;
						static const unsigned int NEAR_TOP_LEFT_NODE     = 7;
						
						// ----------------------------------------------------------------
						// converts data from seprate vertex, normal and uv array to
						// joined vertex attributes to be clipped by the triangle clipper

						void ConvertData(const std::vector<float>* sourcevertexarray, 
							             const std::vector<float>* sourcenormalarray, 
							             const std::vector<float>* sourceuvarray,
							             const std::vector<unsigned int>* sourcesurfaceindices)
						{
							
							for (int i = 0; i < sourcesurfaceindices->size() / 3; ++i)
							{
								
								// indices have a 3 elements stride

								int idx = i * 3;

								int i0 = sourcesurfaceindices->at((size_t)idx    );
								int i1 = sourcesurfaceindices->at((size_t)idx + 1);
								int i2 = sourcesurfaceindices->at((size_t)idx + 2);
								
							//	std::cout << "Surface : " << i << " [ " << i0 << " , " << i1 << " , " << i2 << " ]" << std::endl;
								
								glm::vec4 p0 = glm::vec4(sourcevertexarray->at(i0 * 4), sourcevertexarray->at(i0 * 4 + 1), sourcevertexarray->at(i0 * 4 + 2),1);
								glm::vec4 p1 = glm::vec4(sourcevertexarray->at(i1 * 4), sourcevertexarray->at(i1 * 4 + 1), sourcevertexarray->at(i1 * 4 + 2),1);
								glm::vec4 p2 = glm::vec4(sourcevertexarray->at(i2 * 4), sourcevertexarray->at(i2 * 4 + 1), sourcevertexarray->at(i2 * 4 + 2),1);

								glm::vec3 n0 = glm::vec3(sourcenormalarray->at(i0 * 3), sourcenormalarray->at(i0 * 3 + 1), sourcenormalarray->at(i0 * 3 + 2));
								glm::vec3 n1 = glm::vec3(sourcenormalarray->at(i1 * 3), sourcenormalarray->at(i1 * 3 + 1), sourcenormalarray->at(i1 * 3 + 2));
								glm::vec3 n2 = glm::vec3(sourcenormalarray->at(i2 * 3), sourcenormalarray->at(i2 * 3 + 1), sourcenormalarray->at(i2 * 3 + 2));

								glm::vec2 uv0 = glm::vec2(sourceuvarray->at(i0 * 2), sourceuvarray->at(i0 * 2 + 1));
								glm::vec2 uv1 = glm::vec2(sourceuvarray->at(i1 * 2), sourceuvarray->at(i1 * 2 + 1));
								glm::vec2 uv2 = glm::vec2(sourceuvarray->at(i2 * 2), sourceuvarray->at(i2 * 2 + 1));
								
								VertexArray.emplace_back(vml::geo3d::Vertex(p0, n0, uv0));
								VertexArray.emplace_back(vml::geo3d::Vertex(p1, n1, uv1));
								VertexArray.emplace_back(vml::geo3d::Vertex(p2, n2, uv2));

								SurfaceIndices.emplace_back(vml::geo3d::IndexedTriangle(i, (int)VertexArray.size() - 3, (int)VertexArray.size() - 2, (int)VertexArray.size() - 1));
								
							}
							
						}
						
						// ----------------------------------------------------------------
						// Iterates throigh the input list and check if triangle is 
						// inside or interseting the bounding box, if so , put 
						// int he destination list

						std::vector<vml::geo3d::IndexedTriangle> CheckTrianglesInNode(OctTreeNode* node, const std::vector<vml::geo3d::IndexedTriangle>& surfaceindices) const
						{
							const glm::vec3& bmin = node->GetBoundingBox().GetMin() - BoundingBoxEps;
							const glm::vec3& bmax = node->GetBoundingBox().GetMax() + BoundingBoxEps;

							// enlarge bounding box to clip triangles
							// very near to bouding box faces

							std::vector<vml::geo3d::IndexedTriangle> destsurfaceindices;

							for (int i = 0; i < surfaceindices.size(); ++i)
							{
								// indices have a 3 elements stride

								int i0 = surfaceindices[i].I0;
								int i1 = surfaceindices[i].I1;
								int i2 = surfaceindices[i].I2;

								glm::vec3 p0 = VertexArray[i0].Pos;
								glm::vec3 p1 = VertexArray[i1].Pos;
								glm::vec3 p2 = VertexArray[i2].Pos;

								glm::vec3 tbmin(vml::math::ranges::Min(p0.x, p1.x, p2.x),
												vml::math::ranges::Min(p0.y, p1.y, p2.y),
												vml::math::ranges::Min(p0.z, p1.z, p2.z));

								glm::vec3 tbmax(vml::math::ranges::Max(p0.x, p1.x, p2.x),
												vml::math::ranges::Max(p0.y, p1.y, p2.y),
												vml::math::ranges::Max(p0.z, p1.z, p2.z));

								if (!((tbmax.x < bmin.x || tbmin.x > bmax.x) ||
									  (tbmax.y < bmin.y || tbmin.y > bmax.y) ||
									  (tbmax.z < bmin.z || tbmin.z > bmax.z)))
								{
									destsurfaceindices.emplace_back(i, i0, i1, i2);
								}

							}
							return destsurfaceindices;
						}
												
						// ----------------------------------------------------------------
						//

						bool CreateNode(OctTreeNode* node, const std::vector<vml::geo3d::IndexedTriangle>& surfaceindices)
						{
							
							std::vector<vml::geo3d::Vertex>	 destvertexarray;
							std::vector<vml::geo3d::IndexedTriangle> destsurfacearray;

							for (int i = 0; i < surfaceindices.size(); ++i)
							{

								vml::geo3d::clippers::AABBoxTriangleClipper tc;

								// indices have a 3 elements stride

								int i0 = surfaceindices[i].I0;
								int i1 = surfaceindices[i].I1;
								int i2 = surfaceindices[i].I2;

								int result = tc.ClipTriangleAgainstAABB(VertexArray[i0], VertexArray[i1], VertexArray[i2],
																		node->GetBoundingBox().GetMin() - BoundingBoxEps, node->GetBoundingBox().GetMax() + BoundingBoxEps);

								if (result == vml::geo3d::Results::INSIDE)
								{
									destvertexarray.emplace_back(VertexArray[i0]);
									destvertexarray.emplace_back(VertexArray[i1]);
									destvertexarray.emplace_back(VertexArray[i2]);

									destsurfacearray.emplace_back(vml::geo3d::IndexedTriangle((int)destsurfacearray.size(),
																					          (int)destvertexarray.size() - 3,
																						      (int)destvertexarray.size() - 2,
																						      (int)destvertexarray.size() - 1));
								}

								if (result == vml::geo3d::Results::DOES_INTERSECT)
								{
									for (size_t i = 0; i < tc.GetTrianglesCount(); ++i)
									{
										destvertexarray.emplace_back(tc.GetTriangleV0(i));
										destvertexarray.emplace_back(tc.GetTriangleV1(i));
										destvertexarray.emplace_back(tc.GetTriangleV2(i));

										destsurfacearray.emplace_back(vml::geo3d::IndexedTriangle((int)destsurfacearray.size(),
																								  (int)destvertexarray.size() - 3,
																								  (int)destvertexarray.size() - 2,
																								  (int)destvertexarray.size() - 1));
									}
								}
							}

							// remove redundant vertices and creates arenderable node
							// if there is no dta, node will be an empty leaf and 
							// will be flagged as non leaf

							if (destvertexarray.size() != 0 && destsurfacearray.size() != 0)
							{

								vml::meshes::RemoveDuplicates3D rd;
								rd.Begin(node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax());
								rd.RemoveDuplicates(destvertexarray, destsurfacearray);
							//	rd.Finalize();

								// create node vao
	
								node->CreateVAO(destvertexarray, destsurfacearray);

								// se this node as leaf

								node->SetLeaf();

								return true;
							}
							
							return false;
						}
						
						// ----------------------------------------------------------------
						// recursicve function to split a node into further 8 children and
						// add triangles if they intersects any of the previously split children

						void RecurseNode(OctTreeNode* node, const std::vector<vml::geo3d::IndexedTriangle>& sourcelist)
						{

							std::vector<vml::geo3d::IndexedTriangle> destlist = CheckTrianglesInNode(node, sourcelist);

							if (destlist.size() > MaxSurfaces)
							{

								std::vector<OctTreeNode*> nodes = SplitNode(node);

								std::cout << "Octree : Branch node, Surfaces in node " << destlist.size() << " recursing" << std::endl;

								RecurseNode(nodes[0], destlist);
								RecurseNode(nodes[1], destlist);
								RecurseNode(nodes[2], destlist);
								RecurseNode(nodes[3], destlist);
								RecurseNode(nodes[4], destlist);
								RecurseNode(nodes[5], destlist);
								RecurseNode(nodes[6], destlist);
								RecurseNode(nodes[7], destlist);

							}
							else
							{

								if (destlist.size() > 0)
								{
									//		vml::os::Trace(L"Leaf node, Surfaces in node %d\n", destlist.size());
							
									std::cout << "Octree : Leaf node, Surfaces in node " << destlist.size() << std::endl;

									if (CreateNode(node, destlist))
									{
										LeafNodes++;
									}
								}

							}

						}
						
						// ----------------------------------------------------------------
						//

						OctTreeNode* AddBox(OctTreeNode* parent, int i, const glm::vec3& min, const glm::vec3& max)
						{
							OctTreeNode* node = new OctTreeNode(parent, (int)OctTreeNodes.size(), min, max);
							parent->SetChild(i, node);
							OctTreeNodes.emplace_back(node);
							return node;
						}

						// ----------------------------------------------------------------
						//

						std::vector<OctTreeNode*> SplitNode(OctTreeNode* node)
						{
							glm::vec3 bMin   = node->GetBoundingBox().GetMin();
							float halfWidth  = node->GetBoundingBox().GetHalfExtents().x;
							float halfHeight = node->GetBoundingBox().GetHalfExtents().y;
							float halfDepth  = node->GetBoundingBox().GetHalfExtents().z;
							
							std::vector<OctTreeNode*> nodes;
							
							// far bottom left node

							nodes.emplace_back(AddBox(node, FAR_BOTTOM_LEFT_NODE, bMin, bMin + glm::vec3(halfWidth, halfHeight, halfDepth)));

							// far bottom right node

							nodes.emplace_back(AddBox(node, FAR_BOTTOM_RIGHT_NODE, bMin + glm::vec3(halfWidth, 0, 0), bMin + glm::vec3(2 * halfWidth, halfHeight, halfDepth)));

							// far top right node

							nodes.emplace_back(AddBox(node, FAR_TOP_RIGHT_NODE, bMin + glm::vec3(halfWidth, halfHeight, 0), bMin + glm::vec3(2 * halfWidth, 2 * halfHeight, halfDepth)));

							// far top left node

							nodes.emplace_back(AddBox(node, FAR_TOP_LEFT_NODE, bMin + glm::vec3(0, halfHeight, 0), bMin + glm::vec3(halfWidth, 2 * halfHeight, halfDepth)));

							// near bottom left node

							nodes.emplace_back(AddBox(node, NEAR_BOTTOM_LEFT_NODE, bMin + glm::vec3(0, 0, halfDepth), bMin + glm::vec3(halfWidth, halfHeight, 2 * halfDepth)));

							// near bottom right node

							nodes.emplace_back(AddBox(node, NEAR_BOTTOM_RIGHT_NODE, bMin + glm::vec3(halfWidth, 0, halfDepth), bMin + glm::vec3(2 * halfWidth, halfHeight, 2 * halfDepth)));

							// near top right node

							nodes.emplace_back(AddBox(node, NEAR_TOP_RIGHT_NODE, bMin + glm::vec3(halfWidth, halfHeight, halfDepth), bMin + glm::vec3(2 * halfWidth, 2 * halfHeight, 2 * halfDepth)));

							// near top left node

							nodes.emplace_back(AddBox(node, NEAR_TOP_LEFT_NODE, bMin + glm::vec3(0, halfHeight, halfDepth), bMin + glm::vec3(halfWidth, 2 * halfHeight, 2 * halfDepth)));
							
							return nodes;
						}
						
						// ----------------------------------------------------------------
						// allocate data for breadth-first traversal

						void CreateStack()
						{
							
							size_t n = OctTreeNodes.size();
							
							if (n < 1)
								vml::os::Message::Error("Octree : ","Cannot create stack");

							// allocate stack

							Stack = new OctTreeNode *[n];

							StackCounter = 0;

							for (size_t i = 0; i < n; ++i)
								Stack[i] = nullptr;

							// allocate space for rednered nodes

							RenderedNodes = new OctTreeNode *[n];

							RenderedNodesCount = 0;

							for (size_t i = 0; i < n; ++i)
								RenderedNodes[i] = nullptr;

						}

						// ----------------------------------------------------------------
						// clear data

						void ReleaseAll()
						{
							// release nodes

							for (size_t i = 0; i < OctTreeNodes.size(); ++i)
								vml::os::SafeDelete(OctTreeNodes[i]);

							// release stack

							vml::os::SafeDeleteArray(Stack);
							vml::os::SafeDeleteArray(RenderedNodes);

							// clear arrays

							VertexArray.clear();
							SurfaceIndices.clear();
							OctTreeNodes.clear();

							// null data members

							Root		       = nullptr;
							CurrentNode        = 0;
							LeafNodes	       = 0;
							StackCounter       = 0;
							RenderedNodesCount = 0;
							RenderedNodeRatio  = 0.0f;
						}
						
					public:
						
						// ----------------------------------------------------------------
						// getters

						const OctTreeNode* GetRoot() const 
						{
							return Root; 
						}
						
						size_t GetMaxSurfaces() const 
						{
							return MaxSurfaces; 
						}
						
						size_t GetLeafNodesCount() const 
						{
							return LeafNodes; 
						}
						
						const OctTreeNode* GetCurrentNode() const 
						{ 
							return OctTreeNodes[CurrentNode]; 
						}

						void GetNextCurrentNode() 
						{
							int n = (int)OctTreeNodes.size() - 1;
							CurrentNode++;
							if (CurrentNode >= n) CurrentNode = n;
						}

						void GetPrevCurrentNode()
						{
							CurrentNode--;
							if (CurrentNode <= 0) CurrentNode = 0;
						}

						size_t GetNodesCount() const
						{
							return OctTreeNodes.size();
						}

						size_t GetRenderedNodesCount() const
						{
							return RenderedNodesCount;
						}

						OctTreeNode** GetRenderedNodes() const
						{
							return RenderedNodes;
						}

						const OctTreeNode* GetRenderedNodeAt(size_t idx) const
						{
							return RenderedNodes[idx];
						}

						const OctTreeNode* GetNodeAt(size_t idx) const
						{
							return OctTreeNodes[idx];
						}
												
						int GetQueryOctTreeNodesCount() const
						{
							return QueryOctTreeNodesCount;
						}

						const std::vector<OctTreeNode*> &GetQueryOctTreeNodes()
						{
							return QueryOctTreeNodes;	
						}

						bool IsValid() const
						{
							return Root != nullptr;
						}

						// ----------------------------------------------------------------
						// transform octrees nodes 

						void Transform(vml::views::View* view)
						{
							if (!Root)
								vml::os::Message::Error("Octree : ", "Compile Map before trasnforming");

							// set visible state back to default for all nodes

							for (size_t i = 0; i < OctTreeNodes.size(); ++i)
								OctTreeNodes[i]->ResetVisibleFlag();
							
							// travese tree

							Stack[0] = Root;

							StackCounter = 1;

							RenderedNodesCount = 0;

							for (size_t i = 0; i < StackCounter; ++i)
							{

								if (Stack[i]->TestAABBox(view->GetFrustumPlanes())!= OctTreeNode::OUTSIDE)
								{
									if (Stack[i]->IsLeaf())
									{
										RenderedNodes[RenderedNodesCount++] = Stack[i];
									}
									else
									{
										if (Stack[i]->GetChild(0)) Stack[StackCounter++] = Stack[i]->GetChild(0);
										if (Stack[i]->GetChild(1)) Stack[StackCounter++] = Stack[i]->GetChild(1);
										if (Stack[i]->GetChild(2)) Stack[StackCounter++] = Stack[i]->GetChild(2);
										if (Stack[i]->GetChild(3)) Stack[StackCounter++] = Stack[i]->GetChild(3);
										if (Stack[i]->GetChild(4)) Stack[StackCounter++] = Stack[i]->GetChild(4);
										if (Stack[i]->GetChild(5)) Stack[StackCounter++] = Stack[i]->GetChild(5);
										if (Stack[i]->GetChild(6)) Stack[StackCounter++] = Stack[i]->GetChild(6);
										if (Stack[i]->GetChild(7)) Stack[StackCounter++] = Stack[i]->GetChild(7);
									}
								}
							}

							// transfor rendered nodes to view

							for (size_t i = 0; i < RenderedNodesCount; ++i)
								RenderedNodes[i]->TransformToView(view);

							// compute rednered node ratio

							RenderedNodeRatio = 100.0f * float(RenderedNodesCount) / float(LeafNodes);
							
						}

						// ----------------------------------------------------------------
						// find node(s) the players is in
						// bounding box is the object's bounding box we want to find
						// the position it is inside the octree

						void FindObjectInNode(vml::views::View* view, const vml::geo3d::AABBox &boundingbox)
						{
							
							//	vml::DebugRender->DrawAABBox(view, boundingbox, vml::colors::Yellow);
							
							QueryOctTreeNodesCount = 0;

							Stack[0] = Root;

							StackCounter = 1;

							for (size_t i = 0; i < StackCounter; ++i)
							{

								if (vml::geo3d::intersections::AABBoxVsAABBox(boundingbox.GetMin(), 
									                                          boundingbox.GetMax(), 
									                                          Stack[i]->GetBoundingBox().GetMin(), 
									                                          Stack[i]->GetBoundingBox().GetMax()) != vml::geo3d::intersections::AABBOX_DISJOINT)
								{
									if (Stack[i]->IsLeaf())
									{
										QueryOctTreeNodes[QueryOctTreeNodesCount++] = Stack[i];
									}
									else
									{
										if (Stack[i]->GetChild(0)) Stack[StackCounter++] = Stack[i]->GetChild(0);
										if (Stack[i]->GetChild(1)) Stack[StackCounter++] = Stack[i]->GetChild(1);
										if (Stack[i]->GetChild(2)) Stack[StackCounter++] = Stack[i]->GetChild(2);
										if (Stack[i]->GetChild(3)) Stack[StackCounter++] = Stack[i]->GetChild(3);
										if (Stack[i]->GetChild(4)) Stack[StackCounter++] = Stack[i]->GetChild(4);
										if (Stack[i]->GetChild(5)) Stack[StackCounter++] = Stack[i]->GetChild(5);
										if (Stack[i]->GetChild(6)) Stack[StackCounter++] = Stack[i]->GetChild(6);
										if (Stack[i]->GetChild(7)) Stack[StackCounter++] = Stack[i]->GetChild(7);
									}
								}
							}
						}

						// ----------------------------------------------------------------
						//
						
						void Go(const vml::meshes::Mesh3d *mesh, unsigned int flags=0)
						{
							if (mesh)
							{
								if (!mesh->IsValid())
									vml::os::Message::Error("Octree : ","Compiler : Mesh is not valid");
								
								ResourceFileName = mesh->GetResourceFileName();

							//	std::cout << "Octree File : " << ResourceFileName << std::endl;
								
								// free octree data

								ReleaseAll();

								// release data or not according to flag

								Flags = flags;
																
								// converts mesh data to internal format 

						//		std::cout << "TRIANGLES : " << mesh->GetSurfaceCount() << std::endl;
						//		std::cout << "TRIANGLES / 4: " << mesh->GetSurfaceCount()/4 << std::endl;

								ConvertData(&mesh->GetVertexArray(), &mesh->GetNormalArray(), &mesh->GetUVArray(), &mesh->GetSurfaceIndices());
								
								MaxSurfaces = ceil(mesh->GetSurfaceCount() / 6);

							//	std::cout << "Octree : Max Triangles per Node : " << MaxSurfaces << std::endl;
																
								// create root node
							
								Root = new OctTreeNode(nullptr, 0, mesh->GetBoundingBox().GetMin(), mesh->GetBoundingBox().GetMax());

								// put in the first place in the node list

								OctTreeNodes.emplace_back(Root);
								
								// build octree
					
								RecurseNode(Root, SurfaceIndices);
								
								// create octree node stack

								CreateStack();
								
								// create daa structure for queries

								QueryOctTreeNodes.resize(OctTreeNodes.size());		
								QueryOctTreeNodesCount = 0;
								
								// clear data

								VertexArray.clear();
								SurfaceIndices.clear();
																
								for (size_t i = 0; i < GetNodesCount(); ++i)
								{
						
									const vml::octree::OctTreeNode* node = GetNodeAt(i);
						
									/*
									vml::os::Message::Trace("id: \nbmin :%f %f %f\nbmax: %f %f %f\n", i,
															node->GetBoundingBox().GetMin().x, node->GetBoundingBox().GetMin().y, node->GetBoundingBox().GetMin().z,
															node->GetBoundingBox().GetMax().x, node->GetBoundingBox().GetMax().y, node->GetBoundingBox().GetMax().z);
									*/
								}
								
							}
							else
							{
								vml::os::Message::Error("Octree : ","Mesh is null");
							}
							
						}

						//-----------------------------------------------------------------------------------
						// copy constructor is private
						// no copies allowed since classes
						// are referenced

						OctTree(OctTree& octtree) = delete;

						//-----------------------------------------------------------------------------------
						// overload operator is private,
						// no copies allowed since classes
						// are referenced

						void operator=(const OctTree& octtree) = delete;

						// ----------------------------------------------------------------
						// ctor / dtor

						OctTree()
						{
							Root		           = nullptr;
							Stack		           = nullptr;
							RenderedNodes          = nullptr;
							CurrentNode	           = 0;
							LeafNodes	           = 0;
							StackCounter           = 0;			
							RenderedNodesCount     = 0;			
							RenderedNodeRatio      = 0.0f;
							BoundingBoxEps         = 0.001f;
							MaxSurfaces	           = 512;
							Flags			       = 0;
							QueryOctTreeNodesCount = 0;
						}

						~OctTree()
						{
							ReleaseAll();
						}

				};
				
	}
}

