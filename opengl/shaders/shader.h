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

#include <vml5.0/os/common.h>
#include <vml5.0/os/errormsg.h>
#include <vml5.0/utils/flags.h>
#include <vml5.0/utils/assetstore.h>

#include <GL\glew.h>
#include <GL\wglew.h>

////////////////////////////////////////////////////////////
// shader resource manager node class

namespace vml
{
	namespace shaders
	{
		
		////////////////////////////////////////////////////////////
		// shader resource manager node class
		
		class GlShaderProgram : public vml::utils::SharedResource
		{
			
			private:
				
				// -----------------------------------------------------------------------

				GLuint Id;									// shader id
				GLint  ViewMatrixLocation;					// view matrix location
				GLint  NormalMatrixLocation;				// normal matrix location
				GLint  ProjectionMatrixLocation;			// projection matrix location
				GLint  ModelMatrixLocation;					// model matrix location
				GLint  ModelViewMatrixLocation;				// model * view matrix location
				GLint  ModelViewProjectionMatrixLocation;	// model * view * projection matrix location
				GLint  TextureMatrixLocation;				// texture matrix

				// -----------------------------------------------------------------------
				// Compile shaders given the source code
				
				GLuint CompileShader(GLenum type, const char* src)
				{
					GLuint id = glCreateShader(type);
					glShaderSource(id, 1, &src, nullptr);
					glCompileShader(id);
					GLint compiled;
					glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
					if (!compiled) {
						char buf[1024] = { 0 };
						glGetShaderInfoLog(id, sizeof(buf), nullptr, buf);
						vml::os::Message::Error("GlProgram : Shader compile error : ", buf);
						return 0;
					}
					return id;
				}

				// -----------------------------------------------------------------------
				// load shader file

				const std::string LoadShaderFile(const std::string& filename)
				{
					if (filename.empty())
						vml::os::Message::Error("CGlShaderProgram : Filename is empty");
					// open file stream
					std::ifstream file(filename.c_str(), std::ios::binary);
					if (!file.is_open())
						vml::os::Message::Error("CGlShaderProgram : Cannot  open file : ", filename);
					// load source file
					file.seekg(0, std::ios::end);
					unsigned int fileSize = static_cast<unsigned int>(file.tellg());
					if ( fileSize==0)
						vml::os::Message::Error("CGlShaderProgram : File Size is null : ", filename);
					std::string source;
					source.resize(fileSize);
					file.seekg(0, std::ios::beg);
					file.read(&source[0], fileSize);
					file.close();
					return source;
				}

				// -----------------------------------------------------------------------
				// Create and link shader program

				GLuint CreateShaderProgram(const char *vs, const char *fs)
				{
					GLint vertexShaderId = CompileShader(GL_VERTEX_SHADER, vs);
					GLint fragmnetShaderId = CompileShader(GL_FRAGMENT_SHADER, fs);

					// once shaders are loaded , attach them to the shader program 

					GLuint id = glCreateProgram();

					if (id == 0) {
						vml::os::Message::Error("GlProgram : Error creating gl program");
					}

					// attach sahders

					if (vertexShaderId) glAttachShader(id, vertexShaderId);
					if (fragmnetShaderId) glAttachShader(id, fragmnetShaderId);

					// link shaders 

					GLint linked;
					glLinkProgram(id);
					glGetProgramiv(id, GL_LINK_STATUS, &linked);

					// check if shader porgram has been linked correctly

					if (!linked)
					{
						glDeleteProgram(id);
						GLsizei infoLogSize = 0;
						std::string infoLog;
						glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);
						infoLog.resize(infoLogSize);
						glGetProgramInfoLog(id, infoLogSize, &infoLogSize, &infoLog[0]);
						vml::os::Message::Error("GlProgram : Program linking error : ", infoLog.c_str());
					}

					// detach shaders after linking

					if (vertexShaderId)   glDetachShader(id, vertexShaderId);
					if (fragmnetShaderId) glDetachShader(id, fragmnetShaderId);

					// delete shaders

					if (vertexShaderId)   glDeleteShader(vertexShaderId);
					if (fragmnetShaderId) glDeleteShader(fragmnetShaderId);

					return id;
				}

			public:
				
				// -----------------------------------------------------------------------
				// Attributes layouts.

				static const int ATTRIBUTE_POSITION		= 0;
				static const int ATTRIBUTE_NORMAL		= 1;
				static const int ATTRIBUTE_COLOR		= 2;
				static const int ATTRIBUTE_TEXCOORD		= 3;
				static const int ATTRIBUTE_TANGENT		= 4;
				static const int ATTRIBUTE_BI_TANGENT	= 5;

				// -----------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				GlShaderProgram(const GlShaderProgram& other) = delete;

				// -----------------------------------------------------------------------
				// overload operator = is private
				// no copies allowed since classes
				// are referenced

				GlShaderProgram& operator = (const GlShaderProgram& other) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				GlShaderProgram(GlShaderProgram&& other) = delete;
				GlShaderProgram& operator=(GlShaderProgram&& other) = delete;
				
				// -----------------------------------------------------------------------
				// use this glprogram

