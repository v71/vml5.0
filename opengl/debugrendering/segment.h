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

///////////////////////////////////////////////////////////////////////////////////
// draw a segmetn given two points

namespace vml
{
	namespace debugrendering
	{

			/////////////////////////////////////////////////////////////////
			// mesh class
			
			class Segment
			{
				
				private:

					const unsigned int Vertices = 2; // Vertices for this mesh
					const unsigned int Indices = 2; // vbo indices for surfaces

					// vertices

					inline static float VertexPositionArray[] =
					{
						 0.0f,  0.0f,  0.0f, 1,
						 0.0f,  0.0f,  0.0f, 1
					};

					// indices

					inline static GLuint SurfaceIndices[] =
					{
						0,1
					};

					// vaoid

					GLuint						   VAOid;								// vertex array object id
					GLuint						   BufferObjects[4];					// buffer objects
					GLuint						   IndexBufferObject;					// surface index buffer object
					GLint						   ColorLocation;
					GLint						   ModelViewProjectionMatrixLocation;
					GLuint						   ShaderProgram;
					vml::shaders::GlShaderProgram* Shader;

					// shader filename

					const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_single_color.shd";

				public:

					// ---------------------------------------------------------------
					// axis aligned bounding box drawing

					void Draw(vml::views::View* view,const glm::vec3& p, const glm::vec3& q, const glm::vec4& col) const
					{
						if (view)
						{

							glm::mat4 m;

							float* mptr = glm::value_ptr(m);

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

							float VertexPositionArray2[] =
							{
								 p.x,p.y,p.z, 1,
								 q.x,q.y,q.z, 1,
							};

							glBindVertexArray(VAOid);

							// Buffer orphaning, a common way to improve streaming perf. 

							GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
							glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(VertexPositionArray2), &VertexPositionArray2[0]);

							// 2nd attribute buffer : positions of particles' centers

							glEnableVertexAttribArray(AttributePosition);

							// forse si puo' togliere

							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);

							glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

							glm::mat4 mvp = view->GetViewProjection() * m;

							// Use debug shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(VAOid);
							glDrawArrays(GL_LINES, 0, Vertices);
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							
						}
						else
						{
							vml::os::Message::Error("Segment : ","Null matrix cam for cube rendering");
						}
					}

					// ---------------------------------------------------------------
					// constructors / destructors

					Segment()
					{
				
						VAOid = 0;
						IndexBufferObject = 0;
						BufferObjects[0] = 0;
						BufferObjects[1] = 0;
						BufferObjects[2] = 0;
						BufferObjects[3] = 0;

						// Create the VAO for axis aligned bounding box

						glGenVertexArrays(1, &VAOid);
						glBindVertexArray(VAOid);

						// Create the Vertex Buffer Object 

						glGenBuffers(1, &BufferObjects[0]);

						GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributePosition);
						glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

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

						// attach shader

						Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

						// shader locations

						ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						ShaderProgram = Shader->GetID();
						glUseProgram(ShaderProgram);
						ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
						glUseProgram(0);
					}

					~Segment()
					{
						if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
						if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
						if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
						vml::stores::ShaderStore->UnLoad(ShaderFileName);
					}

			};
			
		} // end of debugrednering namespace

} // end of vml namespace
