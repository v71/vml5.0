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

namespace vml
{
	namespace models
	{
		////////////////////////////////////////////////////////////////////////////

		class ModelHandler
		{

			private:

				//------------------------------------------------------------------

				static const uint32_t MaximumNumberOfObjects = 32678;

				//------------------------------------------------------------------

				std::vector<vml::models::Model3d_2*> Stack;				  // Stacked breadth-first hierarchyical transformation
				std::vector<vml::models::Model3d_2*> RenderedModels;	  // Rendered models array
				std::vector<vml::models::Model3d_2*> Models;			  // Models array
				int									 RenderedModelsCount; // Keep traces of how many models are being rendered
				int									 Last;				  // Last stack index
				int									 First;				  // First stack index
				int									 CurrentModel;		  // Current model
				bool								 Finalized;			  // Finalized flag
				bool							     Initialized;		  // Initilized flag

				//------------------------------------------------------------------
				// release all items

				void ReleaseAll()
				{
					vml::logger::Logger2::GetInstance()->Info({ "ModelHandler","Releasing" });

					// meshes are released through the resource manager
					for (auto it = Models.begin(); it!=Models.end(); ++it)
						vml::os::SafeDelete(*it);

					// delete models array
					Models.clear();

					// delete stack
					Stack.clear();
					RenderedModels.clear();

					vml::logger::Logger2::GetInstance()->Info({ "ModelHandler","Released" });
				}

				//------------------------------------------------------------------
				// Find first non visited node , which is a parent node

				vml::models::Model3d_2* FindParent()
				{
					for (auto it = Models.begin(); it != Models.end(); ++it)
						if (!(*it)->Visited)
							if (!(*it)->GetParent())
								return (*it);
					return nullptr;
				}

				//------------------------------------------------------------------
				// visit parent node to populate the stack with
				// parent's children

				void PopulateStack(vml::models::Model3d_2* node)
				{
					node->Visited = true;
					Stack[Last++] = node;
					for (size_t i = First; i<Last; ++i)
					{
						for (size_t j = 0; j<Stack[i]->GetChildCount(); ++j)
						{
							vml::models::Model3d_2* child = Stack[i]->GetChild(j);
							Stack[Last++] = child;
							child->Visited = true;
						}
					}
					First = Last;
				}

				//------------------------------------------------------------------
				// Compute stack , all linked models are transform in a strict
				// hierarchical order , if a model is not attached to any other
				// object , its considered an isolated node

				void ComputeStack()
				{
					if (Models.size() == 0)
						return;

					// clear visited flag
					
					for (size_t i = 0; i < Models.size(); ++i)
						Models[i]->Visited = false;
						
					vml::models::Model3d_2* parent;

					First  = 0;
					Last   = 0;
					parent = nullptr;

					// populate stack and create compound bounding box
					// for each model 

					do
					{
						parent = FindParent();
						if (parent)
							PopulateStack(parent);
					} while (parent);

					//	std::cout << "Models : " << Models.size() << " , StackSize : " << Last << std::endl;
					//	for (size_t i = 0; i < Last; ++i)
					//	{
					//		if (Stack[i]->GetParent())
					//			std::cout << "# " << i << " Stack [ " << Stack[i]->GetScreenName() << " ] , Parent [ " << Stack[i]->GetParent()->GetScreenName() << " ]\n";
					//		else
					//			std::cout << "# " << i << " Stack [ " << Stack[i]->GetScreenName() << " ]\n";
					//	}
				}

				// ------------------------------------------------------------------
				// recursively traverse the model tree composed by linked children model(s)

				void DumpModelRecursive(vml::models::Model3d_2* model, int depth = 0)
				{
					if (!model)
						return;

					if (model->Visited)
						return;

					// Print indentation spaces

				//	for (int i = 0; i < depth; ++i)
				//		std::cout << "    "; // 4 spaces per depth level

					// print model screenname

				//	std::cout << model->GetScreenName() << std::endl;
					model->Visited = true;

					// recurse

					for (size_t i = 0; i < model->GetChildCount(); ++i)
						DumpModelRecursive(model->GetChild(i), depth + 1);
				}

			public:

				//-------------------------------------------------------------------------
				// adds a model to the current object

				[[nodiscard]] vml::models::Model3d_2* Add(const std::string& screenname,
														  const std::string& filename,
														  const glm::vec3& pos,
														  const glm::vec3& rot,
														  const glm::vec3& scale,
														  const vml::utils::Flags& preferencesflags)
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (AddModel) , Handler is not initialized");

					if (Models.size() >= MaximumNumberOfObjects)
						vml::os::Message::Error("ModelHandler : Exceeded maximum number of objects (", MaximumNumberOfObjects, ")");

					// check if we already have this screenname taken
					for (auto it = Models.begin(); it != Models.end(); ++it)
						if (screenname == (*it)->GetScreenName())
							vml::os::Message::Error("ModelHandler : Model Instance '", screenname.c_str(), "' : ScreenName must be unique");

					// create a new model
					vml::models::Model3d_2* newmodel = new vml::models::Model3d_2(screenname, filename, pos, rot, scale, preferencesflags);

					// store this model into object's model array
					Models.emplace_back(newmodel);

					// recompute stack
					ComputeStack();

					// log out message
					vml::logger::Logger2::GetInstance()->Info({ "ModelHandler : ", "Added Model : '" + screenname + "'" });

					// return model pointer
					return newmodel;
				}

				// ----------------------------------------------------
				// tranform objects 

