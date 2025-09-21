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

#include <vml5.0/libs/stb/stb_image.h>

namespace vml
{
	namespace textures
	{

		// ------------------------------------------------------------
		// Boolean flags for mipmapping,memory layout and gamma

		constexpr uint32_t MIPMAPS	 = 0;				// texture mipmap boolean flags
		constexpr uint32_t RESIDENCY = 1;				// texture residence query flags
		constexpr uint32_t GAMMA	 = 2;				// texture gamma correction flags

		// ------------------------------------------------------------
		// texture memory flags , user might decide to keep data
		// this happens in cases when the user needs to further reuse
		// data not just to use as textures

		constexpr uint32_t DO_NOT_RELEASE_DATA = 8;

		// ------------------------------------------------------------
		// Texture mipmap filtering

		constexpr uint32_t FILTER_MIN_NEAREST			   = 16;	  // Nearest criterion for minification
		constexpr uint32_t FILTER_MIN_LINEAR			   = 17;	  // Bilinear criterion for minification
		constexpr uint32_t FILTER_MIN_NEAREST_MIPMAP	   = 18;	  // Nearest criterion for minification; but on closest mipmap
		constexpr uint32_t FILTER_MIN_LINEAR_MIPMAP		   = 19;      // Bilinear criterion for minification; but on closest mipmap
		constexpr uint32_t FILTER_MIN_LINEAR_MIPMAP_LINEAR = 20;	  // Bilinear criterion for minification on two closest mipmaps; then averaged
		constexpr uint32_t FILTER_MAG_NEAREST			   = 21;      // Nearest criterion for magnification
		constexpr uint32_t FILTER_MAG_LINEAR			   = 22;   	  // Bilinear criterion for magnification

		// ------------------------------------------------------------
		// GL_REPEAT:			The integer part of the coordinate will be ignored and a repeating pattern is formed.
		// GL_MIRRORED_REPEAT : The texture will also be repeated; but it will be mirrored when the integer
		//						part of the coordinate is odd.
		// GL_CLAMP_TO_EDGE :   The coordinate will simply be clamped between 0 and 1.
		// GL_CLAMP_TO_BORDER : The

		constexpr uint32_t REPEAT_S = 32;		// texture repeat for u coordinate
		constexpr uint32_t REPEAT_T = 33;	   	// texture repeat for v coordinate
		constexpr uint32_t MIRRORED_S = 34;	    // texture mirrored for u coordinate
		constexpr uint32_t MIRRORED_T = 35;	   	// texture mirrored for v coordinate
		constexpr uint32_t CLAMP_TO_EDGE_S = 36;		// texture clamp tp edge for u coordinate
		constexpr uint32_t CLAMP_TO_EDGE_T = 37;		// texture clamp tp edge for v coordinate
		constexpr uint32_t CLAMP_TO_BORDER_S = 38;		// texture clamp to border for u coordinate
		constexpr uint32_t CLAMP_TO_BORDER_T = 39;  		// texture clamp to border for u coordinate

		// ------------------------------------------------------------
		// Texture uploading default parameters
		// follow this *strict* order , since each 
		// entry is correlated to the opengl texture parm(s)

		const vml::utils::Flags DefaultTexturesParms = { REPEAT_S,
														 REPEAT_T,
														 FILTER_MAG_LINEAR,
														 FILTER_MIN_LINEAR,
														 MIPMAPS };

		////////////////////////////////////////////////////////////////
		// Texture class asset
		
		class Texture : public vml::utils::SharedResource
		{
			
			private:
				
				int			   Width;				    // Texture width
				int			   Height;				    // Texture height
				GLuint		   ID;					    // Texture Id
				GLint		   BPP;					    // Texture bytes per pixel
				GLenum		   TextureType;			    // Texture Type ( GL_TEXTUTRE_2D,... )
				GLuint		   Minification;		    // Texture Minification parameter
				GLuint		   Magnification;		    // Texture Magnification parameter
				GLuint		   RepeatS;				    // Texture repeat S direction flag
				GLuint		   RepeatT;				    // Texture repeat T direction flag
				GLenum		   Format;				    // Textrure format
				GLenum		   DataFormat;			    // Texture data format
				GLfloat		   Anisotropy;			    // anisotropy
				GLfloat		   MaxAnisotropy;		    // Max anmisotropy value
				GLfloat		   Ratio;				    // image ratio
				bool		   GenerateMipMaps;		    // Texture MipMaps generated ?
				bool		   GammaCorrection;		    // Gamma correction
				bool		   Resident;			    // check if texture is residnet in gpu memory
				bool		   ReleaseTextureData;		// rlease data for this texture if set to 1 the data will be deleted after being uploaded to the gpu
				unsigned char* Data;				    // actual image bytes
				
