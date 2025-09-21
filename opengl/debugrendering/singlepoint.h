#pragma once

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// debug rednering mesh class

		class SinglePoint
		{
			
			private:

				const unsigned int Vertices = 1; // Vertices for this mesh
				const unsigned int Indices = 1; // vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					 0.0f,  0.0f,  0.0f, 1
				};

				// indices

				inline static GLuint SurfaceIndices[] =
				{
					0
				};

				// vaoid

				GLuint						   VAOid;								// vertex array object id
				GLuint						   BufferObjects[4];					// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLint						   ColorLocation;
				GLint						   ModelViewProjectionMatrixLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;
				glm::mat4					   MVP;

				// shader filename

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_single_color.shd";

			public:
				
				// ---------------------------------------------------------------
				// single point drawing

				void Draw(vml::views::View* view,const glm::vec3& pos, const glm::vec4& col)
				{
					if (view)
					{
						glm::mat4 m;

						float* mptr = glm::value_ptr(m);

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
						mptr[12] = pos.x;
						mptr[13] = pos.y;
						mptr[14] = pos.z;
						mptr[15] = 1;

						MVP = view->GetViewProjection() * m;

						// Use debug shader

						glUseProgram(ShaderProgram);

						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glDisable(GL_CULL_FACE);
						glBindVertexArray(VAOid);
						glPointSize(5.0f);
						glDrawArrays(GL_POINTS, 0, Vertices);
						glEnable(GL_CULL_FACE);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("SinglePoint : ","Null matrix cam for cube rendering");
					}
				}

				// ---------------------------------------------------------------

				const glm::mat4& GetMVP() const { return MVP; }

				// ---------------------------------------------------------------
				// constructors / destructors

				SinglePoint()
				{
					VAOid = 0;
					IndexBufferObject = 0;
					BufferObjects[0] = 0;
					BufferObjects[1] = 0;
					BufferObjects[2] = 0;
					BufferObjects[3] = 0;
					MVP = glm::mat4(1.0f);

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

					// attach shader
					
					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});

					// shader locations

					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					glUseProgram(0);
					
				}

				~SinglePoint()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					vml::stores::ShaderStore->UnLoad(ShaderFileName);
				}

		};

	} // end of debugrednering namespace

} // end of vml namespace
