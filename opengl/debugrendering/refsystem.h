#pragma once


namespace vml
{
	namespace debugrendering
	{

		class RefSystem
		{
			private:
				
				// arrow data struct

				// verex and surfaces count

				const static unsigned int ArrowVertices = 16;
				const static unsigned int ArrowIndices = 18;

				// vertices

				inline static float ArrowVertexPositionArray[] =
				{
					   -0.5f ,  0    ,  -0.5f , 1,
						0.5f ,  0    ,  -0.5f , 1,
						0.5f ,  0    ,   0.5f , 1,
					   -0.5f ,  0    ,   0.5f , 1,

					   -0.5f ,  0    ,  -0.5f , 1,
						0.5f ,  0    ,  -0.5f , 1,
						0    ,  1.5f ,      0 , 1,

						0.5f ,  0    ,  -0.5f , 1,
						0    ,  1.5f ,      0 , 1,
						0.5f ,  0    ,   0.5f , 1,

					   -0.5f ,  0    ,   0.5f , 1,
						0    ,  1.5f ,      0 , 1,
						0.5f ,  0    ,   0.5f , 1,

					   -0.5f ,  0    ,  -0.5f , 1,
						0    ,  1.5f ,      0 , 1,
					   -0.5f ,  0    ,   0.5f , 1,
				};

				// normals

				inline static float ArrowVertexNormalArray[] =
				{
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,

					0          , 0.707107f , -0.707107f,
					0          , 0.707107f , -0.707107f,
					0          , 0.707107f , -0.707107f,

					0.707107f  , 0.707107f , 0,
					0.707107f  , 0.707107f , 0,
					0.707107f  , 0.707107f , 0,

					0          , 0.707107f , 0.707107f ,
					0          , 0.707107f , 0.707107f ,
					0          , 0.707107f , 0.707107f ,

					-0.707107f , 0.707107f , 0,
					-0.707107f , 0.707107f , 0,
					-0.707107f , 0.707107f , 0
				};


				// add surfaces 

				inline static unsigned int ArrowTrianglesArray[] =
				{
					 0,  1,  2,
					 0,  2,  3,
					 6,	 5,	 4,
					 7,	 8,	 9,
					 12, 11, 10,
					 15, 14, 13
				};

				// cube data struct

				// verex and surfaces count

				const static unsigned int AxisVertices = 24;
				const static unsigned int AxisIndices = 36;

				// vertices

				inline static float AxisVertexPositionArray[] =
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

				inline static float AxisVertexNormalArray[] =
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

				// add surfaces 

				inline static unsigned int AxisTrianglesArray[] =
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
				  20, 22, 23,
				  20, 21, 22
				};

			private:

				// vaoids for arrow

				GLuint ArrowVAOid;					// vertex array object id
				GLuint ArrowIndexBufferObject;		// surface index buffer object
				GLuint ArrowBufferObjects[4];		// buffer objects

				// vaoids for cubes

				GLuint AxisVAOid;					// vertex array object id
				GLuint AxisIndexBufferObject;		// surface index buffer object
				GLuint AxisBufferObjects[4];		// buffer objects
				
				// matrices

				glm::mat4 AxisM;
				glm::mat3 AxisNV;
				glm::mat4 AxisMV;
				glm::mat4 AxisMVP;
				glm::mat4 ArrowM;
				glm::mat3 ArrowNV;
				glm::mat4 ArrowMV;
				glm::mat4 ArrowMVP;
				glm::mat4 ParentM;
				glm::mat4 ParentMV;
				glm::mat4 ParentMVP;
				glm::mat3 ParentNV;

				float AxisLength;
				float AxisScale;
				float ArrowScale;

				// debug directional light

				vml::debugrendering::DebugDirectionalLight DirectionalLight;

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

				// materials

				vml::debugrendering::DebugMaterial RootMaterial;
				vml::debugrendering::DebugMaterial XAxisMaterial;
				vml::debugrendering::DebugMaterial YAxisMaterial;
				vml::debugrendering::DebugMaterial ZAxisMaterial;

				// paths