				// --------------------------------------------------------------------------
				// create texture according to specified parameters

				void CreateTexture2dFromData()
				{
					// set parameters for this texture
					
					Width         = 0;
					Height        = 0;
					BPP           = 0;
					
					// load image trhouhg stb

					stbi_set_flip_vertically_on_load(true);
					
					// fill data

					Data = stbi_load(GetResourceFileName().c_str(), &Width, &Height, &BPP, 0);

					if (Data)
					{
						// compute aspect ratio

						Ratio = (float)Width / (float)Height;
						
						// bytes per pixel

						if (BPP != 1 &&	BPP != 3 &&	BPP != 4)
								vml::os::Message::Error("Texture : ","Invalid Bpp for texture ' ",GetResourceFileName().c_str(), " ' is out of range");

						// repeat u parameter

						if (RepeatS != GL_REPEAT &&
							RepeatS != GL_MIRRORED_REPEAT &&
							RepeatS != GL_CLAMP_TO_EDGE &&
							RepeatS != GL_CLAMP_TO_BORDER )
								vml::os::Message::Error("Texture : ", "Invalid RepeatS for texture ' ", GetResourceFileName().c_str(), " ' is out of range");
						
						// repeat v parameter

						if (RepeatT != GL_REPEAT &&
							RepeatT != GL_MIRRORED_REPEAT &&
							RepeatT != GL_CLAMP_TO_EDGE &&
							RepeatT != GL_CLAMP_TO_BORDER)
								vml::os::Message::Error("Texture : ", "Invalid RepeatT for texture ' ", GetResourceFileName().c_str(), " ' is out of range");

						if (GenerateMipMaps)
						{
							// magnification filter

							if (Magnification != GL_NEAREST &&
								Magnification != GL_LINEAR)
									vml::os::Message::Error("Texture : ", "Invalid MipMap Mag for texture ' ", GetResourceFileName().c_str(), " ' is out of range");

							// minificationfilter

							if (Minification != GL_NEAREST &&
								Minification != GL_LINEAR &&
								Minification != GL_NEAREST_MIPMAP_NEAREST &&
								Minification != GL_LINEAR_MIPMAP_NEAREST &&
								Minification != GL_LINEAR_MIPMAP_LINEAR)
									vml::os::Message::Error("Texture : ", "Invalid MipMap Min for texture ' ", GetResourceFileName().c_str(), " ' is out of range");
						}

						// byte per pixel

						if (BPP == 1)
						{
							DataFormat = GL_RED;
							Format = DataFormat = GL_RED;
						}

						if (BPP == 3)
						{
							DataFormat = GL_RGB;
							Format = GammaCorrection ? GL_SRGB : GL_RGB;
						}

						if (BPP == 4)
						{
							DataFormat = GL_RGBA;
							Format = GammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
						}
						
						// generate texture

						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
						glGenTextures(1, &ID);
						glBindTexture(GL_TEXTURE_2D, ID);

						// sets max anisotropy

						glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Anisotropy);
						Anisotropy = (Anisotropy > MaxAnisotropy) ? MaxAnisotropy : Anisotropy;
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Anisotropy);

						// generate texture

						glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, DataFormat, GL_UNSIGNED_BYTE, Data);

						// generate mipmaps

						if (GenerateMipMaps)
						{
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

							// create 2d mipmaps for the minifying function
							glGenerateMipmap(GL_TEXTURE_2D);
						}
						
						// checks if texture is resident

						if (Resident)
						{
							GLboolean QueryList[1];
							GLuint TextureNames[] = { ID };
							glAreTexturesResident(1, TextureNames, QueryList);
							GLboolean QueryResidence = QueryList[0];
							if (QueryResidence == GL_TRUE) Resident = true;
							else Resident = false;
						}

