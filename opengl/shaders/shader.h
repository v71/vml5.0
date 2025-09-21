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
//#include <GLFW\glfw3.h>
//#define GLFW_EXPOSE_NATIVE_WGL
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW\glfw3native.h>

////////////////////////////////////////////////////////////
// shader resource manager node class

namespace vml
{
	namespace shaders
	{
		
		////////////////////////////////////////////////////////
		// opengl shader class 2.0
		
		class GlShader
		{
			
			private:

				GLenum		Type;				// type of shader , vertex or fragment
				GLuint		Id;					// the shader program identifier
				std::string ResourceFileName;	// shader filename

				// -----------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				GlShader(const GlShader& other) = delete;

				// -----------------------------------------------------------------------
				// overload operator = is private
				// no copies allowed since classes
				// are referenced

				GlShader& operator = (const GlShader& other) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				GlShader(GlShader&& other) = delete;
				GlShader& operator=(GlShader&& other) = delete;

				// -----------------------------------------------------------------------
				// compile shader

				void Compile(const std::string &source)
				{
					if (source.empty())
						vml::os::Message::Error("CGlShaderProgram : ", ResourceFileName.c_str() ,"shader buffer is empty");
					
					// compile shader
					const char *code = reinterpret_cast<const GLchar *>(source.c_str());
					int compiled;
					glShaderSource(Id, 1, &code, NULL);
					glCompileShader(Id);
					glGetShaderiv(Id, GL_COMPILE_STATUS, &compiled);
					if (!compiled)
					{
						vml::os::Message::Error("CGlShaderProgram : ", ResourceFileName.c_str(),"cannot compile file ",GetInfoLog().c_str());
						glDeleteShader(Id);
					}
				}

			public:

				// -----------------------------------------------------------------------
				// get info log for this shader

				[[nodiscard]] const std::string GetInfoLog() const
				{
					GLsizei infoLogSize = 0;
					std::string infoLog;
					glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &infoLogSize);
					infoLog.resize(infoLogSize);
					glGetShaderInfoLog(Id, infoLogSize, &infoLogSize, &infoLog[0]);
					return infoLog;
				}

				// -----------------------------------------------------------------------
				// load shader program,no args needed

				bool LoadShader(const std::string &filename)
				{
					if (filename.empty())
						vml::os::Message::Error("CGlShaderProgram : ","filename is empty");

					// open file stream
					std::ifstream file(filename.c_str(), std::ios::binary);

					if (!file.is_open())
						return false;

					// load source file
					std::string source;
					file.seekg(0, std::ios::end);
					unsigned int fileSize = static_cast<unsigned int>(file.tellg());
					source.resize(fileSize);
					file.seekg(0, std::ios::beg);
					file.read(&source[0], fileSize);
					file.close();

					// compile
					Compile(source);

					return true;
				}
			
				// -----------------------------------------------------------------------
				// getters functions

				[[nodiscard]] GLuint GetID() const
				{
					return Id;
				}

				// -----------------------------------------------------------------------
				// gets shader type , fragment or vertex shader

				[[nodiscard]] const std::string GetTypeString() const
				{
					switch (Type)
					{
						case GL_VERTEX_SHADER   : return "Vertex Shader";
						case GL_FRAGMENT_SHADER : return "Pixel Shader";
						case GL_GEOMETRY_SHADER : return "Geometry Shader";
					}

					return "type not recognized";
				}

				// -----------------------------------------------------------------------
				// gets filename

				[[nodiscard]] const std::string &GetResourceFileName() const
				{
					return ResourceFileName;
				}

				// -----------------------------------------------------------------------
				// ctor / dtor

				GlShader(GLenum shaderType)
				{
					Type = shaderType;
					Id = glCreateShader(shaderType);
				}

				~GlShader()
				{
					glDeleteShader(Id);
				}

		};
		
		////////////////////////////////////////////////////////////
		// shader resource manager node class
		
		class GlShaderProgram : public vml::utils::SharedResource
		{
			
			private:
				
				// -----------------------------------------------------------------------

				GLuint		Id;											// shader id
				GLint		ViewMatrixLocation;							// view matrix location
				GLint		NormalMatrixLocation;						// normal matrix location
				GLint		ProjectionMatrixLocation;					// projection matrix location
				GLint		ModelMatrixLocation;						// model matrix location
				GLint		ModelViewMatrixLocation;					// model * view matrix location
				GLint		ModelViewProjectionMatrixLocation;			// model * view * projection matrix location
				GLint		TextureMatrixLocation;						// texture matrix
				
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

