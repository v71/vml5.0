#pragma once

///////////////////////////////////////////////////////////////////////////////////
// draw a single color quad

namespace vml
{
	namespace debugrendering
	{

			/////////////////////////////////////////////////////////////////
			// mesh class
			
			class Quad
			{
				
				private:

					const unsigned int Vertices = 4;	// Vertices for this mesh
					const unsigned int Indices  = 6;	// vbo indices for surfaces

					// vertices

					inline static float VertexPositionArray[] =
					{
						-1 ,  1 ,  0 , 1 ,
						 1 ,  1 ,  0 , 1 ,
						 1 , -1 ,  0 , 1 ,
						-1 , -1 ,  0 , 1 ,
					};
					
					// indices

					inline static GLuint SurfaceIndices[] =
					{
						  0 , 1 , 3,
						  1 , 2 , 3,
					};

					GLuint							VAOid;								// vertex array object id
					glm::mat4						M, MV, MVP, V;
					glm::mat3						N;
					GLuint							BufferObjects[4];					// buffer objects
					GLuint							IndexBufferObject;					// surface index buffer object
					GLuint						    ModelViewProjectionMatrixLocation;
					GLuint							ShaderProgram;
					GLuint							SingleColorLocation;
					vml::shaders::GlShaderProgram  *Shader;

					// shader filename

					const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_single_color.shd";

				public:

					// ---------------------------------------------------------------
					// simple single color quad

					void Draw(vml::views::View* view,float scale, const glm::vec3& center, const glm::vec4& col,int mode=1)
					{
						if (view)
						{

							float* mptr = glm::value_ptr(M);

							mptr[ 0] = scale;
							mptr[ 1] = 0;
							mptr[ 2] = 0;
							mptr[ 3] = 0;
							mptr[ 4] = 0;
							mptr[ 5] = scale;
							mptr[ 6] = 0;
							mptr[ 7] = 0;
							mptr[ 8] = 0;
							mptr[ 9] = 0;
							mptr[10] = scale;
							mptr[11] = 0;
							mptr[12] = center.x;
							mptr[13] = center.y;
							mptr[14] = center.z;
							mptr[15] = 1;

							MV = view->GetView() * M;
							MVP = view->GetViewProjection() * M;

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(SingleColorLocation, col[0], col[1], col[2], col[3]);

							glBindVertexArray(VAOid);
							glDisable(GL_CULL_FACE);
				
							if ( mode)
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							else if( !mode)
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					
							glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
						}
					}

					// ---------------------------------------------------------------
					// simple single color quad

					void Draw(vml::views::View* view,const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec4& col,int mode=1)
					{
						if (view)
						{
							float* mptr = glm::value_ptr(M);

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

							VertexPositionArray[ 0] = p0.x;
							VertexPositionArray[ 1] = p0.y;
							VertexPositionArray[ 2] = p0.z;
							VertexPositionArray[ 3] = 1;

							VertexPositionArray[ 4] = p1.x;
							VertexPositionArray[ 5] = p1.y;
							VertexPositionArray[ 6] = p1.z;
							VertexPositionArray[ 7] = 1;

							VertexPositionArray[ 8] = p2.x;
							VertexPositionArray[ 9] = p2.y;
							VertexPositionArray[10] = p2.z;
							VertexPositionArray[11] = 1;

							VertexPositionArray[12] = p3.x;
							VertexPositionArray[13] = p3.y;
							VertexPositionArray[14] = p3.z;
							VertexPositionArray[15] = 1;

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(SingleColorLocation, col[0], col[1], col[2], col[3]);

							glBindVertexArray(VAOid);
							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
							glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

							glDisable(GL_CULL_FACE);
						
							if ( mode)
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							else if ( !mode)
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
						}
					}

					// ---------------------------------------------------------------
					// constructors / destructors

					Quad()
					{
						VAOid             = 0;
						BufferObjects[0]  = 0;
						BufferObjects[1]  = 0;
						BufferObjects[2]  = 0;
						BufferObjects[3]  = 0;
						IndexBufferObject = 0;

						M   = glm::mat4(1.0f);
						N   = glm::mat3(1.0f);
						V   = glm::mat4(1.0f);
						MV  = glm::mat4(1.0f);
						MVP = glm::mat4(1.0f);

						// Create the VAO

						glGenVertexArrays(1, &VAOid);
						glBindVertexArray(VAOid);

						// Create the Vertex Buffer Object 

						// Map the generic vertex attributes used by the mesh's shader program to
						// the mesh's vertex data stored in the vertex buffer object.

						GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

						glGenBuffers(1, &BufferObjects[0]);

						// Generate and populate the buffers with vertex attributes and the indices

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

						ShaderProgram = Shader->GetID();
						glUseProgram(ShaderProgram);
						ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						SingleColorLocation = glGetUniformLocation(ShaderProgram, "Color");
						glUseProgram(0);
					}

