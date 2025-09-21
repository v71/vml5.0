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
//	OUT OF OR IN CONNECTION WITH 7THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////////
// draw a wire cube for debu rendeirng purposes

namespace vml
{
	namespace debugrendering
	{
			
			/////////////////////////////////////////////////////////////////
			// debug rednering mesh class

			class WireCube
			{
				
				private:
					
					const static unsigned int Vertices =  8;	// Vertices for this mesh
					const static unsigned int Indices  = 16;	// vbo indices for surfaces

					// vertices for aligned cube ,these vertices are constant

					inline static float AABBoxVertexPositionArray[] =
					{
						-0.5f, -0.5f, -0.5f, 1,
						 0.5f, -0.5f, -0.5f, 1, 
						 0.5f,  0.5f, -0.5f, 1, 
						-0.5f,  0.5f, -0.5f, 1, 
						-0.5f, -0.5f,  0.5f, 1, 
						 0.5f, -0.5f,  0.5f, 1, 
						-0.5f,  0.5f,  0.5f, 1, 
						 0.5f,  0.5f,  0.5f, 1, 
					};
					
					// indices

					inline static GLuint SurfaceIndices[] =
					{
						0,1,2,3,4,5,7,6,0,4,1,5,2,7,3,6
					};
					
					// vertices for axis oriented bounding box
					// these vertices will be overwritten using
					// actual vertices coordinates

					inline static float AOBBoxVertexPositionArray[] =
					{
						-0.5f, -0.5f, -0.5f, 1,
						 0.5f, -0.5f, -0.5f, 1,
						 0.5f,  0.5f, -0.5f, 1,
						-0.5f,  0.5f, -0.5f, 1,
						-0.5f, -0.5f,  0.5f, 1,
						 0.5f, -0.5f,  0.5f, 1,
						-0.5f,  0.5f,  0.5f, 1,
						 0.5f,  0.5f,  0.5f, 1,
					};

					// vaoid for axis aligned bounding box ( aabbox )

					GLuint							AABBoxVAOid;						// vertex array object id for axis aligned bounding box
					GLuint							AABBoxBufferObject;					// vertex buffer object for axis aligned bounding box
					GLuint							AABBoxIndexBufferObject;			// surface index buffer object for axis aligned bounding box

					// vaoid for axis oriented bounding box ( aobbox )

					GLuint						   AOBBoxVAOid;						// vertex array object id for axis oriented bounding box
					GLuint						   AOBBoxBufferObject;					// vertex buffer object for axis oriented bounding box
					GLuint						   AOBBoxIndexBufferObject;			// surface index buffer object for axis oriented bounding box

					GLint						   ColorLocation;
					GLint						   ModelViewProjectionMatrixLocation;
					GLint						   ShaderProgram;
					vml::shaders::GlShaderProgram* Shader;
					glm::mat4					   M,MVP;

					// shader filename
					const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_single_color.shd";

				public:
					
					// ---------------------------------------------------------------
					// axis aligned bounding box drawing
					
					void Draw(vml::views::View* view,
							  const glm::vec3 &bmin, const glm::vec3 &bmax,
							  const glm::vec4 &col)
					{
						if (view)
						{

							float *mptr = glm::value_ptr(M);
						
							// scale parent matrix

							mptr[ 0] = (bmax.x - bmin.x);
							mptr[ 1] = 0;
							mptr[ 2] = 0;
							mptr[ 3] = 0;
							mptr[ 4] = 0;
							mptr[ 5] = (bmax.y - bmin.y);
							mptr[ 6] = 0;
							mptr[ 7] = 0;
							mptr[ 8] = 0;
							mptr[ 9] = 0;
							mptr[10] = (bmax.z - bmin.z);
							mptr[11] = 0;
							mptr[12] = (bmax.x + bmin.x)*0.5f;
							mptr[13] = (bmax.y + bmin.y)*0.5f;
							mptr[14] = (bmax.z + bmin.z)*0.5f;
							mptr[15] = 1;

							MVP = view->GetViewProjection() * M;
							
							// Use debug shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AABBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
						}
					}
					
					// ---------------------------------------------------------------
					// axis aligned bounding box drawing

					void Draw(vml::views::View* view, const glm::mat4& m, const glm::vec4& col)
					{
						if (view)
						{
							M = m;

							MVP = view->GetViewProjection() * M;

							// Use debug shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AABBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glUseProgram(0);

						}
						else
						{
							vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
						}
					}

					// ---------------------------------------------------------------

