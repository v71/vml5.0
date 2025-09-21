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

#include <vml5.0/strings/stringutils.h>

namespace vml
{
	namespace views
	{
		////////////////////////////////////////////////////////////////////////////////
		// view class 
		// replace the name view with camera and you get the point

		class View 
		{

			private:
						
				uint32_t		  RotationMode;			// rotation mode
				int				  ViewPortWidth;		// viewport width
				int				  ViewPortHeight;		// viewport height
				float			  Aspect;				// viewport aspect
				float			  NearClippingPlane;	// near clipping plane
				float		 	  FarClippingPlane;		// far clipping plane
				float			  Radius;				// frustum radius
				float			  Fov;					// field of view
				glm::vec3		  Right;				// right vector
				glm::vec3		  Forward;			// direction
				glm::vec3		  Up;					// up vector
				glm::mat4		  V;					// view matrix
				glm::mat4		  P;					// projection matrix
				glm::mat4		  VP;					// projection * view
				glm::vec3		  OldAngles;			// euler angles
				glm::vec3		  OldPosition;			// position
				glm::vec4		  Planes[6];			// frustum planes
				glm::quat		  Quaternion;			// quaternion
				std::string		  ScreenName;			// screenname
				double			  MouseX;
				double			  MouseY;
				int				  LeftButtonState;
				int				  RightButtonState;

			public:
						
				// ----------------------------------------------------------------------------------
				// rotation type flags

				static const uint8_t EULER		 = 0;
				static const uint8_t QUATERNIONS = 1;

				// ----------------------------------------------------------------
				// public data

				float	  RotationSpeed;	// rotation speed
				float	  TranslationSpeed;	// translation speed
				glm::vec3 Angles;			// euler angles
				glm::vec3 Position;			// position
						
				// ----------------------------------------------------------------
				//  orbit camera
				// example : View->UpdateView(vml::GlWindow,Model->GetPosition(),40);
						