				void Use() const { glUseProgram(Id); }

				// -----------------------------------------------------------------------
				// unuse this glprogram

				void UnUse() const { glUseProgram(0); }
				
				// -----------------------------------------------------------------------
				// get resource identification parms

				[[nodiscard]] GLuint GetID() const { return Id; }

				// -----------------------------------------------------------------------
				// get transformation locations

				[[nodiscard]] GLint GetViewMatrixLocation()				   const { return ViewMatrixLocation; }
				[[nodiscard]] GLint GetModelMatrixLocation()			   const { return ModelMatrixLocation; }
				[[nodiscard]] GLint GetProjectionMatrixLocation()		   const { return ProjectionMatrixLocation; }
				[[nodiscard]] GLint GetNormalMatrixLocation()			   const { return NormalMatrixLocation; }
				[[nodiscard]] GLint GetModelViewMatrixLocation()		   const { return ModelViewMatrixLocation; }
				[[nodiscard]] GLint GetModelViewProjectionMatrixLocation() const { return ModelViewProjectionMatrixLocation; }
				[[nodiscard]] GLint GetTextureMatrixLocation()			   const { return TextureMatrixLocation; }

				// -----------------------------------------------------------------------
				// utility uniform functions
				
				void SetBoolUniform(const std::string &name, bool value)			 const { glUniform1i(glGetUniformLocation(Id,  name.c_str()), (int)value); }
				void SetIntUniform(const std::string &name, int value)				 const { glUniform1i(glGetUniformLocation(Id,  name.c_str()), value); }
				void SetFloatUniform(const std::string &name, float value)			 const { glUniform1f(glGetUniformLocation(Id,  name.c_str()), value); }
				void SetVec2Uniform(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
				void SetVec2Uniform(const std::string &name, float x, float y)		 const { glUniform2f(glGetUniformLocation(Id,  name.c_str()), x, y); }

				void SetVec3Uniform(const std::string &name, const glm::vec3 &value)			 const { glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
				void SetVec3Uniform(const std::string &name, float x, float y, float z)			 const { glUniform3f(glGetUniformLocation(Id,  name.c_str()), x, y, z); }
				void SetVec4Uniform(const std::string &name, const glm::vec4 &value)			 const { glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]); }
				void SetVec4Uniform(const std::string &name, float x, float y, float z, float w) const { glUniform4f(glGetUniformLocation(Id,  name.c_str()), x, y, z, w); }
				void SetMat2Uniform(const std::string &name, const glm::mat2 &mat)				 const { glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
				void SetMat3Uniform(const std::string &name, const glm::mat3 &mat)				 const { glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
				void SetMat4Uniform(const std::string &name, const glm::mat4 &mat)				 const { glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
				
				// -----------------------------------------------------------------------
				// ctor / dtor
				// no extrapack data for shaders
				// uses raii paradigm to load shader resource
				// ResourceFileName is initted in base class

				GlShaderProgram(const std::string& shaderfilename, const vml::utils::Flags& flags) : vml::utils::SharedResource(shaderfilename,flags)
				{
					// mandatory locations

					Id								  = 0;
					ViewMatrixLocation				  = 0;
					NormalMatrixLocation			  = 0;
					ProjectionMatrixLocation		  = 0;
					ModelViewMatrixLocation			  = 0;
					ModelMatrixLocation				  = 0;
					ModelViewProjectionMatrixLocation = 0;
					
					// check extension

					if (!GetResourceFileName().ends_with(".shd"))
						vml::os::Message::Error("GlProgram :", GetResourceFileName().c_str(), " : Bad extension");
					
					// remove extension substring

					std::string noextfilename = vml::strings::SplitPath::RemoveExtensionFromPath(GetResourceFileName());
					
					// path for vertex and fragment shaders
					
					std::string vertexshadername = noextfilename + ".vert";
					std::string fragmentshadername = noextfilename + ".frag";

					// initialize and create shaders

					Id = CreateShaderProgram(&LoadShaderFile(vertexshadername)[0], &LoadShaderFile(fragmentshadername)[0]);
					
					// use this shader

					glUseProgram(Id);

					// get uniform locations

					NormalMatrixLocation			  = glGetUniformLocation(Id, "NormalMatrix");
					ViewMatrixLocation				  = glGetUniformLocation(Id, "ViewMatrix");
					ModelMatrixLocation				  = glGetUniformLocation(Id, "ModelMatrix");
					ModelViewMatrixLocation			  = glGetUniformLocation(Id, "ModelViewMatrix");
					ProjectionMatrixLocation		  = glGetUniformLocation(Id, "ProjectionMatrix");
					TextureMatrixLocation			  = glGetUniformLocation(Id, "TextureMatrix");
					ModelViewProjectionMatrixLocation = glGetUniformLocation(Id, "ModelViewProjectionMatrix");

					//  unuse this shader once it is loaded

					glUseProgram(0);
				}

				~GlShaderProgram()
				{
					glDeleteProgram(Id);
					glUseProgram(0);
				}
				
		};
		
	}	// end of shaders namespace

}	// end of vml namespace


