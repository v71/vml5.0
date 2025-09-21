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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vml5.0/opengl/debugrendering/debugdirectionallight.h>
#include <vml5.0/opengl/debugrendering/debugmaterial.h>
#include <vml5.0/opengl/debugrendering/rgbutils.h>
#include <vml5.0/opengl/debugrendering/checkeredplane.h>
#include <vml5.0/opengl/debugrendering/cube.h>
#include <vml5.0/opengl/debugrendering/singlepoint.h>
#include <vml5.0/opengl/debugrendering/segment.h>
#include <vml5.0/opengl/debugrendering/quads.h>
#include <vml5.0/opengl/debugrendering/triangles.h>
#include <vml5.0/opengl/debugrendering/billboard.h>
#include <vml5.0/opengl/debugrendering/sphere.h>
#include <vml5.0/opengl/debugrendering/refsystem.h>

///////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
		/////////////////////////////////////////////////
		// debug rendering class
	
		class OpenglDebugRender
		{

			private:

				// debug textures
				vml::textures::Texture* DebugTexture;
				vml::textures::Texture* DirectionalLightTexture;
	
				// colors
				glm::vec4 ClearColor;								// background color
				glm::vec4 WireFrameColor;							// color for wireframe rendering

				// internal flags
				bool Initialized;

			public:

				// ---------------------------------------------------------------------
				// meshes
				
				vml::debugrendering::SinglePoint*			 SinglePointMesh;
				vml::debugrendering::Segment*				 SegmentMesh;
				vml::debugrendering::Quad*					 QuadMesh;
				vml::debugrendering::QuadPerVertexColor*	 QuadPerVertexColorMesh;
				vml::debugrendering::Triangle*				 TriangleMesh;
				vml::debugrendering::TrianglePerVertexColor* TrianglePerVertexColorMesh;
				vml::debugrendering::TexturedTriangle*		 TriangleTexturedMesh;
				vml::debugrendering::TexturedQuad*			 QuadTexturedMesh;
		
				vml::debugrendering::BillBoard*				 BillBoardMesh;
				vml::debugrendering::TexturedCube*			 TexturedCubeMesh;
				
				vml::debugrendering::SolidCube*				 SolidCubeMesh;
				vml::debugrendering::WireCube*				 WireCubeMesh;
				vml::debugrendering::CheckeredPlane*		 CheckeredPlaneMesh;
				vml::debugrendering::Sphere*				 WireSphereMesh;
				vml::debugrendering::RefSystem*				 RefSystem;
				
				// debug shader

				vml::shaders::GlShaderProgram* PhongShader;
				vml::shaders::GlShaderProgram* SingleColorShader;
				vml::shaders::GlShaderProgram* TextureShader;
				vml::shaders::GlShaderProgram* TexturePhongShader;

				vml::shaders::GlShaderProgram* TextureShaderAlpha;
			//	vml::shaders::GlShaderProgram* TextureShaderAlphaColor;
				vml::debugrendering::AlphaTexturedQuad* AlphaTexturedQuadMesh;
		//		vml::debugrendering::AlphaColorTexturedQuad* AlphaTexturedColorQuadMesh;

				// debug directional light shader locations

				GLint PhongLightAmbientLocation;
				GLint PhongLightDiffuseLocation;
				GLint PhongLightSpecularLocation;
				GLint PhongLightDirectionLocation;
				GLint PhongLightPowerLocation;
				GLint PhongLightCameraSpaceLocation;
				
				// debug directiona light debug material 

				GLint PhongMaterialAmbientLocation;
				GLint PhongMaterialDiffuseLocation;
				GLint PhongMaterialSpecularLocation;
				GLint PhongMaterialShininessLocation;
				
				// debug directional light textured shader location

				GLint TexturePhongMaterialAmbientLocation;
				GLint TexturePhongMaterialDiffuseLocation;
				GLint TexturePhongMaterialSpecularLocation;
				GLint TexturePhongMaterialShininessLocation;

				GLint TexturePhongLightAmbientLocation;
				GLint TexturePhongLightDiffuseLocation;
				GLint TexturePhongLightSpecularLocation;
				GLint TexturePhongLightDirectionLocation;
				GLint TexturePhongLightPowerLocation;
				GLint TexturePhongLightCameraSpaceLocation;
				
				// color location for debug rendering

				GLint ColorLocation;
				
				// texture sampler location for textured shader
				
				GLint TextureSamplerLocation;
				GLint TexturePhongSamplerLocation;
			
				// paths

				const std::string FullDebugPath				  = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
				const std::string TextureStorePath			  = FullDebugPath + "/textures/";
				const std::string ShadersStorePath			  = FullDebugPath + "/shaders/";
				const std::string PhongShaderFilename		  = ShadersStorePath + "debug_phong_dir.shd";
				const std::string SingleColorShaderFilename   = ShadersStorePath + "debug_single_color.shd";
				const std::string TextureShaderFilename	 	  = ShadersStorePath + "debug_texture.shd";
				const std::string TexturePhongShaderFilename  = ShadersStorePath + "debug_texture_phong_dir.shd";
				const std::string TextureShaderAlphaFileName  = ShadersStorePath + "debug_texture_alpha.shd";

				// -----------------------------------------------------------------------------------
				// closes debug renderer and release memory

				void Close()
				{
					if (!Initialized)
						vml::os::Message::Error("OpenGLDebugRenderer : Not initializing");

					// delete meshes

					vml::os::SafeDelete(SinglePointMesh);
					vml::os::SafeDelete(SegmentMesh);
					vml::os::SafeDelete(TrianglePerVertexColorMesh);
					vml::os::SafeDelete(TriangleMesh);
					vml::os::SafeDelete(QuadMesh);
					vml::os::SafeDelete(QuadPerVertexColorMesh);
					vml::os::SafeDelete(CheckeredPlaneMesh);
					
					vml::os::SafeDelete(AlphaTexturedQuadMesh);
				//	vml::os::SafeDelete(AlphaTexturedColorQuadMesh);
					
					vml::os::SafeDelete(BillBoardMesh);
					vml::os::SafeDelete(WireCubeMesh);
					vml::os::SafeDelete(SolidCubeMesh);
					vml::os::SafeDelete(TexturedCubeMesh);
					
					vml::os::SafeDelete(QuadTexturedMesh);
					vml::os::SafeDelete(TriangleTexturedMesh);
					vml::os::SafeDelete(WireSphereMesh);
					vml::os::SafeDelete(RefSystem);
					
					// delete texture
				
					vml::os::SafeDelete(DebugTexture);
					vml::os::SafeDelete(DirectionalLightTexture);

					// remove shaders

					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Unloading Shaders" });

					vml::stores::ShaderStore->UnLoad(SingleColorShaderFilename);
					vml::stores::ShaderStore->UnLoad(PhongShaderFilename);
					vml::stores::ShaderStore->UnLoad(TextureShaderFilename);
					vml::stores::ShaderStore->UnLoad(TexturePhongShaderFilename);

					// logging out

					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer","Shutting Down" });
				}
				
			public:
				
				// -----------------------------------------------------------------------------------
				// 

				static constexpr unsigned int NO_DRAW_POINTS	 = 0;
				static constexpr unsigned int DRAW_POINTS		 = 1;

				static constexpr unsigned int DO_NOT_DRAW_AABBOX = 0;
				static constexpr unsigned int DRAW_AABBOX		 = 1;
				
				// -----------------------------------------------------------------------------------
				// set clear color

				void SetClearColor(const glm::vec4& color)
				{
					ClearColor = color;
					glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.w);
				}

				// -----------------------------------------------------------------------------------
				// set wireframe color

				void SetWireFrameColor(const glm::vec4& color)
				{
					WireFrameColor = color;
				}

				// -----------------------------------------------------------------------------------
				// directional light shader and material

				vml::debugrendering::DebugDirectionalLight DirectionalLight;
				
				// -----------------------------------------------------------------------------------

				GLuint GetDirectionalLightTextureID() const { return DirectionalLightTexture->GetID(); }
				GLuint GetDebugTextureID() const { return DebugTexture->GetID(); }
				const glm::vec4& GetClearColor() const { return ClearColor; }
				const glm::vec4& GetWireFrameColor() const { return WireFrameColor; }

				// -----------------------------------------------------------------------------------
				// point drawing

				void DrawPoint(vml::views::View* view,
							   const glm::vec3& p,
							   const glm::vec4& col) const
				{
					if (!view) 
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for point debug rendering");
					SinglePointMesh->Draw(view,p, col);
				}
				
				// -----------------------------------------------------------------------------------
				// line drawing

				void DrawLine(vml::views::View* view,
							  const glm::vec3& p,
							  const glm::vec3& q,
							  const glm::vec4& col, 
							  const int drawpoints) const
				{
					if (view) 
					{
						SegmentMesh->Draw(view, p, q, col);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p, vml::colors::Red);
							DrawPoint(view, q, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for point debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				// alpha textured quad drawing 
				
				void DrawAlphaTexturedQuad(vml::views::View* view, 
					                       const float scale, const glm::vec3& pos,
					                       const GLuint texid,const float alpha, const int blendmode) const
				{
					if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
					AlphaTexturedQuadMesh->Draw(view, scale, pos, texid,alpha,blendmode);
				}
				
				void DrawAlphaTexturedQuad(vml::views::View* view, 
					                       const glm::vec3 &p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, 
					                       const GLuint texid, const float alpha, const int blendmode) const
				{
					if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
					AlphaTexturedQuadMesh->Draw(view, p0,p1,p2,p3, texid, alpha, blendmode);
				}
				/*
				void DrawAlphaColorTexturedQuad(vml::views::View* view, 
					                            const float scale, const glm::vec3& pos, const glm::vec3 &col,
					                            const GLuint texid, const float alpha, const int blendmode) const
				{
					if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
					AlphaTexturedColorQuadMesh->Draw(view, scale, pos, col,texid, alpha, blendmode);
				}
				
				void DrawAlphaColorTexturedQuad(vml::views::View* view, 
					                            const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, 
												const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3,
												const GLuint texid, const float alpha, const int blendmode) const
				{
					if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
					AlphaTexturedColorQuadMesh->Draw(view, p0, p1, p2, p3, c0,c1,c2,c3,texid, alpha, blendmode);
				}
				*/
				// -----------------------------------------------------------------------------------
				// billboards

				void DrawBillBoard(vml::views::View* view,
								   const float scale, const glm::vec3& pos,
								   const GLuint texid) const
				{
					if (view)
						BillBoardMesh->Draw(view, scale, pos, texid);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
				}
				
				// -----------------------------------------------------------------------------------
				// triangle drawing

				void DrawWireTriangle(vml::views::View* view,
									  const float scale, const glm::vec3& pos,
									  const glm::vec4& col,
									  const int drawpoints) const
				{
					if (view) 
					{
						TriangleMesh->Draw(view, scale, pos, col, 0);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, pos + glm::vec3(0, scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(scale, -scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(-scale, -scale, 0), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// triangle drawing

				void DrawWireTriangle(vml::views::View* view,
									  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
									  const glm::vec4& col,
									  const int drawpoints) const
				{
					if (view) 
					{
						TriangleMesh->Draw(view, p0, p1, p2, col, 0);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
						}
						else
						{
							vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
						}
					}
				}

				// -----------------------------------------------------------------------------------
				// textured quad drawing 

				void DrawTexturedTriangle(vml::views::View* view,
										  const float scale, const glm::vec3& pos,
										  const GLuint texid) const
				{
					if (view)
					{
						TriangleTexturedMesh->Draw(view, scale, pos, texid);
						glm::vec3 p0(pos.x - scale, pos.y + scale, pos.z);
						glm::vec3 p1(pos.x + scale, pos.y + scale, pos.z);
						glm::vec3 p2(pos.x + scale, pos.y - scale, pos.z);
						DrawPoint(view, p0, vml::colors::Red);
						DrawPoint(view, p1, vml::colors::Red);
						DrawPoint(view, p2, vml::colors::Red);
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawTexturedTriangle(vml::views::View* view,
										  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
										  const GLuint texid,
										  const int drawpoints) const
				{
					if (view) 
					{
						TriangleTexturedMesh->Draw(view, p0, p1, p2, glm::ivec2(0, 0), glm::ivec2(1, 0), glm::ivec2(1, 1), texid);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// textured triangle drawing

				void DrawTexturedTriangle(vml::views::View* view,
										  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
										  const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
										  const GLuint texid,
										  const int drawpoints) const
				{
					if (view)
					{
						TriangleTexturedMesh->Draw(view, p0, p1, p2, uv0, uv1, uv2, texid);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				// triangle drawing

				void DrawSolidTriangle(vml::views::View* view,
									   const float scale, const glm::vec3& pos,
									   const glm::vec4& col,
									   const int drawpoints) const
				{
					if (view)
					{
						TriangleMesh->Draw(view, scale, pos, col, 1);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, pos + glm::vec3(0, scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(scale, -scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(-scale, -scale, 0), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// triangle drawing

				void DrawSolidTriangle(vml::views::View* view,
									   const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
									   const glm::vec4& col,
									   const int drawpoints) const
				{
					if (view)
					{
						TriangleMesh->Draw(view, p0, p1, p2, col, 1);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawColoredTriangle(vml::views::View* view,
										 const float scale, const glm::vec3& pos,
										 const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2,
										 const int drawpoints) const
				{
					if (view)
					{
						TrianglePerVertexColorMesh->Draw(view, scale, pos, c0, c1, c2);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, pos + glm::vec3(0, scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(scale, -scale, 0), vml::colors::Red);
							DrawPoint(view, pos + glm::vec3(-scale, -scale, 0), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawColoredTriangle(vml::views::View* view,
										 const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
										 const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2,
										 const int drawpoints) const
				{
					if (view)
					{
						TrianglePerVertexColorMesh->Draw(view, p0, p1, p2, c0, c1, c2);
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				// textured quad drawing 

				void DrawTexturedQuad(vml::views::View* view,
									  const float scale, const glm::vec3& pos,
									  const GLuint texid) const
				{
					if (view)
						QuadTexturedMesh->Draw(view, scale, pos, texid);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
				}

				// -----------------------------------------------------------------------------------
				// textured quad drawing 

				void DrawTexturedQuad(vml::views::View* view,
									  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
									  const GLuint texid) const
				{
					if (view)
						QuadTexturedMesh->Draw(view, p0, p1, p2, p3, texid);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
				}

				// -----------------------------------------------------------------------------------
				// single color quad drawing

				void DrawWireQuad(vml::views::View* view,
								  const float scale, const glm::vec3& pos,
								  const glm::vec4& col,
								  const int drawpoints) const
				{
					if (view)
					{
						QuadMesh->Draw(view, scale, pos, col, 0);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y + scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y + scale, pos.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for quad debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// single color quad drawing

				void DrawSolidQuad(vml::views::View* view,
								   const float scale, const glm::vec3& pos,
								   const glm::vec4& col,
								   const int drawpoints) const

				{
					if (view)
					{
						QuadMesh->Draw(view, scale, pos, col, 1);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y + scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y + scale, pos.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// single color wire quad drawing using user coordinates

				void DrawWireQuad(vml::views::View* view,
								  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
								  const glm::vec4& col,
								  const int drawpoints) const
				{
					if (view)
					{
						QuadMesh->Draw(view, p0, p1, p2, p3, col, 0);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
							DrawPoint(view, p3, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// single color solid quad drawing using user coordinates

				void DrawSolidQuad(vml::views::View* view,
								   const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
								   const glm::vec4& col,
								   const int drawpoints) const
				{
					if (view) 
					{
						QuadMesh->Draw(view, p0, p1, p2, p3, col, 1);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
							DrawPoint(view, p3, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for triangle debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				// per vertex color quad drawing

				void DrawColoredQuad(vml::views::View* view,
									 const float scale, const glm::vec3& pos,
									 const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3,
									 const int drawpoints) const
				{
					if (view)
					{
						QuadPerVertexColorMesh->Draw(view, scale, pos, c0, c1, c2, c3);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y - scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x + scale, pos.y + scale, pos.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(pos.x - scale, pos.y + scale, pos.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// per vertex color quad drawing using user coordinates

				void DrawColoredQuad(vml::views::View* view,
									 const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
									 const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3,
									 const int drawpoints) const
				{
					if (view)
					{
						QuadPerVertexColorMesh->Draw(view, p0, p1, p2, p3, c0, c1, c2, c3);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, p0, vml::colors::Red);
							DrawPoint(view, p1, vml::colors::Red);
							DrawPoint(view, p2, vml::colors::Red);
							DrawPoint(view, p3, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				// draw an ortho texture

				void DrawOrtho(vml::views::View* view,
							   const glm::vec2& scale, const glm::vec2& pos,
							   const GLuint texid) const
				{
					if (view)
						BillBoardMesh->DrawOrtho(view, scale, pos, texid);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for triangle debug rendering");
				}

				// -----------------------------------------------------------------------------------
				// axis aligned bounding box drawing

				void DrawAABBox(vml::views::View* view,
								const glm::vec3& bmin, const glm::vec3& bmax,
								const glm::vec4& col,
								const int drawpoints) const
				{
					if (view)
					{
						WireCubeMesh->Draw(view, bmin, bmax, col);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmax.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for bounding box debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				//
				
				void DrawAABBox(vml::views::View* view,
								const vml::geo3d::AABBox& aabbox,
								const glm::vec4& col,
								const int drawpoints) const
				{
					if (view) 
					{
						glm::vec3 bmin = aabbox.GetMin();
						glm::vec3 bmax = aabbox.GetMax();
						WireCubeMesh->Draw(view, bmin, bmax, col);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmax.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for bounding box debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawAOBBox(vml::views::View* view,
								const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3,
								const glm::vec3& b4, const glm::vec3& b5, const glm::vec3& b6, const glm::vec3& b7,
								const glm::vec4& col,
								const bool drawpoints) const
				{
					if (view) 
					{
						WireCubeMesh->Draw(view, b0, b1, b2, b3, b4, b5, b6, b7, col);
						
						if (drawpoints == DRAW_POINTS)
						{
							DrawPoint(view, b0, vml::colors::Red);
							DrawPoint(view, b1, vml::colors::Red);
							DrawPoint(view, b2, vml::colors::Red);
							DrawPoint(view, b3, vml::colors::Red);
							DrawPoint(view, b4, vml::colors::Red);
							DrawPoint(view, b5, vml::colors::Red);
							DrawPoint(view, b6, vml::colors::Red);
							DrawPoint(view, b7, vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for bounding box debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				//

				void DrawAABBox(vml::views::View* view,
								vml::models::Model3d_2* model,
								const glm::vec4& col,
								const int drawpoints) const
				{
					if (view)
					{
						if (model)
						{
							glm::vec3 bmin(-FLT_MAX, -FLT_MAX, -FLT_MAX);
							glm::vec3 bmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
							if (model) bmax = model->GetAABoundingBox().GetMax();
							if (model) bmin = model->GetAABoundingBox().GetMin();
						
							WireCubeMesh->Draw(view, bmin, bmax, col);
							
							if (drawpoints == DRAW_POINTS)
							{
								DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmin.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmin.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmin.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmin.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmax.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmax.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmax.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmax.z), vml::colors::Red);
							}
						}
						else
						{
							vml::os::Message::Error("DebugRender : Null Model for bounding box  box debug rendering");
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for bounding box debug rendering");
					}
				}
	
				// -----------------------------------------------------------------------------------
				//

				void DrawAABBox(vml::views::View* view,
								vml::models::Model3d_2* model,
								const glm::vec4& col,
								const int drawpoints, const float f) const
				{
					if (view) 
					{
						if (model)
						{
							glm::vec3 a(-FLT_MAX, -FLT_MAX, -FLT_MAX);
							glm::vec3 b(-FLT_MAX, -FLT_MAX, -FLT_MAX);
							if (model)a = model->GetAABoundingBox().GetMin();
							if (model)b = model->GetAABoundingBox().GetMax();
							glm::vec3 c = (a + b) * 0.5f;
							glm::vec3 u = glm::normalize(a - c);
							glm::vec3 v = glm::normalize(b - c);
							a += u * f;
							b += v * f;
							
							WireCubeMesh->Draw(view, a, b, col);
							
							if (drawpoints == DRAW_POINTS)
							{
								DrawPoint(view, glm::vec3(a.x, a.y, a.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(b.x, a.y, a.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(b.x, b.y, a.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(a.x, b.y, a.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(a.x, a.y, b.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(b.x, a.y, b.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(b.x, b.y, b.z), vml::colors::Red);
								DrawPoint(view, glm::vec3(a.x, b.y, b.z), vml::colors::Red);
							}
						}
						else
						{
							vml::os::Message::Error("DebugRender : Null Model for bounding box  box debug rendering");
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for bounding box debug rendering");
					}
				}

				
				// -----------------------------------------------------------------------------------
				// oriented bounding box drawing

				void DrawAOBBox(vml::views::View* view,
								vml::models::Model3d_2* model,
								const glm::vec4& col,
								const bool drawpoints) const
				{
					if (view) 
					{

						if (model)
						{
							glm::vec3 boundingbox[8] = { glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX) };

							if (model) boundingbox[0] = model->GetAOBoundingBox()[0];
							if (model) boundingbox[1] = model->GetAOBoundingBox()[1];
							if (model) boundingbox[2] = model->GetAOBoundingBox()[2];
							if (model) boundingbox[3] = model->GetAOBoundingBox()[3];
							if (model) boundingbox[4] = model->GetAOBoundingBox()[4];
							if (model) boundingbox[5] = model->GetAOBoundingBox()[5];
							if (model) boundingbox[6] = model->GetAOBoundingBox()[6];
							if (model) boundingbox[7] = model->GetAOBoundingBox()[7];

							WireCubeMesh->Draw(view,
											   boundingbox[0], boundingbox[1], boundingbox[2], boundingbox[3],
											   boundingbox[4], boundingbox[5], boundingbox[7], boundingbox[6],
											   col);

							if (drawpoints == DRAW_POINTS)
							{
								DrawPoint(view, boundingbox[0], vml::colors::Red);
								DrawPoint(view, boundingbox[1], vml::colors::Red);
								DrawPoint(view, boundingbox[2], vml::colors::Red);
								DrawPoint(view, boundingbox[3], vml::colors::Red);
								DrawPoint(view, boundingbox[4], vml::colors::Red);
								DrawPoint(view, boundingbox[5], vml::colors::Red);
								DrawPoint(view, boundingbox[6], vml::colors::Red);
								DrawPoint(view, boundingbox[7], vml::colors::Red);
							}
						}
						else
						{
							vml::os::Message::Error("DebugRender : Null Model for bounding box  box debug rendering");
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null mview atrix for oriented bounding box debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// oriented bounding box drawing

				void DrawAOBBox(vml::views::View* view,
								vml::models::Model3d_2* model,
								const glm::vec4& col,
								const bool drawpoints, const float f) const
				{
					if(view)
					{
						if (model)
						{
							glm::vec3 boundingbox[8] = { glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX) };

							if (model) boundingbox[0] = model->GetAOBoundingBox()[0];
							if (model) boundingbox[1] = model->GetAOBoundingBox()[1];
							if (model) boundingbox[2] = model->GetAOBoundingBox()[2];
							if (model) boundingbox[3] = model->GetAOBoundingBox()[3];
							if (model) boundingbox[4] = model->GetAOBoundingBox()[4];
							if (model) boundingbox[5] = model->GetAOBoundingBox()[5];
							if (model) boundingbox[6] = model->GetAOBoundingBox()[6];
							if (model) boundingbox[7] = model->GetAOBoundingBox()[7];

							glm::vec3 c = (boundingbox[0] + boundingbox[1] + boundingbox[2] + boundingbox[3] +
										   boundingbox[4] + boundingbox[5] + boundingbox[6] + boundingbox[7]) / 8.0f;

							glm::vec3 p[8] = { boundingbox[0],boundingbox[1],boundingbox[2],boundingbox[3],boundingbox[4],boundingbox[5],boundingbox[6],boundingbox[7] };
							glm::vec3 d[8] = { boundingbox[0] - c,boundingbox[1] - c,boundingbox[2] - c,boundingbox[3] - c,boundingbox[4] - c,boundingbox[5] - c,boundingbox[6] - c,boundingbox[7] - c };

							d[0] = glm::normalize(d[0]);
							d[1] = glm::normalize(d[1]);
							d[2] = glm::normalize(d[2]);
							d[3] = glm::normalize(d[3]);
							d[4] = glm::normalize(d[4]);
							d[5] = glm::normalize(d[5]);
							d[6] = glm::normalize(d[6]);
							d[7] = glm::normalize(d[7]);

							p[0] += d[0] * f;
							p[1] += d[1] * f;
							p[2] += d[2] * f;
							p[3] += d[3] * f;
							p[4] += d[4] * f;
							p[5] += d[5] * f;
							p[6] += d[6] * f;
							p[7] += d[7] * f;

							WireCubeMesh->Draw(view, p[0], p[1], p[2], p[3], p[4], p[5], p[7], p[6], col);

							if (drawpoints == DRAW_POINTS)
							{
								DrawPoint(view, p[0], vml::colors::Red);
								DrawPoint(view, p[1], vml::colors::Red);
								DrawPoint(view, p[2], vml::colors::Red);
								DrawPoint(view, p[3], vml::colors::Red);
								DrawPoint(view, p[4], vml::colors::Red);
								DrawPoint(view, p[5], vml::colors::Red);
								DrawPoint(view, p[6], vml::colors::Red);
								DrawPoint(view, p[7], vml::colors::Red);
							}
						}
						else
						{
							vml::os::Message::Error("DebugRender : Null Model for bounding box  box debug rendering");
						}
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null mview atrix for oriented bounding box debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				// draw bounding sphere for model's object

				void DrawSphere(vml::views::View* view,
								const glm::vec3& pos,
								const float radius,
								const glm::vec4& col) const
				{
					if (view)
						WireSphereMesh->Draw(view, pos, radius, col);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for sphere rendering");
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawSphere(vml::views::View* view,
								const glm::mat4& m,
								const glm::vec4& col) const
				{
					if (view)
						WireSphereMesh->Draw(view, m, col);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for sphere rendering");
				}

				// -----------------------------------------------------------------------------------
				// draw bounding sphere for model's object

				void DrawSphere(vml::views::View* view,
								vml::models::Model3d_2* model,
								const glm::vec4& col) const
				{
					if (view) 
					{
						if (model)
							WireSphereMesh->Draw(view, model->GetTransformedPosition(), model->GetRadius(), col);
						else
							vml::os::Message::Error("DebugRender : Null model pointer for bounding sphere rendering");
					}
					else
						vml::os::Message::Error("DebugRender : Null view matrix for sphere rendering");
				}

				// -----------------------------------------------------------------------------------
				//

				void DrawSolidAABBox(vml::views::View* view,
								   const glm::vec3& bmin, const glm::vec3& bmax,
								   const int drawpoints = NO_DRAW_POINTS) 
				{
					if(view)
					{
						SolidCubeMesh->Draw(view, bmin, bmax, DirectionalLight, vml::debugrendering::Material0);
						
						if (drawpoints)
						{
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmax.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view  matrix for bounding box debug rendering");
					}
				}
				
				// -----------------------------------------------------------------------------------
				//

				void DrawSolidAABBox(vml::views::View* view,
									 const vml::geo3d::AABBox& aabbox,
									 const int drawpoints = NO_DRAW_POINTS)
				{
					if(view)
					{
						SolidCubeMesh->Draw(view, aabbox.GetMin(), aabbox.GetMax(), DirectionalLight, vml::debugrendering::Material0);
						
						if (drawpoints)
						{
							glm::vec3 bmin(aabbox.GetMin().x, aabbox.GetMin().y, aabbox.GetMin().z);
							glm::vec3 bmax(aabbox.GetMax().x, aabbox.GetMax().y, aabbox.GetMax().z);
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmin.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmin.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmax.x, bmax.y, bmax.z), vml::colors::Red);
							DrawPoint(view, glm::vec3(bmin.x, bmax.y, bmax.z), vml::colors::Red);
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view  matrix for bounding box debug rendering");
					}
				}

				// -----------------------------------------------------------------------------------
				//
				
				void DrawTexturedCube(vml::views::View* view,
									  const glm::vec3& bmin, const glm::vec3& bmax,
									  const GLuint texid)
				{
					if (view)
						TexturedCubeMesh->Draw(view, bmin, bmax, texid);
					else
						vml::os::Message::Error("DebugRender : Null view matrix for textured cube debug rendering");
				}
				
				// -----------------------------------------------------------------------------------
				//

				void DrawTexturedCube(vml::views::View* view,
									  glm::mat4& m,
							 		  const GLuint texid)
				{
					if (view)
						TexturedCubeMesh->Draw(view, m, texid);
					else
						vml::os::Message::Error("DebugRender : Null view  matrix for textured cube rendering");
				}

				// -----------------------------------------------------------------------------------
				// draw model

				void DrawDebugModelSolid(vml::views::View* view, 
										 vml::models::Model3d_2* model,
										 const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0) 
				{
					if (view)
					{
						if (model)
						{
							if (!model->IsVisbile())
								return;

							//		DrawAABBox(view, model, vml::colors::Yellow, true);
							//		DrawAOBBox(view, model, vml::colors::Blue, true);

							// get shader

							glUseProgram(PhongShader->GetID());

							// set shader locations

							glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
							glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
							glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, model->GetMptr());
							glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, model->GetNVptr());
							glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, model->GetMVptr());
							glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, model->GetMVPptr());

							glUniform4fv(PhongMaterialAmbientLocation, 1, &material.Ambient[0]);
							glUniform4fv(PhongMaterialDiffuseLocation, 1, &material.Diffuse[0]);
							glUniform4fv(PhongMaterialSpecularLocation, 1, &material.Specular[0]);
							glUniform1f(PhongMaterialShininessLocation, material.Shininess);

							DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

							glUniform4fv(PhongLightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
							glUniform4fv(PhongLightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
							glUniform4fv(PhongLightSpecularLocation, 1, &DirectionalLight.Specular[0]);
							glUniform4fv(PhongLightDirectionLocation, 1, &DirectionalLight.Direction[0]);
							glUniform4fv(PhongLightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
							glUniform1f(PhongLightPowerLocation, DirectionalLight.Power);

							// draw mesh

							glEnable(GL_CULL_FACE);
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

							glBindVertexArray(model->GetCurrentMesh()->GetVAOId());

							glDrawElements(
								GL_TRIANGLES,								// mode
								model->GetCurrentMesh()->GetIndicesCount(),	// count
								GL_UNSIGNED_INT,							// type
								(void*)0									// element array buffer offset
							);

							glBindVertexArray(0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("OpenGLDebugRenderer : Null model pointer");
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view  matrix for bounding box debug rendering");
					}
				}


				// -----------------------------------------------------------------------------------
				// draw rendered models

				void DrawRenderedModel(vml::views::View* view, vml::models::ModelHandler* modelhandler)
				{

				//	std::cout << "Rendered Models : " << modelhandler->GetRenderedModelsCount() << std::endl;
					
					for (size_t i = 0; i < modelhandler->GetRenderedModelsCount(); ++i)
					{

						vml::models::Model3d_2* model = modelhandler->GetRenderedModels()[i];

						if (model->GetCullingFlags() != vml::views::frustum::OUTSIDE)
						{
							if (model->IsVisbile())
							{
								if (model->IsWireFrame())
								{
									DrawModelWire(view, model);
								}
								else
								{
									DrawDebugModelSolid(view, model);
								}
							}
						}

					//	if (model->GetCullingFlags() == vml::views::frustum::INSIDE)
					//		DrawAABBox(view, model, vml::colors::Green, 1);
					//	if (model->GetCullingFlags() == vml::views::frustum::INTERSECTED)
					//		DrawAABBox(view, model, vml::colors::Red, 1);
					}
				}

				/*
				// -----------------------------------------------------------------------------------
				// draw model

				void DrawModelTextured(vml::views::View* view,vml::models::Model3d_2* model, const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0)
				{
					if (view)
					{

					//	DrawAABBox(view, model, vml::colors::Yellow, true);
					//	DrawAOBBox(view, model, vml::colors::Blue, true);

						// get shader

						GLuint Id = TexturePhongShader->GetID();

						glUseProgram(Id);

						// set shader locations

						glUniformMatrix4fv(TexturePhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(TexturePhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(TexturePhongShader->GetModelMatrixLocation(), 1, GL_FALSE, model->GetMptr());
						glUniformMatrix3fv(TexturePhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, model->GetNVptr());
						glUniformMatrix4fv(TexturePhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, model->GetMVptr());
						glUniformMatrix4fv(TexturePhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, model->GetMVPptr());
						glUniformMatrix2fv(TexturePhongShader->GetTextureMatrixLocation(), 1, GL_FALSE, model->GetTMptr());

						glUniform4fv(TexturePhongMaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(TexturePhongMaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(TexturePhongMaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(TexturePhongMaterialShininessLocation, material.Shininess);

						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						glUniform4fv(TexturePhongLightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(TexturePhongLightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(TexturePhongLightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(TexturePhongLightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(TexturePhongLightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(TexturePhongLightPowerLocation, DirectionalLight.Power);
						glUniform1i(TexturePhongSamplerLocation, 0);

						// draw mesh
	
						GLuint texid = model->GetDiffuseTexture()->GetID();

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texid);

						glBindVertexArray(model->GetCurrentMesh()->GetVAOId());

						glDrawElements(
							GL_TRIANGLES,								// mode
							model->GetCurrentMesh()->GetIndicesCount(),	// count
							GL_UNSIGNED_INT,							// type
							(void*)0									// element array buffer offset
						);

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glUseProgram(0);

					}
				}
				*/
				// -----------------------------------------------------------------------------------
				//
							
				void DrawModelWire(vml::views::View* view, vml::models::Model3d_2* model)
				{
					if (view)
					{

						if (model)
						{
							//	DrawAABBox(view, model, vml::colors::Yellow, true);
							//	DrawAOBBox(view, model, vml::colors::Blue, true);

							// get shader

							GLuint Id = SingleColorShader->GetID();

							glUseProgram(Id);

							// set shader locations

							glUniformMatrix4fv(SingleColorShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
							glUniformMatrix4fv(SingleColorShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
							glUniformMatrix4fv(SingleColorShader->GetModelMatrixLocation(), 1, GL_FALSE, model->GetMptr());
							glUniformMatrix3fv(SingleColorShader->GetNormalMatrixLocation(), 1, GL_FALSE, model->GetNVptr());
							glUniformMatrix4fv(SingleColorShader->GetModelViewMatrixLocation(), 1, GL_FALSE, model->GetMVptr());
							glUniformMatrix4fv(SingleColorShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, model->GetMVPptr());

							glUniform4f(ColorLocation, WireFrameColor[0], WireFrameColor[1], WireFrameColor[2], WireFrameColor[3]);

							// draw mesh

							glEnable(GL_CULL_FACE);
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glBindVertexArray(model->GetCurrentMesh()->GetVAOId());

							glDrawElements(
								GL_TRIANGLES,								// mode
								model->GetCurrentMesh()->GetIndicesCount(),	// count
								GL_UNSIGNED_INT,							// type
								(void*)0									// element array buffer offset
							);

							glBindVertexArray(0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("OpenGLDebugRenderer : Null Mesh pointer");
						}
					}
					else
					{
						vml::os::Message::Error("OpenGLDebugRenderer : Null view  matrix for draw wire debug object");
					}
				}
				
				// -----------------------------------------------------------------------------------
				//

				void DrawDebugMeshSolid(vml::views::View* view, 
										vml::meshes::Mesh3d* mesh, 
										const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc,
										const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0)
				{
					if (view)
					{
						glm::mat4 M;	// model matrix
						glm::mat4 MV;	// model * view matrix
						glm::mat4 MVP;	// model * view * projection matrix
						glm::mat3 NV;	// viewnormal matrix
						glm::mat4 V;	// view matrix 
						glm::mat4 P;	// projection matrix

						// get scale

						float scx = sc.x;
						float scy = sc.y;
						float scz = sc.z;

						// get angles

						float Alpha = rot.x;
						float Phi = rot.y;
						float Theta = rot.z;

						// clamp angles

						if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

						if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

						if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

						// cache trigvalues

						float sx = sin(Alpha);
						float sy = sin(Phi);
						float sz = sin(Theta);
						float cx = cos(Alpha);
						float cy = cos(Phi);
						float cz = cos(Theta);

						// get parent matrix pointer

						float* matrix = glm::value_ptr(M);

						matrix[0] = (cy * cz) * scx;
						matrix[1] = (sx * sy * cz + cx * sz) * scx;
						matrix[2] = (-cx * sy * cz + sx * sz) * scx;
						matrix[3] = 0;
						matrix[4] = (-cy * sz) * scy;
						matrix[5] = (-sx * sy * sz + cx * cz) * scy;
						matrix[6] = (cx * sy * sz + sx * cz) * scy;
						matrix[7] = 0;
						matrix[8] = (sy)*scz;
						matrix[9] = (-sx * cy) * scz;
						matrix[10] = (cx * cy) * scz;
						matrix[11] = 0;
						matrix[12] = pos.x;
						matrix[13] = pos.y;
						matrix[14] = pos.z;
						matrix[15] = 1;

						V = view->GetView();
						P = view->GetProjection();

						MV = V * M;
						MVP = P * MV;

						// get normal matrix pointer

						float* NormalViewMatrix = glm::value_ptr(NV);

						// get model * view matrix

						float* ModelViewMatrix = glm::value_ptr(MV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the 
						// first 3x3 order for this matrix is meant to be divided 
						// by each scaling factor along correspondig axes, so 
						// we need to unskew normal matrix

						float Determinant = + ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
											- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
											+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

						if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
							else Determinant = 1.0f / Determinant;

						NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
						NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
						NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
						NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
						NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;

						// get shader

						glUseProgram(PhongShader->GetID());

						// set shader locations

						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4fv(PhongMaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(PhongMaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(PhongMaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(PhongMaterialShininessLocation, material.Shininess);

						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						glUniform4fv(PhongLightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(PhongLightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(PhongLightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(PhongLightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(PhongLightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(PhongLightPowerLocation, DirectionalLight.Power);

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						glBindVertexArray(mesh->GetVAOId());

						glDrawElements(
							GL_TRIANGLES,				// mode
							mesh->GetIndicesCount(),	// count
							GL_UNSIGNED_INT,			// type
							(void*)0					// element array buffer offset
						);

						glBindVertexArray(0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("DebugRender : Null view  matrix for mesh rendering");
					}
				}
				/*
				// -----------------------------------------------------------------------------------
				//

				void DrawDebugMeshTextured(vml::views::View* view,
										   vml::meshes::Mesh3d* mesh,
										   const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc, const float ts,
										   const GLuint texid) const
				{
					if (view)
					{
						glm::mat4 M;				// model matrix
						glm::mat4 MV;				// model * view matrix
						glm::mat4 MVP;				// model * view * projection matrix
						glm::mat3 NV;				// viewnormal matrix
						glm::mat4 V;				// view matrix 
						glm::mat4 P;				// projection matrix
						glm::mat2 TM{ ts,0,0,ts };	// texture coordinates matrix

						// get scale

						float scx = sc.x;
						float scy = sc.y;
						float scz = sc.z;

						// get angles

						float Alpha = rot.x;
						float Phi = rot.y;
						float Theta = rot.z;

						// clamp angles

						if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

						if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

						if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

						// cache trigvalues

						float sx = sin(Alpha);
						float sy = sin(Phi);
						float sz = sin(Theta);
						float cx = cos(Alpha);
						float cy = cos(Phi);
						float cz = cos(Theta);

						// get parent matrix pointer

						float* matrix = glm::value_ptr(M);

						matrix[0] = (cy * cz) * scx;
						matrix[1] = (sx * sy * cz + cx * sz) * scx;
						matrix[2] = (-cx * sy * cz + sx * sz) * scx;
						matrix[3] = 0;
						matrix[4] = (-cy * sz) * scy;
						matrix[5] = (-sx * sy * sz + cx * cz) * scy;
						matrix[6] = (cx * sy * sz + sx * cz) * scy;
						matrix[7] = 0;
						matrix[8] = (sy)*scz;
						matrix[9] = (-sx * cy) * scz;
						matrix[10] = (cx * cy) * scz;
						matrix[11] = 0;
						matrix[12] = pos.x;
						matrix[13] = pos.y;
						matrix[14] = pos.z;
						matrix[15] = 1;

						V = view->GetView();
						P = view->GetProjection();

						MV = V * M;
						MVP = P * MV;

						// get normal matrix pointer

						float* NormalViewMatrix = glm::value_ptr(NV);

						// get model * view matrix

						float* ModelViewMatrix = glm::value_ptr(MV);

						// normal matrix is computed as the inverse transpose
						// of the model view matrix, this causes the 
						// first 3x3 order for this matrix is meant to be divided 
						// by each scaling factor along correspondig axes, so 
						// we need to unskew normal matrix

						float Determinant = + ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
											- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
											+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

						if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
						else Determinant = 1.0f / Determinant;

						NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
						NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
						NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
						NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
						NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[ 6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[ 6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
						NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[ 5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;

						// get shader

						GLuint Id = TextureShader->GetID();
						
						glUseProgram(Id);
						
						glUniformMatrix4fv(TextureShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(TextureShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(TextureShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix4fv(TextureShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(TextureShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix2fv(TextureShader->GetTextureMatrixLocation(), 1, GL_FALSE, glm::value_ptr(TM));
						glUniform1i(TextureSamplerLocation, 0);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texid);
						glBindVertexArray(mesh->GetVAOId());
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
						glBindTexture(GL_TEXTURE_2D, 0);
						glBindVertexArray(0);
						glUseProgram(0);
												
					}
					else
					{
						vml::os::Message::Error("DebugRender : ", "Null view  matrix for mesh rendering");
					}
				}
				*/
				
				// -----------------------------------------------------------------------------------
				//

				void DrawDebugMeshWire(vml::views::View* view, 
					                   vml::meshes::Mesh3d* mesh, 
					                   const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc, const glm::vec4& col) const
				{
					if (view)
					{
						glm::mat4 M;							// model matrix
						glm::mat4 MV;							// model * view matrix
						glm::mat4 MVP;							// model * view * projection matrix
						glm::mat3 NV;							// viewnormal matrix
						glm::mat4 V;
						glm::mat4 P;

						// get scale

						float scx = sc.x;
						float scy = sc.y;
						float scz = sc.z;

						// get angles

						float Alpha = rot.x;
						float Phi = rot.y;
						float Theta = rot.z;

						// clamp angles

						if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

						if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

						if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

						// cache trigvalues

						float sx = sin(Alpha);
						float sy = sin(Phi);
						float sz = sin(Theta);
						float cx = cos(Alpha);
						float cy = cos(Phi);
						float cz = cos(Theta);
						
						// get parent matrix pointer

						float* matrix = glm::value_ptr(M);

						matrix[0] = (cy * cz) * scx;
						matrix[1] = (sx * sy * cz + cx * sz) * scx;
						matrix[2] = (-cx * sy * cz + sx * sz) * scx;
						matrix[3] = 0;
						matrix[4] = (-cy * sz) * scy;
						matrix[5] = (-sx * sy * sz + cx * cz) * scy;
						matrix[6] = (cx * sy * sz + sx * cz) * scy;
						matrix[7] = 0;
						matrix[8] = (sy)*scz;
						matrix[9] = (-sx * cy) * scz;
						matrix[10] = (cx * cy) * scz;
						matrix[11] = 0;
						matrix[12] = pos.x;
						matrix[13] = pos.y;
						matrix[14] = pos.z;
						matrix[15] = 1;

						V = view->GetView();
						P = view->GetProjection();

						MV = V * M;
						MVP = P * MV;
				
						// get shader

						GLuint Id = SingleColorShader->GetID();

						glUseProgram(Id);

						// set shader locations

						glUniformMatrix4fv(SingleColorShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(SingleColorShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(SingleColorShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(SingleColorShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(SingleColorShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(SingleColorShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						glBindVertexArray(mesh->GetVAOId());

						glDrawElements(
							GL_TRIANGLES,				// mode
							mesh->GetIndicesCount(),	// count
							GL_UNSIGNED_INT,			// type
							(void*)0					// element array buffer offset
						);

						glBindVertexArray(0);
						glUseProgram(0);
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for mesh rendering");
					}
				}
			
				// -----------------------------------------------------------------------------------
				// checkred plane rendering

				void DrawCheckeredQuad(vml::views::View* view,
									   float scale, const glm::vec3& center,	
									   unsigned int n, const glm::vec4& c1, const glm::vec4& c2,
									   const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0)
				{
					if (!view) vml::os::Message::Error("OpenGLDebugRenderer : Null view matrix for triangle debug rendering");
					CheckeredPlaneMesh->Draw(view, scale, center, n, c1, c2, DirectionalLight, material);
				}
				
				// ---------------------------------------------------------------------------------------------------------------
				// draw a forward vector using model matrix
				
				void DrawModelForwardVector(vml::views::View* view, vml::models::Model3d_2* model, float length)
				{
					RefSystem->DrawForwardVector(view, model->GetM());
				}

				// ---------------------------------------------------------------------------------------------------------------
				// draw a right vector using model matrix

				void DrawModelRightVector(vml::views::View* view, vml::models::Model3d_2* model, float length)
				{
					RefSystem->DrawRightVector(view, model->GetM());
				}

				// ---------------------------------------------------------------------------------------------------------------
				// draw an up vector using model matrix

				void DrawModelUpVector(vml::views::View* view, vml::models::Model3d_2* model, float length)
				{
					RefSystem->DrawUpVector(view, model->GetM());
				}

				// -----------------------------------------------------------------------------------
				// Draws an arrow

				void DrawArrow(vml::views::View* view, const glm::vec3& a, const glm::vec3& b, const glm::vec4& color, float axisscale, float arrowscale, float axislength)
				{
					RefSystem->DrawArrow(view, a, b, color, axisscale, arrowscale, axislength);
				}

				// -----------------------------------------------------------------------------------
				// Draws a reference system given positon, rotation and scaling

				void DrawRefSystem(vml::views::View* view, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scaling)
				{
					RefSystem->Draw(view, ComputeEulerMatrix(position, rotation, scaling));
				}

				// -----------------------------------------------------------------------------------
				// Draws a reference system given a model model matrix

				void DrawRefSystem(vml::views::View* view, vml::models::Model3d_2* model)
				{
					RefSystem->Draw(view, model->GetM());
				}

				// ------------------------------------------------------------------
				// 

				void DrawPathFinderBoundingBox(vml::views::View* view, vml::geo2d::PathFinder* pathfinder,bool drawpoints=DRAW_POINTS) const
				{
					DrawAABBox(view, pathfinder->GetBoundingBoxMin(), pathfinder->GetBoundingBoxMax(), vml::colors::Red, drawpoints);
				}

				// ------------------------------------------------------------------
				// 

				void DrawPathFinderCell(vml::views::View* view, vml::geo2d::PathFinder* pathfinder, const glm::ivec2& p, const glm::vec4& col0, const glm::vec4& col1) const
				{
					vml::geo2d::PathFinder::DebugCell c0;
					c0 = pathfinder->GetCellAt(p.x, p.y);
					glm::vec3 center=pathfinder->GetCellCenterFromIndices(p);
					DrawPoint(view, center, vml::colors::White);
					if (c0.IdData != -1)
					{
						DrawSolidTriangle(view, c0.Pa, c0.Pb, c0.Pc, col0, 0);
						DrawSolidTriangle(view, c0.Pb, c0.Pc, c0.Pd, col0, 0);
					}
					else
					{
						DrawSolidTriangle(view, c0.Pa, c0.Pb, c0.Pc, col1, 0);
						DrawSolidTriangle(view, c0.Pb, c0.Pc, c0.Pd, col1, 0);
					}
				}

				// ------------------------------------------------------------------------------

				void DrawPathFinderPath(vml::views::View* view, vml::geo2d::PathFinder* pathfinder) const
				{
					int n = pathfinder->GetPathCount();
					if (n <= 1)
						return;
					const glm::vec3* path = pathfinder->GetPAvgrOut();
					for (int i = 0; i < n - 1; ++i)
						DrawLine(view, path[i], path[i + 1], vml::colors::White, true);
				}
				
				// -------------------------------------------------------------------
				//

				void DrawDebugOctreeNode(vml::views::View* view, const vml::octree::OctTreeNode* node, unsigned int mode = DO_NOT_DRAW_AABBOX)
				{
					if (node->IsOut())
						return;

					if (mode == DRAW_AABBOX)
					{
						if (node->IsIntersected())
							DrawAABBox(view, node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax(), vml::colors::Red, true);
						else
							DrawAABBox(view, node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax(), vml::colors::Green, true);
					}
					
					const glm::mat4& M = node->GetM();
					const glm::mat4& MV = node->GetMV();
					const glm::mat4& MVP = node->GetMVP();
					const glm::mat3& NV = node->GetNV();

					// get shader

					const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0;

					//vml::shaders::GlShaderProgram* PhongShader = ogldbg->GetPhongShader();

					glUseProgram(PhongShader->GetID());

					// set shader locations

					glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
					glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
					glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

					glUniform4fv(PhongMaterialAmbientLocation, 1, &material.Ambient[0]);
					glUniform4fv(PhongMaterialDiffuseLocation, 1, &material.Diffuse[0]);
					glUniform4fv(PhongMaterialSpecularLocation, 1, &material.Specular[0]);
					glUniform1f(PhongMaterialShininessLocation, material.Shininess);

					DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

					glUniform4fv(PhongLightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(PhongLightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(PhongLightSpecularLocation, 1, &DirectionalLight.Specular[0]);
					glUniform4fv(PhongLightDirectionLocation, 1, &DirectionalLight.Direction[0]);
					glUniform4fv(PhongLightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(PhongLightPowerLocation, DirectionalLight.Power);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					glBindVertexArray(node->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,						// mode
						(GLsizei)node->GetIndicesCount(),	// count
						GL_UNSIGNED_INT,					// type
						(void*)0							// element array buffer offset
					);

					glBindVertexArray(0);
					glUseProgram(0);

				}

				// -------------------------------------------------------------------
				//

				void DrawDebugOctree(vml::views::View* view, vml::octree::OctTree* oct, unsigned int mode = DO_NOT_DRAW_AABBOX)
				{
					if (!oct->IsValid())
						vml::os::Message::Error("Octree : ", "Octree is not valid");

					size_t renderednodescount = oct->GetRenderedNodesCount();

					vml::octree::OctTreeNode** renderednodes = oct->GetRenderedNodes();

					for (size_t i = 0; i < renderednodescount; ++i)
					{
						vml::octree::OctTreeNode* node = renderednodes[i];

						if (mode == DRAW_AABBOX)
						{
							if (node->IsIntersected())
								DrawAABBox(view, node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax(), vml::colors::Red, true);
							else
								DrawAABBox(view, node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax(), vml::colors::Green, true);
						}

						const glm::mat4& M = node->GetM();
						const glm::mat4& MV = node->GetMV();
						const glm::mat4& MVP = node->GetMVP();
						const glm::mat3& NV = node->GetNV();

						// get shader

						const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0;

						glUseProgram(PhongShader->GetID());

						// set shader locations

						glUniformMatrix4fv(PhongShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
						glUniformMatrix4fv(PhongShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
						glUniformMatrix4fv(PhongShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
						glUniformMatrix3fv(PhongShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
						glUniformMatrix4fv(PhongShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(PhongShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

						glUniform4fv(PhongMaterialAmbientLocation, 1, &material.Ambient[0]);
						glUniform4fv(PhongMaterialDiffuseLocation, 1, &material.Diffuse[0]);
						glUniform4fv(PhongMaterialSpecularLocation, 1, &material.Specular[0]);
						glUniform1f(PhongMaterialShininessLocation, material.Shininess);

						DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

						glUniform4fv(PhongLightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
						glUniform4fv(PhongLightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
						glUniform4fv(PhongLightSpecularLocation, 1, &DirectionalLight.Specular[0]);
						glUniform4fv(PhongLightDirectionLocation, 1, &DirectionalLight.Direction[0]);
						glUniform4fv(PhongLightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
						glUniform1f(PhongLightPowerLocation, DirectionalLight.Power);

						// draw mesh

						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						glBindVertexArray(node->GetVAOId());

						glDrawElements(
							GL_TRIANGLES,						// mode
							(GLsizei)node->GetIndicesCount(),	// count
							GL_UNSIGNED_INT,					// type
							(void*)0							// element array buffer offset
						);

						glBindVertexArray(0);
						glUseProgram(0);
					}
				}

				// -----------------------------------------------------------------------------------
				// test axis aligned bounding box vs infinte ray

				bool TriangleVsRay(vml::views::View* view,
								   const glm::vec3& p0, const glm::vec3& p1,const glm::vec3& p2,
								   float MouseX, float MouseY,
								   glm::vec3& p, float& tmin)
				{
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::TriangleVsRay(p0, p1, p2, start, end, p, tmin) == vml::geo3d::intersections::TRIANGLE_INTERSECTS_RAY)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for triangle vs ray");
					}
					return false;
				}
				
				// -----------------------------------------------------------------------------------
				// test axis aligned bounding box vs infinte ray

				bool QuadVsRay(vml::views::View* view,
							   const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
							   float MouseX, float MouseY,
							   glm::vec3& p, float& tmin)
				{
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::QuadVsRay(p0, p1, p2, p3,start, end, p, tmin) == vml::geo3d::intersections::TRIANGLE_INTERSECTS_RAY)
								return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for quad vs ray");
					}
					return false;
				}
				
				// -----------------------------------------------------------------------------------
				// test axis aligned bounding box vs infinte ray
				
				bool AABBoxVsRay(vml::views::View* view,
								 const glm::vec3 &boundingboxmin,const glm::vec3 &boundingboxmax,
								 float MouseX, float MouseY,
						  		 glm::vec3& p, glm::vec3& q,float &tmin,float &tmax)
				{
					
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::AABBoxVsRay(boundingboxmin, boundingboxmax, 
																   start, end, 
																   p, q, tmin, tmax) == vml::geo3d::intersections::AABBOX_RAY_INTERSECTS)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
					}
				
					return false;
				}
								
				// -----------------------------------------------------------------------------------
				// test axis aligned bounding box vs infinte ray

				bool AABBoxVsRay(vml::views::View* view,
								 vml::models::Model3d_2* model,
								 float MouseX, float MouseY,
								 glm::vec3& p, glm::vec3& q, float& tmin, float& tmax)
				{
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::AABBoxVsRay(model->GetAABoundingBox().GetMin(), model->GetAABoundingBox().GetMax(),
																   start, end,
																   p, q, tmin, tmax) == vml::geo3d::intersections::AABBOX_RAY_INTERSECTS)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
					}
					return false;
				}

				// -----------------------------------------------------------------------------------
				// test axis aligned bounding box vs infinte ray

				bool AOBBoxVsRay(vml::views::View* view,
								 vml::models::Model3d_2* model,
								 float MouseX, float MouseY,
								 glm::vec3& p, glm::vec3& q, float& tmin, float& tmax)
				{
					if (view)
					{
						glm::vec3 start, end;
						const glm::vec3& boundingboxmin = model->GetCurrentMesh()->GetBoundingBox().GetMin();
						const glm::vec3& boundingboxmax = model->GetCurrentMesh()->GetBoundingBox().GetMax();
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::AOBBoxVsRay(start, end, boundingboxmin, boundingboxmax, model->GetM(), p, q, tmin, tmax) == vml::geo3d::intersections::AOBBOX_RAY_INTERSECTS)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
					}
					return false;
				}

				// -----------------------------------------------------------------------------------
				// test sphere against infinite ray

				bool SphereVsRay(vml::views::View* view,
								 const glm::vec3& pos, const float radius,
								 float MouseX, float MouseY, glm::vec3& p, glm::vec3& q,
								 float& tmin, float& tmax)
				{
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::SphereVsRay(pos, radius,
							start, end, p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for sphere vs ray");
					}
					return false;
				}
				
				// -----------------------------------------------------------------------------------
				// test sphere against infinite ray

				bool SphereVsRay(vml::views::View* view,
								 vml::models::Model3d_2* model,
								 float MouseX, float MouseY, glm::vec3& p, glm::vec3& q,
								 float& tmin, float& tmax)
				{
					if (view)
					{
						glm::vec3 start, end;
						view->UnprojectPosition(MouseX, MouseY, start, end);
						if (vml::geo3d::intersections::SphereVsRay(model->GetAABoundingBox().GetCenter(), model->GetRadius(),
							start, end,
							p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
							return true;
						return false;
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
					}
					return false;
				}

				// -----------------------------------------------------------------------------------
				// Compute euler matrix
				
				void ComputeEulerMatrix(vml::models::Model3d_2* model, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scaling = glm::vec3(1, 1, 1))
				{
					float scx = scaling.x;
					float scy = scaling.y;
					float scz = scaling.z;

					// get angles

					float alpha = rotation.x;
					float phi = rotation.y;
					float theta = rotation.z;

					// clamp angles

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(alpha);
					float sy = sin(phi);
					float sz = sin(theta);
					float cx = cos(alpha);
					float cy = cos(phi);
					float cz = cos(theta);

					// get parent matrix pointer

					float* matrix = model->GetMptr();

					matrix[ 0] = (cy * cz) * scx;
					matrix[ 1] = (sx * sy * cz + cx * sz) * scx;
					matrix[ 2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[ 3] = 0;
					matrix[ 4] = (-cy * sz) * scy;
					matrix[ 5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[ 6] = (cx * sy * sz + sx * cz) * scy;
					matrix[ 7] = 0;
					matrix[ 8] = (sy)*scz;
					matrix[ 9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;
					matrix[12] = position.x;
					matrix[13] = position.y;
					matrix[14] = position.z;
					matrix[15] = 1;
				}

				// -----------------------------------------------------------------------------------
				// Compute euler matrix

				glm::mat4 ComputeEulerMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scaling)
				{
					float scx = scaling.x;
					float scy = scaling.y;
					float scz = scaling.z;

					// get angles

					float alpha = rotation.x;
					float phi   = rotation.y;
					float theta = rotation.z;

					// clamp angles

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(alpha);
					float sy = sin(phi);
					float sz = sin(theta);
					float cx = cos(alpha);
					float cy = cos(phi);
					float cz = cos(theta);

					// get parent matrix pointer
				
					glm::mat4 m( 1,0,0,0,
								 0,1,0,0,
								 0,0,1,0,
								 0,0,0,1 );

					float* matrix = glm::value_ptr(m);

					matrix[0] = (cy * cz) * scx;
					matrix[1] = (sx * sy * cz + cx * sz) * scx;
					matrix[2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[3] = 0;
					matrix[4] = (-cy * sz) * scy;
					matrix[5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[6] = (cx * sy * sz + sx * cz) * scy;
					matrix[7] = 0;
					matrix[8] = (sy)*scz;
					matrix[9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;
					matrix[12] = position.x;
					matrix[13] = position.y;
					matrix[14] = position.z;
					matrix[15] = 1;

					return m;
				}

				// -----------------------------------------------------------------------------------
				// init debug shader

				void Init()
				{
					// check if class has been initialised
					if (Initialized)
						vml::os::Message::Error("OpenGLDebugRenderer : Already Initted");

					// clear internal flags
					Initialized=0;

					// logging initialisation
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Initializing" });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Path : " + FullDebugPath });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Shader Path : " + ShadersStorePath });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Phong Shader Path : " + PhongShaderFilename });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Single Color Shader FilePath : " + SingleColorShaderFilename });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Texture Shader FilePath : " + TextureShaderFilename });
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer", "Texture Phong Shader FilePath : " + TexturePhongShaderFilename });

					// create debug meshes

					SinglePointMesh				= new vml::debugrendering::SinglePoint();
					SegmentMesh				    = new vml::debugrendering::Segment();
					TriangleMesh				= new vml::debugrendering::Triangle();
					TrianglePerVertexColorMesh  = new vml::debugrendering::TrianglePerVertexColor();
					
					AlphaTexturedQuadMesh		= new vml::debugrendering::AlphaTexturedQuad();
				//	AlphaTexturedColorQuadMesh  = new vml::debugrendering::AlphaColorTexturedQuad();
					
					BillBoardMesh				= new vml::debugrendering::BillBoard();
					
					QuadMesh					= new vml::debugrendering::Quad();
					QuadPerVertexColorMesh	    = new vml::debugrendering::QuadPerVertexColor();
					QuadTexturedMesh			= new vml::debugrendering::TexturedQuad();
					CheckeredPlaneMesh		    = new vml::debugrendering::CheckeredPlane();
					WireCubeMesh			    = new vml::debugrendering::WireCube();
					SolidCubeMesh				= new vml::debugrendering::SolidCube();
					TexturedCubeMesh			= new vml::debugrendering::TexturedCube();
					TriangleTexturedMesh		= new vml::debugrendering::TexturedTriangle;
					WireSphereMesh			    = new vml::debugrendering::Sphere();
					RefSystem					= new vml::debugrendering::RefSystem();
				
					// load texture

					DebugTexture = new vml::textures::Texture(TextureStorePath + "signaltest.png", vml::textures::DefaultTexturesParms);
					DirectionalLightTexture = new vml::textures::Texture(TextureStorePath + "directional_light.png", vml::textures::DefaultTexturesParms);

					// sets initial debug directional light, direction vector has w=0

					DirectionalLight.Ambient   = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f);
					DirectionalLight.Diffuse   = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f);
					DirectionalLight.Specular  = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f);
				//	DirectionalLight.Direction = glm::vec4( 0.0f, 0.56f, -0.5f, 0.0f);
				//	DirectionalLight.Direction = glm::vec4(-10.0f, 12.0f, -0.5f, 0.0f);
					DirectionalLight.Direction = glm::vec4( 20.0f, 86.0f, 100,0.0f);
					DirectionalLight.Power     = 1.5f;
					
					// attach debug phong dir shader

					GLuint Id;
					
					PhongShader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(PhongShaderFilename, {});
					Id = PhongShader->GetID();
					glUseProgram(Id);
					PhongMaterialAmbientLocation   = glGetUniformLocation(Id, "Material.ambient");
					PhongMaterialDiffuseLocation   = glGetUniformLocation(Id, "Material.diffuse");
					PhongMaterialSpecularLocation  = glGetUniformLocation(Id, "Material.specular");
					PhongMaterialShininessLocation = glGetUniformLocation(Id, "Material.shininess");
					PhongLightAmbientLocation      = glGetUniformLocation(Id, "DirectionalLight.ambient");
					PhongLightDiffuseLocation      = glGetUniformLocation(Id, "DirectionalLight.diffuse");
					PhongLightSpecularLocation     = glGetUniformLocation(Id, "DirectionalLight.specular");
					PhongLightDirectionLocation    = glGetUniformLocation(Id, "DirectionalLight.direction");
					PhongLightPowerLocation        = glGetUniformLocation(Id, "DirectionalLight.power");
					PhongLightCameraSpaceLocation  = glGetUniformLocation(Id, "DirectionalLight.cameraspacedirection");
					glUseProgram(0);
					if (PhongMaterialAmbientLocation   == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'Material.ambient' uniform, check shader source code");
					if (PhongMaterialDiffuseLocation   == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialDiffuseLocation' uniform, check shader source code");
					if (PhongMaterialSpecularLocation  == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialSpecularLocation' uniform, check shader source code");
					if (PhongMaterialShininessLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialShininessLocation' uniform, check shader source code");
					if (PhongLightAmbientLocation      == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.ambient' uniform, check shader source code");
					if (PhongLightDiffuseLocation      == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.diffuse' uniform, check shader source code");
					if (PhongLightSpecularLocation     == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.specular' uniform, check shader source code");
					if (PhongLightPowerLocation        == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.power' uniform, check shader source code");
					if (PhongLightCameraSpaceLocation  == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.cameraspacedirection' uniform, check shader source code");
					
					// retrive shader locations for single color debug shader

					SingleColorShader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(SingleColorShaderFilename, {});
					Id= SingleColorShader->GetID();
					glUseProgram(Id);
					ColorLocation = glGetUniformLocation(Id, "Color");
					glUseProgram(0);
					if (ColorLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_single_color.shd requires 'Color' uniform, check shader source code");
					
					// retrive shader locations for texture debug shader

					TextureShader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(TextureShaderFilename, {});
					Id = TextureShader->GetID();
					glUseProgram(Id);
					TextureSamplerLocation = glGetUniformLocation(Id, "TextureSampler");
					glUseProgram(0);
					if (TextureSamplerLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_texturer.shd requires 'TextureSamplerLocation' uniform, check shader source code");
					
					// retrive shader locations for texture debug shader

					TexturePhongShader = vml::stores::ShaderStore->Load<vml::shaders::GlShaderProgram>(TexturePhongShaderFilename, {});
					Id = TexturePhongShader->GetID();
					glUseProgram(Id);
					TexturePhongSamplerLocation           = glGetUniformLocation(Id, "TextureSampler");
					TexturePhongMaterialAmbientLocation   = glGetUniformLocation(Id, "Material.ambient");
					TexturePhongMaterialDiffuseLocation   = glGetUniformLocation(Id, "Material.diffuse");
					TexturePhongMaterialSpecularLocation  = glGetUniformLocation(Id, "Material.specular");
					TexturePhongMaterialShininessLocation = glGetUniformLocation(Id, "Material.shininess");
					TexturePhongLightAmbientLocation      = glGetUniformLocation(Id, "DirectionalLight.ambient");
					TexturePhongLightDiffuseLocation      = glGetUniformLocation(Id, "DirectionalLight.diffuse");
					TexturePhongLightSpecularLocation     = glGetUniformLocation(Id, "DirectionalLight.specular");
					TexturePhongLightDirectionLocation    = glGetUniformLocation(Id, "DirectionalLight.direction");
					TexturePhongLightPowerLocation        = glGetUniformLocation(Id, "DirectionalLight.power");
					TexturePhongLightCameraSpaceLocation  = glGetUniformLocation(Id, "DirectionalLight.cameraspacedirection");
					glUseProgram(0);
					
					// retrieve shader alpha textured

					TextureShaderAlpha = vml::stores::ShaderStore->Load< vml::shaders::GlShaderProgram>(TextureShaderAlphaFileName);

					// retrieve shader alpha textured colored
 
				//	TextureShaderAlphaColor = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_texture_alpha_color.shd");
					
					// set context as started
					Initialized = true;

					// logging initialisation
					vml::logger::Logger2::GetInstance()->Info({ "OpenGLDebugRenderer","Initialized" });

					// set initials states
					Begin();
				}
				
				// -----------------------------------------------------------------------------------
				// begin render state , starts with default values
				
				void Begin() const
				{
					// check if class has been initialised
					if (!IsInitialized())
						vml::os::Message::Error("DebugRender : Not initted");
					// enable polygon mode
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					// Enable depth test
					glEnable(GL_DEPTH_TEST);
					// Accept fragment if it closer to the camera than the former one
					glDepthFunc(GL_LESS);
					// Cull triangles whose normal is not towards the camera
					glEnable(GL_CULL_FACE);
					// sets ccw face 
					glFrontFace(GL_CCW);
					// unbind any texture
					glBindTexture(GL_TEXTURE_2D, 0);
					// unbind any texture 2d array
					glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
					// disable blending
					glDisable(GL_BLEND);
					// ubind texture sampler
					glBindSampler(GL_TEXTURE0, 0);
					// no shaders
					glUseProgram(0);
					// Clear required buffers
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				}

				// -----------------------------------------------------------------------------------
				// set default render states back

				void End() const
				{
					// check if class has been initialised
					if (!IsInitialized())
						vml::os::Message::Error("DebugRender : Not initted");
					// enable polygon mode
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					// Enable depth test
					glDisable(GL_DEPTH_TEST);
					// Accept fragment if it closer to the camera than the former one
					glDepthFunc(GL_LESS);
					// Cull triangles whose normal is not towards the camera
					glDisable(GL_CULL_FACE);
					// unbind any texture
					glBindTexture(GL_TEXTURE_2D, 0);
					// unbind any texture 2d array
					glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
					// disable blending
					glDisable(GL_BLEND);
					// no shaders
					glUseProgram(0);
					// flush 
					glFlush();
				}

				// -----------------------------------------------------------------------------------
				// getters

				[[nodiscard]] bool IsInitialized() const 
				{
					return Initialized; 
				}

				// -----------------------------------------------------------------------------------
				//
				
				void Reset()
				{
				}

				// -----------------------------------------------------------------------------------
				// ctor / dtor
				
				OpenglDebugRender()
				{
					// clear flags
					Initialized = false;

					// default colors
					ClearColor		           = vml::colors::LightSkyBlue;
					WireFrameColor             = vml::colors::White;
				
					// meshes
					TriangleMesh			   = nullptr;
					CheckeredPlaneMesh		   = nullptr;
					QuadMesh				   = nullptr;
					QuadPerVertexColorMesh	   = nullptr;
					
					AlphaTexturedQuadMesh	   = nullptr;
				//	AlphaTexturedColorQuadMesh = nullptr;
					
					BillBoardMesh			   = nullptr;
					WireCubeMesh			   = nullptr;
					SolidCubeMesh			   = nullptr;
					TexturedCubeMesh		   = nullptr;
					
					TrianglePerVertexColorMesh = nullptr;
					SegmentMesh				   = nullptr;
					SinglePointMesh			   = nullptr;
					QuadTexturedMesh		   = nullptr;
					TriangleTexturedMesh	   = nullptr;
					WireSphereMesh			   = nullptr;
					RefSystem				   = nullptr;
			
					// debug texture

					DebugTexture			   = nullptr;
					DirectionalLightTexture    = nullptr;

					// shaders

					PhongShader				   = nullptr;
					SingleColorShader		   = nullptr;
					TextureShader			   = nullptr;
					TexturePhongShader		   = nullptr;
					
					TextureShaderAlpha		   = nullptr;
				//	TextureShaderAlphaColor	   = nullptr;
					
					// shaders locations

					ColorLocation						 = -1;
					
					PhongLightAmbientLocation			 = -1;
					PhongLightDiffuseLocation			 = -1;
					PhongLightSpecularLocation			 = -1;
					PhongLightDirectionLocation			 = -1;
					PhongLightPowerLocation				 = -1;
					PhongLightCameraSpaceLocation		 = -1;

					PhongMaterialAmbientLocation		 = -1;
					PhongMaterialDiffuseLocation		 = -1;
					PhongMaterialSpecularLocation		 = -1;
					PhongMaterialShininessLocation		 = -1;
					
					TexturePhongLightAmbientLocation     = -1;
					TexturePhongLightDiffuseLocation     = -1;
					TexturePhongLightSpecularLocation    = -1;
					TexturePhongLightDirectionLocation   = -1;
					TexturePhongLightPowerLocation		 = -1;
					TexturePhongLightCameraSpaceLocation = -1;
					
					TextureSamplerLocation				 = -1;
					TexturePhongSamplerLocation			 = -1;

					TexturePhongMaterialAmbientLocation   =-1;
					TexturePhongMaterialDiffuseLocation   = -1;
					TexturePhongMaterialSpecularLocation  = -1;
					TexturePhongMaterialShininessLocation = -1;
				}

				~OpenglDebugRender()
				{
					Close();
				}
			
		};
	
} // end of vml namesapce