				const std::string PhongShaderFilename = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "\\shaders\\debug_phong_dir.shd";

				// debug shader

				vml::shaders::GlShaderProgram* PhongShader;

			public:

				// ------------------------------------------------------------------------------
				// draw arrow

				void DrawArrow( vml::views::View* view,
								const glm::vec3& a, const glm::vec3& b,
								const glm::vec4& color,
								float axisscale, float arrowscale, float axislength)
				{
					if (view)
					{

						glm::mat4 ArrowM;
						glm::mat4 ArrowMV;
						glm::mat4 ArrowMVP;
						glm::mat3 ArrowNV;

						glm::mat4 ParentM;
						glm::vec3 up;
						glm::vec3 right;
						glm::vec3 forward;
						glm::vec3 direction;

						// keep the axisscale constant 
						// even at far distances to discern axis

						direction = glm::normalize(b - a);

						// get parent matrix pointer

						up = glm::vec3(0, 1, 0);

						if (direction.x == 0.0f && direction.y == 1.0f && direction.z == 0.0f)
						{
							right = glm::vec3(1, 0, 0);
							forward = glm::vec3(0, 0, -1);
						}
						else
						{
							right = glm::normalize(glm::cross(up, direction));
							forward = glm::normalize(glm::cross(direction, right));
						}

						// get view and project matrices

						const glm::mat4& V = view->GetView();
						const glm::mat4& P = view->GetProjection();

						// compute parent matrix

						ParentM = glm::mat4(direction.x, direction.y, direction.z, 0,
											right.x, right.y, right.z, 0,
											forward.x, forward.y, forward.z, 0,
											a.x, a.y, a.z, 1);

						// create axis matrix

						AxisM = glm::mat4(axislength, 0, 0, 0,
										  0, axisscale, 0, 0,
										  0, 0, axisscale, 0,
										  axislength * 0.5f, 0, 0, 1);

						// concatenate matrices

						AxisM = ParentM * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get child arrowe matrix

						ArrowM = glm::mat4(0, -arrowscale, 0, 0,
										   arrowscale, 0, 0, 0,
										   0, 0, arrowscale, 0,
										   axislength, 0, 0, 1);

						// concatenate parent and child matrices

						ArrowM = ParentM * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// drawing

						DirectionalLight.Ambient = glm::vec4(1, 1, 1, 1);
						DirectionalLight.Diffuse = glm::vec4(1, 1, 1, 1);
						DirectionalLight.Specular = glm::vec4(1, 1, 1, 1);
						DirectionalLight.Direction = glm::vec4(0, 1, 0, 0);
						DirectionalLight.Power = 1.0f;
						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						// Setup a silver material to show off specular lighting.

						vml::debugrendering::DebugMaterial material(glm::vec4(color.x, color.y, color.z, 1.0f),		// ambient
																	glm::vec4(1, 1, 1, 1),				// diffuse
																	glm::vec4(1, 1, 1, 1),				// specular
																	glm::vec4(0, 0, 0, 1),				// emission
																	5);									// shininess

						// draw axis 

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations

						glUniform4fv(MaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(MaterialShininessLocation, material.Shininess);

						glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices

						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						glBindVertexArray(AxisVAOid);

						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);

						// arrow matrices

						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh

						glBindVertexArray(ArrowVAOid);

						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,			// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);

						glBindVertexArray(0);
						//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null matrix cam for ref system rendering");
					}
				}

				// ------------------------------------------------------------------------------
				// draw a reference system using a matrix as argument