						// clamping

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RepeatS);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RepeatT);

						// filtering

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Minification);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Magnification);

						// free data

						if (ReleaseTextureData)
						{
							stbi_image_free(Data);
							Data = nullptr;
					//		std::cout << "RELEASED" << std::endl;
						}
						
					}
					else
					{
						vml::os::Message::Error("Texture :","failed to load at path : ' ", GetResourceFileName().c_str(), " '");
						stbi_image_free(Data);
					}
					
				}
				
			public:

				// ------------------------------------------------------------
				// Converts texture parameter to string 

				static std::string GetString(uint32_t value)
				{
					switch (value)
					{
						case MIPMAPS: return "Texture_MipMap"; break;
						case RESIDENCY: return "Texture_Resident"; break;
						case GAMMA: return "Texture_Gamma";  break;
						case DO_NOT_RELEASE_DATA: return "Texture_Do Not Release_Data"; break;
						case FILTER_MIN_NEAREST: return "Texture_Filter_Min_Nearest"; break;
						case FILTER_MIN_LINEAR: return "Texture_Filter_Min_Linear"; break;
						case FILTER_MIN_NEAREST_MIPMAP: return "Texture_Filter_Min_Nearest_MipMap"; break;
						case FILTER_MIN_LINEAR_MIPMAP: return "Texture_Filter_Min_Linear_MipMap"; break;
						case FILTER_MIN_LINEAR_MIPMAP_LINEAR: return "Texture_Filter_Min_Linear_MipMap_Linear"; break;
						case FILTER_MAG_NEAREST: return "Texture_Filter_Mag_Nearest"; break;
						case FILTER_MAG_LINEAR: return "Texture_Filter_Mag_Linear"; break;
						case REPEAT_S: return "Texture_Repeat_S"; break;
						case REPEAT_T: return "Texture_Repeat_T"; break;
						case MIRRORED_S: return "Texture_Mirrored_S"; break;
						case MIRRORED_T: return "Texture_Mirrored_T"; break;
						case CLAMP_TO_EDGE_S: return "Texture_Clamp_To_Edge_S"; break;
						case CLAMP_TO_EDGE_T: return "Texture_Clamp_To_Edge_T"; break;
						case CLAMP_TO_BORDER_S: return "Texture_Clamp_To_Border_S"; break;
						case CLAMP_TO_BORDER_T: return "Texture_Clamp_To_Border_T"; break;
					}
					return "Unknown Flag";
				}

				// -----------------------------------------------------------------
				//	Binds texture to texture unit 

				void BindTexture(int TextureUnit = 0) const
				{
					glActiveTexture(GL_TEXTURE0 + TextureUnit);
					glBindTexture(GL_TEXTURE_2D, ID);
				}
								
				//----------------------------------------------------------------
				// dump info

				std::string Dump()
				{
					std::string text  = "FileName               : " + GetResourceFileName() + "\n";
								text += "Width                  : " + std::to_string(Width) + "\n";
								text += "Height                 : " + std::to_string(Height) + "\n";
					
					// texture type

					if (TextureType== GL_TEXTURE_2D) 
								text += "TextureType            : GL_TEXTURE_2D\n";
								text += "Id                     : " + std::to_string(ID) + "\n";
								text += "BPP                    : " + std::to_string(BPP) + "\n";

					// mipmaps

					if (GenerateMipMaps) 
								text += "MipMapsGenerated       : Yes\n";
					if (!GenerateMipMaps)
								text += "MipMapsGenerated       : No\n";
				
					// residency

					if (Resident)
								text += "Texture Resident Check : Yes\n";
					if (!Resident)
								text += "Texture Resident Check : No\n";

					// gamma 

					if (GammaCorrection)
								text += "Gamma Correction       : Yes\n";
					if (!GammaCorrection)
								text += "Gamma Correction       : No\n";
					
					// data release

					if (ReleaseTextureData)
								text += "Texture Data Released  : Yes\n";
					if (!ReleaseTextureData)
								text += "Texture Data Released  : No\n";

					// anisoptrpy

								text += "Anisotropy             : " + std::to_string(Anisotropy) + "\n";

					// repeat s

					if (RepeatS == GL_REPEAT)
								text += "RepeatS                : GL_REPEAT_S\n";
					if (RepeatS == GL_MIRRORED_REPEAT)
								text += "RepeatS                : GL_MIRRORED_REPEAT_S\n";
					if (RepeatS == GL_CLAMP_TO_EDGE)
								text += "RepeatS                : GL_CLAMP_TO_EDGE_S\n";
					if (RepeatS == GL_CLAMP_TO_BORDER)
								text += "RepeatS                : GL_CLAMP_TO_BORDER_S\n";

					// repeat t

					if (RepeatT == GL_REPEAT)
								text += "RepeatT                : GL_REPEAT_T\n";
					if (RepeatT == GL_MIRRORED_REPEAT)
								text += "RepeatT                : GL_MIRRORED_REPEAT_T\n";
					if (RepeatT == GL_CLAMP_TO_EDGE)
								text += "RepeatT                : GL_CLAMP_TO_EDGE_T\n";
					if (RepeatT == GL_CLAMP_TO_BORDER)
								text += "RepeatT                : GL_CLAMP_TO_BORDER_T\n";

					// minification

					if (Minification == GL_NEAREST)
								text += "Minification           : GL_NEAREST\nn";
					if (Minification == GL_LINEAR)
								text += "Minification           : GL_LINEAR\n";
					if (Minification == GL_NEAREST_MIPMAP_NEAREST )
								text += "Minification           : GL_NEAREST_MIPMAP_NEAREST\n";
					if (Minification == GL_LINEAR_MIPMAP_NEAREST)
								text += "Minification           : GL_LINEAR_MIPMAP_NEAREST\n";
					if (Minification == GL_LINEAR_MIPMAP_LINEAR)
								text += "Minification           : GL_LINEAR_MIPMAP_LINEAR\n";
					
					// magnification

					if (Magnification == GL_NEAREST)
								text += "Magnification          : GL_NEAREST\n";
					if (Magnification == GL_LINEAR)
								text += "Magnification          : GL_LINEAR\n";
					
					// format

					if (Format == GL_RED)		 
						text += "Format                 : GL_RED\n";
					if (Format == GL_RGB)		 
						text += "Format                 : GL_RGB\n";
					if (Format == GL_SRGB)		 
						text += "Format                 : GL_SRGB\n";
					if (Format == GL_RGBA)		 
						text += "Format                 : GL_RGBA\n";
					if (Format == GL_SRGB_ALPHA) 
						text += "Format                 : GL_SRGB_ALPHA\n";
					if (DataFormat == GL_RED)
						text += "DataFormat             : GL_RED\n";
					if (DataFormat == GL_RGB)
						text += "DataFormat             : GL_RGB\n";
					if (DataFormat == GL_RGBA)
						text += "DataFormat             : GL_RGBA\n";

					// instances

					text += "Instances              : " + std::to_string(GetInstancesCount()) + "\n";

					return text;
				}
				
				// -----------------------------------------------------------
				// query functions

				[[nodiscard]] constexpr GLuint		 GetMinificationFilter()  const { return Minification; }
				[[nodiscard]] constexpr GLuint		 GetMagnificationFilter() const { return Magnification; }
				[[nodiscard]] constexpr unsigned int GetWidth()               const { return Width; }
				[[nodiscard]] constexpr unsigned int GetHeight()              const { return Height; }
				[[nodiscard]] constexpr unsigned int GetBPP()                 const { return BPP; }
				[[nodiscard]] constexpr GLuint		 GetID()                  const { return ID; }
				[[nodiscard]] constexpr GLenum		 GetFormat()              const { return Format; }
				[[nodiscard]] constexpr GLfloat		 GetAnisotropy()          const { return Anisotropy; }
				[[nodiscard]] constexpr float		 GetAspectRatio()         const { return Ratio; }
				[[nodiscard]] constexpr unsigned int GetSize()                const { return Width*Height; }
				[[nodiscard]] constexpr GLenum		 GetType()                const { return TextureType;  }
				[[nodiscard]] constexpr GLuint		 GetRepeatS()             const { return RepeatS; }
				[[nodiscard]] constexpr GLuint		 GetRepeatT()             const { return RepeatT; }
				[[nodiscard]] const unsigned char*	 GetData()                const { return Data; }
				[[nodiscard]] constexpr bool	     IsValid()                const { return ( Data != nullptr) && (Width!=0) && (Height!=0); }
				[[nodiscard]] constexpr bool		 AreMipMapsGenerated()    const { return GenerateMipMaps; }
				[[nodiscard]] constexpr bool	     IsResident()             const { return Resident; }
				[[nodiscard]] constexpr bool	     IsDataReleased()         const { return ReleaseTextureData; }
				[[nodiscard]] constexpr bool		 IsGammaCorrected()		  const { return GammaCorrection; }

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Texture(const Texture& other) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				Texture& operator=(const Texture& other) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				Texture(Texture&& other) = delete;
				Texture& operator=(Texture&& other) = delete;

				// ---------------------------------------------------------
				// ctor / dtor

				Texture( const std::string &resourcefilename, const vml::utils::Flags& flags) : vml::utils::SharedResource(resourcefilename,flags)
				{
					TextureType			    = GL_TEXTURE_2D;
					Width				    = 0;
					Height				    = 0;	
					ID					    = 0;	
					BPP					    = 0;		
					Format				    = 0;
					DataFormat			    = 0;
					Anisotropy			    = 0;
					MaxAnisotropy		    = 8.0f;
					Data				    = nullptr;						// image buffer

					RepeatS				    = GL_REPEAT;					// repeats;
					RepeatT				    = GL_REPEAT;					// repeatt;
					Magnification		    = GL_LINEAR;					// magnification;
					Minification		    = GL_LINEAR;					// minification;
					GenerateMipMaps		    = true;							// generate mipmaps
					GammaCorrection		    = false;						// gammacorrection;
					ReleaseTextureData		= true;							// preservedata;
					Resident			    = false;						// resident;

					// boolean flags to enable requested features

					if (flags.Get(MIPMAPS))				GenerateMipMaps    = true;
					if (flags.Get(RESIDENCY))			Resident		   = true;
					if (flags.Get(GAMMA))				GammaCorrection    = true;
					if (flags.Get(DO_NOT_RELEASE_DATA)) ReleaseTextureData = false;
					
					// GL_REPEAT:			The integer part of the coordinate will be ignored and a repeating pattern is formed.
					// GL_MIRRORED_REPEAT : The texture will also be repeated, but it will be mirrored when the integer 
					//						part of the coordinate is odd.
					// GL_CLAMP_TO_EDGE :   The coordinate will simply be clamped between 0 and 1.
					// GL_CLAMP_TO_BORDER : The

					if (flags.Get(REPEAT_S))          RepeatS = GL_REPEAT;
					if (flags.Get(MIRRORED_S))        RepeatS = GL_MIRRORED_REPEAT;
					if (flags.Get(CLAMP_TO_EDGE_S))   RepeatS = GL_CLAMP_TO_EDGE;
					if (flags.Get(CLAMP_TO_BORDER_S)) RepeatS = GL_CLAMP_TO_BORDER;

					if (flags.Get(REPEAT_T))          RepeatT = GL_REPEAT;
					if (flags.Get(MIRRORED_T))        RepeatT = GL_MIRRORED_REPEAT;
					if (flags.Get(CLAMP_TO_EDGE_T))   RepeatT = GL_CLAMP_TO_EDGE;
					if (flags.Get(CLAMP_TO_BORDER_T)) RepeatT = GL_CLAMP_TO_BORDER;

					// texture filtering flags
					// OpenGL has two parameters that can be set :
					//
					// GL_TEXTURE_MIN_FILTER
					// GL_TEXTURE_MAG_FILTER
					//
					// GL_TEXTURE_MIN_FILTER accepts the following options :
					//
					//	GL_NEAREST
					//	GL_LINEAR
					//	GL_NEAREST_MIPMAP_NEAREST
					//	GL_NEAREST_MIPMAP_LINEAR
					//	GL_LINEAR_MIPMAP_NEAREST
					//	GL_LINEAR_MIPMAP_LINEAR
					//
					// GL_TEXTURE_MAG_FILTER accepts the following options :
					//
					// GL_NEAREST
					// GL_LINEAR

					if (flags.Get(FILTER_MIN_NEAREST))              Minification  = GL_NEAREST;
					if (flags.Get(FILTER_MIN_LINEAR))               Minification  = GL_LINEAR;
					if (flags.Get(FILTER_MIN_NEAREST_MIPMAP))       Minification  = GL_NEAREST_MIPMAP_NEAREST;
					if (flags.Get(FILTER_MIN_LINEAR_MIPMAP))        Minification  = GL_LINEAR_MIPMAP_NEAREST;
					if (flags.Get(FILTER_MIN_LINEAR_MIPMAP_LINEAR)) Minification  = GL_LINEAR_MIPMAP_LINEAR;
					if (flags.Get(FILTER_MAG_NEAREST))				Magnification = GL_NEAREST;
					if (flags.Get(FILTER_MAG_LINEAR))				Magnification = GL_LINEAR;

					// create opengl texture
					CreateTexture2dFromData();

					//	std::cout << Dump() << std::endl;

				}

				~Texture()
				{
					if (ID) glDeleteTextures(1, &ID);
					vml::os::SafeDelete(Data);
				}
		};
	
	}	// end of image namespace

} // end of vml namespace