				[[nodiscard]] GLint GetViewMatrixLocation()					 const { return ViewMatrixLocation; }
				[[nodiscard]] GLint GetModelMatrixLocation()				 const { return ModelMatrixLocation; }
				[[nodiscard]] GLint GetProjectionMatrixLocation()			 const { return ProjectionMatrixLocation; }
				[[nodiscard]] GLint GetNormalMatrixLocation()				 const { return NormalMatrixLocation; }
				[[nodiscard]] GLint GetModelViewMatrixLocation()			 const { return ModelViewMatrixLocation; }
				[[nodiscard]] GLint GetModelViewProjectionMatrixLocation()	 const { return ModelViewProjectionMatrixLocation; }
				[[nodiscard]] GLint GetTextureMatrixLocation()				 const { return TextureMatrixLocation; }

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

				GlShaderProgram(const std::string& shaderfilename, const vml::utils::Flags& flags) : vml::utils::SharedResource(shaderfilename,flags)
				{
					// uses raii paradigm to load shader resource
					// ResourceFileName is initted in base class

					Id = 0;
					
					// mandatory locations

					ViewMatrixLocation				  = 0;
					NormalMatrixLocation			  = 0;
					ProjectionMatrixLocation		  = 0;
					ModelViewMatrixLocation			  = 0;
					ModelMatrixLocation				  = 0;
					ModelViewProjectionMatrixLocation = 0;
					
					// check extension

					if ( !GetResourceFileName().ends_with(".shd"))
						vml::os::Message::Error("GlProgram :", GetResourceFileName().c_str(), "bad extension");
					
					// remove extension substring

					std::string noextfilename = vml::strings::SplitPath::RemoveExtensionFromPath(GetResourceFileName());
					
					// path for vertex and fragment shaders
					
					std::string vertexshadername   = noextfilename + ".vert";
					std::string fragmentshadername = noextfilename + ".frag";
					std::string geometryshadername = noextfilename + ".geom";

					// initialize and create shaders
					
					GlShader *shVertex   = new GlShader(GL_VERTEX_SHADER);
					GlShader *shFragment = new GlShader(GL_FRAGMENT_SHADER);

					// load shaders from file path
				
					if (!shVertex->LoadShader(vertexshadername))
						vml::os::Message::Error("CGlShaderProgram : ", vertexshadername.c_str()," : ", GetResourceFileName().c_str(), " cannot open file");
					
					if (!shFragment->LoadShader(fragmentshadername))
						vml::os::Message::Error("CGlShaderProgram : ", fragmentshadername.c_str()," : ", GetResourceFileName().c_str(), " cannot open file");

					// once shaders are loaded , attach them to the shader program 

					Id = glCreateProgram();

					if (Id == 0)
						vml::os::Message::Error("GlProgram : error creating gl program");

					glAttachShader(Id, shVertex->GetID());
					glAttachShader(Id, shFragment->GetID());

					// geometry shader is not mandatory, if its present load it

					GlShader* shGeometry = new GlShader(GL_GEOMETRY_SHADER);
					bool hasgeometryshader = shGeometry->LoadShader(geometryshadername);
					if (hasgeometryshader) glAttachShader(Id, shGeometry->GetID());

					// link 

					int linked;

					glLinkProgram(Id);
					glGetProgramiv(Id, GL_LINK_STATUS, &linked);

					if (!linked)
					{
						glDeleteProgram(Id);
						GLsizei infoLogSize = 0;
						std::string infoLog;
						glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &infoLogSize);
						infoLog.resize(infoLogSize);
						glGetProgramInfoLog(Id, infoLogSize, &infoLogSize, &infoLog[0]);
						vml::os::Message::Error("GlProgram : Program linking error ", infoLog.c_str());
					}

					// detach shaders after linking

					glDetachShader(Id, shVertex->GetID());
					glDetachShader(Id, shFragment->GetID());
					
					// check if a geometry shader is present

					if (hasgeometryshader)
						glDetachShader(Id, shGeometry->GetID());
		
					// delete shaders

					vml::os::SafeDelete(shVertex);
					vml::os::SafeDelete(shFragment);
					vml::os::SafeDelete(shGeometry);

					// get uniform locations

					glUseProgram(Id);
					NormalMatrixLocation			  = glGetUniformLocation(Id, "NormalMatrix");
					ViewMatrixLocation				  = glGetUniformLocation(Id, "ViewMatrix");
					ModelMatrixLocation				  = glGetUniformLocation(Id, "ModelMatrix");
					ModelViewMatrixLocation			  = glGetUniformLocation(Id, "ModelViewMatrix");
					ProjectionMatrixLocation		  = glGetUniformLocation(Id, "ProjectionMatrix");
					ModelViewProjectionMatrixLocation = glGetUniformLocation(Id, "ModelViewProjectionMatrix");
					TextureMatrixLocation			  = glGetUniformLocation(Id, "TextureMatrix");

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


