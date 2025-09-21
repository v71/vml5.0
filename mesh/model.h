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

//////////////////////////////////////////////////////////////////////////////////////
// TO DO:
// 
// nicer rotation mode selection

#include <vml5.0/opengl/view/frustum3d.h>
#include <vml5.0/opengl/view/view.h>
#include <vml5.0/opengl/textures/texture.h>
#include <vml5.0/strings/stringutils.h>
#include <vml5.0/stores.h>
#include <vml5.0/mesh/mesh.h>

namespace vml
{
	namespace models
	{
		// ---------------------------------------------------------------
		// specify rotation transformation

		static constexpr uint32_t EULER					= 0;
		static constexpr uint32_t SPHERICAL_QUATERNION  = 1;
		static constexpr uint32_t QUATERNION			= 2;

		// ---------------------------------------------------------------
		// visiblity flag

		static constexpr uint32_t NOT_VISIBLE = 8;

		// ---------------------------------------------------------------
		// specify rendering modes

		static constexpr uint32_t WIREFRAME = 16;

		////////////////////////////////////////////////////////////////////
		// model class

		class Model3d_2
		{
				
			private:
				
				Model3d_2						  *Parent;							// Parent object
				std::string						   ScreenName;						// Object ScreenName
				std::string						   FullPathScreenName;				// Screenname based on model hierarchy
				bool							   Initialized;						// initialzied flag
				vml::utils::Flags				   PreferencesFlags;				// Bitfield used to store preferences flags
				uint32_t	 					   CullingFlags;					// Bitfield used to store internal flags
				glm::vec3						   Position;						// position
				glm::vec3						   OriginalPosition;				// old vector position , used for deltas
				glm::vec3						   Rotation;						// euler rotation angles
				glm::vec3						   OriginalRotation;				// old rotation position , used for deltas
				glm::vec3						   Scaling;							// scaling factor
				glm::vec3						   OriginalScaling;					// old scaling position , used for deltas
				glm::quat						   Quaternion;						// Quaternion
				std::vector<vml::meshes::Mesh3d*>  Meshes;							// Mesh pointer
				int								   CurrentMesh;						// Currentmesh indx
				vml::geo3d::AABBox				   AABoundingBox;					// Bounding box
				vml::geo3d::AOBBox				   AOBoundingBox;					// Bounding box
				float							   Radius;							// Radius
				glm::vec3						   Right;							// Right direction vector
				glm::vec3						   Forward;							// Forward direction vector
				glm::vec3						   Up;								// Up direction vector
				glm::mat4						   M;								// model matrix
				glm::mat4						   R;								// model matrix
				glm::mat4						   MV;								// model * view matrix
				glm::mat4						   MVP;								// model * view * projection matrix
				glm::mat3						   NV;								// viewnormal matrix
				glm::mat2						   TM;								// texture matrix
				vml::textures::Texture			  *DiffuseTexture;
			
				// ---------------------------------------------------------------
				// remove mesh if uniqe , if mesh is still in use, the
				// resource manager waits until no more models use it.

				void ReleaseAll();

				// ------------------------------------------------------------------
				// Compute quaternion matrix

				void ComputeSphericalQuaternionMatrix();

				// ------------------------------------------------------------------
				// Compute quaternion matrix

				void ComputeQuaternionMatrix();

				// -----------------------------------------------------------------------
				// Compute euler matrix

				void ComputeEulerMatrix();

				// -------------------------------------------------------------
				// compute transform matrix

				void ComputeMatrix();

				// -----------------------------------------------------------------------
				// transform model bounding box

				void TransformBoundingBoxes();

				// -----------------------------------------------------------------------
				// recursive function to get the compound axis aligned bounding box

				void GetAACompoundBoundingBoxRecursive(glm::vec3& bmin, glm::vec3& bmax, vml::models::Model3d_2* model);

			public:

				std::vector<Model3d_2*>			   Child;							// children objects

				// -----------------------------------------------------------------------
				// public data

				bool Visited;

				// -------------------------------------------------------------
				// transform objet to view