				void Draw(vml::views::View* view,const glm::mat4 &m)
				{

					if (view)
					{
						// get view and project matrices
						const glm::mat4& V = view->GetView();
						const glm::mat4& P = view->GetProjection();
						float dx = 0.1f;
						
						// compute light direction in camera space
						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						// create x axis matrix
						AxisM = glm::mat4(AxisLength, 0, 0, 0,
										  0, AxisScale, 0, 0,
										  0, 0, AxisScale, 0,
										  AxisLength * 0.5f+dx, 0, 0, 1);

						// concatenate matrices
						AxisM   = m * AxisM;
						AxisMV  = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV  = glm::transpose(glm::inverse(AxisMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &XAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &XAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &XAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, XAxisMaterial.Shininess);

						// set light locations 
						glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
			//			glUseProgram(0);

						// create x arrow  matrix
						ArrowM = glm::mat4(0, -ArrowScale, 0, 0,
											ArrowScale, 0, 0, 0,
											0, 0, ArrowScale, 0,
											AxisLength+dx, 0, 0, 1);

						ArrowM = m * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
				//		glUseProgram(PhongShader->GetID());

						// set shader locations
			//			glUniform4fv(MaterialAmbientLocation, 1, &XAxisMaterial.Ambient[0]);
			//			glUniform4fv(MaterialDiffuseLocation, 1, &XAxisMaterial.Diffuse[0]);
			//			glUniform4fv(MaterialSpecularLocation, 1, &XAxisMaterial.Specular[0]);
			//			glUniform1f(MaterialShininessLocation, XAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
			//			glUseProgram(0);

						// create y axis matrix
						AxisM = glm::mat4(AxisScale, 0, 0, 0,
										0, AxisLength, 0, 0,
										0, 0, AxisScale, 0,
										0, AxisLength * 0.5f+dx, 0, 1);

						// concatenate matrices
						AxisM = m * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get shader
			//			glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &YAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &YAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &YAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, YAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
		//				glUseProgram(0);

						// create y arrow  matrix
						ArrowM = glm::mat4(0, 0, -ArrowScale, 0,
											0, ArrowScale, 0, 0,
											ArrowScale, 0, 0, 0,
											0, AxisLength+dx, 0, 1);

						ArrowM = m * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
			//			glUseProgram(PhongShader->GetID());

						// set shader locations
				//		glUniform4fv(MaterialAmbientLocation, 1, &YAxisMaterial.Ambient[0]);
				//		glUniform4fv(MaterialDiffuseLocation, 1, &YAxisMaterial.Diffuse[0]);
				//		glUniform4fv(MaterialSpecularLocation, 1, &YAxisMaterial.Specular[0]);
				//		glUniform1f(MaterialShininessLocation, YAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
			//			glUseProgram(0);

						// create z axis matrix
						AxisM = glm::mat4(AxisScale, 0, 0, 0,
											0, AxisScale, 0, 0,
											0, 0, AxisLength, 0,
											0, 0, AxisLength * 0.5f+dx, 1);

						// concatenate matrices
						AxisM = m * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &ZAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &ZAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &ZAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, ZAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
			//			glUseProgram(0);

						// create z arrow  matrix
						ArrowM = glm::mat4(ArrowScale, 0, 0, 0,
											0, 0, ArrowScale, 0,
											0, -ArrowScale, 0, 0,
											0, 0, AxisLength+dx, 1);

						ArrowM = m * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
				//		glUniform4fv(MaterialAmbientLocation, 1, &ZAxisMaterial.Ambient[0]);
				//		glUniform4fv(MaterialDiffuseLocation, 1, &ZAxisMaterial.Diffuse[0]);
				//		glUniform4fv(MaterialSpecularLocation, 1, &ZAxisMaterial.Specular[0]);
				//		glUniform1f(MaterialShininessLocation, ZAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null matrix cam for ref system rendering");
					}

				}

				// ------------------------------------------------------------------------------
				// draw a reference system using a matrix as argument

				void DrawRightVector(vml::views::View* view, const glm::mat4& m)
				{
					if (view)
					{
						// get view and project matrices
						const glm::mat4& V = view->GetView();
						const glm::mat4& P = view->GetProjection();
						float dx = 0.1f;

						// compute light direction in camera space
						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						//create root matrix
						ParentM = m;
						ParentMV = V * ParentM;
						ParentMVP = P * ParentMV;
						ParentNV = glm::transpose(glm::inverse(ParentMV));

						// create x axis matrix
						AxisM = glm::mat4(AxisLength, 0, 0, 0,
										  0, AxisScale, 0, 0,
										  0, 0, AxisScale, 0,
										  AxisLength * 0.5f+dx, 0, 0, 1);

						// concatenate matrices
						AxisM = ParentM * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &XAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &XAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &XAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, XAxisMaterial.Shininess);

						// set light locations 
						glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						//			glUseProgram(0);

						// create x arrow  matrix
						ArrowM = glm::mat4(0, -ArrowScale, 0, 0,
										   ArrowScale, 0, 0, 0,
										   0, 0, ArrowScale, 0,
										   AxisLength+dx, 0, 0, 1);

						ArrowM = ParentM * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
				//		glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &XAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &XAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &XAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, XAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null matrix cam for ref system rendering");
					}

				}

