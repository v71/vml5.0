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
//	The above copyright notice and this permission notice shall be included in-
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
	namespace debugrendering
	{
		
		/////////////////////////////////////////////////////////////////
		// mesh class
			
		class CheckeredPlane
		{
			private:
	
				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices  = 6;	// vbo indices for surfaces
					
				// vertices

				inline static float VertexPositionArray[] =
				{
						1 , 0 ,  1 , 1 ,
						1 , 0 , -1 , 1 ,
					   -1 , 0 , -1 , 1 ,
					   -1 , 0 ,  1 , 1 ,
				};

				// normals

				inline static float VertexNormalArray[] =
				{
						0 , 1 , 0 ,
						0 , 1 , 0 ,
						0 , 1 , 0 ,
						0 , 1 , 0 ,
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

				inline static const GLuint SurfaceIndices[] =
				{
						0 , 1 , 3,
						1 , 2 , 3,
				};

				// shader filenames

				const std::string ShaderFileName = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/shaders/debug_checkered.shd";

			public:
					
				GLuint							VAOid;								// vertex array object id
				glm::mat4						M, MV, NV,MVP, V;
				glm::mat3						N;
				glm::vec4						FinalColor;
				GLuint							BufferObjects[4];					// buffer objects
				GLuint							IndexBufferObject;					// surface index buffer object
				GLint						    ModelViewProjectionMatrixLocation;
				GLint							ViewMatrixLocation;
				GLint							ProjectionMatrixLocation;
				GLint							ModelMatrixLocation;
				GLint							NormalMatrixLocation;
				GLint							ModelViewMatrixLocation;
				GLint							CameraPositionLocation;
				GLint							ShaderProgram;
				GLint							ColorLocation1;
				GLint							ColorLocation2;
				GLint							CheckSize;
				vml::shaders::GlShaderProgram  *Shader;
				
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
				
			public:
				
				// ---------------------------------------------------------------
				// simple textured quad

				void Draw(vml::views::View* view,
					      float scale, 
					      const glm::vec3& center, 
						  unsigned int n, 
					      const glm::vec4& c1, 
					      const glm::vec4& c2,
						  vml::debugrendering::DebugDirectionalLight& dirlight,
						  const vml::debugrendering::DebugMaterial& material) 
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

						// normal matrix

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

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));

				//		std::cout << view->GetPosition().x << " " << view->GetPosition().y << " " << view->GetPosition().z << std::endl;

						glUniform3fv(CameraPositionLocation, 1, &view->GetPosition()[0]);

						glUniform1ui(CheckSize, n);
						glUniform4f(ColorLocation1, c1.r, c1.g, c1.b, 1);
						glUniform4f(ColorLocation2, c2.r, c2.g, c2.b, 1);

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

						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_BLEND);
					
						glBindVertexArray(VAOid);
						glDisable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
						glEnable(GL_CULL_FACE);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

						glDisable(GL_BLEND);

					}
					else
					{
						vml::os::Message::Error("CheckeredPlane :","Null matrix cam for plane debug rendering");
					}
				}
					
				// ---------------------------------------------------------------
				// constructors / destructors

				CheckeredPlane()
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
					NV  = glm::mat4(1.0f);
					MVP = glm::mat4(1.0f);
					FinalColor = glm::vec4(0, 0, 0, 1);

					// Create the VAO
						
					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeNormal   = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					GLuint AttributeColor    = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;
			
					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);
					glGenBuffers(1, &BufferObjects[2]);

					// Generate and populate the buffers with vertex attributes and the indices

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0],GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalArray), &VertexNormalArray[0],	GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
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

					glDisableVertexAttribArray(AttributePosition);
					glDisableVertexAttribArray(AttributeNormal);
					glDisableVertexAttribArray(AttributeTexCoord);
					glDisableVertexAttribArray(AttributeColor);
					
					// load shader
					Shader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(ShaderFileName, {});
					
					// no lit checkred plane shader locations

					ShaderProgram					  = Shader->GetID();
					ViewMatrixLocation				  = Shader->GetViewMatrixLocation();
					ProjectionMatrixLocation		  = Shader->GetProjectionMatrixLocation();
					ModelMatrixLocation				  = Shader->GetModelMatrixLocation();
					NormalMatrixLocation			  = Shader->GetNormalMatrixLocation();
					ModelViewMatrixLocation			  = Shader->GetModelViewMatrixLocation();
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						
					// store locations for grid rednering

					glUseProgram(ShaderProgram);

					CheckSize				  = glGetUniformLocation(ShaderProgram, "chsize");
					ColorLocation1			  = glGetUniformLocation(ShaderProgram, "col1");
					ColorLocation2			  = glGetUniformLocation(ShaderProgram, "col2");
					
					CameraPositionLocation	  = glGetUniformLocation(ShaderProgram, "CameraPosition");

					MaterialAmbientLocation   = glGetUniformLocation(ShaderProgram, "Material.ambient");
					MaterialDiffuseLocation   = glGetUniformLocation(ShaderProgram, "Material.diffuse");
					MaterialSpecularLocation  = glGetUniformLocation(ShaderProgram, "Material.specular");
					MaterialShininessLocation = glGetUniformLocation(ShaderProgram, "Material.shininess");
					LightAmbientLocation      = glGetUniformLocation(ShaderProgram, "DirectionalLight.ambient");
					LightDiffuseLocation      = glGetUniformLocation(ShaderProgram, "DirectionalLight.diffuse");
					LightSpecularLocation     = glGetUniformLocation(ShaderProgram, "DirectionalLight.specular");
					LightDirectionLocation    = glGetUniformLocation(ShaderProgram, "DirectionalLight.direction");
					LightPowerLocation        = glGetUniformLocation(ShaderProgram, "DirectionalLight.power");
					LightCameraSpaceLocation  = glGetUniformLocation(ShaderProgram, "DirectionalLight.cameraspacedirection");

					// validate debug shaders locations
						
					if (MaterialAmbientLocation   == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'Material.ambient' uniform, check shader source code");
					if (MaterialDiffuseLocation   == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'MaterialDiffuseLocation' uniform, check shader source code");
					if (MaterialSpecularLocation  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'MaterialSpecularLocation' uniform, check shader source code");
					if (MaterialShininessLocation == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'MaterialShininessLocation' uniform, check shader source code");
					if (LightAmbientLocation      == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'DirectionalLight.ambient' uniform, check shader source code");
					if (LightDiffuseLocation      == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'DirectionalLight.diffuse' uniform, check shader source code");
					if (LightSpecularLocation     == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'DirectionalLight.specular' uniform, check shader source code");
					if (LightPowerLocation        == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'DirectionalLight.power' uniform, check shader source code");
					if (LightCameraSpaceLocation  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'DirectionalLight.cameraspacedirection' uniform, check shader source code");
					if (CheckSize				  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'CheckSize' uniform, check shader source code");
					if (ColorLocation1			  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'ColorLocation1' uniform, check shader source code");
					if (ColorLocation2			  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'ColorLocation2' uniform, check shader source code");
			//		if (CameraPositionLocation	  == -1) vml::os::Message::Error("GlProgram : CheckredPlane requires 'CameraPositionLocation2' uniform, check shader source code");

					glUseProgram(0);
					
				}

				~CheckeredPlane()
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
			
		} // end of debugrender namespace

} // end of vml namespace

