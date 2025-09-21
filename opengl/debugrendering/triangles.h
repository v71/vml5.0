#pragma once

///////////////////////////////////////////////////////////////////////////////////
// draw a triangle 

namespace vml
{
	namespace debugrendering
	{

			/////////////////////////////////////////////////////////////////
			// mesh class
			
			class Triangle
			{
				
				private:

					const unsigned int Vertices = 3;	// Vertices for this mesh
					const unsigned int Indices  = 3;	// vbo indices for surfaces

					// vertices

					inline static float VertexPositionArray[] =
					{
						 0 ,  1 ,  0 , 1 ,
						 1 , -1 ,  0 , 1 ,
						-1 , -1 ,  0 , 1 ,
					};
					
					// indices

					inline static GLuint SurfaceIndices[] =
					{
						  0 , 1 , 2,
					};

					GLuint							VAOid;								// vertex array object id
					glm::mat4						M, MV, MVP, V;
					glm::mat3						N;
					GLuint							BufferObjects[4];					// buffer objects
					GLuint							IndexBufferObject;					// surface index buffer object
					GLint						    ModelViewProjectionMatrixLocation;
					GLint							ShaderProgram;
					GLuint							SingleColorLocation;
					vml::shaders::GlShaderProgram  *Shader;

					// shader filename

					const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_single_color.shd";

				public:

					// ---------------------------------------------------------------
					// simple single color Triangle

					void Draw(vml::views::View* view,float scale, const glm::vec3& center, const glm::vec4& col, bool wire=false)
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
							if ( wire )
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							else if (!wire )
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
							vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
						}
					}

					// ---------------------------------------------------------------
					// simple single color Triangle

					void Draw(vml::views::View* view,const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& col, bool wire = false)
					{
						if (view)
						{

							float* mptr = glm::value_ptr(M);

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

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
							glUniform4f(SingleColorLocation, col[0], col[1], col[2], col[3]);

							glBindVertexArray(VAOid);

							glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
							glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);

							glDisable(GL_CULL_FACE);
							if (wire)
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							else if (!wire)
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
							vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
						}
					}

					// ---------------------------------------------------------------
					// constructors / destructors

					Triangle()
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

					~Triangle()
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

		class TrianglePerVertexColor
		{
			
			private:

				const unsigned int Vertices = 3;	// Vertices for this mesh
				const unsigned int Indices  = 3;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					 0 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
					-1 , -1 ,  0 , 1 ,
				};

				inline static float VertexColorArray[] =
				{
					 1 ,  0 ,  0 , 1,
					 0 ,  1 ,  0 , 1,
					 0 ,  0 ,  1 , 1,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 2,
				};

			public:

				GLuint						   VAOid;								// vertex array object id
				glm::mat4					   M, MV, MVP, V;
				glm::mat3					   N;
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLint						   ModelViewProjectionMatrixLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;
				
				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_color.shd";

			public:
				
				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, float scale, const glm::vec3& center, const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2)
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

						VertexColorArray[8] = c2.r;
						VertexColorArray[9] = c2.g;
						VertexColorArray[10] = c2.b;
						VertexColorArray[11] = c2.a;

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
						vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view,
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
						  const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2)
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
						vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
					}
				}
				
				// ---------------------------------------------------------------
				// constructors / destructors

				TrianglePerVertexColor()
				{
					VAOid			  = 0;
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

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
				}

				~TrianglePerVertexColor()
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

		class TexturedTriangle
		{

			private:

				const unsigned int Vertices = 3;	// Vertices for this mesh
				const unsigned int Indices  = 3;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					-1 ,  1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
				};

				// uv coords

				inline static float VertexUVArray[] =
				{
					 0 , 0 ,
					 1 , 0 ,
					 1 , 1 ,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 2,
				};

				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, MV, MVP, V;
				glm::mat3						N;
				glm::mat2						TM;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLint						    ModelViewProjectionMatrixLocation;
				GLint						    TextureMatrixLocation;
				GLint							TextureSamplerLocation;
				GLuint						    ShaderProgram;
				vml::shaders::GlShaderProgram*  Shader;

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
						vml::os::Message::Error("Quads ; ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const GLuint textureid)
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
						vml::os::Message::Error("Quads : ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view,
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
						  const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
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

						VertexPositionArray[8] = p2.x;
						VertexPositionArray[9] = p2.y;
						VertexPositionArray[10] = p2.z;
						VertexPositionArray[11] = 1;

						VertexUVArray[0] = uv0.x;
						VertexUVArray[1] = uv0.y;

						VertexUVArray[2] = uv1.x;
						VertexUVArray[3] = uv1.y;

						VertexUVArray[4] = uv2.x;
						VertexUVArray[5] = uv2.y;

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
						vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				TexturedTriangle()
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

				~TexturedTriangle()
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

/*
namespace vml
{
	namespace debugrendering
	{
		/////////////////////////////////////////////////////////////////
		// mesh class

		class TexturedTriangle
		{
			private:

				const unsigned int Vertices = 3;	// Vertices for this mesh
				const unsigned int Indices  = 3;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					-1 ,  1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
				};

				// uv coords

				inline static float VertexUVArray[] =
				{
					 0 , 0 ,
					 1 , 0 ,
					 1 , 1 ,
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 2,
				};

				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, MV, MVP, V;
				glm::mat3						N;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLuint						    ModelViewProjectionMatrixLocation;
				GLuint							TextureSamplerLocation;
				GLuint						    ShaderProgram;
				vml::shaders::GlShaderProgram*  Shader;

				// shader filename

				const std::string ShaderFilename = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\shaders\\debug_texture.shd";

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
						vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// simple single color quad

				void Draw(vml::views::View* view, 
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, 
						  const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
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

						VertexUVArray[0] = uv0.x;
						VertexUVArray[1] = uv0.y;

						VertexUVArray[2] = uv1.x;
						VertexUVArray[3] = uv1.y;

						VertexUVArray[4] = uv2.x;
						VertexUVArray[5] = uv2.y;

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
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
						vml::os::Message::Error("Triangles : ", "Null matrix cam for plane debug rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				TexturedTriangle()
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

					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);

				}

				~TexturedTriangle()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFilename);
				}

			};
		
	} // end of debugrednering namespace

} // end of vml namespace
*/