				void TransformToView(vml::views::View* View);

				// -------------------------------------------------------------
				// trasnform model

				void Transform();

				// -----------------------------------------------------------------------
				// link child model
				
				void Link(vml::models::Model3d_2 *childmodel);

				// -----------------------------------------------------------------------
				// un link child model

				void UnLink(vml::models::Model3d_2* childmodel);

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Model3d_2(Model3d_2& other) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				Model3d_2 operator=(const Model3d_2& other) = delete;
				
				//-----------------------------------------------------------------------------------
				// class is non mmovable

				Model3d_2(Model3d_2&& other) = delete;
				Model3d_2& operator=(Model3d_2&& other) = delete;
				
				// --------------------------------------------------------------------------------
				// getters
		
				[[nodiscard]] const std::string&	   GetScreenName()			const;
				[[nodiscard]] const std::string&	   GetFullPathScreenName()	const; 
				[[nodiscard]] bool					   IsInitialized()			const; 
				[[nodiscard]] const vml::utils::Flags& GetPreferencesFlags()	const; 
				[[nodiscard]] Model3d_2*			   GetParent()				const; 
				[[nodiscard]] size_t				   GetChildCount()			const; 
				
				// --------------------------------------------------------------------------
				// metrics getters

				[[nodiscard]] const vml::geo3d::AABBox&			   GetAABoundingBox()		   const; 
				[[nodiscard]] const vml::geo3d::AOBBox&			   GetAOBoundingBox()		   const; 
				[[nodiscard]] float								   GetRadius()				   const; 
				[[nodiscard]] const glm::vec3&					   GetRightVector()			   const; 
				[[nodiscard]] const glm::vec3&					   GetForwardVector()		   const; 
				[[nodiscard]] const glm::vec3&					   GetUpVector()			   const; 
				[[nodiscard]] glm::vec3							   GetTransformedPosition()    const; 
				[[nodiscard]] glm::vec3							   GetEulerAngles()			   const;
				[[nodiscard]] uint32_t							   GetCullingFlags()		   const; 
				[[nodiscard]] bool								   IsInFrustum()			   const; 
				[[nodiscard]] bool								   IsWireFrame()			   const; 
				[[nodiscard]] bool								   IsVisbile()				   const; 
				[[nodiscard]] bool								   IsSolid()				   const; 
				[[nodiscard]] const vml::geo3d::AABBox			   GetAACompoundBoundingBox();
				[[nodiscard]] std::vector<vml::models::Model3d_2*> GetModelsTree();
				void GetModelsTreeRecursive(vml::models::Model3d_2* model, int depth=0);

				// --------------------------------------------------------------------------
				// hierarchy getters

				[[nodiscard]] Model3d_2*				GetChild(size_t pos)				   const; 
				[[nodiscard]] vml::meshes::Mesh3d*		GetCurrentMesh()					   const; 
				[[nodiscard]] vml::meshes::Mesh3d*		GetMeshAt(const size_t pos)			   const; 
				[[nodiscard]] size_t					GetMeshesCount()					   const; 
				[[nodiscard]] Model3d_2*			    GetChild(const std::string& childname) const;
				
				// --------------------------------------------------------------------------
				// matrix getters

				[[nodiscard]] float* GetMptr(); 
				[[nodiscard]] float* GetRptr();
				[[nodiscard]] float* GetNVptr(); 
				[[nodiscard]] float* GetMVptr(); 
				[[nodiscard]] float* GetMVPptr(); 
				[[nodiscard]] float* GetTMptr(); 
				[[nodiscard]] const glm::quat& GetQuaternion() const; 
				[[nodiscard]] const glm::mat4& GetM()   const; 
				[[nodiscard]] const glm::mat4& GetR()   const; 
				[[nodiscard]] const glm::mat4& GetMV()  const; 
				[[nodiscard]] const glm::mat4& GetMVP() const; 
				[[nodiscard]] const glm::mat3& GetNV()  const; 
				
				// rotation / postion / scaling getters

