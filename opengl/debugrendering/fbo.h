#pragma once

namespace vml
{
	namespace debugrendering
	{
		class FrameBufferObject
		{
			private:

				// Fullscreen quad in NDC space [-1,1]
				// we’ll transform it for a smaller rectrangle
				
				inline static const float rectangleVerts[] = {
					-1,-1, 0,0,
					 1,-1, 1,0,
					-1, 1, 0,1,
					-1, 1, 0,1,
					 1,-1, 1,0,
					 1, 1, 1,1
				};

				// Vertex shader: applies a model transform to position the quad
				const char* vsSrc = R"glsl(
											#version 400 core
											layout(location=0) in vec2 aPos;
											layout(location=1) in vec2 aUV;
											out vec2 uv;
											uniform mat4 uModel;
											void main() {
												uv = aUV;
												gl_Position = uModel * vec4(aPos, 0.0, 1.0);
											}
				)glsl";

				// Fragment shader: sample the texture and apply a filter
				const char* fsSrc = R"glsl(
											#version 400 core
											in vec2 uv;
											out vec4 FragColor;
											uniform sampler2D uTexture;
											uniform int filterType;
											void main() {
												FragColor = texture(uTexture, uv);
											}
				)glsl";

				// ----------------------------------------------------------------------------------

				static GLuint compileShader(GLenum type, const char* src)
				{
					GLuint s = glCreateShader(type);
					glShaderSource(s, 1, &src, nullptr);
					glCompileShader(s);
					GLint ok;
					glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
					if (!ok) {
						char buf[8192];
						glGetShaderInfoLog(s, sizeof(buf), nullptr, buf);
						vml::os::Message::Error("Shader compile error: {0}", buf);
					}
					return s;
				}

				// ----------------------------------------------------------------------------------

				static GLuint createShaderProgram(const char* vs, const char* fs)
				{
					GLuint p = glCreateProgram();
					GLuint vsID = compileShader(GL_VERTEX_SHADER, vs);
					GLuint fsID = compileShader(GL_FRAGMENT_SHADER, fs);
					glAttachShader(p, vsID);
					glAttachShader(p, fsID);
					glLinkProgram(p);
					GLint ok; 
					glGetProgramiv(p, GL_LINK_STATUS, &ok);
					if (!ok) {
						char buf[512]; glGetProgramInfoLog(p, 512, nullptr, buf);
						vml::os::Message::Error("Linker error: {0}", buf);
					}
					glDeleteShader(vsID);
					glDeleteShader(fsID);
					return p;
				}

	       public:

				unsigned int FBO;
				unsigned int RBO;
				unsigned int Width;
				unsigned int Height;
				unsigned int TextureID;
				unsigned int VAO;
				unsigned int VBO;
				unsigned int ShaderProgram;
				unsigned int ModelMatrixLocation;

			public:

				// ----------------------------------------------------------------------------------

				[[nodiscard]] unsigned int GetTextureId() const { return TextureID; }
				[[nodiscard]] unsigned int GetFBO()		  const { return FBO; }
				[[nodiscard]] unsigned int GetRBO()		  const { return RBO; }
				[[nodiscard]] unsigned int GetWidth()	  const { return Width; }
				[[nodiscard]] unsigned int GetHeight()    const	{ return Height; }
				[[nodiscard]] const char* GetVsShader()	  const { return vsSrc; }
				[[nodiscard]] const char* GetFsShader()	  const { return fsSrc; }

				// ----------------------------------------------------------------------------------

				void Bind() const
				{
					if (!FBO)
						vml::os::Message::Error("FBO is null");

					// Bind the custom framebuffer
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);
					glBindRenderbuffer(GL_RENDERBUFFER, RBO);
					// PROBABILMENTE INUTILE
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);
					glViewport(0, 0, Width, Height);
					// Enable depth testing since it's disabled when drawing the framebuffer rectangle
					glEnable(GL_DEPTH_TEST);
				}

				// ----------------------------------------------------------------------------------

				void Unbind() const
				{
					if (!FBO)
						vml::os::Message::Error("FBO is null");

					// Bind the default framebuffer
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
				}

				// ----------------------------------------------------------------------------------

				void Create(const int  width, const int height)
				{
					if (FBO)
						vml::os::Message::Error("FBO Already created");
					if ( width<=0 || height<=0)
						vml::os::Message::Error("FBO Dimension mismatch");

					// Shader setup
					ShaderProgram = createShaderProgram(vsSrc, fsSrc);
					glUseProgram(ShaderProgram);
					ModelMatrixLocation=glGetUniformLocation(ShaderProgram, "uModel");
					glUseProgram(0);

					// copy window dimensions
					Width = width;
					Height = height;

					//  Quad setup 
					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);
					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVerts), rectangleVerts, GL_STATIC_DRAW);
					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);

					// Create Frame Buffer Object
					glGenFramebuffers(1, &FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);

					// Create Framebuffer Texture
					glGenTextures(1, &TextureID);
					glBindTexture(GL_TEXTURE_2D, TextureID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

					// Create Render Buffer Object
					glGenRenderbuffers(1, &RBO);
					glBindRenderbuffer(GL_RENDERBUFFER, RBO);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

					// Error checking framebuffer
					if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
						vml::os::Message::Error("FBO ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				
				}

				// --------------------------------------------------------------
				
				void Draw(const glm::vec2 &scale,const glm::vec2 &pos ) const
				{
					const float matrix[16] = {
											scale.x        , 0              , 0, 0,
											0              , scale.y        , 0, 0,
											0              , 0              , 1, 0,
											pos.x + scale.x, pos.y - scale.y, 0, 1
					};

					glActiveTexture(GL_TEXTURE0);
					glUseProgram(ShaderProgram);
					glBindTexture(GL_TEXTURE_2D, TextureID);
					glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, matrix);
					glBindVertexArray(VAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUseProgram(0);
				}

				// --------------------------------------------------------------
				// ctor / dtor

				FrameBufferObject()
				{
					FBO				    = 0;
					TextureID		    = 0;
					RBO				    = 0;
					Width			    = 0;
					Height			    = 0;
					VAO				    = 0;
					VBO				    = 0;
					ShaderProgram	    = 0;
					ModelMatrixLocation = 0;
				}

				~FrameBufferObject()
				{
					if (FBO) { glDeleteFramebuffers(1, &FBO); FBO = 0; }
					if (RBO) { glDeleteRenderbuffers(1, &RBO); RBO = 0; }
					if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
					if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
					if (TextureID) { glDeleteTextures(1, &TextureID); TextureID = 0; }
					if(ShaderProgram) { glDeleteProgram(ShaderProgram);	glUseProgram(0);}
				}

		};

	}
}