					void Draw(vml::views::View* view,
							  const glm::vec3& a0, const glm::vec3& a1, const glm::vec3& a2, const glm::vec3& a3,
							  const glm::vec3& a4, const glm::vec3& a5, const glm::vec3& a6, const glm::vec3& a7,
							  const glm::vec4& col) 
					{
						
						if (view)
						{

							// vertices for aligned cube ,these vertices are constant
							
							AOBBoxVertexPositionArray[ 0] = a0.x;	AOBBoxVertexPositionArray[ 1] = a0.y;	AOBBoxVertexPositionArray[ 2] = a0.z; AOBBoxVertexPositionArray[ 3] = 1;
							AOBBoxVertexPositionArray[ 4] = a1.x;	AOBBoxVertexPositionArray[ 5] = a1.y;	AOBBoxVertexPositionArray[ 6] = a1.z; AOBBoxVertexPositionArray[ 7] = 1;
							AOBBoxVertexPositionArray[ 8] = a2.x;	AOBBoxVertexPositionArray[ 9] = a2.y;	AOBBoxVertexPositionArray[10] = a2.z; AOBBoxVertexPositionArray[11] = 1;
							AOBBoxVertexPositionArray[12] = a3.x;	AOBBoxVertexPositionArray[13] = a3.y;	AOBBoxVertexPositionArray[14] = a3.z; AOBBoxVertexPositionArray[15] = 1;
							AOBBoxVertexPositionArray[16] = a4.x;	AOBBoxVertexPositionArray[17] = a4.y;	AOBBoxVertexPositionArray[18] = a4.z; AOBBoxVertexPositionArray[19] = 1;
							AOBBoxVertexPositionArray[20] = a5.x;	AOBBoxVertexPositionArray[21] = a5.y;	AOBBoxVertexPositionArray[22] = a5.z; AOBBoxVertexPositionArray[23] = 1;
							AOBBoxVertexPositionArray[24] = a6.x;	AOBBoxVertexPositionArray[25] = a6.y;	AOBBoxVertexPositionArray[26] = a6.z; AOBBoxVertexPositionArray[27] = 1;
							AOBBoxVertexPositionArray[28] = a7.x;	AOBBoxVertexPositionArray[29] = a7.y;	AOBBoxVertexPositionArray[30] = a7.z; AOBBoxVertexPositionArray[31] = 1;

							float* mptr = glm::value_ptr(M);

							// scale parent matrix

							mptr[ 0] = 1;
							mptr[ 1] = 0;
							mptr[ 2] = 0;
							mptr[ 3] = 0;
							mptr[ 4] = 0;
							mptr[ 5] = 1;
							mptr[ 6] = 0;
							mptr[ 7] = 0;
							mptr[ 8] = 0;
							mptr[ 9] = 0;
							mptr[10] = 1;
							mptr[11] = 0;
							mptr[12] = 0;
							mptr[13] = 0;
							mptr[14] = 0;
							mptr[15] = 1;

							MVP = view->GetViewProjection() * M;

							glBindVertexArray(AOBBoxVAOid);
						
							// Buffer orphaning, a common way to improve streaming perf. 

							glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
							glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(AABBoxVertexPositionArray), &AOBBoxVertexPositionArray[0]);

							// 2nd attribute buffer : positions of particles' centers

							glEnableVertexAttribArray(vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION);
							
							// forse si puo' togliere

							glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
							glVertexAttribPointer(vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION,        // attribute. No particular reason for 1, but must match the layout in the shader.
									 			  4,														// size : x + y + z 
												  GL_FLOAT,													// type
												  GL_FALSE,													// normalized?
												  0,														// stride
												  (void*)0													// array buffer offset
												 );

							// use shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AOBBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glUseProgram(0);

						}
						else
						{
							vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
						}
						
					}
				
					// ---------------------------------------------------------------

					const glm::mat4& GetMVP() const { return MVP; }
					const glm::mat4& GetM() const { return M; }

					// ---------------------------------------------------------------
					// constructors / destructors

					WireCube()
					{
						AOBBoxVAOid						  = 0;
						AOBBoxBufferObject				  = 0;		
						AOBBoxIndexBufferObject			  = 0;
						AABBoxVAOid						  = 0;
						AABBoxIndexBufferObject			  = 0;
						AABBoxBufferObject				  = 0;
						AABBoxIndexBufferObject			  = 0;	
						ColorLocation					  = 0;
						ModelViewProjectionMatrixLocation = 0;
						ShaderProgram					  = 0;
						Shader							  = nullptr;
						
						M = glm::mat4(1.0f);
						MVP = glm::mat4(1.0f);

						// Create the VAO for axis aligned bounding box

						glGenVertexArrays(1, &AABBoxVAOid);
						glBindVertexArray(AABBoxVAOid);

						// Create the Vertex Buffer Object 

						glGenBuffers(1, &AABBoxBufferObject);

						GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

						glBindBuffer(GL_ARRAY_BUFFER, AABBoxBufferObject);
						glBufferData(GL_ARRAY_BUFFER, sizeof(AABBoxVertexPositionArray), &AABBoxVertexPositionArray[0], GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributePosition);
						glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

						// Create the Index Buffer Object 

						glGenBuffers(1, &AABBoxIndexBufferObject);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBoxIndexBufferObject);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SurfaceIndices), &SurfaceIndices[0], GL_STATIC_DRAW);

						// unbinds buffers

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

						// vao for axis oriented bounding box vertices cube

						glGenVertexArrays(1, &AOBBoxVAOid);
						glBindVertexArray(AOBBoxVAOid);
						glGenBuffers(1, &AOBBoxBufferObject);
						glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
						// Initialize with empty (NULL) buffer : it will be updated later, each frame.
						glBufferData(GL_ARRAY_BUFFER, sizeof(AABBoxVertexPositionArray), NULL, GL_STREAM_DRAW);
						// Create the Index Buffer Object 
						glGenBuffers(1, &AOBBoxIndexBufferObject);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AOBBoxIndexBufferObject);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SurfaceIndices), &SurfaceIndices[0], GL_STREAM_DRAW);
						glUseProgram(0);

						glDisableVertexAttribArray(AttributePosition);

						// attach shader

						Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

						// shader locations

						ShaderProgram = Shader->GetID();
						glUseProgram(ShaderProgram);
						ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
						glUseProgram(0);
						
					}

					~WireCube()
					{
						// release opengl vao
						if (AABBoxVAOid) { glDeleteVertexArrays(1, &AABBoxVAOid); AABBoxVAOid = 0; }
						if (AABBoxIndexBufferObject) { glDeleteBuffers(1, &AABBoxIndexBufferObject); AABBoxIndexBufferObject = 0; }
						if (AABBoxBufferObject) { glDeleteBuffers(1, &AABBoxBufferObject); AABBoxBufferObject = 0; }
						if (AOBBoxVAOid) { glDeleteVertexArrays(1, &AOBBoxVAOid); AOBBoxVAOid = 0; }
						if (AOBBoxIndexBufferObject) { glDeleteBuffers(1, &AOBBoxIndexBufferObject); AOBBoxIndexBufferObject = 0; }
						if (AOBBoxBufferObject) { glDeleteBuffers(1, &AOBBoxBufferObject); AOBBoxBufferObject = 0; }
						// ulnoad shader
						vml::stores::ShaderStore->UnLoad(ShaderFileName);
					}

			};
			
	} // end of debugrednering namespace

} // end of vml namespace

