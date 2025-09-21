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

namespace vml
{
	namespace overlays
	{
	
		//////////////////////////////////////////////////////////////////
		// effect overlay class

		class EffectOverlay
		{

			protected:

				std::string					   ScreenName;
				std::string					   TextureFileName;
				std::string					   ShaderFileName;
				vml::textures::Texture*		   Texture;
				vml::shaders::GlShaderProgram* Shader;
				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V;
				glm::mat3					   N;
				glm::mat2					   TM;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ViewMatrixLocation;					// view matrix shader location
				GLuint						   ProjectionMatrixLocation;			// projection matrix shader location
				GLuint						   ModelMatrixLocation;
				GLuint						   NormalMatrixLocation;
				GLuint						   ModelViewMatrixLocation;
				GLuint						   TextureSamplerLocation;
				GLuint						   TextureMatrixLocation;
				GLuint						   ShaderProgram;
				GLuint						   AlphaLocation;
				float						   Alpha;
				unsigned int				   BlendMode;
				uint32_t	 				   CullingFlags;						// Bitfield used to store internal flags

				GLsizei						   SurfaceIndicesCount;
				GLsizei						   VertexCount;
				std::vector<GLuint>			   SurfaceIndices;
				std::vector<float>			   VertexUVArray;
				std::vector<float>			   VertexNormalArray;
				std::vector<float>			   VertexPositionArray;

				vml::models::Model3d_2*		   AttachedModel;
				glm::vec3					   BoundingBoxCenter;					// bounding box center
				glm::vec3					   BoundingBoxMin;						// bounding box minimum
				glm::vec3					   BoundingBoxMax;						// bounding box maximum
				glm::vec3					   BoundingBoxExtents;					// bounding box dimensions
				glm::vec3					   BoundingBoxHalfExtents;				// bounding box dimensions
				float						   Radius;
				glm::vec3				       Scaling;
				glm::vec3			           Rotation;
				glm::vec3					   Position;
				glm::vec3					   OriginalPosition;					// old vector position , used for deltas
				glm::vec3					   OriginalRotation;					// old rotation position , used for deltas
				glm::vec3					   OriginalScaling;						// old scaling position , used for deltas
				vml::geo3d::AABBox			   AABoundingBox;						// Bounding box
				vml::geo3d::AOBBox			   AOBoundingBox;						// Bounding box
				glm::vec3					   Right;								// Right direction vector
				glm::vec3					   Forward;								// Forward direction vector
				glm::vec3					   Up;									// Up direction vector
				glm::vec3					   Pivot;
				bool						   Visible;
				bool						   DisableDepth;
								
				// ---------------------------------------------------------------
				//

				void ComputeMetrics()
				{
					BoundingBoxMin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					BoundingBoxMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < VertexCount; i++)
					{
						size_t j = i * 4;
						float px = VertexPositionArray[j    ];
						float py = VertexPositionArray[j + 1];
						float pz = VertexPositionArray[j + 2];
						if (px <= BoundingBoxMin.x) BoundingBoxMin.x = px;
						if (py <= BoundingBoxMin.y) BoundingBoxMin.y = py;
						if (pz <= BoundingBoxMin.z) BoundingBoxMin.z = pz;
						if (px >= BoundingBoxMax.x) BoundingBoxMax.x = px;
						if (py >= BoundingBoxMax.y) BoundingBoxMax.y = py;
						if (pz >= BoundingBoxMax.z) BoundingBoxMax.z = pz;
					}

					glm::vec3 Center = (BoundingBoxMax + BoundingBoxMin) * 0.5f;

					// subtract center from every vertex this centres
					// the mesh at (0,0,0) in mesh's coordinates

					for (size_t i = 0; i < VertexCount; i++)
					{
						size_t j = i * 4;
						VertexPositionArray[j    ] -= Center.x;
						VertexPositionArray[j + 1] -= Center.y;
						VertexPositionArray[j + 2] -= Center.z;
					}

					BoundingBoxMax -= Center;
					BoundingBoxMin -= Center;
					BoundingBoxExtents	= BoundingBoxMax - BoundingBoxMin;
					BoundingBoxHalfExtents = (BoundingBoxMax - BoundingBoxMin) * 0.5f;

					// compute mesh bounding sphere

					Radius = -FLT_MAX;

