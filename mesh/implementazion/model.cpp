
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

#include <vml5.0/mesh/mesh.h>
#include <vml5.0/mesh/model.h>

namespace vml
{
	namespace models
	{
		// ---------------------------------------------------------------
		// remove mesh if uniqe , if mesh is still in use, the
		// resource manager waits until no more models use it.

		void Model3d_2::ReleaseAll()
		{
			// release attached mesh(es) trhough the meshstore
			for (size_t i = 0; i < Meshes.size(); ++i)
				vml::stores:: MeshStore->UnLoad(Meshes[i]->GetResourceFileName());

			// release textures
			if (DiffuseTexture)
				vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());

			// clear meshes array
			Meshes.clear();
		}
	
		// ------------------------------------------------------------------
		// Compute quaternion matrix

		void Model3d_2::ComputeQuaternionMatrix()
		{
			
		}

		// ------------------------------------------------------------------
		// Compute quaternion matrix

		void Model3d_2::ComputeSphericalQuaternionMatrix()
		{
			// cache scaling factors

			float scx = Scaling.x;
			float scy = Scaling.y;
			float scz = Scaling.z;

			// get angles

			float alpha = Rotation.x;
			float phi   = Rotation.y;
			float theta = Rotation.z;

			if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
			else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

			if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
			else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

			if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
			else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

			float cx = cosf(phi);
			float sx = sinf(phi);
			float cy = cosf(theta);
			float sy = sinf(theta);

			// polar coordinates
			// direction axis in spherical coordinates

			glm::vec3 axis(cx * cy, sx, cx * sy);

			// create quaternion around this axis

			Quaternion = glm::angleAxis(alpha, axis);

			glm::mat4 Q = glm::toMat4(Quaternion);

			float* QMatrix = glm::value_ptr(Q);

			float f0  = QMatrix[ 0];
			float f1  = QMatrix[ 1];
			float f2  = QMatrix[ 2];
		    float f3  = 0.0f;	// f3 is 0
			float f4  = QMatrix[ 4];
			float f5  = QMatrix[ 5];
			float f6  = QMatrix[ 6];
			float f7  = 0.0f;	// f7 is 0
			float f8  = QMatrix[ 8];
			float f9  = QMatrix[ 9];
			float f10 = QMatrix[10];
		    float f11 = 0.0f;	// f11 is 0
		    float f12 = 0.0f;	// f12 is 0
		    float f13 = 0.0f;	// f13 is 0
		    float f14 = 0.0f;	// f14 is 0
		    float f15 = 0.0f;	// f15 is 0
			
			// assumes axis is already normalised

			//	glm::vec3 up(0, 1, 0);
			//	glm::vec3 p(cx * cy, sx, cx * sy);
			//	glm::vec3 s(glm::normalize(glm::cross(p, up)));
			//	glm::vec3 u(glm::cross(s, p));
			//	glm::mat4 O( p.x , p.y , p.z , 0.0f ,
			//			 	 u.x , u.y , u.z , 0.0f ,
			//		 		 s.x , s.y , s.z , 0.0f ,
			//				 0   ,   0 ,   0 , 1.0f );

			// hardcoded version of the above code

			float r0  =  cx * cy;
			float r1  =  sx;
			float r2  =  cx * sy;
			float r4  = -cy * sx;
			float r5  =  cx;
			float r6  = -sx * sy;
			float r8  = -sy;
			float r10 =  cy;

			// get rotation matrix pointer

			float* rmatrix = glm::value_ptr(R);

			rmatrix[ 0] = -r8 * f0 - r10 *  f8;
			rmatrix[ 1] = -r8 * f1 - r10 *  f9;
			rmatrix[ 2] = -r8 * f2 - r10 * f10;
			rmatrix[ 3] =  0;
			rmatrix[ 4] =  r4 * f0 + r5 * f4 + r6 *  f8;
			rmatrix[ 5] =  r4 * f1 + r5 * f5 + r6 *  f9;
			rmatrix[ 6] =  r4 * f2 + r5 * f6 + r6 * f10;
			rmatrix[ 7] =  0;
			rmatrix[ 8] =  r0 * f0 + r1 * f4 + r2 *  f8;
			rmatrix[ 9] =  r0 * f1 + r1 * f5 + r2 *  f9;
			rmatrix[10] =  r0 * f2 + r1 * f6 + r2 * f10;
			rmatrix[11] =  0;
			rmatrix[12] =  0;
			rmatrix[13] =  0;
			rmatrix[14] =  0;
			rmatrix[15] =  1;

			// get parent matrix pointer

			float* matrix = glm::value_ptr(M);

			matrix[ 0] = rmatrix[ 0] * scx;
			matrix[ 1] = rmatrix[ 1] * scx;
			matrix[ 2] = rmatrix[ 2] * scx;
			matrix[ 3] = 0;
			matrix[ 4] = rmatrix[ 4] * scy;
			matrix[ 5] = rmatrix[ 5] * scy;
			matrix[ 6] = rmatrix[ 6] * scy;
			matrix[ 7] = 0;
			matrix[ 8] = rmatrix[ 8] * scz;
			matrix[ 9] = rmatrix[ 9] * scz;
			matrix[10] = rmatrix[10] * scz;
			matrix[11] = 0;
			matrix[12] = Position.x;
			matrix[13] = Position.y;
			matrix[14] = Position.z;
			matrix[15] = 1;

		}
		