				// ------------------------------------------------------------------------------
				// draw a reference system using a matrix as argument

				void DrawUpVector(vml::views::View* view, const glm::mat4& m)
				{
					if (view)
					{
						// get view and project matrices
						const glm::mat4& V = view->GetView();
						const glm::mat4& P = view->GetProjection();
						float dx = 0.1f;

						// compute light direction in camera space
						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						//create root matrix
						ParentM = m;
						ParentMV = V * ParentM;
						ParentMVP = P * ParentMV;
						ParentNV = glm::transpose(glm::inverse(ParentMV));

						// create y axis matrix
						AxisM = glm::mat4(AxisScale, 0, 0, 0,
										  0, AxisLength, 0, 0,
										  0, 0, AxisScale, 0,
										  0, AxisLength * 0.5f+dx, 0, 1);

						// concatenate matrices
						AxisM = ParentM * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &YAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &YAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &YAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, YAxisMaterial.Shininess);

						// set light locations 
						glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						//			glUseProgram(0);

						// create x arrow  matrix
						ArrowM = glm::mat4(0, 0, -ArrowScale, 0,
										   0, ArrowScale, 0, 0,
										   ArrowScale, 0, 0, 0,
										   0, AxisLength+dx, 0, 1);

						ArrowM = ParentM * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
				//		glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &YAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &YAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &YAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, YAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null matrix cam for ref system rendering");
					}

				}

				// ------------------------------------------------------------------------------
				// draw a reference system using a matrix as argument

				void DrawForwardVector(vml::views::View* view, const glm::mat4& m)
				{
					if (view)
					{
						// get view and project matrices
						const glm::mat4& V = view->GetView();
						const glm::mat4& P = view->GetProjection();
						float dx = 0.1f;

						// compute light direction in camera space
						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						//create root matrix
						ParentM = m;
						ParentMV = V * ParentM;
						ParentMVP = P * ParentMV;
						ParentNV = glm::transpose(glm::inverse(ParentMV));

						// create z axis matrix
						AxisM = glm::mat4(AxisScale, 0, 0, 0,
										  0, AxisScale, 0, 0,
										  0, 0, AxisLength, 0,
										  0, 0, AxisLength * 0.5f+dx, 1);

						// concatenate matrices
						AxisM = ParentM * AxisM;
						AxisMV = V * AxisM;
						AxisMVP = P * AxisMV;
						AxisNV = glm::transpose(glm::inverse(AxisMV));

						// get shader
						glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &ZAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &ZAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &ZAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, ZAxisMaterial.Shininess);

						// set light locations 
						glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(AxisVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							AxisIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						//			glUseProgram(0);

						// create z arrow  matrix
						ArrowM = glm::mat4(ArrowScale, 0, 0, 0,
							0, 0, ArrowScale, 0,
							0, -ArrowScale, 0, 0,
							0, 0, AxisLength+dx, 1);

						ArrowM = ParentM * ArrowM;
						ArrowMV = V * ArrowM;
						ArrowMVP = P * ArrowMV;
						ArrowNV = glm::transpose(glm::inverse(ArrowMV));

						// get shader
				//		glUseProgram(PhongShader->GetID());

						// set shader locations
						glUniform4fv(MaterialAmbientLocation, 1, &ZAxisMaterial.Ambient[0]);
						glUniform4fv(MaterialDiffuseLocation, 1, &ZAxisMaterial.Diffuse[0]);
						glUniform4fv(MaterialSpecularLocation, 1, &ZAxisMaterial.Specular[0]);
						glUniform1f(MaterialShininessLocation, ZAxisMaterial.Shininess);

						// set light locations 
			//			glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
			//			glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
			//			glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
			//			glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
			//			glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
			//			glUniform1f(LightPowerLocation, DirectionalLight.Power);

						// axis matrices
						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowM));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowNV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