					for (size_t i = 0; i < VertexCount; i++)
					{
						size_t j = i * 4;

						float d = VertexPositionArray[j    ] * VertexPositionArray[j    ] +
								  VertexPositionArray[j + 1] * VertexPositionArray[j + 1] +
								  VertexPositionArray[j + 2] * VertexPositionArray[j + 2];

						if (d >= Radius)
							Radius = d;
					}

					Radius = sqrtf(Radius);
				}

				// ---------------------------------------------------------------
				//

				void TransformAttached(vml::views::View* view)
				{

					float* ParentMatrix = AttachedModel->GetMptr();

					// cache scaling factors

					float ParentScale[3] = { 0 };
					float InvParentScale[3] = { 0 };

					ParentScale[0] = AttachedModel->GetScaling().x;
					ParentScale[1] = AttachedModel->GetScaling().y;
					ParentScale[2] = AttachedModel->GetScaling().z;

					InvParentScale[0] = 1.0f / ParentScale[0];
					InvParentScale[1] = 1.0f / ParentScale[1];
					InvParentScale[2] = 1.0f / ParentScale[2];

					// compute eluler matrix

					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;
					float invscx = 1.0f / Scaling.x;
					float invscy = 1.0f / Scaling.y;
					float invscz = 1.0f / Scaling.z;

					// get angles

					float alpha = Rotation.x;
					float phi = Rotation.y;
					float theta = Rotation.z;

					// clamp angles

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(alpha);
					float sy = sin(phi);
					float sz = sin(theta);
					float cx = cos(alpha);
					float cy = cos(phi);
					float cz = cos(theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[ 0] = (cy * cz) * scx;
					matrix[ 1] = (sx * sy * cz + cx * sz) * scx;
					matrix[ 2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[ 3] = 0;
					matrix[ 4] = (-cy * sz) * scy;
					matrix[ 5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[ 6] = (cx * sy * sz + sx * cz) * scy;
					matrix[ 7] = 0;
					matrix[ 8] = (sy)*scz;
					matrix[ 9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;
					
				//	matrix[12] = Position.x;
				//	matrix[13] = Position.y;
				//	matrix[14] = Position.z;
					
					matrix[12] = -Pivot.x * matrix[0] * invscx - Pivot.y * matrix[4] * invscy - Pivot.z * matrix[ 8] * invscz + Pivot.x + Position.x;
					matrix[13] = -Pivot.x * matrix[1] * invscx - Pivot.y * matrix[5] * invscy - Pivot.z * matrix[ 9] * invscz + Pivot.y + Position.y;
					matrix[14] = -Pivot.x * matrix[2] * invscx - Pivot.y * matrix[6] * invscy - Pivot.z * matrix[10] * invscz + Pivot.z + Position.z;
				
					matrix[15] = 1;

					// scale parent matrix

					ParentMatrix[ 0] *= InvParentScale[0];
					ParentMatrix[ 1] *= InvParentScale[0];
					ParentMatrix[ 2] *= InvParentScale[0];
					ParentMatrix[ 4] *= InvParentScale[1];
					ParentMatrix[ 5] *= InvParentScale[1];
					ParentMatrix[ 6] *= InvParentScale[1];
					ParentMatrix[ 8] *= InvParentScale[2];
					ParentMatrix[ 9] *= InvParentScale[2];
					ParentMatrix[10] *= InvParentScale[2];

					// concatenate parent and child matrices

					M = AttachedModel->GetM() * M;

					// rescale parent matrix by its own scaling matrix

					ParentMatrix[ 0] *= ParentScale[0];
					ParentMatrix[ 1] *= ParentScale[0];
					ParentMatrix[ 2] *= ParentScale[0];
					ParentMatrix[ 4] *= ParentScale[1];
					ParentMatrix[ 5] *= ParentScale[1];
					ParentMatrix[ 6] *= ParentScale[1];
					ParentMatrix[ 8] *= ParentScale[2];
					ParentMatrix[ 9] *= ParentScale[2];
					ParentMatrix[10] *= ParentScale[2];

					MVP = view->GetViewProjection() * M;
				}

				// ---------------------------------------------------------------
				// transform matrix if object is not attached 

				void TransformUnattached(vml::views::View* view)
				{

					// compute eluler matrix

					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;
					float invscx = 1.0f / Scaling.x;
					float invscy = 1.0f / Scaling.y;
					float invscz = 1.0f / Scaling.z;

					// get angles

					float Alpha = Rotation.x;
					float Phi = Rotation.y;
					float Theta = Rotation.z;

					// clamp angles

					if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

					if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

					if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(Alpha);
					float sy = sin(Phi);
					float sz = sin(Theta);
					float cx = cos(Alpha);
					float cy = cos(Phi);
					float cz = cos(Theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[0] = (cy * cz) * scx;
					matrix[1] = (sx * sy * cz + cx * sz) * scx;
					matrix[2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[3] = 0;
					matrix[4] = (-cy * sz) * scy;
					matrix[5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[6] = (cx * sy * sz + sx * cz) * scy;
					matrix[7] = 0;
					matrix[8] = (sy)*scz;
					matrix[9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;

				//	matrix[12] = Position.x;
				//	matrix[13] = Position.y;
				//	matrix[14] = Position.z;

					matrix[12] = -Pivot.x * matrix[0] * invscx - Pivot.y * matrix[4] * invscy - Pivot.z * matrix[ 8] * invscz + Pivot.x + Position.x;
					matrix[13] = -Pivot.x * matrix[1] * invscx - Pivot.y * matrix[5] * invscy - Pivot.z * matrix[ 9] * invscz + Pivot.y + Position.y;
					matrix[14] = -Pivot.x * matrix[2] * invscx - Pivot.y * matrix[6] * invscy - Pivot.z * matrix[10] * invscz + Pivot.z + Position.z;

					matrix[15] = 1;

					// note that you have to compute manually this matrix

				//	M = OptM * M;

					// concatenate optional parent matrix and child matrices

					MV = view->GetView() * M;
					MVP = view->GetViewProjection() * M;

				}

				// ---------------------------------------------------------------
				//

				void CreateQuad()
				{
					Init();
					AddVertex(glm::vec4(-0.5f,  0.5f, 0, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1));
					AddVertex(glm::vec4( 0.5f,  0.5f, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1));
					AddVertex(glm::vec4( 0.5f, -0.5f, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0));
					AddVertex(glm::vec4(-0.5f, -0.5f, 0, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0));
					AddSurface(0, 1, 2);
					AddSurface(0, 2, 3);
					Finalize();
				}

				// ---------------------------------------------------------------
				//

				void CreateFan2()
				{
					int step;
					float rad;
					float radstep;

					glm::vec3 p1(-0.5, 0,  0.5);
					glm::vec3 p2( 0.5, 0,  0.5);
					glm::vec3 p3( 0.5, 0, -0.5);
					glm::vec3 p4(-0.5, 0, -0.5);

					Init();

					step = 3;
					rad = 0.0f;
					radstep = 180.0f * vml::math::DEGTORAD / float(step);

					for ( int i=0; i<step; ++i)
					{
						float sx = sin(rad);
						float cx = cos(rad);

						float m00 = 1 , m01 = 0  , m02 =   0;
						float m10 = 0 , m11 = cx , m12 = -sx;
						float m20 = 0 , m21 = sx , m22 =  cx;

						glm::vec3 r1(p1.x * m00 + p1.y * m01 + p1.z * m02,
									 p1.x * m10 + p1.y * m11 + p1.z * m12,
									 p1.x * m20 + p1.y * m21 + p1.z * m22);

						glm::vec3 r2(p2.x * m00 + p2.y * m01 + p2.z * m02,
									 p2.x * m10 + p2.y * m11 + p2.z * m12,
									 p2.x * m20 + p2.y * m21 + p2.z * m22);

						glm::vec3 r3(p3.x * m00 + p3.y * m01 + p3.z * m02,
									 p3.x * m10 + p3.y * m11 + p3.z * m12,
									 p3.x * m20 + p3.y * m21 + p3.z * m22);

						glm::vec3 r4(p4.x * m00 + p4.y * m01 + p4.z * m02,
									 p4.x * m10 + p4.y * m11 + p4.z * m12,
									 p4.x * m20 + p4.y * m21 + p4.z * m22);

						AddVertex(glm::vec4(r1.x, r1.y, r1.z, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1));
						AddVertex(glm::vec4(r2.x, r2.y, r2.z, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1));
						AddVertex(glm::vec4(r3.x, r3.y, r3.z, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0));
						AddVertex(glm::vec4(r4.x, r4.y, r4.z, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0));

						int j = i * 4;

						AddSurface(j, j + 1, j + 2);
						AddSurface(j, j + 2, j + 3);

						rad += radstep;
	
					}
		
					Finalize();
				}

				// ---------------------------------------------------------------
				//

				void CreateMesh(uint32_t meshtype)
				{
					switch (meshtype)
					{
						case QUAD:
							CreateQuad();
						break;

						case FAN :
							CreateFan2();
						break;

						default :
							vml::os::Message::Error("Overlay", "Unknown Mesh");
						break;
					}
				}

			public:

				// ---------------------------------------------------------------
				//

				static const unsigned int ADDICTIVE_BLENDING		  = 0;  // for additive blending
				static const unsigned int TRANSPARENCY				  = 1;	// for real transparency
				static const unsigned int ADDICTIVE_BLENDING_NO_ALPHA = 2;  // for additive blending without alpha affecting the result

				// ---------------------------------------------------------------
				//

				static const unsigned int QUAD = 0;		// Quad mesh
				static const unsigned int FAN  = 1;		// Fan mesh

				// ---------------------------------------------------------------
				//

				void Init()
				{

					SurfaceIndicesCount = 0;
					VertexCount = 0;

					// load texture

					const vml::utils::Flags TexturesParms = { vml::textures::CLAMP_TO_BORDER_S,
															  vml::textures::REPEAT_T,
															  vml::textures::FILTER_MAG_LINEAR,
															  vml::textures::FILTER_MIN_LINEAR,
															  vml::textures::MIPMAPS };

					Texture = vml::stores::TextureStore->Load<vml::textures::Texture>(TextureFileName, TexturesParms);

					// load shader
					const std::string FullDebugPath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
					ShaderFileName = FullDebugPath + "/shaders/debug_texture_alpha.shd";
					Shader = vml::stores::ShaderStore->Load< vml::shaders::GlShaderProgram>(ShaderFileName, {});

					// shader locations
					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ModelMatrixLocation = Shader->GetModelMatrixLocation();
					ModelViewMatrixLocation = Shader->GetModelViewMatrixLocation();
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					NormalMatrixLocation = Shader->GetNormalMatrixLocation();
					TextureMatrixLocation = Shader->GetTextureMatrixLocation();
					ViewMatrixLocation = Shader->GetViewMatrixLocation();
					ProjectionMatrixLocation = Shader->GetProjectionMatrixLocation();
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					AlphaLocation = glGetUniformLocation(ShaderProgram, "Alpha");
					glUseProgram(0);
				}

				// ---------------------------------------------------------------
				//

				void AddVertex(const glm::vec4& pos, const glm::vec3& normal, const glm::vec2& uv)
				{
					// add position
					VertexPositionArray.emplace_back(pos.x);
					VertexPositionArray.emplace_back(pos.y);
					VertexPositionArray.emplace_back(pos.z);
					VertexPositionArray.emplace_back(1);

					// add normal
					VertexNormalArray.emplace_back(normal.x);
					VertexNormalArray.emplace_back(normal.y);
					VertexNormalArray.emplace_back(normal.z);

					// add uv
					VertexUVArray.emplace_back(uv.x);
					VertexUVArray.emplace_back(uv.y);

					// increase counter
					VertexCount++;
				}

				// ---------------------------------------------------------------
				//

				void AddSurface(const int i0, const int i1, const int i2)
				{
					// add surface indices
					SurfaceIndices.emplace_back(i0);
					SurfaceIndices.emplace_back(i1);
					SurfaceIndices.emplace_back(i2);

					// increase counter
					SurfaceIndicesCount += 3;
				}

				// ---------------------------------------------------------------
				//

				void Finalize()
				{
					if (VertexPositionArray.size() == 0 || VertexUVArray.size() == 0 || SurfaceIndices.size() == 0)
						vml::os::Message::Error("Overlay : ", "Cannot create vaoid");

					// compue metrics
					ComputeMetrics();

					// Create the VAO
					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 
					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.
					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;

					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);

					// Generate and populate the buffers with vertex attributes and the indices
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, VertexCount * 4 * sizeof(float), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, VertexCount * 2 * sizeof(float), &VertexUVArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeTexCoord);
					glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 
					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, SurfaceIndices.size() * sizeof(GLuint), &SurfaceIndices[0], GL_STATIC_DRAW);

					// unbinds buffers
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);

					glDisableVertexAttribArray(AttributePosition);
					glDisableVertexAttribArray(AttributeTexCoord);
				}

				// ---------------------------------------------------------------
				// transform model bounding box

				void Transform(vml::views::View* view)
				{
					if (AttachedModel)
						TransformAttached(view);
					else
						TransformUnattached(view);

					// compute oriented bounding box
					// the bounding box is computed transforming
					// models' mesh's bounding box by the model's
					// M matrix , which is the T * R * S matrix
					// see aabbox.h

					// cache model's M matrix

					const float* m = glm::value_ptr(M);

					// get's model's mesh bounding box coordinates

					AOBoundingBox.Set(BoundingBoxMin, BoundingBoxMax, m);

					// compute axis aligned bounding box

					AABoundingBox.Set(AOBoundingBox.GetMin(), AOBoundingBox.GetMax());

					// compute bounding sphere

					Radius = sqrtf(AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
								   AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
								   AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z) * 0.5f;

					// compute direction vectors
					// the oriented bounding box is centered at root's center

					Right	= glm::normalize(glm::vec3(m[0], m[1], m[2]));
					Up		= glm::normalize(glm::vec3(m[4], m[5], m[6]));
					Forward = glm::normalize(glm::vec3(m[8], m[9], m[10]));
				}

				// ---------------------------------------------------------------
				// 

				[[nodiscard]] float						GetRadius()				const { return Radius; }
				[[nodiscard]] float						GetWidth()				const { return BoundingBoxExtents[0]; }
				[[nodiscard]] float						GetHeight()				const { return BoundingBoxExtents[1]; }
				[[nodiscard]] float						GetDepth()			    const { return BoundingBoxExtents[2]; }
				[[nodiscard]] const glm::vec3&			GetBoundingBoxMin()	    const { return BoundingBoxMin; }
				[[nodiscard]] const glm::vec3&			GetBoundingBoxMax()	    const { return BoundingBoxMax; }
				[[nodiscard]] const glm::vec3&			GetBoundingBoxExtents() const { return BoundingBoxExtents; }
				[[nodiscard]] const std::string&		GetScreenName()			const { return ScreenName; }
				[[nodiscard]] const std::string&		GetTextureFileName()    const { return TextureFileName; }
				[[nodiscard]] const std::string&		GetShaderFileName()     const { return ShaderFileName; }
				[[nodiscard]] const vml::geo3d::AABBox& GetAABoundingBox()		const { return AABoundingBox; }
				[[nodiscard]] const vml::geo3d::AOBBox& GetAOBoundingBox()		const { return AOBoundingBox; }
				[[nodiscard]] const glm::vec3&			GetRightVector()		const { return Right; }
				[[nodiscard]] const glm::vec3&			GetForwardVector()		const { return Forward; }
				[[nodiscard]] const glm::vec3&			GetUpVector()			const { return Up; }
				[[nodiscard]] bool						IsVisible()				const { return Visible; }
				[[nodiscard]] uint32_t					GetCullingFlags()		const { return CullingFlags; }
				[[nodiscard]] bool						IsInFrustum()			const { return CullingFlags != vml::views::frustum::OUTSIDE; }
				[[nodiscard]] const glm::vec3&			GetPosition()			const { return Position; }
				[[nodiscard]] const glm::vec3&			GetRotation()			const { return Rotation; }
				[[nodiscard]] const glm::vec3&			GetScaling()			const { return Scaling; }
				[[nodiscard]] const glm::vec3&			GetOriginalPosition()	const { return OriginalPosition; }
				[[nodiscard]] const glm::vec3&			GetOriginalRotation()	const { return OriginalRotation; }
				[[nodiscard]] const glm::vec3&			GetOriginalScaling()	const { return OriginalScaling; }

				// ---------------------------------------------------------------
				// get tranformed positoion

				glm::vec3 GetTransformedPosition() const
				{
					const float* m = glm::value_ptr(M);
					return glm::vec3(m[12], m[13], m[14]);
				}

				// ---------------------------------------------------------------
				// setters

				void SetVisible() { Visible = true; }
				void SetInVisible() { Visible = false; }
				void SetDepthEnabled() { DisableDepth = false; }
				void SetDepthDisabled() { DisableDepth = true; }

				void SetPosition(const glm::vec3& pos) { Position = pos; }								 // Set model position given a vector
				void Move(const glm::vec3& disp) { Position += disp; }									 // Move object to position
				void MoveX(const float px) { Position.x += px; }										 // Move object to position
				void MoveY(const float py) { Position.y += py; }										 // Move object to position
				void MoveZ(const float pz) { Position.z += pz; }										 // Move object to position
				void SetPosX(const float px) { Position.x = px; }										 // Set model position given a vector
				void SetPosY(const float py) { Position.y = py; }										 // Set model position given a vector
				void SetPosZ(const float pz) { Position.z = pz; }										 // Set model position given a vector
				void SetAngles(const glm::vec3& angles) { Rotation = angles * vml::math::DEGTORAD; }	 // Rotation angles
				void AddAngles(const glm::vec3& angles) { Rotation += angles * vml::math::DEGTORAD; }	 // Add rotation angles
				void SetAngleX(const float ax) { Rotation.x = ax * vml::math::DEGTORAD; }				 // Set Rotation angle x
				void SetAngleY(const float ay) { Rotation.y = ay * vml::math::DEGTORAD; }				 // Set Rotation angle y
				void SetAngleZ(const float az) { Rotation.z = az * vml::math::DEGTORAD; }				 // Set Rotation angle z
				void AddAngleX(const float ax) { Rotation.x += ax * vml::math::DEGTORAD; }				 // Increase Rotation angle x
				void AddAngleY(const float ay) { Rotation.y += ay * vml::math::DEGTORAD; }				 // Increase Rotation angle y
				void AddAngleZ(const float az) { Rotation.z += az * vml::math::DEGTORAD; }				 // Increase Rotation angle z
				void SetScaling(const glm::vec3& scaling) { Scaling = scaling; }
				void SetScaleX(const float sx) { Scaling.x = sx; }										 // Set Model x scale
				void SetScaleY(const float sy) { Scaling.y = sy; }										 // Set Model y scale
				void SetScaleZ(const float sz) { Scaling.z = sz; }										 // Set Model z scale
				void AddScaleX(const float sx) { Scaling.x *= sx; }										 // Increase Model x scale
				void AddScaleY(const float sy) { Scaling.y *= sy; }										 // Increase Model y scale
				void AddScaleZ(const float sz) { Scaling.z *= sz; }										 // Increase Model z scale
				void ResetScaling() { Scaling = OriginalScaling; }										 // Resets model scale
				void ResetPosition() { Position = OriginalPosition; }								 	 // Reset model position
				void ResetRotation() { Rotation = OriginalRotation; }									 // Reset angle rotation with initial values

				void ResetTransformations()																 // reset all transofrmations
				{
					Position = OriginalPosition;
					Rotation = OriginalRotation;
					Scaling = OriginalScaling;
				}

				void SetCullingFlags(unsigned int cullingflags) { CullingFlags = cullingflags; }
				void SetCullingFlagToOutside()				    { CullingFlags = vml::views::frustum::OUTSIDE; }
				void SetCullingFlagToIntersected()			    { CullingFlags = vml::views::frustum::INTERSECTED; }
				void SetCullingFlagToInside()					{ CullingFlags = vml::views::frustum::INSIDE; }
				void SetPivot(const glm::vec3& pos)				{ Pivot = pos; }		

				// ---------------------------------------------------------------
				//

				void AttachModel(vml::models::Model3d_2* model)
				{
					if(!model)
						vml::os::Message::Error("Overlay : ", "Cannot attach to null model pointer");

					AttachedModel = model;
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view)
				{
					if (!Visible)
						return;

					if (view)
					{
						CullingFlags = vml::views::frustum::TestAABBox(view->GetFrustumPlanes(), AABoundingBox.GetCenter(), AABoundingBox.GetHalfExtents());

						if (CullingFlags != vml::views::frustum::OUTSIDE)
						{

				//			std::cout << Position.x << " " << Position.y << " " << Position.z << " " << std::endl;

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform1i(TextureSamplerLocation, 0);
							glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
							glUniform1f(AlphaLocation, Alpha);

							glEnable(GL_TEXTURE_2D);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, Texture->GetID());

							glBindVertexArray(VAOid);

							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							glDisable(GL_CULL_FACE);
							glEnable(GL_BLEND);

							if (!DisableDepth)
								glDisable(GL_DEPTH_TEST);
							else
								glEnable(GL_DEPTH_TEST);

							if (BlendMode == ADDICTIVE_BLENDING)		  glBlendFunc(GL_SRC_ALPHA, GL_ONE);				 // for additive blending
							if (BlendMode == TRANSPARENCY)				  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // for real transparency
							if (BlendMode == ADDICTIVE_BLENDING_NO_ALPHA) glBlendFunc(GL_ONE, GL_ONE);					     // for additive blending without alpha affecting the result

							glDepthMask(GL_FALSE);
							glDrawElements(GL_TRIANGLES, SurfaceIndicesCount, GL_UNSIGNED_INT, 0);
							glDepthMask(GL_TRUE);

							if (!DisableDepth)
								glEnable(GL_DEPTH_TEST);

							glEnable(GL_CULL_FACE);
							glDisable(GL_BLEND);
							glDisable(GL_TEXTURE_2D);

							glBindTexture(GL_TEXTURE_2D, 0);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);
						}
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}


				// ---------------------------------------------------
				// ctor / dtor

				EffectOverlay(uint32_t meshtype,const std::string &screenname,const std::string& texturepath, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling)
				{
					if (screenname.size() == 0)
						vml::os::Message::Error("Overlay : ", "ScreenName name cannot be null");
					if (texturepath.size() == 0)
						vml::os::Message::Error("Overlay : ", "ResourceName cannot be null");

					ScreenName = screenname;
					TextureFileName = texturepath;

					VAOid				= 0;
					BufferObjects[0]    = 0;
					BufferObjects[1]    = 0;
					BufferObjects[2]    = 0;
					BufferObjects[3]    = 0;
					IndexBufferObject   = 0;
					M					= glm::mat4(1.0f);
					N					= glm::mat3(1.0f);
					V					= glm::mat4(1.0f);
					MV					= glm::mat4(1.0f);
					MVP					= glm::mat4(1.0f);
					TM					= glm::mat2(1, 0, 0, 1);
					Alpha				= 1.0f;
					SurfaceIndicesCount = 0;
					VertexCount			= 0;
					Texture				= nullptr;
					Shader				= nullptr;
					AttachedModel		= nullptr;
					CullingFlags		= 0;

					// shader locations

					NormalMatrixLocation			  = 0;
					ModelMatrixLocation				  = 0;
					ModelViewMatrixLocation           = 0;
					ModelViewProjectionMatrixLocation = 0;
					ProjectionMatrixLocation		  = 0;
					ViewMatrixLocation				  = 0;
					TextureSamplerLocation			  = 0;
					TextureMatrixLocation			  = 0;
					ShaderProgram					  = 0;
					AlphaLocation					  = 0;

					//

					BlendMode	 = ADDICTIVE_BLENDING;
					Visible		 = true;
					DisableDepth = true;

					Position		 = pos;
					Rotation		 = rot*vml::math::DEGTORAD;
					Scaling			 = scaling;
					OriginalPosition = Position;
					OriginalRotation = Rotation;
					OriginalScaling  = Scaling;

					// init direction vector

					Right   = glm::vec3(1, 0, 0);
					Up	    = glm::vec3(0, 1, 0);
					Forward = glm::vec3(0, 0, 1);

					// initialise metrics

					Pivot				   = glm::vec3(0, 0, 0);
					BoundingBoxMin         = glm::vec3(0, 0, 0);
					BoundingBoxMax         = glm::vec3(0, 0, 0);
					BoundingBoxExtents     = glm::vec3(0, 0, 0);
					BoundingBoxHalfExtents = glm::vec3(0, 0, 0);
					BoundingBoxCenter      = glm::vec3(0, 0, 0);
					AOBoundingBox		   = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AABoundingBox		   = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius				   = 0;
			
					// create mesh

					CreateMesh(meshtype);
				}

				virtual ~EffectOverlay()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::stores::TextureStore->UnLoad(TextureFileName);
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};
	}
}