					~Quad()
					{
						if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
						if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
						if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
						vml::stores::ShaderStore->UnLoad(ShaderFileName);
					}

			};

		} // end of debugrednering namespace

} // end of vml namespace

///////////////////////////////////////////////////////////////////////////////////
// draw a per vertex color quad

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class QuadPerVertexColor
		{
			
			private:

				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices = 6;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					-1 ,  1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
					-1 , -1 ,  0 , 1 ,
				};

				inline static float VertexColorArray[] =
				{
					 1 ,  0 ,  0 , 1,
					 0 ,  1 ,  0 , 1,
					 0 ,  0 ,  1 , 1,
					 1 ,  1 ,  0 , 1,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 3,
					  1 , 2 , 3,
				};

				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V;
				glm::mat3					   N;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_color.shd";

			public:
				
				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3)
				{
					
					if (view)
					{

						float* mptr = glm::value_ptr(M);

						mptr[0] = scale;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = scale;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = scale;
						mptr[11] = 0;
						mptr[12] = center.x;
						mptr[13] = center.y;
						mptr[14] = center.z;
						mptr[15] = 1;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						VertexColorArray[0] = c0.r;
						VertexColorArray[1] = c0.g;
						VertexColorArray[2] = c0.b;
						VertexColorArray[3] = c0.a;

						VertexColorArray[4] = c1.r;
						VertexColorArray[5] = c1.g;
						VertexColorArray[6] = c1.b;
						VertexColorArray[7] = c1.a;

						VertexColorArray[ 8] = c2.r;
						VertexColorArray[ 9] = c2.g;
						VertexColorArray[10] = c2.b;
						VertexColorArray[11] = c2.a;

						VertexColorArray[12] = c3.r;
						VertexColorArray[13] = c3.g;
						VertexColorArray[14] = c3.b;
						VertexColorArray[15] = c3.a;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
					
				}
				
				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view,
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
						  const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3)
				{
					
					if (view)
					{
						float* mptr = glm::value_ptr(M);

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

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						VertexColorArray[0] = c0.r;
						VertexColorArray[1] = c0.g;
						VertexColorArray[2] = c0.b;
						VertexColorArray[3] = c0.a;

						VertexColorArray[4] = c1.r;
						VertexColorArray[5] = c1.g;
						VertexColorArray[6] = c1.b;
						VertexColorArray[7] = c1.a;

						VertexColorArray[8] = c2.r;
						VertexColorArray[9] = c2.g;
						VertexColorArray[10] = c2.b;
						VertexColorArray[11] = c2.a;

						VertexColorArray[12] = c3.r;
						VertexColorArray[13] = c3.g;
						VertexColorArray[14] = c3.b;
						VertexColorArray[15] = c3.a;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
					
				}
				
				// ---------------------------------------------------------------
				// constructors / destructors

				QuadPerVertexColor()
				{
					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					BufferObjects[2] = 0;
					BufferObjects[3] = 0;
					IndexBufferObject = 0;

					M   = glm::mat4(1.0f);
					N   = glm::mat3(1.0f);
					V   = glm::mat4(1.0f);
					MV  = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);

					// Create the VAO

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);

					// Generate and populate the buffers with vertex attributes and the indices

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeColor);
					glVertexAttribPointer(AttributeColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

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
					glDisableVertexAttribArray(AttributeColor);

					// attach shader

					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

					// shader locations

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
					
				}

				~QuadPerVertexColor()
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
// draw a textured quad

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class TexturedQuad
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

				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, MV, MVP, V;
				glm::mat3						N;
				glm::mat2						TM;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLuint						    ModelViewProjectionMatrixLocation;
				GLuint						    TextureMatrixLocation;
				GLuint							TextureSamplerLocation;
				GLuint						    ShaderProgram;
				vml::shaders::GlShaderProgram  *Shader;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_texture.shd";

			public:
				
				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const GLuint textureid)
				{
					if (view)
					{
						float* mptr = glm::value_ptr(M);

						mptr[0] = scale;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = scale;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = scale;
						mptr[11] = 0;
						mptr[12] = center.x;
						mptr[13] = center.y;
						mptr[14] = center.z;
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

						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads ; Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const GLuint textureid)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

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

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, 
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, 
					 	  const glm::vec3& s0, const glm::vec3& s1, const glm::vec3& s2, const glm::vec3& s3,
						  const GLuint textureid)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

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

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[ 8] = p2.x;
						VertexPositionArray[ 9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						VertexUVArray[0] = s0.x;
						VertexUVArray[1] = s0.y;
						VertexUVArray[2] = s1.x;
						VertexUVArray[3] = s1.y;
						VertexUVArray[4] = s2.x;
						VertexUVArray[5] = s2.y;
						VertexUVArray[6] = s3.x;
						VertexUVArray[7] = s3.y;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				TexturedQuad()
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

				~TexturedQuad()
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
// draw a textured quad

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// alpha textured quad

		class AlphaTexturedQuad
		{
			
			private:

				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices = 6;	// vbo indices for surfaces

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
					  3 , 1 , 0,
					  3 , 2 , 1,
				};
				
				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, MV, MVP, V;
				glm::mat3						N;
				glm::mat2						TM;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLuint						    ModelViewProjectionMatrixLocation;
				GLuint							TextureSamplerLocation;
				GLuint						    TextureMatrixLocation;
				GLuint						    ShaderProgram;
				GLuint							AlphaLocation;
				vml::shaders::GlShaderProgram*  Shader;
				float							Alpha;
				unsigned int					BlendMode;
				
				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_texture_alpha.shd";

			public:
				
				static const unsigned int ADDICTIVE_BLENDING		  = 0;  // for additive blending
				static const unsigned int TRANSPARENCY				  = 1;  // for real transparency
				static const unsigned int ADDICTIVE_BLENDING_NO_ALPHA = 2;  // for additive blending without alpha affecting the result

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const GLuint textureid,const float alpha,const int blendmode)
				{
					if (view)
					{
						float* mptr = glm::value_ptr(M);

						mptr[0] = scale;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = scale;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = scale;
						mptr[11] = 0;
						mptr[12] = center.x;
						mptr[13] = center.y;
						mptr[14] = center.z;
						mptr[15] = 1;

						Alpha = alpha;
						BlendMode = blendmode;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));

						glUniform1f(AlphaLocation, Alpha);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glDisable(GL_CULL_FACE);
						glEnable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
							
						if (BlendMode == ADDICTIVE_BLENDING)			glBlendFunc(GL_SRC_ALPHA, GL_ONE);				   //  for additive blending
						if (BlendMode == TRANSPARENCY)					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //  for real transparency
						if (BlendMode == ADDICTIVE_BLENDING_NO_ALPHA)	glBlendFunc(GL_ONE, GL_ONE);					   // for additive blending without alpha affecting the result

						glDepthMask(GL_FALSE);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glDepthMask(GL_TRUE);

						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const GLuint textureid, const float alpha, const int blendmode)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

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

						Alpha = alpha;
						BlendMode = blendmode;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);
						glUniformMatrix2fv(TextureMatrixLocation, 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1f(AlphaLocation, Alpha);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glEnable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);

						if (BlendMode == ADDICTIVE_BLENDING)			glBlendFunc(GL_SRC_ALPHA, GL_ONE);				   //  for additive blending
						if (BlendMode == TRANSPARENCY)					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //  for real transparency
						if (BlendMode == ADDICTIVE_BLENDING_NO_ALPHA)	glBlendFunc(GL_ONE, GL_ONE);					   // for additive blending without alpha affecting the result

						glDepthMask(GL_FALSE);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glDepthMask(GL_TRUE);

						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
	
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}
				
				// ---------------------------------------------------------------
				// constructors / destructors

				AlphaTexturedQuad()
				{
					
					VAOid = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					BufferObjects[2] = 0;
					BufferObjects[3] = 0;
					IndexBufferObject = 0;

					M   = glm::mat4(1.0f);
					N   = glm::mat3(1.0f);
					V   = glm::mat4(1.0f);
					MV  = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);
					TM = glm::mat2(1, 0, 0, 1);

					Alpha = 1.0f;

					BlendMode=ADDICTIVE_BLENDING;

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
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					TextureMatrixLocation = Shader->GetTextureMatrixLocation();
					AlphaLocation = glGetUniformLocation(ShaderProgram, "Alpha");
					glUseProgram(0);
				}

				~AlphaTexturedQuad()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};

		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE
		// RICORDARSI DI AGGIUNGERE LE ISTRUZIONI PER LA MATICE DELLE TESSITURE

		/////////////////////////////////////////////////////////////////
		// alpha colored textured quad

		class AlphaColorTexturedQuad
		{
			
			private:

				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices = 6;	// vbo indices for surfaces

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

				// vertices

				inline static float VertexColorArray[] =
				{
					 1 ,  1 ,  1 , 1 ,
					 1 ,  1 ,  1 , 1 ,
					 1 ,  1 ,  1 , 1 ,
					 1 ,  1 ,  1 , 1 ,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  3 , 1 , 0,
					  3 , 2 , 1,
				};

				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V;
				glm::mat3					   N;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   TextureSamplerLocation;
				GLuint						   ShaderProgram;
				GLuint						   AlphaLocation;
				vml::shaders::GlShaderProgram* Shader;
				float						   Alpha;
				unsigned int				   BlendMode;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_texture_alpha_color.shd";
				
			public:
				
				static const unsigned int ADDICTIVE_BLENDING		  = 0;  // for additive blending
				static const unsigned int TRANSPARENCY				  = 1;  // for real transparency
				static const unsigned int ADDICTIVE_BLENDING_NO_ALPHA = 2;  // for additive blending without alpha affecting the result

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const glm::vec3 &col,const GLuint textureid, const float alpha, const int blendmode)
				{
					if (view)
					{
						float* mptr = glm::value_ptr(M);

						mptr[0] = scale;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = scale;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = scale;
						mptr[11] = 0;
						mptr[12] = center.x;
						mptr[13] = center.y;
						mptr[14] = center.z;
						mptr[15] = 1;

						Alpha = alpha;
						BlendMode = blendmode;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);

						glUniform1f(AlphaLocation, Alpha);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						//

						VertexColorArray[0] = col.r;
						VertexColorArray[1] = col.g;
						VertexColorArray[2] = col.b;
						VertexColorArray[3] = 1;

						VertexColorArray[4] = col.r;
						VertexColorArray[5] = col.g;
						VertexColorArray[6] = col.b;
						VertexColorArray[7] = 1;

						VertexColorArray[ 8] = col.r;
						VertexColorArray[ 9] = col.g;
						VertexColorArray[10] = col.b;
						VertexColorArray[11] = 1;

						VertexColorArray[12] = col.r;
						VertexColorArray[13] = col.g;
						VertexColorArray[14] = col.b;
						VertexColorArray[15] = 1;

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);

						//

						glDisable(GL_CULL_FACE);
						glEnable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);

						if (BlendMode == ADDICTIVE_BLENDING)			glBlendFunc(GL_SRC_ALPHA, GL_ONE);				   // for additive blending
						if (BlendMode == TRANSPARENCY)					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // for real transparency
						if (BlendMode == ADDICTIVE_BLENDING_NO_ALPHA)	glBlendFunc(GL_ONE, GL_ONE);					   // for additive blending without alpha affecting the result

						glDepthMask(GL_FALSE);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glDepthMask(GL_TRUE);

						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads ; Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, 
										 		  const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3,
												  const GLuint textureid, const float alpha, const int blendmode)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

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

						Alpha = alpha;
						BlendMode = blendmode;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						VertexColorArray[0] = c0.r;
						VertexColorArray[1] = c0.g;
						VertexColorArray[2] = c0.b;
						VertexColorArray[3] = 1;

						VertexColorArray[4] = c1.r;
						VertexColorArray[5] = c1.g;
						VertexColorArray[6] = c1.b;
						VertexColorArray[7] = 1;

						VertexColorArray[8] = c2.r;
						VertexColorArray[9] = c2.g;
						VertexColorArray[10] = c2.b;
						VertexColorArray[11] = 1;

						VertexColorArray[12] = c3.r;
						VertexColorArray[13] = c3.g;
						VertexColorArray[14] = c3.b;
						VertexColorArray[15] = 1;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(TextureSamplerLocation, 0);
						glUniform1f(AlphaLocation, Alpha);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, textureid);

						glBindVertexArray(VAOid);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);
						glEnable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);

						if (BlendMode == ADDICTIVE_BLENDING)			glBlendFunc(GL_SRC_ALPHA, GL_ONE);				   //  for additive blending
						if (BlendMode == TRANSPARENCY)					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //  for real transparency
						if (BlendMode == ADDICTIVE_BLENDING_NO_ALPHA)	glBlendFunc(GL_ONE, GL_ONE);					   // for additive blending without alpha affecting the result

						glDepthMask(GL_FALSE);
						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glDepthMask(GL_TRUE);

						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Quads : ", "Null matrix cam for plane debug rendering");
					}
				}
				
				// ---------------------------------------------------------------
				// constructors / destructors

				AlphaColorTexturedQuad()
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
					MVP = glm::mat4(1.0f);

					Alpha = 1.0f;

					BlendMode = ADDICTIVE_BLENDING;

					// Create the VAO

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition   = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeTexCoord   = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					GLuint AttributeColorCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);
					glGenBuffers(1, &BufferObjects[2]);

					// Generate and populate the buffers with vertex attributes and the indices

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeTexCoord);
					glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColorArray), &VertexColorArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeColorCoord);
					glVertexAttribPointer(AttributeColorCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);

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
					glDisableVertexAttribArray(AttributeColorCoord);

					// attach shader

					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

					// shader locations

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					AlphaLocation = glGetUniformLocation(ShaderProgram, "Alpha");
					glUseProgram(0);
					
				}

				~AlphaColorTexturedQuad()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2]) { glDeleteBuffers(2, &BufferObjects[2]); BufferObjects[2] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};
		
	} // end of debugrednering namespace

} // end of vml namespace