						// draw mesh
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glBindVertexArray(ArrowVAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							ArrowIndices,		// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);
						glBindVertexArray(0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null matrix cam for ref system rendering");
					}

				}

				// ---------------------------------------------------------------
				// ctor / dtor

				RefSystem()
				{
					// arrow vao
					ArrowVAOid = 0;
					ArrowIndexBufferObject = 0;
					ArrowBufferObjects[0]  = 0;
					ArrowBufferObjects[1]  = 0;
					ArrowBufferObjects[2]  = 0;
					ArrowBufferObjects[3]  = 0;

					// axis vao
					AxisVAOid = 0;
					AxisIndexBufferObject = 0;
					AxisBufferObjects[0]  = 0;
					AxisBufferObjects[1]  = 0;
					AxisBufferObjects[2]  = 0;
					AxisBufferObjects[3]  = 0;

					AxisM   = glm::mat4(1.0f);
					AxisNV  = glm::mat4(1.0f);
					AxisMV  = glm::mat4(1.0f);
					AxisMVP = glm::mat4(1.0f);

					ArrowM    = glm::mat4(1.0f);
					ArrowNV   = glm::mat2(1.0f);
					ArrowMV   = glm::mat4(1.0f);
					ArrowMVP  = glm::mat4(1.0f);
					ParentM   = glm::mat4(1.0f);
					ParentMV  = glm::mat4(1.0f);
					ParentMVP = glm::mat4(1.0f);
					ParentNV  = glm::mat3(1.0f);

					// directional light

					DirectionalLight.Ambient = glm::vec4(1, 1, 1, 1);
					DirectionalLight.Diffuse = glm::vec4(1, 1, 1, 1);
					DirectionalLight.Specular = glm::vec4(1, 1, 1, 1);
					DirectionalLight.Direction = glm::vec4(1, 1, 0, 0);
					DirectionalLight.Power = 1.0f;

					AxisLength = 8.0f;
					AxisScale  = 0.05f;
					ArrowScale = 0.2f;

					// initialise materials

					RootMaterial = vml::debugrendering::DebugMaterial(glm::vec4(0.1, 0.1, 0.1, 1),		// ambient
																	  glm::vec4(1, 1, 1, 1),			// diffuse
																	  glm::vec4(1, 1, 1, 1),			// specular
																	  glm::vec4(0, 0, 0, 1),			// emission
																	  5);								// shininess

					XAxisMaterial = vml::debugrendering::DebugMaterial(glm::vec4(1, 0.1, 0.1, 1),		// ambient
																	   glm::vec4(1, 1, 1, 1),			// diffuse
																	   glm::vec4(1, 1, 1, 1),			// specular
																	   glm::vec4(0, 0, 0, 1),			// emission
																	   5);								// shininess

					YAxisMaterial = vml::debugrendering::DebugMaterial(glm::vec4(0.1, 1, 0.1, 1),		// ambient
																	   glm::vec4(1, 1, 1, 1),			// diffuse
																	   glm::vec4(1, 1, 1, 1),			// specular
																	   glm::vec4(0, 0, 0, 1),			// emission
																	   5);								// shininess

					ZAxisMaterial = vml::debugrendering::DebugMaterial(glm::vec4(0.1, 0.1, 1, 1),		// ambient
																		glm::vec4(1, 1, 1, 1),			// diffuse
																		glm::vec4(1, 1, 1, 1),			// specular
																		glm::vec4(0, 0, 0, 1),			// emission
																		5);								// shininess

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

					GLuint AttributePosition;
					GLuint AttributeNormal;
					GLuint AttributeTexCoord;
					GLuint AttributeColor;

					AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					AttributeNormal = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenVertexArrays(1, &ArrowVAOid);
					glBindVertexArray(ArrowVAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &ArrowBufferObjects[0]);
					glBindBuffer(GL_ARRAY_BUFFER, ArrowBufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, (long long)ArrowVertices * 4 * sizeof(float), &ArrowVertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glGenBuffers(1, &ArrowBufferObjects[1]);
					glBindBuffer(GL_ARRAY_BUFFER, ArrowBufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, (long long)ArrowVertices * 3 * sizeof(float), &ArrowVertexNormalArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &ArrowIndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ArrowIndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)ArrowIndices * sizeof(unsigned int), &ArrowTrianglesArray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
										
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

					AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					AttributeNormal = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenVertexArrays(1, &AxisVAOid);
					glBindVertexArray(AxisVAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &AxisBufferObjects[0]);
					glBindBuffer(GL_ARRAY_BUFFER, AxisBufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, (long long)AxisVertices * 4 * sizeof(float), &AxisVertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glGenBuffers(1, &AxisBufferObjects[1]);
					glBindBuffer(GL_ARRAY_BUFFER, AxisBufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, (long long)AxisVertices * 3 * sizeof(float), &AxisVertexNormalArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &AxisIndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxisIndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)AxisIndices * sizeof(unsigned int), &AxisTrianglesArray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					// attach debug phong dir shader

					PhongShader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(PhongShaderFilename, {});

					// retrive shader color locations for debug shader

					GLuint Id = PhongShader->GetID();

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

					if (MaterialAmbientLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'Material.ambient' uniform, check shader source code");
					if (MaterialDiffuseLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialDiffuseLocation' uniform, check shader source code");
					if (MaterialSpecularLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialSpecularLocation' uniform, check shader source code");
					if (MaterialShininessLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialShininessLocation' uniform, check shader source code");
					if (LightAmbientLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.ambient' uniform, check shader source code");
					if (LightDiffuseLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.diffuse' uniform, check shader source code");
					if (LightSpecularLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.specular' uniform, check shader source code");
					if (LightPowerLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.power' uniform, check shader source code");
					if (LightCameraSpaceLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.cameraspacedirection' uniform, check shader source code");

				}

				~RefSystem()
				{
					if (AxisVAOid) { glDeleteVertexArrays(1, &AxisVAOid); AxisVAOid = 0; }
					if (AxisIndexBufferObject) { glDeleteBuffers(1, &AxisIndexBufferObject); AxisIndexBufferObject = 0; }
					if (AxisBufferObjects[0]) { glDeleteBuffers(1, &AxisBufferObjects[0]); AxisBufferObjects[0] = 0; }
					if (AxisBufferObjects[1]) { glDeleteBuffers(1, &AxisBufferObjects[1]); AxisBufferObjects[1] = 0; }
					if (AxisBufferObjects[2]) { glDeleteBuffers(1, &AxisBufferObjects[2]); AxisBufferObjects[2] = 0; }
					if (AxisBufferObjects[3]) { glDeleteBuffers(1, &AxisBufferObjects[3]); AxisBufferObjects[3] = 0; }

					if (ArrowVAOid) { glDeleteVertexArrays(1, &ArrowVAOid); ArrowVAOid = 0; }
					if (ArrowIndexBufferObject) { glDeleteBuffers(1, &ArrowIndexBufferObject); ArrowIndexBufferObject = 0; }
					if (ArrowBufferObjects[0]) { glDeleteBuffers(1, &ArrowBufferObjects[0]); ArrowBufferObjects[0] = 0; }
					if (ArrowBufferObjects[1]) { glDeleteBuffers(1, &ArrowBufferObjects[1]); ArrowBufferObjects[1] = 0; }
					if (ArrowBufferObjects[2]) { glDeleteBuffers(1, &ArrowBufferObjects[2]); ArrowBufferObjects[2] = 0; }
					if (ArrowBufferObjects[3]) { glDeleteBuffers(1, &ArrowBufferObjects[3]); ArrowBufferObjects[3] = 0; }

					vml::stores::ShaderStore->UnLoad(PhongShaderFilename);
				}

		};
	}
}
