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
	namespace views
	{
		class ViewHandler
		{

			private:

				//------------------------------------------------------------------

				static const uint32_t MaximumNumberOfViews = 32678;

				//------------------------------------------------------------------

				std::vector<vml::views::View*> Views;			// Views array	
				bool						   Initialized;		// Initilized flag

				// ----------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Releasing" });
				
					// release views array
					for (auto it = Views.begin(); it != Views.end(); ++it)
					{
						vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Releasing :'" + (*it)->GetScreenName() + "'" });
						vml::os::SafeDelete(*it);
					}
					
					// clear views array
					Views.clear();
					
					vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Released" });
				}

			public:

				// ----------------------------------------------------
				// Add view

				[[nodiscard]] vml::views::View *Add(const std::string& screenname,
												    float			   nearclippingplane,
												    float			   farclippingplane,
												    float			   fov,
												    const glm::vec3&   position,
												    const glm::vec3&   angles,
												    float			   translationspeed,
												    float			   rotationspeed,
												    const uint8_t	   rotationmode)
				{
					if(!Initialized)
						vml::os::Message::Error("ViewHandler : (Add) , Handler is not initialized");
			
					if (Views.size()>= MaximumNumberOfViews)
						vml::os::Message::Error("ViewHandler : Exceeded maximum number of views (", MaximumNumberOfViews, ")");

					// duplicates names for object aren't allowed

					for (auto it = Views.begin(); it != Views.end(); ++it)
						if ((*it)->GetScreenName() == screenname)
							vml::os::Message::Error("ViewHandler : (Add) , Duplicated names for views are not allowed");

					vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Adding :'" + screenname + "'" });

					// if we get here, we can store the object in the pointer vector
					
					vml::views::View *view=new vml::views::View(screenname,
															    nearclippingplane,
															    farclippingplane,
															    fov,
															    position,
															    angles,
															    translationspeed,
															    rotationspeed,
															    rotationmode);
					
					// store this view
					Views.emplace_back(view);

					// return newly allocated view
					return view;
				}

				// -----------------------------------------------------------------
				// retrieves objects given its name

				vml::views::View* Get(const size_t pos) const
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (GetViewAt) , Handler is not initialized");
					return Views[pos];
				}

				// -----------------------------------------------------------------
				// get objcet by screenanme

				vml::views::View* Get(const std::string& screenname) const
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (GetViewByName) , Handler is not initialized");
					for (auto it = Views.begin(); it != Views.end(); ++it)
						if ((*it)->GetScreenName() == screenname) return *it;
					return nullptr;
				}

				// -----------------------------------------------------------------
				// get object array

				const std::vector<vml::views::View*>* GetArray() const
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (GetViewsArray) , Handler is not initialized");
					return &Views;
				}

				// -----------------------------------------------------------------
				// gets number of objects

				size_t GetCount() const
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (GetViewsCount) , Handler is not initialized");
					return Views.size();
				}

				// -----------------------------------------------------------------
				// return finalized flag

				bool IsInitialized() const
				{
					return Initialized;
				}

				// -----------------------------------------------------------------
				// removes an object

				void Remove(const size_t pos)
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (RemoveView) , Handler is not initialized");

					if (pos >= Views.size())
						vml::os::Message::Error("ViewHandler : (RemoveView) , View index is out of range");

					// release object memenory 
					vml::os::SafeDelete(Views[pos]);

					// remove from array
					Views.erase(Views.begin() + pos);
				}

				// -----------------------------------------------------------------
				// remove all objects 

				void RemoveAll()
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (RemoveAllViews) , Handler is not initialized");

					ReleaseAll();
					vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Resetting View Handler" });
				}

				// -----------------------------------------------------------------
				// Initialize handler

				void Init()
				{
					if (Initialized)
						vml::os::Message::Error("ViewHandler : (Init) , Handler is already initialized");
					Views.reserve(MaximumNumberOfViews);
					Initialized = true;
				}

				// -----------------------------------------------------------------
				// dump objects

				void Dump()
				{
					if (!Initialized)
						vml::os::Message::Error("ViewHandler : (Dump) ,  Handler is not initialized (11)");

					for (size_t i = 0; i < Views.size(); ++i)
						std::cout << i << " : " << Views[i]->GetScreenName() << std::endl;
				}

				// ----------------------------------------------------
				// ctor / dtor

				ViewHandler()
				{
					vml::logger::Logger2::GetInstance()->Info({ "ViewHandler","Initting" });
					Initialized =false;
				}

				~ViewHandler()
				{
					ReleaseAll();
				}

		};

	}	// end of objects namespace

} // end of namespace vml
