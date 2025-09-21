#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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

///////////////////////////////////////////////////////////////////////////////////
// draw a textured quad

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class BillBoard
		{
			private:
				
				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices = 6;		// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					-1 ,  1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
					-1 , -1 ,  0 , 1 ,
				};

				// uv coords

				inline static float VertexUVArray[] =
				{
					 0 , 0 ,
					 1 , 0 ,
					 1 , 1 ,
					 0 , 1 ,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 3,
					  1 , 2 , 3,
				};

				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V;
				glm::mat2					   TM;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLint						   ModelViewProjectionMatrixLocation;
				GLint						   TextureMatrixLocation;
				GLint						   TextureSamplerLocation;
				GLint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;

				// shader filename

				const std::string ShaderFilename = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_texture.shd";

		public:
			
				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const GLuint textureid)
				{
					if (view)
					{
						
						const float* v = view->GetVptr();
						float* mv = glm::value_ptr(MV);

						// Row1

						mv[0] = scale;
						mv[1] = 0;
						mv[2] = 0;
						mv[3] = scale * v[3];

						// Row2

						mv[4] = 0;
						mv[5] = scale;
						mv[6] = 0;
						mv[7] = scale * v[7];

						// Row3

						mv[ 8] = 0;
						mv[ 9] = 0;
						mv[10] = scale;
						mv[11] = scale * v[11];

						// Row 4

						mv[12] = center.x * v[0] + center.y * v[4] + center.z * v[8] + v[12];
						mv[13] = center.x * v[1] + center.y * v[5] + center.z * v[9] + v[13];
						mv[14] = center.x * v[2] + center.y * v[6] + center.z * v[10] + v[14];
						mv[15] = center.x * v[3] + center.y * v[7] + center.z * v[11] + v[15];

						MVP = view->GetProjection() * MV;

						glUseProgram(ShaderProgram);
						//		glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, view->GetVptr());
						//		glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, view->GetPptr());
						//		glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(M));
						//		glUniformMatrix4fv(NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(N));
						//		glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);
						glBindVertexArray(VAOid);
						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);
						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Sphere : ","Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void DrawOrtho(vml::views::View* view, const glm::vec2 &scale, const glm::vec2& pos, const GLuint textureid)
				{
					if (view)
					{
						float width = view->GetViewPortWidth();
						float height = view->GetViewPortHeight();

						float *m = glm::value_ptr(M);
						
						glm::mat4 projMat = glm::ortho(0.f, width, 0.f, height, -1.f, 1.f);

						m[ 0] = scale.x;
						m[ 1] = 0;
						m[ 2] = 0;
						m[ 3] = 0;

						m[ 4] = 0;
						m[ 5] = -scale.y;
						m[ 6] = 0;
						m[ 7] = 0;

						m[ 8] = 0;
						m[ 9] = 0;
						m[10] = 1;
						m[11] = 0;

						m[12] =  pos.x;
						m[13] = -pos.y + height;
						m[14] = 0;
						m[15] = 1;

						MVP = projMat * M;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);
						glBindVertexArray(VAOid);
						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);
						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Billboard : ","Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void DrawOrtho(float width,float height, const glm::vec2& scale, const glm::vec2& pos, const GLuint textureid)
				{

					float* m = glm::value_ptr(M);

					glm::mat4 projMat = glm::ortho(0.f, width, 0.f, height, -1.f, 1.f);

					m[0] = scale.x;
					m[1] = 0;
					m[2] = 0;
					m[3] = 0;

					m[4] = 0;
					m[5] = -scale.y;
					m[6] = 0;
					m[7] = 0;

					m[8] = 0;
					m[9] = 0;
					m[10] = 1;
					m[11] = 0;

					m[12] = pos.x;
					m[13] = -pos.y + height;
					m[14] = 0;
					m[15] = 1;

					MVP = projMat * M;

					glUseProgram(ShaderProgram);
					//		glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, view->GetVptr());
					//		glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, view->GetPptr());
					//		glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(M));
					//		glUniformMatrix4fv(NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(N));
					//		glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
					glUniform1i(TextureSamplerLocation, 0);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureid);

					glBindVertexArray(VAOid);

					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
					glEnable(GL_CULL_FACE);

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					glUseProgram(0);

				}
				
				// ---------------------------------------------------------------
				// constructors / destructors

				BillBoard()
				{
					
					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					BufferObjects[2] = 0;
					BufferObjects[3] = 0;
					IndexBufferObject = 0;

					M = glm::mat4(1.0f);
					V = glm::mat4(1.0f);
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
					//	GLuint AttributeNormal = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					// GLuint AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);
					//	glGenBuffers(1, &BufferObjects[2]);

						// Generate and populate the buffers with vertex attributes and the indices

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeTexCoord);
					glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					//	glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
					//	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
					//	glEnableVertexAttribArray(AttributeTexCoord);
					//	glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &SurfaceIndices[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glUseProgram(0);

					glDisableVertexAttribArray(AttributePosition);
					//	glDisableVertexAttribArray(AttributeNormal);
					glDisableVertexAttribArray(AttributeTexCoord);
					// glDisableVertexAttribArray(AttributeColor);
					
					// attach shader

					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFilename, {});

					// shader locations

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					//	ViewMatrixLocation = Shader->GetViewMatrixLocation();
					//	ProjectionMatrixLocation = Shader->GetProjectionMatrixLocation();
					//	ModelMatrixLocation = Shader->GetModelMatrixLocation();
					//	NormalMatrixLocation = Shader->GetNormalMatrixLocation();
					//	ModelViewMatrixLocation = Shader->GetModelViewMatrixLocation();
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					TextureMatrixLocation = Shader->GetTextureMatrixLocation();
					glUseProgram(0);
					
				}

				~BillBoard()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					//	if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					//	if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFilename);
				}

		};

	} // end of debugrednering namespace

} // end of vml namespace