///////////////////////////////////////////////////////////////////////////////////
// draw a single color quad

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class MyQuad
		{

			private:

				const unsigned int Vertices = 6;	// Vertices for this mesh
				const unsigned int Indices = 6;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					 1 ,  1 ,  0 , 1 ,
					-1 , -1 ,  0 , 1 ,
					-1 ,  1 ,  0 , 1 ,

					-1 , -1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 2,
					  3 , 4 , 5,
				};

				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V,P;
				glm::mat3					   N;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLint						   ModelViewProjectionMatrixLocation;
				GLint						   ViewMatrixLocation;
				GLint						   ProjectionMatrixLocation;
				GLint						   ShaderProgram;
				GLint						   SingleColorLocation;
				vml::shaders::GlShaderProgram* Shader;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_myquad.shd";

			public:

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const glm::vec4& col, int mode = 1)
				{
					if (view)
					{

						float* mptr = glm::value_ptr(M);

						mptr[0] = scale;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = scale;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = scale;
						mptr[11] = 0;
						mptr[12] = center.x;
						mptr[13] = center.y;
						mptr[14] = center.z;
						mptr[15] = 1;

						MV = view->GetView() * M;
						MVP = view->GetViewProjection() * M;
						V = view->GetView();
						P = view->GetProjection();

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(V));
						glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(P));

						glUniform4f(SingleColorLocation, col[0], col[1], col[2], col[3]);

						glBindVertexArray(VAOid);
						glDisable(GL_CULL_FACE);

						if (mode)
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						else if (!mode)
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec4& col, int mode = 1)
				{
					if (view)
					{
						float* mptr = glm::value_ptr(M);

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

						VertexPositionArray[0] = p0.x;
						VertexPositionArray[1] = p0.y;
						VertexPositionArray[2] = p0.z;
						VertexPositionArray[3] = 1;

						VertexPositionArray[4] = p1.x;
						VertexPositionArray[5] = p1.y;
						VertexPositionArray[6] = p1.z;
						VertexPositionArray[7] = 1;

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexPositionArray[12] = p3.x;
						VertexPositionArray[13] = p3.y;
						VertexPositionArray[14] = p3.z;
						VertexPositionArray[15] = 1;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform4f(SingleColorLocation, col[0], col[1], col[2], col[3]);

						glBindVertexArray(VAOid);
						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

						glDisable(GL_CULL_FACE);

						if (mode)
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						else if (!mode)
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

						glEnable(GL_CULL_FACE);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("Quads : Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				MyQuad()
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
					P = glm::mat4(1.0f);
					MV = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);

					// Create the VAO

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

					glGenBuffers(1, &BufferObjects[0]);

					// Generate and populate the buffers with vertex attributes and the indices

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

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					ViewMatrixLocation=Shader->GetViewMatrixLocation();
					ProjectionMatrixLocation=Shader->GetProjectionMatrixLocation();
					SingleColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					glUseProgram(0);
				}

				~MyQuad()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};

	} // end of debugrednering namespace

} // end of vml namespace
