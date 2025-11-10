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

		class CheckeredBoard
		{
			public:

				// ------------------------------------------------------------------------------
				// triangle going outsize screen boundaries
				// in such way opengl will clip the triangle to
				// the entire viewport avoiding creating 
				// two triangles

				const float Triangle[6] = { -1.f, -1.f,
											 3.f, -1.f,
											-1.f,  3.f };

				// ------------------------------------------------------------------------------

				GLuint VaoTriangle;
				GLuint VboTriangle;
				GLuint GridProg;
				GLuint ProjectionMatrixLocation;
				GLuint ViewMatrixLocation;
				GLuint InvProjectionMatrixLocation;
				GLuint InvViewMatrixLocation;
				GLuint CamPosLocation;
				GLuint CellSizeLocation;
				GLuint FogColorLocation;
				GLuint FogDensityLocation;
				GLuint ColorALocation;
				GLuint ColorBLocation;

				// ------------------------------------------------------------------------------
				// vertex shader

				const char* grid_vert = R"glsl(
														#version 400 core
														layout(location = 0) in vec2 inPos;
														out vec2 vUV;

														void main() {
															gl_Position = vec4(inPos, 0.0, 1.0);
															vUV = inPos * 0.5 + 0.5;
														}
														)glsl";

				// ------------------------------------------------------------------------------
				// fragment shader
				
				const char* grid_frag = R"glsl(
														#version 400 core
														in vec2 vUV;
														out vec4 FragColor;

														uniform mat4 u_proj;
														uniform mat4 u_view;
														uniform mat4 u_invProj;
														uniform mat4 u_invView;
														uniform vec3 u_camPos;

														uniform float u_cellSize = 1.0;
														uniform vec3 u_fogColor=vec3(1.0,1.0,1.0);
														uniform float u_fogDensity = 0.002;

														uniform vec3 colorA = vec3(1.0);  // white
														uniform vec3 colorB = vec3(0.5);  // dark

														void main()
														{
															// --- Reconstruct world-space ray ---
															vec2 ndc = vUV * 2.0 - 1.0;
															vec4 clip = vec4(ndc, -1.0, 1.0);
															vec4 viewPos = u_invProj * clip;
															viewPos /= viewPos.w;
															vec3 rayDir_world = normalize((u_invView * vec4(viewPos.xyz, 0.0)).xyz);
															vec3 rayOrigin_world = u_camPos;

															// --- Intersect plane Y=0 ---
															float denom = rayDir_world.y;
															if (abs(denom) < 1e-6) discard;
															float t = -rayOrigin_world.y / denom;
															if (t <= 0.0) discard;
															vec3 hit = rayOrigin_world + rayDir_world * t;

															// --- Depth buffer (so other geometry occludes correctly) ---
															vec4 clipHit = u_proj * u_view * vec4(hit, 1.0);
															gl_FragDepth = clipHit.z / clipHit.w * 0.5 + 0.5;

															// --- Checkerboard coordinates (in tiles) ---
															vec2 grid = hit.xz / u_cellSize;

															// --- Per-axis fractional coordinate inside tile (0..1) ---
															vec2 f = fract(grid);

															// --- Compute safe filter width per axis ---
															// fwidth(grid) gives approximate size of one pixel in tile-space.
															vec2 fw = fwidth(grid);
															// protect against fw==0 (e.g. extremely zoomed out or mobile drivers)
															fw = max(fw, vec2(1.0/1024.0)); // min width -> tweakable (1/1024 tiles)

															// half-width in which we smoothly transition (in tile units)
															vec2 w = fw * 0.5;

															// --- Soft (antialiased) step around 0.5 for each axis ---
															// map fract to a smooth "is-right-half?" value in [0,1]
															float ax = smoothstep(0.5 - w.x, 0.5 + w.x, f.x);
															float ay = smoothstep(0.5 - w.y, 0.5 + w.y, f.y);

															// --- Soft XOR (for checker) using floats ---
															// XOR(a,b) = a*(1-b) + b*(1-a) = a + b - 2ab
															float checker_f = ax + ay - 2.0 * ax * ay;
															checker_f = clamp(checker_f, 0.0, 1.0);

															// But we must flip parity depending on integer tile coords:
															// (if p.x + p.y is odd, invert the checker)
															vec2 p = floor(grid);
															float parity = mod(p.x + p.y, 2.0);
															// final blend factor: when parity==0 use checker_f, when parity==1 invert it
															float blend = (parity < 0.5) ? checker_f : 1.0 - checker_f;

															// --- Final color (AA-ed checker) ---
															vec3 baseColor = mix(colorB, colorA, blend);

															// --- Fog ---
															float dist = length(hit - u_camPos);
															float fogFactor = 1.0 - exp(-u_fogDensity * dist);
															fogFactor = clamp(fogFactor, 0.0, 1.0);
															vec3 color = mix(baseColor, u_fogColor, fogFactor);

															FragColor = vec4(color, 1.0);
														}
														)glsl";
			

		
				// ------------------------------------------------------------------------------

				GLuint compileShader(GLenum type, const std::string& src)
				{
					GLuint s = glCreateShader(type);
					const char* c = src.c_str();
					glShaderSource(s, 1, &c, nullptr);
					glCompileShader(s);
					GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
					if (!ok) {
						char log[512] = { 0 };
						glGetShaderInfoLog(s, 512, nullptr, log);
						vml::os::Message::Error("CheckeredPlane : Shader compile error : ", log);
					}
					return s;
				}

				// ------------------------------------------------------------------------------

				GLuint makeProgram(const std::string& vert, const std::string& frag)
				{
					GLuint v = compileShader(GL_VERTEX_SHADER, vert);
					GLuint f = compileShader(GL_FRAGMENT_SHADER, frag);
					GLuint p = glCreateProgram();
					glAttachShader(p, v);
					glAttachShader(p, f);
					glLinkProgram(p);
					glDeleteShader(v);
					glDeleteShader(f);
					return p;
				}

			public:

				// ------------------------------------------------------------------------------

				glm::vec3 FogColor;
				float     FogDensity;
				glm::vec3 ColorA;
				glm::vec3 ColorB;
				float	  CSize;

				// ------------------------------------------------------------------------------

				void Init()
				{
					// Fullscreen triangle
					glGenVertexArrays(1, &VaoTriangle);
					glGenBuffers(1, &VboTriangle);
					glBindVertexArray(VaoTriangle);
					glBindBuffer(GL_ARRAY_BUFFER, VboTriangle);
					glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
					glBindVertexArray(0);

					// Shaders
					GridProg = makeProgram(grid_vert, grid_frag);

					// get shaders locations
					glUseProgram(GridProg);
					ProjectionMatrixLocation = glGetUniformLocation(GridProg, "u_proj");
					ViewMatrixLocation = glGetUniformLocation(GridProg, "u_view");
					InvProjectionMatrixLocation = glGetUniformLocation(GridProg, "u_invProj");
					InvViewMatrixLocation = glGetUniformLocation(GridProg, "u_invView");
					CamPosLocation = glGetUniformLocation(GridProg, "u_camPos");
					CellSizeLocation = glGetUniformLocation(GridProg, "u_cellSize");
					FogColorLocation = glGetUniformLocation(GridProg, "u_fogColor");
					FogDensityLocation = glGetUniformLocation(GridProg, "u_fogDensity");
					ColorALocation = glGetUniformLocation(GridProg, "colorA");
					ColorBLocation = glGetUniformLocation(GridProg, "colorB");
					glUseProgram(0);
				}

				// ------------------------------------------------------------------------------

				void Draw(vml::views::View* view) const
				{
					const glm::mat4& viewmatrix = view->GetView();
					const glm::mat4& projmatrix = view->GetProjection();
					const glm::mat4& invView = view->GetInvView();
					const glm::mat4& invProj = view->GetInvProjection();
					const glm::vec3& camera_pos = view->GetPosition();

					// Draw infinite grid
					glUseProgram(GridProg);

					glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projmatrix));
					glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewmatrix));
					glUniformMatrix4fv(InvProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(invProj));
					glUniformMatrix4fv(InvViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(invView));
					glUniform3fv(FogColorLocation, 1, glm::value_ptr(FogColor));
					glUniform3fv(CamPosLocation, 1, glm::value_ptr(camera_pos));
					glUniform1f(CellSizeLocation, CSize);
					glUniform1f(FogDensityLocation, FogDensity);
					glUniform3fv(ColorALocation, 1, glm::value_ptr(ColorA));
					glUniform3fv(ColorBLocation, 1, glm::value_ptr(ColorB));
					glBindVertexArray(VaoTriangle);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glUseProgram(0);
				}

				// ----------------------------------------------
				// ctor / dtor

				CheckeredBoard()
				{
					VaoTriangle = 0;
					VboTriangle = 0;
					GridProg = 0;
					ProjectionMatrixLocation = 0;
					ViewMatrixLocation = 0;
					InvProjectionMatrixLocation = 0;
					InvViewMatrixLocation = 0;
					CamPosLocation = 0;
					CellSizeLocation = 0;
					FogColorLocation = 0;
					FogDensityLocation = 0;
					ColorALocation = 0;
					ColorBLocation = 0;
					FogDensity = 0.001f;
					//		FogColor = glm::vec3(0.6, 0.7, 0.9);
					FogColor = glm::vec3(0.1f, 0.1f, 0.2f);
					ColorA = glm::vec3(0.1f);
					ColorB = glm::vec3(0.0f);
					CSize = 40.0f;
				}

				~CheckeredBoard()
				{
					glDeleteProgram(GridProg);
					glUseProgram(0);
				}

		};
			
	} // end of debugrender namespace

} // end of vml namespace