		// -----------------------------------------------------------------------
		// Compute euler matrix

		void Model3d_2::ComputeEulerMatrix()
		{
			float scx = Scaling.x;
			float scy = Scaling.y;
			float scz = Scaling.z;

			// get angles

			float alpha = Rotation.x;
			float phi   = Rotation.y;
			float theta = Rotation.z;

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

			// get rotation matrix pointer

			float* rmatrix = glm::value_ptr(R);

			rmatrix[ 0] =  cy * cz;
			rmatrix[ 1] =  sx * sy * cz + cx * sz;
			rmatrix[ 2] = -cx * sy * cz + sx * sz;
			rmatrix[ 3] =  0;
			rmatrix[ 4] = -cy * sz;
			rmatrix[ 5] = -sx * sy * sz + cx * cz;
			rmatrix[ 6] =  cx * sy * sz + sx * cz;
			rmatrix[ 7] =  0;
			rmatrix[ 8] =  sy;
			rmatrix[ 9] = -sx * cy;
			rmatrix[10] =  cx * cy;
			rmatrix[11] =  0;
			rmatrix[12] =  0;
			rmatrix[13] =  0;
			rmatrix[14] =  0;
			rmatrix[15] =  1;

			// get parent matrix pointer

			float* matrix = glm::value_ptr(M);

			matrix[ 0] = rmatrix[0] * scx;
			matrix[ 1] = rmatrix[1] * scx;
			matrix[ 2] = rmatrix[2] * scx;
			matrix[ 3] = 0;
			matrix[ 4] = rmatrix[4] * scy;
			matrix[ 5] = rmatrix[5] * scy;
			matrix[ 6] = rmatrix[6] * scy;
			matrix[ 7] = 0;
			matrix[ 8] = rmatrix[ 8] * scz;
			matrix[ 9] = rmatrix[ 9] * scz;
			matrix[10] = rmatrix[10] * scz;
			matrix[11] = 0;
			matrix[12] = Position.x;
			matrix[13] = Position.y;
			matrix[14] = Position.z;
			matrix[15] = 1;
		}

		// -------------------------------------------------------------
		// transform objet to view