namespace vml
{
	namespace debugrendering
	{
		
		class SolidCube 
		{
			private:

				// verex and surfaces count

				const static unsigned int Vertices = 24;
				const static unsigned int Indices  = 36;

				// vertices

				inline static float VertexPositionArray[] =
				{
					// front surface

					-0.5f,  0.5f, 0.5f, 1,
					 0.5f,  0.5f, 0.5f, 1,
					 0.5f, -0.5f, 0.5f, 1,
					-0.5f, -0.5f, 0.5f, 1,

					// left surfaces 

					-0.5f,  0.5f,  0.5f, 1,
					-0.5f,  0.5f, -0.5f, 1,
					-0.5f, -0.5f, -0.5f, 1,
					-0.5f, -0.5f,  0.5f, 1,
					
					// right surfaces

					 0.5f,  0.5f, -0.5f, 1,
					 0.5f,  0.5f,  0.5f, 1,
					 0.5f, -0.5f, -0.5f, 1,
					 0.5f, -0.5f,  0.5f, 1,
					 
					 // back surfaces

					-0.5f,  0.5f, -0.5f, 1,
					 0.5f,  0.5f, -0.5f, 1,
					 0.5f, -0.5f, -0.5f, 1,
					-0.5f, -0.5f, -0.5f, 1,
					 
					 // top surfaces

					-0.5f, 0.5f,  0.5f, 1,
					-0.5f, 0.5f, -0.5f, 1,
					 0.5f, 0.5f, -0.5f, 1,
					 0.5f, 0.5f,  0.5f, 1,
					 
					  // bottom surfaces

					-0.5f, -0.5f,  0.5f, 1,
					-0.5f, -0.5f, -0.5f, 1,
					 0.5f, -0.5f, -0.5f, 1,
					 0.5f, -0.5f,  0.5f, 1,
					
				};

				// normals

				inline static float VertexNormalArray[] =
				{
					// front surface

					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					
					// left surfaces 

					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,
					
					// right surfaces

					1, 0, 0,
					1, 0, 0,
					1, 0, 0,
					1, 0, 0,
					
					// back surfaces

					0, 0, -1,
					0, 0, -1,
					0, 0, -1,
					0, 0, -1,
					
					// top surfaces

					0, 1, 0,
					0, 1, 0,
					0, 1, 0,
					0, 1, 0,
					
					// bottom surfaces

					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					
				};

				// uvs

				inline static float VertexUVArray[] =
				{
					// front surface

					0, 0,
					1, 0,
					1, 1,
					0, 1,
					
					// left surfaces

					1, 0,
					0, 0,
					0, 1,
					1, 1,
					
					// right surfaces

					1, 0,
					0, 0,
					1, 1,
					0, 1,
					
					// back surfaces

					1, 1,
					0, 1,
					0, 0,
					1, 0,
					
					// top surfaces

					0, 1,
					0, 0,
					1, 0,
					1, 1,
					
					// bottom surfaces

					0, 0,
					0, 1,
					1, 1,
					1, 0,
					
				};
			
				// add surfaces 

				inline static GLuint SurfaceIndices[] =
				{
			  		   // front surface

					   0,  2,  1,
					   0,  3,  2,

					   // left surfaces

					   4,  5,  6,
					   4,  6,  7,

					   // right surfaces

					   8,  9, 10,
					   9, 11, 10,

					   // back surfaces

					  12, 13, 14,
					  12, 14, 15,

					  // top surfaces

					  16, 18, 17,
					  16, 19, 18,

					  // bottom surfaces

					  20, 21, 22,
					  20, 22, 23,

				};

				
			private:
				
				GLuint						   VAOid;					// vertex array object id
				GLuint						   IndexBufferObject;		// surface index buffer object
				GLuint						   BufferObjects[4];		// buffer objects
				vml::shaders::GlShaderProgram* Shader;

				// debug directional light shader locations

				GLint LightAmbientLocation;
				GLint LightDiffuseLocation;
				GLint LightSpecularLocation;
				GLint LightDirectionLocation;
				GLint LightPowerLocation;
				GLint LightCameraSpaceLocation;

				// debug directiona light debug material 