				void UpdateView(const float w, const float h, const glm::vec3 &pos, float distance)
				{
					// Set aspect

					ViewPortWidth  = w;
					ViewPortHeight = h;

					// check if view port is valid
					// this happens when window is minimized

					if (ViewPortHeight == 0)
						return;

					Aspect = (float)ViewPortWidth / (float)ViewPortHeight;

					// projection matrix

					P = glm::perspective(Fov, Aspect, NearClippingPlane, FarClippingPlane);

					float sx = sin(Angles.x);
					float sy = sin(Angles.y);
					float cx = cos(Angles.x);
					float cy = cos(Angles.y);

					glm::vec3 eye = glm::vec3(cy*sx, sy, cy*cx)*distance + pos;

					float *ptr = glm::value_ptr(V);

					glm::vec3 up(0, 1, 0);
					glm::vec3 f(glm::normalize(pos - eye));
					glm::vec3 s(glm::normalize(glm::cross(f, up)));
					glm::vec3 u(glm::cross(s, f));
							
					ptr[ 0] =  s.x;
					ptr[ 1] =  u.x;
					ptr[ 2] = -f.x;
					ptr[ 3] =  0;
					ptr[ 4] =  s.y;
					ptr[ 5] =  u.y;
					ptr[ 6] = -f.y;
					ptr[ 7] =  0;
					ptr[ 8] =  s.z;
					ptr[ 9] =  u.z;
					ptr[10] = -f.z;
					ptr[11] =  0;
					ptr[12] = -dot(s, eye);
					ptr[13] = -dot(u, eye);
					ptr[14] =  dot(f, eye);
					ptr[15] =  1;
											
					// extract frustum planes

					ptr = glm::value_ptr(VP);

					Right.x = ptr[0];
					Right.y = ptr[4];
					Right.z = ptr[8];

					Up.x = ptr[1];
					Up.y = ptr[5];
					Up.z = ptr[9];

					Forward.x = ptr[2];
					Forward.y = ptr[6];
					Forward.z = ptr[10];

					// compute vp matrix

					VP = P * V;

					// Left clipping plane

					Planes[0].x = ptr[ 3] + ptr[ 0];
					Planes[0].y = ptr[ 7] + ptr[ 4];
					Planes[0].z = ptr[11] + ptr[ 8];
					Planes[0].w = ptr[15] + ptr[12];

					// Right clipping plane

					Planes[1].x = ptr[ 3] - ptr[ 0];
					Planes[1].y = ptr[ 7] - ptr[ 4];
					Planes[1].z = ptr[11] - ptr[ 8];
					Planes[1].w = ptr[15] - ptr[12];

					// Bottom clipping plane

					Planes[2].x = ptr[ 3] + ptr[ 1];
					Planes[2].y = ptr[ 7] + ptr[ 5];
					Planes[2].z = ptr[11] + ptr[ 9];
					Planes[2].w = ptr[15] + ptr[13];

					// Bottom clipping plane

					Planes[3].x = ptr[ 3] - ptr[ 1];
					Planes[3].y = ptr[ 7] - ptr[ 5];
					Planes[3].z = ptr[11] - ptr[ 9];
					Planes[3].w = ptr[15] - ptr[13];

					// Near clipping plane

					Planes[4].x = ptr[ 3] + ptr[ 2];
					Planes[4].y = ptr[ 7] + ptr[ 6];
					Planes[4].z = ptr[11] + ptr[10];
					Planes[4].w = ptr[15] + ptr[14];

					// Far clipping plane

					Planes[5].x = ptr[ 3] - ptr[ 2];
					Planes[5].y = ptr[ 7] - ptr[ 6];
					Planes[5].z = ptr[11] - ptr[10];
					Planes[5].w = ptr[15] - ptr[14];

					// Normalize the plane equations

					float d;

					d = sqrtf(Planes[0].x*Planes[0].x + Planes[0].y*Planes[0].y + Planes[0].z*Planes[0].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					d = 1.0f / d;

					Planes[0].x *= d;
					Planes[0].y *= d;
					Planes[0].z *= d;
					Planes[0].w *= d;

					d = sqrtf(Planes[1].x*Planes[1].x + Planes[1].y*Planes[1].y + Planes[1].z*Planes[1].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					d = 1.0f / d;

					Planes[1].x *= d;
					Planes[1].y *= d;
					Planes[1].z *= d;
					Planes[1].w *= d;

					d = sqrtf(Planes[2].x*Planes[2].x + Planes[2].y*Planes[2].y + Planes[2].z*Planes[2].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[2].x *= d;
					Planes[2].y *= d;
					Planes[2].z *= d;
					Planes[2].w *= d;

					d = sqrtf(Planes[3].x*Planes[3].x + Planes[3].y*Planes[3].y + Planes[3].z*Planes[3].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[3].x *= d;
					Planes[3].y *= d;
					Planes[3].z *= d;
					Planes[3].w *= d;

					d = sqrtf(Planes[4].x*Planes[4].x + Planes[4].y*Planes[4].y + Planes[4].z*Planes[4].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[4].x *= d;
					Planes[4].y *= d;
					Planes[4].z *= d;
					Planes[4].w *= d;

					d = sqrtf(Planes[5].x*Planes[5].x + Planes[5].y*Planes[5].y + Planes[5].z*Planes[5].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[5].x *= d;
					Planes[5].y *= d;
					Planes[5].z *= d;
					Planes[5].w *= d;

					// minimum bounding sphere of a view frustum
					// https://lxjk.github.io/2017/04/15/Calculate-Minimal-Bounding-Sphere-of-Frustum.html

					d = ViewPortWidth * ViewPortWidth;
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = vml::math::EPSILON;
					else d = 1.0f / d;

					float k1 = tanf(Fov*0.5f);
					float k2 = (1.0f + (ViewPortHeight * ViewPortHeight * d)) * k1 *k1;

					if (k2 > (FarClippingPlane - NearClippingPlane) / (FarClippingPlane + NearClippingPlane))
					{
						// glm::vec3 Center = glm::vec3(0, 0, FarClippingPlane);

						Radius = FarClippingPlane * std::sqrt(k2);
					}
					else
					{
						// glm::vec3 Center = glm::vec3(0, 0, 0.5f * (FarClippingPlane + NearClippingPlane) * (1.0f + k2));

						Radius = 0.5f * std::sqrt(		(FarClippingPlane - NearClippingPlane) * (FarClippingPlane - NearClippingPlane) +
													2 * (FarClippingPlane * FarClippingPlane   + NearClippingPlane * NearClippingPlane) * k2 +
														(FarClippingPlane + NearClippingPlane) * (FarClippingPlane + NearClippingPlane) * k2 * k2);
					}

					// remeber to set the view port soon after calling this function
					//	glViewport(0, 0, ViewPortWidth, ViewPortHeight);

				}
						
				// ----------------------------------------------------------------
				//  fps camera

				void UpdateView(const float w, const float h)
				{
					// Set aspect
					ViewPortWidth = w;
					ViewPortHeight = h;

					// check if view port is valid
					// this happens when window is minimized

					if (ViewPortHeight == 0)
						return;

					Aspect = (float)ViewPortWidth / (float)ViewPortHeight;

					// projection matrix

					P = glm::perspective(Fov, Aspect, NearClippingPlane, FarClippingPlane);

					float *ptr = glm::value_ptr(V);

					if (RotationMode==EULER)
					{
						// compute view matrix

						float sx = sin(Angles.x);
						float sy = sin(Angles.y);
						float sz = sin(Angles.z);
						float cx = cos(Angles.x);
						float cy = cos(Angles.y);
						float cz = cos(Angles.z);

						ptr[ 0] =  cx * cz;
						ptr[ 1] =  sx * sy*cz + cy * sz;
						ptr[ 2] = -cy * sx*cz + sy * sz;
						ptr[ 3] =  0.0f;
						ptr[ 4] = -cx * sz;
						ptr[ 5] = -sx * sy*sz + cy * cz;
						ptr[ 6] =  cy * sx*sz + sy * cz;
						ptr[ 7] =  0.0f;
						ptr[ 8] =  sx;
						ptr[ 9] = -sy * cx;
						ptr[10] =  cx * cy;
						ptr[11] =  0.0f;
						ptr[12] = -Position.x*ptr[0] - Position.y*ptr[4] - Position.z*ptr[ 8];
						ptr[13] = -Position.x*ptr[1] - Position.y*ptr[5] - Position.z*ptr[ 9];
						ptr[14] = -Position.x*ptr[2] - Position.y*ptr[6] - Position.z*ptr[10];
						ptr[15] =  1.0f;

					}
					else if (RotationMode==QUATERNIONS)
					{
						Quaternion = glm::quat(glm::vec3(Angles.y, Angles.x, Angles.z));
						V = glm::toMat4(Quaternion);
						ptr[12] = -Position.x*ptr[0] - Position.y*ptr[4] - Position.z*ptr[ 8];
						ptr[13] = -Position.x*ptr[1] - Position.y*ptr[5] - Position.z*ptr[ 9];
						ptr[14] = -Position.x*ptr[2] - Position.y*ptr[6] - Position.z*ptr[10];
						ptr[15] = 1.0f;
					}

					// direction vectors

					Right.x = ptr[0];
					Right.y = ptr[4];
					Right.z = ptr[8];

					Up.x = ptr[1];
					Up.y = ptr[5];
					Up.z = ptr[9];

					Forward.x = ptr[2];
					Forward.y = ptr[6];
					Forward.z = ptr[10];

					// compute vp matrix

					VP = P * V;

					// extract frustum planes

					ptr = glm::value_ptr(VP);

					// Left clipping plane

					Planes[0].x = ptr[ 3] + ptr[ 0];
					Planes[0].y = ptr[ 7] + ptr[ 4];
					Planes[0].z = ptr[11] + ptr[ 8];
					Planes[0].w = ptr[15] + ptr[12];

					// Right clipping plane

					Planes[1].x = ptr[ 3] - ptr[ 0];
					Planes[1].y = ptr[ 7] - ptr[ 4];
					Planes[1].z = ptr[11] - ptr[ 8];
					Planes[1].w = ptr[15] - ptr[12];

					// Bottom clipping plane

					Planes[2].x = ptr[ 3] + ptr[ 1];
					Planes[2].y = ptr[ 7] + ptr[ 5];
					Planes[2].z = ptr[11] + ptr[ 9];
					Planes[2].w = ptr[15] + ptr[13];

					// Bottom clipping plane

					Planes[3].x = ptr[ 3] - ptr[ 1];
					Planes[3].y = ptr[ 7] - ptr[ 5];
					Planes[3].z = ptr[11] - ptr[ 9];
					Planes[3].w = ptr[15] - ptr[13];

					// Near clipping plane

					Planes[4].x = ptr[ 3] + ptr[ 2];
					Planes[4].y = ptr[ 7] + ptr[ 6];
					Planes[4].z = ptr[11] + ptr[10];
					Planes[4].w = ptr[15] + ptr[14];

					// Far clipping plane

					Planes[5].x = ptr[ 3] - ptr[ 2];
					Planes[5].y = ptr[ 7] - ptr[ 6];
					Planes[5].z = ptr[11] - ptr[10];
					Planes[5].w = ptr[15] - ptr[14];

					// Normalize the plane equations

					float d;

					d = sqrtf(Planes[0].x*Planes[0].x + Planes[0].y*Planes[0].y + Planes[0].z*Planes[0].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					d = 1.0f / d;

					Planes[0].x *= d;
					Planes[0].y *= d;
					Planes[0].z *= d;
					Planes[0].w *= d;

					d = sqrtf(Planes[1].x*Planes[1].x + Planes[1].y*Planes[1].y + Planes[1].z*Planes[1].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					d = 1.0f / d;

					Planes[1].x *= d;
					Planes[1].y *= d;
					Planes[1].z *= d;
					Planes[1].w *= d;

					d = sqrtf(Planes[2].x*Planes[2].x + Planes[2].y*Planes[2].y + Planes[2].z*Planes[2].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[2].x *= d;
					Planes[2].y *= d;
					Planes[2].z *= d;
					Planes[2].w *= d;

					d = sqrtf(Planes[3].x*Planes[3].x + Planes[3].y*Planes[3].y + Planes[3].z*Planes[3].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[3].x *= d;
					Planes[3].y *= d;
					Planes[3].z *= d;
					Planes[3].w *= d;

					d = sqrtf(Planes[4].x*Planes[4].x + Planes[4].y*Planes[4].y + Planes[4].z*Planes[4].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[4].x *= d;
					Planes[4].y *= d;
					Planes[4].z *= d;
					Planes[4].w *= d;

					d = sqrtf(Planes[5].x*Planes[5].x + Planes[5].y*Planes[5].y + Planes[5].z*Planes[5].z);
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					Planes[5].x *= d;
					Planes[5].y *= d;
					Planes[5].z *= d;
					Planes[5].w *= d;

					// minimum bounding sphere of a view frustum
					// https://lxjk.github.io/2017/04/15/Calculate-Minimal-Bounding-Sphere-of-Frustum.html

					d = ViewPortWidth * ViewPortWidth;
					if (d > -vml::math::EPSILON && d < vml::math::EPSILON) d = 1.0f / vml::math::EPSILON;
					else d = 1.0f / d;

					float k1 = tanf(Fov*0.5f);
					float k2 = (1.0f + (ViewPortHeight * ViewPortHeight * d )) * k1 *k1;

					if (k2 > (FarClippingPlane - NearClippingPlane) / (FarClippingPlane + NearClippingPlane))
					{
						// glm::vec3 Center = glm::vec3(0, 0, FarClippingPlane);

						Radius = FarClippingPlane * std::sqrt(k2);
					}
					else
					{
						// glm::vec3 Center = glm::vec3(0, 0, 0.5f * (FarClippingPlane + NearClippingPlane) * (1.0f + k2));

						Radius = 0.5f * std::sqrt(      (FarClippingPlane - NearClippingPlane) * (FarClippingPlane - NearClippingPlane) +
													2 * (FarClippingPlane * FarClippingPlane   + NearClippingPlane * NearClippingPlane) * k2 +
														(FarClippingPlane + NearClippingPlane) * (FarClippingPlane + NearClippingPlane) * k2 * k2);
					}

				}
						
				// ----------------------------------------------------------------
				//
						
				void UpdateGlViewPort() const
				{
					glViewport(0, 0, ViewPortWidth, ViewPortHeight);
				}

				// ----------------------------------------------------------------
				//  transforms a vector using view matrix

				[[nodiscard]] glm::vec3 TransformForward(const glm::vec3 &dir)
				{
					const float *ptr = glm::value_ptr(V);
					return glm::vec3(ptr[0] * dir[0] + ptr[1] * dir[1] + ptr[ 2] * dir[2],
									 ptr[4] * dir[0] + ptr[5] * dir[1] + ptr[ 6] * dir[2],
									 ptr[8] * dir[0] + ptr[9] * dir[1] + ptr[10] * dir[2]);
				}

				// ----------------------------------------------------------------
				// reset parameters

				void Reset()
				{
					Position = OldPosition;
					Angles   = OldAngles;
				}

				// ----------------------------------------------------------------
				// default method of handling view transformation

				void UpdateInput(const double mousex, const double mousey, const int leftbuttonstate, const int rightbuttonstate)
				{
					MouseX = mousex;
					MouseY = mousey;
					LeftButtonState = leftbuttonstate;
					RightButtonState = rightbuttonstate;
				}

				// ----------------------------------------------------------------
				// default method of handling view transformation

				void Controller()
				{

					// rotation

					if (LeftButtonState && !RightButtonState)
					{
						Angles.x += MouseX * RotationSpeed;
						Angles.y -= MouseY * RotationSpeed;
					}

					// forward movement

					if (RightButtonState && !LeftButtonState)
					{

						if (MouseY < 0)
						{
							Position -= Forward * TranslationSpeed;
						}
						else if (MouseY > 0)
						{
							Position += Forward * TranslationSpeed;
						}

					}

					// strafe movement

					if (LeftButtonState && RightButtonState)
					{

						if (MouseX < 0)
						{
							Position -= Right * TranslationSpeed;
						}
						else if (MouseX > 0)
						{
							Position += Right * TranslationSpeed;
						}

						if (MouseY > 0)
						{
							Position -= Up * TranslationSpeed;
						}
						else if (MouseY < 0)
						{
							Position += Up * TranslationSpeed;
						}
					}
				}
				
				// ----------------------------------------------------------------
				//  generates a ray from mouse screen position

				void UnprojectPosition( float x,float y,glm::vec3 &from,glm::vec3 &to ) const
				{
					float my=(float)ViewPortHeight-y-1.0f;
					glm::vec4 viewport = glm::vec4(0,0,(float)ViewPortWidth,(float)ViewPortHeight);
					from = glm::unProject(glm::vec3(x, my, 0), V, P, viewport);
					to   = glm::unProject(glm::vec3(x, my, 1), V, P, viewport);
				}
				
				// ----------------------------------------------------------------
				//  

				[[nodiscard]] float HFOVtoVFOV(float HFOV_deg) const
				{
					const float rhf = HFOV_deg * vml::math::DEGTORAD;
					return 2.f*atanf(tanf(rhf*.5f) / Aspect) * vml::math::DEGTORAD;
				}

				// ----------------------------------------------------------------
				//  

				[[nodiscard]] float VFOVtoHFOV(float VFOV_deg) const
				{
					const float rhf = VFOV_deg * vml::math::DEGTORAD;
					return 2.f*atanf( Aspect * tanf(rhf*.5f)) * vml::math::DEGTORAD;
				}
						
				// ----------------------------------------------------------------
				// value getting functions

				constexpr [[nodiscard]] float	 GetNearPlaneDist()	 const { return NearClippingPlane; }
				constexpr [[nodiscard]] float	 GetFarPlaneDist()	 const { return FarClippingPlane; }
				constexpr [[nodiscard]] float	 GetAspect()		 const { return Aspect; }
				constexpr [[nodiscard]] float	 GetFov()			 const { return Fov; }
				constexpr [[nodiscard]] int		 GetViewPortWidth()	 const { return ViewPortWidth; }
				constexpr [[nodiscard]] int		 GetViewPortHeight() const { return ViewPortHeight; }
				constexpr [[nodiscard]] uint32_t GetRotationMode()	 const { return RotationMode; }
				constexpr [[nodiscard]] float	 GetRadius()		 const { return Radius; }
						
				// ----------------------------------------------------------------
				// const pointers getters

				const [[nodiscard]] glm::vec3&   GetForwardVector()	 const { return Forward; }
				const [[nodiscard]] glm::vec3&   GetUpVector()		 const { return Up; }
				const [[nodiscard]] glm::vec3&   GetRightVector()    const { return Right; }
				const [[nodiscard]] glm::mat4&   GetViewProjection() const { return VP; }
				const [[nodiscard]] glm::mat4&   GetView()		     const { return V; }
				const [[nodiscard]] glm::mat4&   GetProjection()	 const { return P; }
				const [[nodiscard]] glm::vec3&   GetPosition()	     const { return Position; }
				const [[nodiscard]] glm::vec3&   GetAngle()		     const { return Angles; }
				const [[nodiscard]] std::string& GetScreenName()	 const { return ScreenName; }

				// ----------------------------------------------------------------
				// const pointers getters

				const [[nodiscard]]glm::vec4* GetFrustumPlanes() const { return Planes; }
				[[nodiscard]] float* GetVPptr() { return glm::value_ptr(VP); }
				[[nodiscard]] float* GetPptr()  { return glm::value_ptr(P); }
				[[nodiscard]] float* GetVptr()  { return glm::value_ptr(V); }
						
				// ---------------------------------------------------------------
				// set rotation mode

				void SetQuaternionsRotation()
				{
					RotationMode = QUATERNIONS;
				}

				void SetEulerRotation()
				{
					RotationMode = EULER;
				}

				[[nodiscard]] bool IsRotationModeQuaternions() const
				{
					return RotationMode == QUATERNIONS;
				}

				[[nodiscard]] bool IsRotationModeEuler() const
				{
					return RotationMode == EULER;
				}

				//----------------------------------------------------------------
				// dump info

				const std::string Dump()
				{
					std::string text;
							
					text =  std::format("ViewPort Width : {}\n", ViewPortWidth);
					text += std::format("ViewPort Height : {0}\n", ViewPortHeight);
					text += std::format("Aspect : {0}\n", Aspect);
					text += std::format("Near Clipping Plane : {0}\n", NearClippingPlane);
					text += std::format("Far Clipping Plane : {0}\n", FarClippingPlane);
					text += std::format("Fov: {0}\n", Fov);
					text += std::format("Position: {0} , {1} , {2}\n", Position.x, Position.y, Position.z);
					text += std::format("Angles {0} , {1} , {2}\n", Angles.x, Angles.y, Angles.z);
					text += std::format("Translation Speed: {0}\n", TranslationSpeed);
					text += std::format("Rotation Speed: {0}\n", RotationSpeed);
					if(RotationMode==EULER)
						text += std::format("Rotation Mode : Euler\n");
					else if (RotationMode == QUATERNIONS)
						text += std::format("Rotation Mode : Quaternions\n");
					return text;
				}
						
				//----------------------------------------------------------------
				// ctor / dtor

				View( const std::string& screenname, 
				  	  float				 nearclippingplane,
					  float				 farclippingplane,
					  float				 fov,
					  glm::vec3			 position,
					  glm::vec3			 angles,
					  float				 translationspeed,
					  float				 rotationspeed,
					  const uint8_t		 rotationmode ) 
				{
					// validate rotationmode
					if(rotationmode !=EULER && rotationmode !=QUATERNIONS)
						vml::os::Message::Error("View : RotationMode must be EULER or QUATERNIONS");
					// valida screen name
					if(screenname.empty())
						vml::os::Message::Error("View : ScreenName cannot be empty");

					// fill in data

					RotationMode	  = rotationmode;
					Position		  = position;												// initial position
					Angles			  = angles;												// euler angles
					OldPosition		  = position;												// needed for resetting position
					OldAngles		  = angles;												// needed for resetting angles
					NearClippingPlane = nearclippingplane;									// near z clipping plane
					FarClippingPlane  = farclippingplane;										// far z clipping plane
					Fov               = fov*vml::math::DEGTORAD;								// field of view
					RotationSpeed     = rotationspeed;										// rotation speed
					TranslationSpeed  = translationspeed;										// translation speed
					Right			  = glm::vec3(1, 0, 0);									// right vector	( x axis )
					Up				  = glm::vec3(0, 1, 0);									// up vector ( y axis )
					Forward			  = glm::vec3(0, 0, 1);									// direction vector ( z axis )
					Aspect			  = 0;													// aspect ratio
					ViewPortWidth	  = 0;													// view port width
					ViewPortHeight	  = 0;													// view port height
					Radius			  = 0;													// radius
					V				  = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// view matrix
					P				  = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// projection matrix
					VP				  = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);			// projection * view
					Planes[0]		  = glm::vec4(0,0,0,0);									// frustum planes
					Planes[1]		  = glm::vec4(0,0,0,0);									// frustum planes
					Planes[2]		  = glm::vec4(0,0,0,0);									// frustum planes
					Planes[3]		  = glm::vec4(0,0,0,0);									// frustum planes
					Planes[4]		  = glm::vec4(0,0,0,0);									// frustum planes
					Planes[5]		  = glm::vec4(0,0,0,0);									// frustum planes
					Quaternion		  = glm::quat(0,0,0,0);									// quaternion
					ScreenName		  = vml::strings::StringUtils::MakeLower(screenname);		// screenname
					MouseX			  = 0.0;
					MouseY			  = 0.0;
					LeftButtonState   = 0;
					RightButtonState  = 0;
				}

				~View()
				{
				}
						
		};
				
	} // end of view namespace

} //end of vml namespace