		void Model3d_2::TransformToView(vml::views::View* View)
		{
			// get View and projection matrices

			const glm::mat4& p = View->GetProjection();
			const glm::mat4& v = View->GetView();

			// concatenate v and p matrices

			MV  = v * M;
			MVP = p * MV;

			// floowing code is this transformation sequence
			// using glm  matrix lib

			//model->NV  = glm::mat3(glm::transpose(glm::inverse(model->MV)));

			// get normal matrix pointer

			float *normalviewmatrix = glm::value_ptr(NV);

			// get model * view matrix

			float *modelviewmatrix = glm::value_ptr(MV);

			// normal matrix is computed as the inverse transpose
			// of the model view matrix, this causes the
			// first 3x3 order for this matrix is meant to be divided
			// by each scaling factor along correspondig axes, so
			// we need to unskew normal matrix

			float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
								- modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
								+ modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[5] * modelviewmatrix[8]);

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
		}
		
		// -----------------------------------------------------------------------
		// transform model bounding box

		void Model3d_2::TransformBoundingBoxes()
		{
			// compute oriented bounding box
			// the bounding box is computed transforming
			// models' mesh's bounding box by the model's
			// M matrix , which is the T * R * S matrix
			// see aabbox.h

			// cache model's M matrix
			
			const float* m = glm::value_ptr(M);

			// get's model's mesh bounding box coordinates

			AOBoundingBox.Set(GetCurrentMesh()->GetBoundingBox().GetMin(), GetCurrentMesh()->GetBoundingBox().GetMax(), m);

			// compute axis aligned bounding box

			AABoundingBox.Set(AOBoundingBox.GetMin(), AOBoundingBox.GetMax());

			// compute bounding sphere

			Radius = sqrtf(AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
						   AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
						   AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z)*0.5f;

			// compute direction vectors
			// the oriented bounding box is centered at root's center

			Right   = glm::normalize(glm::vec3(m[0], m[1], m[ 2]));
			Up      = glm::normalize(glm::vec3(m[4], m[5], m[ 6]));
			Forward = glm::normalize(glm::vec3(m[8], m[9], m[10]));
		}
	
		// -------------------------------------------------------------
		// trasnform model

		void Model3d_2::Transform()
		{
			// transform model

			ComputeMatrix();

			// if parent is null, it means the model is the root
			// model, so we need only to compute the transformed
			// bounding boxes, if model has parent, compute
			// the rotation matrix concatenation according to
			// parent's rotation matrix

			if (Parent)
			{
				// if we get here, we need to perform the
				// full matrix concatenazion

				float  parentscale[3]    = { 0 };
				float  invparentscale[3] = { 0 };
				float  tempmatrix[16]    = { 0 };
				float* childmatrix       = nullptr;
				float* parentmatrix      = nullptr;

				// rescale parent matrix , this is necessary
				// becasue of child-parent matrix multiplication
				// actually this performs a multiplicatiuon
				// between the parent model matrix and the
				// inverse of its own scaling matrix

				// cache scaling factors

				parentscale[0] = Parent->GetScaling().x;
				parentscale[1] = Parent->GetScaling().y;
				parentscale[2] = Parent->GetScaling().z;

				// compute inverse scaling

				invparentscale[0] = 1.0f / parentscale[0];
				invparentscale[1] = 1.0f / parentscale[1];
				invparentscale[2] = 1.0f / parentscale[2];

				// get parent matrix pointer

				parentmatrix = Parent->GetMptr();

				// scale parent matrix by its inverse scaling vector

				parentmatrix[ 0] *= invparentscale[0];
				parentmatrix[ 1] *= invparentscale[0];
				parentmatrix[ 2] *= invparentscale[0];
				parentmatrix[ 4] *= invparentscale[1];
				parentmatrix[ 5] *= invparentscale[1];
				parentmatrix[ 6] *= invparentscale[1];
				parentmatrix[ 8] *= invparentscale[2];
				parentmatrix[ 9] *= invparentscale[2];
				parentmatrix[10] *= invparentscale[2];

				// concatenate parent and child matrices

				childmatrix = glm::value_ptr(M);

				// save child matrix to a temporary matrix

				memcpy(tempmatrix, childmatrix, sizeof(float) * 16);

				// compute matrix multiplication this is
				// the equvalent of this equation
				// child->M = parent->M * child->M;

				childmatrix[ 0] = tempmatrix[ 0] * parentmatrix[0] + tempmatrix[ 1] * parentmatrix[4] + tempmatrix[ 2] * parentmatrix[ 8] + tempmatrix[ 3] * parentmatrix[12];
				childmatrix[ 1] = tempmatrix[ 0] * parentmatrix[1] + tempmatrix[ 1] * parentmatrix[5] + tempmatrix[ 2] * parentmatrix[ 9] + tempmatrix[ 3] * parentmatrix[13];
				childmatrix[ 2] = tempmatrix[ 0] * parentmatrix[2] + tempmatrix[ 1] * parentmatrix[6] + tempmatrix[ 2] * parentmatrix[10] + tempmatrix[ 3] * parentmatrix[14];
				childmatrix[ 3] = tempmatrix[ 0] * parentmatrix[3] + tempmatrix[ 1] * parentmatrix[7] + tempmatrix[ 2] * parentmatrix[11] + tempmatrix[ 3] * parentmatrix[15];
				childmatrix[ 4] = tempmatrix[ 4] * parentmatrix[0] + tempmatrix[ 5] * parentmatrix[4] + tempmatrix[ 6] * parentmatrix[ 8] + tempmatrix[ 7] * parentmatrix[12];
				childmatrix[ 5] = tempmatrix[ 4] * parentmatrix[1] + tempmatrix[ 5] * parentmatrix[5] + tempmatrix[ 6] * parentmatrix[ 9] + tempmatrix[ 7] * parentmatrix[13];
				childmatrix[ 6] = tempmatrix[ 4] * parentmatrix[2] + tempmatrix[ 5] * parentmatrix[6] + tempmatrix[ 6] * parentmatrix[10] + tempmatrix[ 7] * parentmatrix[14];
				childmatrix[ 7] = tempmatrix[ 4] * parentmatrix[3] + tempmatrix[ 5] * parentmatrix[7] + tempmatrix[ 6] * parentmatrix[11] + tempmatrix[ 7] * parentmatrix[15];
				childmatrix[ 8] = tempmatrix[ 8] * parentmatrix[0] + tempmatrix[ 9] * parentmatrix[4] + tempmatrix[10] * parentmatrix[ 8] + tempmatrix[11] * parentmatrix[12];
				childmatrix[ 9] = tempmatrix[ 8] * parentmatrix[1] + tempmatrix[ 9] * parentmatrix[5] + tempmatrix[10] * parentmatrix[ 9] + tempmatrix[11] * parentmatrix[13];
				childmatrix[10] = tempmatrix[ 8] * parentmatrix[2] + tempmatrix[ 9] * parentmatrix[6] + tempmatrix[10] * parentmatrix[10] + tempmatrix[11] * parentmatrix[14];
				childmatrix[11] = tempmatrix[ 8] * parentmatrix[3] + tempmatrix[ 9] * parentmatrix[7] + tempmatrix[10] * parentmatrix[11] + tempmatrix[11] * parentmatrix[15];
				childmatrix[12] = tempmatrix[12] * parentmatrix[0] + tempmatrix[13] * parentmatrix[4] + tempmatrix[14] * parentmatrix[ 8] + tempmatrix[15] * parentmatrix[12];
				childmatrix[13] = tempmatrix[12] * parentmatrix[1] + tempmatrix[13] * parentmatrix[5] + tempmatrix[14] * parentmatrix[ 9] + tempmatrix[15] * parentmatrix[13];
				childmatrix[14] = tempmatrix[12] * parentmatrix[2] + tempmatrix[13] * parentmatrix[6] + tempmatrix[14] * parentmatrix[10] + tempmatrix[15] * parentmatrix[14];
				childmatrix[15] = tempmatrix[12] * parentmatrix[3] + tempmatrix[13] * parentmatrix[7] + tempmatrix[14] * parentmatrix[11] + tempmatrix[15] * parentmatrix[15];

				// rescale parent matrix by its own scaling matrix

				parentmatrix[ 0] *= parentscale[0];
				parentmatrix[ 1] *= parentscale[0];
				parentmatrix[ 2] *= parentscale[0];
				parentmatrix[ 4] *= parentscale[1];
				parentmatrix[ 5] *= parentscale[1];
				parentmatrix[ 6] *= parentscale[1];
				parentmatrix[ 8] *= parentscale[2];
				parentmatrix[ 9] *= parentscale[2];
				parentmatrix[10] *= parentscale[2];
			}

			// compute bounding boxes and oriented vectors

			TransformBoundingBoxes();
		}

		// --------------------------------------------------------------------------------
		// link chil dmodel

		void Model3d_2::Link(vml::models::Model3d_2* childmodel)
		{
			if (childmodel)
			{
				// check if same nodes are linked

				if (this == childmodel)
					vml::os::Message::Error("Model : (Link) , Cannot link same models");

				// check for cyclic connections, this is a fatal error

				for (size_t i = 0; i < childmodel->GetChildCount(); ++i)
				{
					if (childmodel->GetChild(i) == this)
						vml::os::Message::Error("Model : (Link) , Cyclic connections aren't allowed from '",
												childmodel->GetChild(i)->GetScreenName().c_str(), "' to '", this->GetScreenName().c_str(), "'");
				}

				// link child to parent

				childmodel->Parent = this;
				Child.emplace_back(childmodel);
			}
			else
			{
				vml::os::Message::Error("Model : (Link) , Child model pointer is null");
			}
		}

		// --------------------------------------------------------------------------------
		// link chil dmodel

		void Model3d_2::UnLink(vml::models::Model3d_2* childmodel)
		{
			if (childmodel)
			{
				//	std::cout << "Parent : " << ScreenName << std::endl;

				for (auto it = Child.begin(); it!=Child.end(); ++it)
				{
					if ( (*it) == childmodel)
					{
						//	std::cout << "Child " << (*it)->GetScreenName() << std::endl;
				
						vml::models::Model3d_2* child_to_unlink = (*it);
						
						// unlink child from parent child array
						child_to_unlink->Parent = nullptr;
						
						// copy children of node to be unlinkd in parent child array
						for (auto jt = child_to_unlink->Child.begin(); jt != child_to_unlink->Child.end(); ++jt)
							Child.emplace_back(*jt);

						// remove all children  from child to unlink
						child_to_unlink->Child.clear();
						
						// get current position from 'M' matrix and set as worlkd position
						child_to_unlink->SetPosition(child_to_unlink->GetTransformedPosition());
						child_to_unlink->SetAngles(GetEulerAngles()*vml::math::RADTODEG);

						// remove this child model 
						Child.erase( it );
				
						return;
					}
				}
			}
			else 
			{
				vml::os::Message::Error("Model : (Link) , Child model pointer is null");
			}
		}

		// --------------------------------------------------------------------------------
		// getters

		[[nodiscard]] const std::string&	   Model3d_2::GetScreenName()		  const { return ScreenName; }
		[[nodiscard]] const std::string&	   Model3d_2::GetFullPathScreenName() const { return FullPathScreenName; }
		[[nodiscard]] bool					   Model3d_2::IsInitialized()		  const { return Initialized; }
		[[nodiscard]] const vml::utils::Flags& Model3d_2::GetPreferencesFlags()	  const { return PreferencesFlags; }
		[[nodiscard]] Model3d_2*			   Model3d_2::GetParent()			  const { return Parent; }
		[[nodiscard]] size_t				   Model3d_2::GetChildCount()		  const { return Child.size(); }
		
		// --------------------------------------------------------------------------
		// metrics getters

		[[nodiscard]] const vml::geo3d::AABBox& Model3d_2::GetAABoundingBox()		const { return AABoundingBox; }
		[[nodiscard]] const vml::geo3d::AOBBox& Model3d_2::GetAOBoundingBox()		const { return AOBoundingBox; }
		[[nodiscard]] float					    Model3d_2::GetRadius()				const { return Radius; }
		[[nodiscard]] const glm::vec3&			Model3d_2::GetRightVector()			const { return Right; }
		[[nodiscard]] const glm::vec3&			Model3d_2::GetForwardVector()		const { return Forward; }
		[[nodiscard]] const glm::vec3&			Model3d_2::GetUpVector()			const { return Up; }
		[[nodiscard]] glm::vec3					Model3d_2::GetTransformedPosition() const { const float* m = glm::value_ptr(M); return glm::vec3(m[12], m[13], m[14]); }
		[[nodiscard]] uint32_t					Model3d_2::GetCullingFlags()		const { return CullingFlags; }
		[[nodiscard]] bool						Model3d_2::IsInFrustum()			const { return CullingFlags != vml::views::frustum::OUTSIDE; }
		[[nodiscard]] bool						Model3d_2::IsWireFrame()			const { return PreferencesFlags.Get(WIREFRAME); }
		[[nodiscard]] bool						Model3d_2::IsVisbile()				const { return !PreferencesFlags.Get(NOT_VISIBLE); }
		[[nodiscard]] bool						Model3d_2::IsSolid()				const { return !PreferencesFlags.Get(WIREFRAME); }
		
		// ------------------------------------------------------------------
		// recursively traverse the model tree composed by linked children model(s)

		void Model3d_2::GetAACompoundBoundingBoxRecursive(glm::vec3 &bmin, glm::vec3 &bmax,vml::models::Model3d_2* model)
		{
			// get model bounding box

			glm::vec3 model_bmin = model->AABoundingBox.GetMin();
			glm::vec3 model_bmax = model->AABoundingBox.GetMax();

			// expand current compound bounding box

			if (model_bmin.x < bmin.x) bmin.x = model_bmin.x;
			if (model_bmin.y < bmin.y) bmin.y = model_bmin.y;
			if (model_bmin.z < bmin.z) bmin.z = model_bmin.z;
			if (model_bmax.x > bmax.x) bmax.x = model_bmax.x;
			if (model_bmax.y > bmax.y) bmax.y = model_bmax.y;
			if (model_bmax.z > bmax.z) bmax.z = model_bmax.z;

			// recurse

			for (size_t i = 0; i < model->GetChildCount(); ++i)
				GetAACompoundBoundingBoxRecursive(bmin,bmax,model->GetChild(i));
		}

		// --------------------------------------------------------------------------
		// Compute recursively the compound bounding box

		[[nodiscard]] const vml::geo3d::AABBox Model3d_2::GetAACompoundBoundingBox() 
		{
			glm::vec3 bmin = AABoundingBox.GetMin();
			glm::vec3 bmax = AABoundingBox.GetMax();
			for (auto& model : Child)
				GetAACompoundBoundingBoxRecursive(bmin, bmax, model);
			return vml::geo3d::AABBox(bmin, bmax);
		}

		// ------------------------------------------------------------------
		// recursively traverse the model tree composed by linked children model(s)

		void Model3d_2::GetModelsTreeRecursive(vml::models::Model3d_2* model,int depth)
		{
			if (!model)
				return;

			if (model->Visited)
				return;

			// Print indentation spaces

			for (int i = 0; i < depth; ++i)
				std::cout << "    "; // 4 spaces per depth level

			// print model screenname

			std::cout << model->GetScreenName() << std::endl;
				model->Visited = true;

			// recurse

			for (size_t i = 0; i < model->GetChildCount(); ++i)
				GetModelsTreeRecursive(model->GetChild(i), depth + 1);
		 }

		// --------------------------------------------------------------------------

		[[nodiscard]] std::vector<vml::models::Model3d_2*> Model3d_2::GetModelsTree()
		{
			std::vector<vml::models::Model3d_2*> modellist;
			std::cout <<GetScreenName() << std::endl;
			for (auto& model : Child)
				GetModelsTreeRecursive(model);
			for ( auto &model : modellist)
				model->Visited = false;
			return modellist;
		}

		// --------------------------------------------------------------------------
		// hierarchy getters

		[[nodiscard]] Model3d_2*				Model3d_2::GetChild(size_t pos)		   const { return *(Child.begin() + pos); }
		[[nodiscard]] vml::meshes::Mesh3d*		Model3d_2::GetCurrentMesh()			   const { return Meshes[CurrentMesh]; }
		[[nodiscard]] vml::meshes::Mesh3d*		Model3d_2::GetMeshAt(const size_t pos) const { return Meshes[pos]; }
		[[nodiscard]] size_t					Model3d_2::GetMeshesCount()			   const { return Meshes.size(); }

		[[nodiscard]] Model3d_2* Model3d_2::GetChild(const std::string& childname) const
		{
			if (childname.empty())
				vml::os::Message::Error("Model : ", "Model3d : Null name is not allowed");
			for (size_t i = 0; i < Child.size(); ++i)
				if (Child[i]->GetScreenName() == childname) return Child[i];
			return nullptr;
		}

		// --------------------------------------------------------------------------
		// extract angles from a 4x4 matrix

		glm::vec3 Model3d_2::GetEulerAngles() const
		{
			float T1 = atan2(M[2][1], M[2][2]);
			float C2 = sqrtf(M[0][0] * M[0][0] + M[1][0] * M[1][0]);
			float T2 = atan2(-M[2][0], C2);
			float S1 = sin(T1);
			float C1 = cos(T1);
			float T3 = atan2(S1 * M[0][2] - C1 * M[0][1], C1 * M[1][1] - S1 * M[1][2]);
			return glm::vec3(-T1,-T2,-T3);
		}

		// --------------------------------------------------------------------------
		// matrix getters

		[[nodiscard]] float* Model3d_2::GetMptr()	{ return glm::value_ptr(M); }
		[[nodiscard]] float* Model3d_2::GetRptr()   { return glm::value_ptr(R); }
		[[nodiscard]] float* Model3d_2::GetNVptr()	{ return glm::value_ptr(NV); }
		[[nodiscard]] float* Model3d_2::GetMVptr()  { return glm::value_ptr(MV); }
		[[nodiscard]] float* Model3d_2::GetMVPptr() { return glm::value_ptr(MVP); }
		[[nodiscard]] float* Model3d_2::GetTMptr()  { return glm::value_ptr(TM); }
		[[nodiscard]] const glm::quat& Model3d_2::GetQuaternion() const { return Quaternion; }
		[[nodiscard]] const glm::mat4& Model3d_2::GetM()   const { return M; }
		[[nodiscard]] const glm::mat4& Model3d_2::GetR()   const { return R; }
		[[nodiscard]] const glm::mat4& Model3d_2::GetMV()  const { return MV; }
		[[nodiscard]] const glm::mat4& Model3d_2::GetMVP() const { return MVP; }
		[[nodiscard]] const glm::mat3& Model3d_2::GetNV()  const { return NV; }
		
		// rotation / postion / scaling getters

		[[nodiscard]] const glm::vec3& Model3d_2::GetPosition() const { return Position; }
		[[nodiscard]] const glm::vec3& Model3d_2::GetAngles()   const { return Rotation; }
		[[nodiscard]] const glm::vec3& Model3d_2::GetScaling()  const { return Scaling; }
		
		// -----------------------------------------------------------------------
		// Get rotation mode

		[[nodiscard]] int Model3d_2::GetRotationMode() const
		{
			if (PreferencesFlags.Get(EULER)) return EULER;
			if (PreferencesFlags.Get(SPHERICAL_QUATERNION)) return SPHERICAL_QUATERNION;
			if (PreferencesFlags.Get(QUATERNION)) return QUATERNION;
			return -1;
		}
		
		// --------------------------------------------------------------------------
		// texturing

		[[nodiscard]] vml::textures::Texture* Model3d_2::GetDiffuseTexture() const { return DiffuseTexture; }
		
		// ----------------------------------------------------------------------------------------
		// Setters

		void Model3d_2::SetParent(Model3d_2* parent)								 { Parent = parent; }
		void Model3d_2::ClearChild(Model3d_2* child)								 { Child.clear(); }
		void Model3d_2::AddChild(Model3d_2* child)									 { Child.emplace_back(child); }
		void Model3d_2::SetInVisible()												 { PreferencesFlags.SetToTrue(NOT_VISIBLE); }
		void Model3d_2::SetVisible()												 { PreferencesFlags.SetToFalse(NOT_VISIBLE); }
		void Model3d_2::SetCullingFlags(unsigned int cullingflags)					 { CullingFlags = cullingflags; }
		void Model3d_2::SetCullingFlagToOutside()									 { CullingFlags = vml::views::frustum::OUTSIDE; }
		void Model3d_2::SetCullingFlagToIntersected()								 { CullingFlags = vml::views::frustum::INTERSECTED; }
		void Model3d_2::SetCullingFlagToInside()									 { CullingFlags = vml::views::frustum::INSIDE; }
		
		void Model3d_2::SetRotationMode(int mode)
		{
			if (mode != EULER && mode != SPHERICAL_QUATERNION && mode!=QUATERNION )
				vml::os::Message::Error("Model : unacceptable rotation mode");
			// set flags to false
			PreferencesFlags.SetToFalse(EULER);
			PreferencesFlags.SetToFalse(SPHERICAL_QUATERNION);
			PreferencesFlags.SetToFalse(QUATERNION);
			// set selected flag to true
			PreferencesFlags.SetToTrue(mode);
		}
		
		void Model3d_2::SetPosition(const glm::vec3& pos)  { Position  = pos; }									 // Set model position given a vector
		void Model3d_2::Move(const glm::vec3& disp)		   { Position += disp; }								 // Move object to position
		void Model3d_2::MoveX(const float px)			   { Position.x += px; }								 // Move object to position
		void Model3d_2::MoveY(const float py)			   { Position.y += py; }								 // Move object to position
		void Model3d_2::MoveZ(const float pz)			   { Position.z += pz; }								 // Move object to position
		void Model3d_2::SetPosX(const float px)			   { Position.x = px; }									 // Set model position given a vector
		void Model3d_2::SetPosY(const float py)			   { Position.y = py; }									 // Set model position given a vector
		void Model3d_2::SetPosZ(const float pz)			   { Position.z = pz; }									 // Set model position given a vector
		void Model3d_2::SetAngles(const glm::vec3& angles) { Rotation  = angles * vml::math::DEGTORAD; }		 // Rotation angles
		void Model3d_2::AddAngles(const glm::vec3& angles) { Rotation += angles * vml::math::DEGTORAD; }		 // Add rotation angles
		void Model3d_2::SetAngleX(const float ax)		   { Rotation.x = ax * vml::math::DEGTORAD; }			 // Set Rotation angle x
		void Model3d_2::SetAngleY(const float ay)		   { Rotation.y = ay * vml::math::DEGTORAD; }			 // Set Rotation angle y
		void Model3d_2::SetAngleZ(const float az)		   { Rotation.z = az * vml::math::DEGTORAD; }			 // Set Rotation angle z
		void Model3d_2::AddAngleX(const float ax)		   { Rotation.x += ax * vml::math::DEGTORAD; }			 // Increase Rotation angle x
		void Model3d_2::AddAngleY(const float ay)		   { Rotation.y += ay * vml::math::DEGTORAD; }			 // Increase Rotation angle y
		void Model3d_2::AddAngleZ(const float az)		   { Rotation.z += az * vml::math::DEGTORAD; }			 // Increase Rotation angle z
		void Model3d_2::SetScaleX(const float sx)		   { Scaling.x = sx; SetScale(Scaling); }				 // Set Model x scale
		void Model3d_2::SetScaleY(const float sy)		   { Scaling.y = sy; SetScale(Scaling); }				 // Set Model y scale
		void Model3d_2::SetScaleZ(const float sz)		   { Scaling.z = sz; SetScale(Scaling); }				 // Set Model z scale
		void Model3d_2::AddScaleX(const float sx)		   { Scaling.x *= sx; SetScale(Scaling); }				 // Increase Model x scale
		void Model3d_2::AddScaleY(const float sy)		   { Scaling.y *= sy; SetScale(Scaling); }				 // Increase Model y scale
		void Model3d_2::AddScaleZ(const float sz)		   { Scaling.z *= sz; SetScale(Scaling); }				 // Increase Model z scale
		
		void Model3d_2::SetScale(const glm::vec3 &scale)
		{
			std::vector<Model3d_2*> stack;
			stack.emplace_back(this);
			stack[0]->Scaling *= scale;
			for (size_t i = 0; i < stack.size(); ++i)
			{
				for (size_t j = 0; j < stack[i]->Child.size(); ++j)
				{
					Model3d_2 *child = stack[i]->Child[j];
					child->Scaling *= scale;
					child->SetPosition(child->GetPosition() * scale);
					stack.emplace_back(child);
				}
			}
		//	for (size_t i = 0; i < stack.size(); ++i)
		//	{
		//		if ( stack[i]->GetParent())
		//			std::cout << "Parent : " << stack[i]->GetParent()->GetScreenName() << " , " << stack[i]->GetScreenName() << std::endl;
		//		else
		//			std::cout << "Root , " << stack[i]->GetScreenName() << std::endl;
		//	}
		}

		// --------------------------------------------------------------------------------

		void Model3d_2::SetTextureMatrixScale(const glm::vec2& scale) { TM[0][0] = scale.x;	TM[0][1] = 0; TM[1][0] = 0; TM[1][1] = scale.y; }
		
		// --------------------------------------------------------------------------------

		void Model3d_2::ResetScaling()	 { Scaling  = OriginalScaling;  }	// Resets model scale
		void Model3d_2::ResetPosition()  { Position = OriginalPosition; }	// Reset model position
		void Model3d_2::ResetRotation()  { Rotation = OriginalRotation; }	// Reset angle rotation with initial values
		
		// -------------------------------------------------------------
		// reset all transofrmations

		void Model3d_2::ResetTransformations()
		{
			Position = OriginalPosition;
			Rotation = OriginalRotation;
			Scaling  = OriginalScaling;
		}
		
		// -------------------------------------------------------------
		// compute transform matrix

		void Model3d_2::ComputeMatrix()
		{
			if (PreferencesFlags.Get(EULER))				ComputeEulerMatrix();
			if (PreferencesFlags.Get(QUATERNION))			ComputeQuaternionMatrix();
			if (PreferencesFlags.Get(SPHERICAL_QUATERNION)) ComputeSphericalQuaternionMatrix();
		}
		
		// -------------------------------------------------------------
		// add diffuse texture

		vml::textures::Texture* Model3d_2::AddDiffuseTexture(const std::string& filename, const vml::utils::Flags &flags=vml::textures::DefaultTexturesParms)
		{
			if ( DiffuseTexture)
				vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());
			DiffuseTexture = vml::stores::TextureStore->Load<vml::textures::Texture>(filename,flags);
			return DiffuseTexture;
		}
		
		// -------------------------------------------------------------

		void Model3d_2::RemoveDiffuseTexture()
		{
			if (DiffuseTexture)
			{
				vml::stores::TextureStore->UnLoad(DiffuseTexture->GetResourceFileName());
				DiffuseTexture = nullptr;
			}
		}
		
		// ---------------------------------------------------------------
		// dumps mesh

		std::string Model3d_2::Dump()
		{
			if (!Initialized)
				vml::os::Message::Error("Model : Model is not initialized");
			std::string text = "ScreenName    : " + GetScreenName() + "\n";
			if (Parent)
				text += "Parent	      : Not a Root Model\n";
			else if (!Parent)
				text += "Parent	      : Root Model\n";
			bool visible = PreferencesFlags.Get(NOT_VISIBLE);
			if ( visible )
				text += "Visible       : Yes\n";
			else if (!visible)
				text += "Visible       : No\n";
			if ( PreferencesFlags.Get(EULER))
				text += "Rotation Mode : Euler\n";
			if (PreferencesFlags.Get(SPHERICAL_QUATERNION))
				text += "Rotation Mode : Quaternion\n";
			bool wire = PreferencesFlags.Get(WIREFRAME);
			if (wire)
				text += "Wire Mode      : Yes\n";
			else if (!wire)
				text += "Wire Mode      : No\n";
			text += std::format("Position      : {} , {} , {}\n", Position.x, Position.y, Position.z);
			text += std::format("Rotation      : {} , {} , {}\n", Rotation.x, Rotation.y, Rotation.z);
			text += std::format("Scaling       : {} , {} , {}\n", Scaling.x, Scaling.y, Scaling.z);
			text += std::format("Quaternion    : {} , {} , {} , {}\n", Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w);
			text += std::format("Meshes        : {}\n", Meshes.size());
			text += std::format("CurrentMesh   : {}\n", CurrentMesh);
			return text;
		}
		
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

		Model3d_2::Model3d_2(const std::string& screenname,
							 const std::string& filename,
							 const glm::vec3& pos,
							 const glm::vec3& angles,
							 const glm::vec3& scaling,
							 const vml::utils::Flags& preferencesflags)
		{

			if (screenname.empty())
				vml::os::Message::Error("Model : Cannot assign empty screenname");
			if (filename.empty())
				vml::os::Message::Error("Model : Null mesh resource filename is not allowed fro model ' ", ScreenName.c_str()," '");

			// init member data

			ScreenName		  = screenname;
			Parent            = nullptr;
			Initialized		  = false;
			PreferencesFlags  = preferencesflags;
			CullingFlags	  = 0;
			CurrentMesh       = 0;

			// textures

			DiffuseTexture	  = nullptr;

			// set model transform data

			Position	     = pos;
			OriginalPosition = Position;
			Rotation		 = angles * vml::math::DEGTORAD;
			OriginalRotation = Rotation;
			Scaling		     = scaling;
			OriginalScaling  = Scaling;
			Quaternion		 = glm::quat(0,0,0,0);

			// set scaling

			if (fabs(Scaling.x) < vml::math::EPSILON ||
				fabs(Scaling.y) < vml::math::EPSILON ||
				fabs(Scaling.z) < vml::math::EPSILON)
					vml::os::Message::Error("Model : Scale cannot be negative or null for model ' ", ScreenName.c_str(), " '");

			// if angles flag is not euler, quaternion or spherical polar, emit error

			if (!( PreferencesFlags.Get(EULER) && !PreferencesFlags.Get(SPHERICAL_QUATERNION) && !PreferencesFlags.Get(QUATERNION)) &&
				!(!PreferencesFlags.Get(EULER) &&  PreferencesFlags.Get(SPHERICAL_QUATERNION) && !PreferencesFlags.Get(QUATERNION)) &&
				!(!PreferencesFlags.Get(EULER) && !PreferencesFlags.Get(SPHERICAL_QUATERNION) &&  PreferencesFlags.Get(QUATERNION)))
			{
				vml::os::Message::Error("Model : Choose rotation mode for model ' ", ScreenName.c_str(), " '");
			}

			// uploads the requested mesh
			// as default loading a mesh within a model
			// releases vertex data *see mesh class*

			Meshes.emplace_back(vml::stores::MeshStore->Load<vml::meshes::Mesh3d>(filename, {}));

			// init matrices

			M   = glm::mat4(0);
			R	= glm::mat4(0);
			MV  = glm::mat4(0);
			MVP = glm::mat4(0);
			NV  = glm::mat3(0);
			TM  = glm::mat2(1, 0, 0, 1);

			// init direction vector

			Right   = glm::vec3(1, 0, 0);
			Up      = glm::vec3(0, 1, 0);
			Forward = glm::vec3(0, 0, 1);

			// initialise metrics

			AOBoundingBox = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
			AABoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
			Radius		  = 0.0f;

			Visited		  = false;

			// flags

			Initialized = true;
		}

		// ------------------------------------------------------------------
		// Your base class destructor will always be automatically
		// called upon deletion of the derived instance since
		// destructors don't take parameters.
		// If you're using polymorphism and your derived
		// instance is pointed to by a base class pointer,
		// then the derived class destructor is only
		// called if the base destructor is virtual.

		Model3d_2::~Model3d_2()
		{
			ReleaseAll();
		}
	}
}