				GLint MaterialAmbientLocation;
				GLint MaterialDiffuseLocation;
				GLint MaterialSpecularLocation;
				GLint MaterialShininessLocation;

				glm::mat4 M,NV,MV,MVP;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_phong_dir.shd";

			public:
			
				GLuint		 GetVAOId()			 const { return VAOid; }		// get mesh vao id
				unsigned int GetIndicesCount()	 const { return Indices; }	// get mesh indices array

				// ---------------------------------------------------------------
				//

				void Draw(vml::views::View* view,
						  const glm::vec3& bmin, const glm::vec3& bmax,
						  vml::debugrendering::DebugDirectionalLight &dirlight,
						  vml::debugrendering::DebugMaterial& material)
				{

					if (view)
					{
						float* mptr = glm::value_ptr(M);

						// scale parent matrix

						mptr[ 0] = (bmax.x - bmin.x);
						mptr[ 1] = 0;
						mptr[ 2] = 0;
						mptr[ 3] = 0;
						mptr[ 4] = 0;
						mptr[ 5] = (bmax.y - bmin.y);
						mptr[ 6] = 0;
						mptr[ 7] = 0;
						mptr[ 8] = 0;
						mptr[ 9] = 0;
						mptr[10] = (bmax.z - bmin.z);
						mptr[11] = 0;
						mptr[12] = (bmax.x + bmin.x) * 0.5f;
						mptr[13] = (bmax.y + bmin.y) * 0.5f;
						mptr[14] = (bmax.z + bmin.z) * 0.5f;
						mptr[15] = 1;

						// normal matrix

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						float* modelviewmatrix = glm::value_ptr(MV);
						float* normalviewmatrix = glm::value_ptr(NV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the
						// first 3x3 order for this matrix is meant to be divided
						// by each scaling factor along correspondig axes, so
						// we need to unskew normal matrix
						
						float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
											- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
											+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[5] * modelviewmatrix[8]);

						if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

						normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
						normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
						normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[ 6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[ 6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[ 5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

						// get shader

						GLuint Id = Shader->GetID();

						glUseProgram(Id);

						// set shader locations

						glUniformMatrix4fv(Shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(Shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(Shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(Shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(Shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(Shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4fv(MaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(MaterialShininessLocation, material.Shininess);

						dirlight.CameraSpaceDirection = glm::normalize(view->GetView() * dirlight.Direction);

						glUniform4fv(LightAmbientLocation, 1, &dirlight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &dirlight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &dirlight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &dirlight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &dirlight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, dirlight.Power);
					
						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						glBindVertexArray(VAOid);

						glDrawElements(
							GL_TRIANGLES,		// mode
							Indices ,			// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);

						glBindVertexArray(0);
						glUseProgram(0);
					
					}
					else
					{
						vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
					}

				}

				// ---------------------------------------------------------------
				// 

				void Draw(vml::views::View* view, 
						  const glm::mat4& m,
						  vml::debugrendering::DebugDirectionalLight& dirlight,
						  vml::debugrendering::DebugMaterial& material)
				{
					if (view)
					{

						M = m;

						const float* mptr = glm::value_ptr(M);

						// normal matrix
						
						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						float* modelviewmatrix = glm::value_ptr(MV);
						float* normalviewmatrix = glm::value_ptr(NV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the
						// first 3x3 order for this matrix is meant to be divided
						// by each scaling factor along correspondig axes, so
						// we need to unskew normal matrix

						float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
											- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
											+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[5] * modelviewmatrix[8]);

						if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

						normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
						normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
						normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

						// get shader

						GLuint Id = Shader->GetID();

						glUseProgram(Id);

						// set shader locations

						glUniformMatrix4fv(Shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(Shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(Shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(m));
						glUniformMatrix3fv(Shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(Shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(Shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4fv(MaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(MaterialShininessLocation, material.Shininess);

						dirlight.CameraSpaceDirection = glm::normalize(view->GetView() * dirlight.Direction);

						glUniform4fv(LightAmbientLocation, 1, &dirlight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &dirlight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &dirlight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &dirlight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &dirlight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, dirlight.Power);

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(VAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							Indices,			// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
					}
				}

				// ---------------------------------------------------------------
				//

				void Draw(vml::views::View* view,
						  const glm::vec3& a0, const glm::vec3& a1, const glm::vec3& a2, const glm::vec3& a3,
						  const glm::vec3& a4, const glm::vec3& a5, const glm::vec3& a6, const glm::vec3& a7,
						  vml::debugrendering::DebugDirectionalLight& dirlight,
						  vml::debugrendering::DebugMaterial& material)
				{

					if (view)
					{
						// vertices for aligned cube ,these vertices are constant

						VertexPositionArray[ 0] = a0.x;	VertexPositionArray[ 1] = a0.y;	VertexPositionArray[ 2] = a0.z; VertexPositionArray[ 3] = 1;
						VertexPositionArray[ 4] = a1.x;	VertexPositionArray[ 5] = a1.y;	VertexPositionArray[ 6] = a1.z; VertexPositionArray[ 7] = 1;
						VertexPositionArray[ 8] = a2.x;	VertexPositionArray[ 9] = a2.y;	VertexPositionArray[10] = a2.z; VertexPositionArray[11] = 1;
						VertexPositionArray[12] = a3.x;	VertexPositionArray[13] = a3.y;	VertexPositionArray[14] = a3.z; VertexPositionArray[15] = 1;
						VertexPositionArray[16] = a4.x;	VertexPositionArray[17] = a4.y;	VertexPositionArray[18] = a4.z; VertexPositionArray[19] = 1;
						VertexPositionArray[20] = a5.x;	VertexPositionArray[21] = a5.y;	VertexPositionArray[22] = a5.z; VertexPositionArray[23] = 1;
						VertexPositionArray[24] = a6.x;	VertexPositionArray[25] = a6.y;	VertexPositionArray[26] = a6.z; VertexPositionArray[27] = 1;
						VertexPositionArray[28] = a7.x;	VertexPositionArray[29] = a7.y;	VertexPositionArray[30] = a7.z; VertexPositionArray[31] = 1;

						float* mptr = glm::value_ptr(M);

						// scale parent matrix

						mptr[0] = 1;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = 1;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = 1;
						mptr[11] = 0;
						mptr[12] = 0;
						mptr[13] = 0;
						mptr[14] = 0;
						mptr[15] = 1;

						// normal matrix

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						float* modelviewmatrix = glm::value_ptr(MV);
						float* normalviewmatrix = glm::value_ptr(NV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the
						// first 3x3 order for this matrix is meant to be divided
						// by each scaling factor along correspondig axes, so
						// we need to unskew normal matrix

						float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
											- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
											+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[5] * modelviewmatrix[8]);

						if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

						normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
						normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
						normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

						// get shader

						glUseProgram(Shader->GetID());

						// set shader locations

						glUniformMatrix4fv(Shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(Shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(Shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(Shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(Shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(Shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4fv(MaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(MaterialShininessLocation, material.Shininess);

						dirlight.CameraSpaceDirection = glm::normalize(view->GetView() * dirlight.Direction);

						glUniform4fv(LightAmbientLocation, 1, &dirlight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &dirlight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &dirlight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &dirlight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &dirlight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, dirlight.Power);

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						glBindVertexArray(VAOid);

						glDrawElements(
							GL_TRIANGLES,		// mode
							Indices,			// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);

						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
					}

				}

				// ---------------------------------------------------------------

				const glm::mat4& GetMVP() const { return MVP; }
				const glm::mat4& GetMV() const { return MV; }
				const glm::mat4& GetNV() const { return NV; }
				const glm::mat4& GetM() const { return M; }

				// ---------------------------------------------------------------
				// constructors / destructors

				SolidCube() 
				{
					VAOid             = 0;
					IndexBufferObject = 0;
					BufferObjects[0]  = 0;
					BufferObjects[1]  = 0;
					BufferObjects[2]  = 0;
					BufferObjects[3]  = 0;
					Shader			  = nullptr;

					M   = glm::mat4(1.0f);
					NV  = glm::mat4(1.0f);
					MV  = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);

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
					GLuint AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices * 4 * sizeof(float), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glGenBuffers(1, &BufferObjects[1]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices * 3 * sizeof(float), &VertexNormalArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)Indices * sizeof(unsigned int), &SurfaceIndices[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					// attach debug phong dir shader
				
					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});
					
					// retrive shader color locations for debug shader

					GLuint Id = Shader->GetID();
					glUseProgram(Id);
					
					MaterialAmbientLocation = glGetUniformLocation(Id, "Material.ambient");
					MaterialDiffuseLocation = glGetUniformLocation(Id, "Material.diffuse");
					MaterialSpecularLocation = glGetUniformLocation(Id, "Material.specular");
					MaterialShininessLocation = glGetUniformLocation(Id, "Material.shininess");
					LightAmbientLocation = glGetUniformLocation(Id, "DirectionalLight.ambient");
					LightDiffuseLocation = glGetUniformLocation(Id, "DirectionalLight.diffuse");
					LightSpecularLocation = glGetUniformLocation(Id, "DirectionalLight.specular");
					LightDirectionLocation = glGetUniformLocation(Id, "DirectionalLight.direction");
					LightPowerLocation = glGetUniformLocation(Id, "DirectionalLight.power");
					LightCameraSpaceLocation = glGetUniformLocation(Id, "DirectionalLight.cameraspacedirection");

					// validate debug shaders locations

					if (MaterialAmbientLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'Material.ambient' uniform, check shader source code");
					if (MaterialDiffuseLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'MaterialDiffuseLocation' uniform, check shader source code");
					if (MaterialSpecularLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'MaterialSpecularLocation' uniform, check shader source code");
					if (MaterialShininessLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'MaterialShininessLocation' uniform, check shader source code");
					if (LightAmbientLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'DirectionalLight.ambient' uniform, check shader source code");
					if (LightDiffuseLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'DirectionalLight.diffuse' uniform, check shader source code");
					if (LightSpecularLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'DirectionalLight.specular' uniform, check shader source code");
					if (LightPowerLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'DirectionalLight.power' uniform, check shader source code");
					if (LightCameraSpaceLocation == -1) vml::os::Message::Error("GlProgram : debug_phong_dir requires 'DirectionalLight.cameraspacedirection' uniform, check shader source code");
				}

				// destructor

				~SolidCube()
				{
					// release opengl vao
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
					// ulnoad shader
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};
		
	} // end of debugrednering namespace

} // end of vml namespace

///////////////////////////////////////////////////////////////////////////////////
// draw a textured cube

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class TexturedCube
		{

		private:

			const unsigned int Vertices = 24;		// Vertices for this mesh
			const unsigned int Indices = 36;		// vbo indices for surfaces

			// vertices

			inline static float VertexPositionArray[] =
			{
				// front surface

				-0.5f,  0.5f, 0.5f, 1,
				 0.5f,  0.5f, 0.5f, 1,
				 0.5f, -0.5f, 0.5f, 1,
				-0.5f, -0.5f, 0.5f, 1,

				// left surfaces 

				-0.5f,  0.5f,  0.5f, 1,
				-0.5f,  0.5f, -0.5f, 1,
				-0.5f, -0.5f, -0.5f, 1,
				-0.5f, -0.5f,  0.5f, 1,

				// right surfaces

				 0.5f,  0.5f, -0.5f, 1,
				 0.5f,  0.5f,  0.5f, 1,
				 0.5f, -0.5f, -0.5f, 1,
				 0.5f, -0.5f,  0.5f, 1,

				 // back surfaces

				-0.5f,  0.5f, -0.5f, 1,
				 0.5f,  0.5f, -0.5f, 1,
				 0.5f, -0.5f, -0.5f, 1,
				-0.5f, -0.5f, -0.5f, 1,

				// top surfaces

				-0.5f, 0.5f,  0.5f, 1,
				-0.5f, 0.5f, -0.5f, 1,
				 0.5f, 0.5f, -0.5f, 1,
				 0.5f, 0.5f,  0.5f, 1,

				 // bottom surfaces

				-0.5f, -0.5f,  0.5f, 1,
				-0.5f, -0.5f, -0.5f, 1,
				 0.5f, -0.5f, -0.5f, 1,
				 0.5f, -0.5f,  0.5f, 1,

			};

			// uv coords

			inline static float VertexUVArray[] =
			{
				// front surface

				0, 0,
				1, 0,
				1, 1,
				0, 1,

				// left surfaces

				1, 0,
				0, 0,
				0, 1,
				1, 1,

				// right surfaces

				1, 0,
				0, 0,
				1, 1,
				0, 1,

				// back surfaces

				1, 1,
				0, 1,
				0, 0,
				1, 0,

				// top surfaces

				0, 1,
				0, 0,
				1, 0,
				1, 1,

				// bottom surfaces

				0, 0,
				0, 1,
				1, 1,
				1, 0,

			};

			// indices

			inline static GLuint SurfaceIndices[] =
			{
			  	   // front surface

				   0,  2,  1,
				   0,  3,  2,

				   // left surfaces

				   4,  5,  6,
				   4,  6,  7,

				   // right surfaces

				   8,  9, 10,
				   9, 11, 10,

				   // back surfaces

				  12, 13, 14,
				  12, 14, 15,

				  // top surfaces

				  16, 18, 17,
				  16, 19, 18,

				  // bottom surfaces

				  20, 21, 22,
				  20, 22, 23,

			};

			GLuint						   VAOid;								// vertex array object id
			glm::mat4					   M, MV, MVP;
			glm::mat3					   N;
			glm::mat2					   TM;
			GLuint						   BufferObjects[4];					// buffer objects
			GLuint						   IndexBufferObject;					// surface index buffer object
			GLuint						   ModelViewProjectionMatrixLocation;
			GLuint						   TextureMatrixLocation;
			GLuint						   TextureSamplerLocation;
			GLuint						   ShaderProgram;
			vml::shaders::GlShaderProgram* Shader;

			// shader filename

			const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_texture.shd";

		public:

			// ---------------------------------------------------------------
			// simple single color quad

			void Draw(vml::views::View* view, const glm::vec3 &bmin, const glm::vec3 &bmax, const GLuint textureid)
			{
				if (view)
				{
					float* mptr = glm::value_ptr(M);

					// scale parent matrix

					mptr[0] = (bmax.x - bmin.x);
					mptr[1] = 0;
					mptr[2] = 0;
					mptr[3] = 0;
					mptr[4] = 0;
					mptr[5] = (bmax.y - bmin.y);
					mptr[6] = 0;
					mptr[7] = 0;
					mptr[8] = 0;
					mptr[9] = 0;
					mptr[10] = (bmax.z - bmin.z);
					mptr[11] = 0;
					mptr[12] = (bmax.x + bmin.x) * 0.5f;
					mptr[13] = (bmax.y + bmin.y) * 0.5f;
					mptr[14] = (bmax.z + bmin.z) * 0.5f;
					mptr[15] = 1;

					MV = view->GetView() * M;
					MVP = view->GetViewProjection() * M;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
					glUniform1i(TextureSamplerLocation, 0);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureid);
					glBindVertexArray(VAOid);

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					glUseProgram(0);

				}
				else
				{
					vml::os::Message::Error("Quads ; ", "Null matrix cam for plane debug rendering");
				}
			}

			// ---------------------------------------------------------------
			// 

			void Draw(vml::views::View* view,
					  const glm::mat4& m,
					  const GLuint textureid)
			{
				if (view)
				{
					M = m;

					const float* mptr = glm::value_ptr(M);

					MV = view->GetView() * M;
					MVP = view->GetViewProjection() * M;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
					glUniform1i(TextureSamplerLocation, 0);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureid);
					glBindVertexArray(VAOid);

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					glUseProgram(0);
				}
				else
				{
					vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
				}
			}

			// ---------------------------------------------------------------
//

			void Draw(vml::views::View* view,
					  const glm::vec3& a0, const glm::vec3& a1, const glm::vec3& a2, const glm::vec3& a3,
					  const glm::vec3& a4, const glm::vec3& a5, const glm::vec3& a6, const glm::vec3& a7,
					  const GLuint textureid)
			{

				if (view)
				{
					// vertices for aligned cube ,these vertices are constant

					VertexPositionArray[0] = a0.x;	VertexPositionArray[1] = a0.y;	VertexPositionArray[2] = a0.z; VertexPositionArray[3] = 1;
					VertexPositionArray[4] = a1.x;	VertexPositionArray[5] = a1.y;	VertexPositionArray[6] = a1.z; VertexPositionArray[7] = 1;
					VertexPositionArray[8] = a2.x;	VertexPositionArray[9] = a2.y;	VertexPositionArray[10] = a2.z; VertexPositionArray[11] = 1;
					VertexPositionArray[12] = a3.x;	VertexPositionArray[13] = a3.y;	VertexPositionArray[14] = a3.z; VertexPositionArray[15] = 1;
					VertexPositionArray[16] = a4.x;	VertexPositionArray[17] = a4.y;	VertexPositionArray[18] = a4.z; VertexPositionArray[19] = 1;
					VertexPositionArray[20] = a5.x;	VertexPositionArray[21] = a5.y;	VertexPositionArray[22] = a5.z; VertexPositionArray[23] = 1;
					VertexPositionArray[24] = a6.x;	VertexPositionArray[25] = a6.y;	VertexPositionArray[26] = a6.z; VertexPositionArray[27] = 1;
					VertexPositionArray[28] = a7.x;	VertexPositionArray[29] = a7.y;	VertexPositionArray[30] = a7.z; VertexPositionArray[31] = 1;

					float* mptr = glm::value_ptr(M);

					// scale parent matrix

					mptr[0] = 1;
					mptr[1] = 0;
					mptr[2] = 0;
					mptr[3] = 0;
					mptr[4] = 0;
					mptr[5] = 1;
					mptr[6] = 0;
					mptr[7] = 0;
					mptr[8] = 0;
					mptr[9] = 0;
					mptr[10] = 1;
					mptr[11] = 0;
					mptr[12] = 0;
					mptr[13] = 0;
					mptr[14] = 0;
					mptr[15] = 1;

					MV = view->GetView() * M;
					MVP = view->GetViewProjection() * M;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
					glUniform1i(TextureSamplerLocation, 0);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureid);
					glBindVertexArray(VAOid);

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					glUseProgram(0);

				}
				else
				{
					vml::os::Message::Error("Cube : Null matrix cam for cube rendering");
				}
			}

			// ---------------------------------------------------------------

			const glm::mat4& GetMVP() const { return MVP; }
			const glm::mat4& GetMV() const { return MV; }
			const glm::mat4& GetM() const { return M; }
			const glm::mat4& GetN() const { return N; }

			// ---------------------------------------------------------------
			// constructors / destructors

			TexturedCube()
			{

				VAOid = 0;
				BufferObjects[0] = 0;
				BufferObjects[1] = 0;
				BufferObjects[2] = 0;
				BufferObjects[3] = 0;
				IndexBufferObject = 0;

				M = glm::mat4(1.0f);
				N = glm::mat3(1.0f);
				MV = glm::mat4(1.0f);
				MVP = glm::mat4(1.0f);
				TM = glm::mat2(1, 0, 0, 1);

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
				glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(AttributePosition);
				glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(AttributeTexCoord);
				glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

				// Create the Index Buffer Object 

				glGenBuffers(1, &IndexBufferObject);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &SurfaceIndices[0], GL_STATIC_DRAW);

				// unbinds buffers

				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glUseProgram(0);

				glDisableVertexAttribArray(AttributePosition);
				glDisableVertexAttribArray(AttributeTexCoord);

				// attach shader

				Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

				// shader locations

				ShaderProgram = Shader->GetID();
				glUseProgram(ShaderProgram);
				TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
				TextureMatrixLocation = Shader->GetTextureMatrixLocation();
				ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
				glUseProgram(0);

			}

			~TexturedCube()
			{
				if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
				if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
				if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
				if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
				vml::stores::ShaderStore->UnLoad(ShaderFileName);
			}

		};

	} // end of debugrednering namespace

} // end of vml namespace

///////////////////////////////////////////////////////////////////////////////////
// draw a textured cube
/*
namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class TexturedLitCube
		{

			private:

				const unsigned int Vertices = 24;		// Vertices for this mesh
				const unsigned int Indices = 36;		// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					// front surface

					-0.5f,  0.5f, 0.5f, 1,
					 0.5f,  0.5f, 0.5f, 1,
					 0.5f, -0.5f, 0.5f, 1,
					-0.5f, -0.5f, 0.5f, 1,

					// left surfaces 

					-0.5f,  0.5f,  0.5f, 1,
					-0.5f,  0.5f, -0.5f, 1,
					-0.5f, -0.5f, -0.5f, 1,
					-0.5f, -0.5f,  0.5f, 1,

					// right surfaces

					 0.5f,  0.5f, -0.5f, 1,
					 0.5f,  0.5f,  0.5f, 1,
					 0.5f, -0.5f, -0.5f, 1,
					 0.5f, -0.5f,  0.5f, 1,

					 // back surfaces

					 -0.5f,  0.5f, -0.5f, 1,
					  0.5f,  0.5f, -0.5f, 1,
					  0.5f, -0.5f, -0.5f, 1,
					 -0.5f, -0.5f, -0.5f, 1,

					 // top surfaces

					 -0.5f, 0.5f,  0.5f, 1,
					 -0.5f, 0.5f, -0.5f, 1,
					  0.5f, 0.5f, -0.5f, 1,
					  0.5f, 0.5f,  0.5f, 1,

					  // bottom surfaces

					 -0.5f, -0.5f,  0.5f, 1,
					 -0.5f, -0.5f, -0.5f, 1,
					  0.5f, -0.5f, -0.5f, 1,
					  0.5f, -0.5f,  0.5f, 1,
				};

				// normals

				inline static float VertexNormalArray[] =
				{
					// front surface

					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1,

					// left surfaces 

					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,

					// right surfaces

					1, 0, 0,
					1, 0, 0,
					1, 0, 0,
					1, 0, 0,

					// back surfaces

					0, 0, -1,
					0, 0, -1,
					0, 0, -1,
					0, 0, -1,

					// top surfaces

					0, 1, 0,
					0, 1, 0,
					0, 1, 0,
					0, 1, 0,

					// bottom surfaces

					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
				};

				// uvs

				inline static float VertexUVArray[] =
				{
					// front surface

					0, 0,
					1, 0,
					1, 1,
					0, 1,

					// left surfaces

					1, 0,
					0, 0,
					0, 1,
					1, 1,

					// right surfaces

					1, 0,
					0, 0,
					1, 1,
					0, 1,


					// back surfaces

					1, 1,
					0, 1,
					0, 0,
					1, 0,

					// top surfaces

					0, 1,
					0, 0,
					1, 0,
					1, 1,

					// bottom surfaces

					0, 0,
					0, 1,
					1, 1,
					1, 0,
				};

				// add surfaces 

				inline static GLuint SurfaceIndices[] =
				{
				   0,  2,  1,
				   0,  3,  2,

				   4,  5,  6,
				   4,  6,  7,

				   8,  9, 10,
				   9, 11, 10,

				  12, 13, 14,
				  12, 14, 15,

				  16, 18, 17,
				  16, 19, 18,

				  20, 21, 22,
				  20, 22, 23,

				};

				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, NV, MV, MVP, V;
				glm::mat3						N;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLuint						    ModelViewProjectionMatrixLocation;
				GLuint							TextureSamplerLocation;
				GLuint						    ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;

				// shader filename

				const std::string ShaderFilename = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\shaders\\debug_texture_dir.shd";

			public:

				// ---------------------------------------------------------------
				// simple textured cube

				void Draw(vml::views::View* view,
						  const glm::vec3& bmin, const glm::vec3& bmax,
						  const GLuint textureid)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

						// scale parent matrix

						mptr[0] = (bmax.x - bmin.x);
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = (bmax.y - bmin.y);
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = (bmax.z - bmin.z);
						mptr[11] = 0;
						mptr[12] = (bmax.x + bmin.x) * 0.5f;
						mptr[13] = (bmax.y + bmin.y) * 0.5f;
						mptr[14] = (bmax.z + bmin.z) * 0.5f;
						mptr[15] = 1;

						// normal matrix

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						float* modelviewmatrix = glm::value_ptr(MV);
						
						float* normalviewmatrix = glm::value_ptr(NV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the
						// first 3x3 order for this matrix is meant to be divided
						// by each scaling factor along correspondig axes, so
						// we need to unskew normal matrix

						float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
											- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
											+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[5] * modelviewmatrix[8]);

						if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

						normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
						normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
						normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(Shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(Shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(Shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						//						glUniformMatrix3fv(Shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(Shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(Shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads ; ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple textured cube

				void Draw(vml::views::View* view,
					const glm::mat4& m,
					const GLuint textureid)
				{
					if (view)
					{

						M = m;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						const float* mptr = glm::value_ptr(M);
						float* modelviewmatrix = glm::value_ptr(MV);

						float* normalviewmatrix = glm::value_ptr(NV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the
						// first 3x3 order for this matrix is meant to be divided
						// by each scaling factor along correspondig axes, so
						// we need to unskew normal matrix

						float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
											- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
											+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[5] * modelviewmatrix[8]);

						if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

						normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
						normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
						normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
						normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
						normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

						glUseProgram(ShaderProgram);

						glUniformMatrix4fv(Shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(Shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(Shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						//						glUniformMatrix3fv(Shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(Shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(Shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads ; ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				TexturedLitCube()
				{

					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					BufferObjects[2] = 0;
					BufferObjects[3] = 0;
					IndexBufferObject = 0;

					M = glm::mat4(1.0f);
					N = glm::mat3(1.0f);
					V = glm::mat4(1.0f);
					MV = glm::mat4(1.0f);
					NV = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);

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
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeTexCoord);
					glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &SurfaceIndices[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);

					glDisableVertexAttribArray(AttributePosition);
					glDisableVertexAttribArray(AttributeTexCoord);

					// attach shader

					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFilename);

					// shader locations

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);

				}

				~TexturedLitCube()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFilename);
				}

		};

	} // end of debugrednering namespace

} // end of vml namespace
*/