				void Transform(vml::views::View* view)
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (Transform ) , Handler is not initialized");

					// traverse stack

					RenderedModelsCount = 0;

					for (size_t i = 0; i < Last; ++i)
					{
						// get object pointer
						
						vml::models::Model3d_2* model = Stack[i];

						// transform objects

						model->Transform();
					
						// test for visibility only if the model is visible
						
						if (model->IsVisbile())
						{
							// cull models
						
							model->SetCullingFlags(vml::views::frustum::TestAABBox(view->GetFrustumPlanes(),
																				   model->GetAABoundingBox().GetCenter(),
																				   model->GetAABoundingBox().GetHalfExtents()));

							// if model is inside frustum , then transform to view
							
							if (model->GetCullingFlags() != vml::views::frustum::OUTSIDE)
							{
								model->TransformToView(view);
								RenderedModels[RenderedModelsCount++] = model;
							}
						}
					}
				}

				// -------------------------------------------------------------
				// link models

				void Link(vml::models::Model3d_2* parentmodel, vml::models::Model3d_2* childmodel) const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (Link) , Handler is not initialized");
					parentmodel->Link(childmodel);
				}
			
				// -----------------------------------------------------------------
				// Initialize handler

				void Init()
				{
					if (Initialized)
						vml::os::Message::Error("ModelHandler : (Init) , Handler is already initialized");
					// allocate stack 
					Stack.resize(MaximumNumberOfObjects);
					RenderedModels.resize(MaximumNumberOfObjects);
					Initialized = true;
				}

				//-------------------------------------------------------------
				// set current model

				vml::models::Model3d_2* SetCurrentModel(const int pos)
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (SetCurrentModel) , Handler is not initialized");
					if ( pos<0 || pos>=Models.size())
						vml::os::Message::Error("ModelHandler : (SetCurrentModel) , Current Model is out of range");
					CurrentModel = pos;
					return Models[CurrentModel];
				}

				// -----------------------------------------------------------------
				// get object array

				const std::vector<vml::models::Model3d_2*>* GetArray() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetModelsArray) , Handler is not initialized");
					return &Models;
				}

				// -----------------------------------------------------------------
				// gets number of objects

				size_t GetCount() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetObjectsCount) , Handler is not initialized");
					return Models.size();
				}

				//------------------------------------------------------------------
				// gets model by screen name

				vml::models::Model3d_2* GetAt(const std::string& fullpathscreenname) const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetModelAt) , Handler is not initialized");
					for (auto it = Models.begin(); it != Models.end(); ++it)
						if (fullpathscreenname == (*it)->GetScreenName())
							return (*it);
					return nullptr;
				}

				//------------------------------------------------------------------
				// gets model by position

				vml::models::Model3d_2* GetAt(const size_t pos) const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetModelAt) , Handler is not initialized");
					return Models[pos];
				}

				//-------------------------------------------------------------
				// increases currentmodel index and get model's pointer

				vml::models::Model3d_2* GetNextModel()
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetNextModel) , Handler is not initialized");
					CurrentModel++;
					int modelscount = (int)Models.size() - 1;
					if (CurrentModel > modelscount) CurrentModel = modelscount;
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// decreases currentmodel index and get model's pointer

				vml::models::Model3d_2* GetPrevModel()
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetPrevModel) , Handler is not initialized");
					CurrentModel--;
					if (CurrentModel <= 0) CurrentModel = 0;
					return Models[CurrentModel];
				}

				// -----------------------------------------------------------------
				// return finalized flag

				bool IsInitialized() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (IsInitialized) , Handler is not initialized");
					return Initialized;
				}

				// -----------------------------------------------------------------
				// return rendered models count

				int GetRenderedModelsCount() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetRenderedModelsCount) , Handler is not initialized");
					return RenderedModelsCount;
				}

				// -----------------------------------------------------------------
				// return Rendered models array

				const std::vector<vml::models::Model3d_2*> &GetRenderedModels() const	  
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetRenderedModels) , Handler is not initialized");
					return RenderedModels;
				}

				// -----------------------------------------------------------------
				// Models array

				const std::vector<vml::models::Model3d_2*> GetModels() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetModels) , Handler is not initialized");
					return Models;
				}

				// -----------------------------------------------------------------
				// return finalized flag

				bool IsFinalized() const
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (IsFinalized) , Handler is not initialized");
					return Finalized;
				}

				// ------------------------------------------------------------------
				// get all model which are root models

				std::vector<vml::models::Model3d_2*> GetRootModels()
				{
					if (!Initialized)
						vml::os::Message::Error("ModelHandler : (GetRootModels) , Handler is not initialized");

					std::vector<vml::models::Model3d_2*> modellist;

					if (Models.empty())
						return modellist;

					// if a model doesn't have a parent , its a root model

					for (auto& model : Models)
						if (!model->GetParent())
							modellist.emplace_back(model);

					return modellist;
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				ModelHandler()
				{
					RenderedModelsCount = 0;
					Last				= 0;
					First				= 0;
					CurrentModel		= 0;
					Finalized			= false;
					Initialized			= false;

					// log out constructor message
					vml::logger::Logger2::GetInstance()->Info({ "ModelHandler","Initting ModelHandler" });
				}

				~ModelHandler()
				{
					// log out destructor message
					vml::logger::Logger2::GetInstance()->Info({ "ModelHandler","Releasing ModelHandler" });
					ReleaseAll();
				}

		};
	}
}