				[[nodiscard]] const glm::vec3& GetPosition() const; 
				[[nodiscard]] const glm::vec3& GetAngles()   const; 
				[[nodiscard]] const glm::vec3& GetScaling()  const; 

				// -----------------------------------------------------------------------
				// Get rotation mode

				[[nodiscard]] int GetRotationMode() const;
				
				// --------------------------------------------------------------------------
				// texturing

				[[nodiscard]] vml::textures::Texture* GetDiffuseTexture() const; 
				
				// ----------------------------------------------------------------------------------------
				// Setters

				void SetParent(Model3d_2* parent); 
				void ClearChild(Model3d_2* child); 
				void AddChild(Model3d_2* child); 
				void SetInVisible();									// Forces model to be invisible
				void SetVisible();										// Forces model to be visible
				void SetCullingFlags(unsigned int cullingflags);		// sets culling flags
				void SetCullingFlagToOutside();							// Forces culling flag to ouutside
				void SetCullingFlagToIntersected();						// Forces culling flag to intersected
				void SetCullingFlagToInside();							// Forces culling falg to inside
				void SetRotationMode(int mode);							// Sets rotation mode, euler or quaternion
				void SetPosition(const glm::vec3& pos); 				// Set model position given a vector
				void Move(const glm::vec3& disp); 						// Move object to position
				void MoveX(const float px); 							// Move object to position
				void MoveY(const float py); 							// Move object to position
				void MoveZ(const float pz); 							// Move object to position
				void SetPosX(const float px); 							// Set model position given a vector
				void SetPosY(const float py); 							// Set model position given a vector
				void SetPosZ(const float pz); 							// Set model position given a vector
				void SetAngles(const glm::vec3& angles); 				// Rotation angles
				void AddAngles(const glm::vec3& angles); 				// Add rotation angles
				void SetAngleX(const float ax);							// Set Rotation angle x
				void SetAngleY(const float ay); 						// Set Rotation angle y
				void SetAngleZ(const float az); 						// Set Rotation angle z
				void AddAngleX(const float ax); 						// Increase Rotation angle x
				void AddAngleY(const float ay); 						// Increase Rotation angle y
				void AddAngleZ(const float az); 						// Increase Rotation angle z
				void SetScale(const glm::vec3& scale); 					// Scale model by a scaling vector
				void SetScaleX(const float sx); 						// Set Model x scale 
				void SetScaleY(const float sy); 						// Set Model y scale
				void SetScaleZ(const float sz); 						// Set Model z scale
				void AddScaleX(const float sx); 						// Increase Model x scale	
				void AddScaleY(const float sy); 						// Increase Model y scale
				void AddScaleZ(const float sz);							// Increase Model z scale

				// --------------------------------------------------------------------------------

				void SetTextureMatrixScale(const glm::vec2& scale);
				
				// --------------------------------------------------------------------------------

				void ResetScaling();
				void ResetPosition();
				void ResetRotation();
				
				// -------------------------------------------------------------
				// reset all transofrmations

				void ResetTransformations();
												
				// -------------------------------------------------------------
				// add diffuse texture

				vml::textures::Texture* AddDiffuseTexture(const std::string& filename, const vml::utils::Flags& flags);

				// -------------------------------------------------------------
				// remove diffuse texture
	
				void RemoveDiffuseTexture();
				
				// ---------------------------------------------------------------
				// dumps mesh 

				std::string Dump();

				// ------------------------------------------------------------------
				// ctor / dtor
				// A base class destructor may be responsible for cleaning 
				// up resources that were allocated by the base class constructor.
				// If your base class has a default constructor
				// (one that doesn't take parameters or has defaults
				// for all its parameters) that constructor is 
				// automatically called upon construction of a derived instance.
				// If your base class has a constructor that requires parameters, 
				// you must call it manually in the initializer list of 
				// the derived class constructor.

				Model3d_2(const std::string& screenname,
						  const std::string& filename,
						  const glm::vec3& pos,
						  const glm::vec3& angles,
						  const glm::vec3& scaling,
						  const vml::utils::Flags& preferencesflags);

				~Model3d_2();

		};

	}
}


