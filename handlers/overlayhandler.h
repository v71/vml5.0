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
	namespace overlays
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// array of effects overlay 
		// overlay must be rendered afeter all opaque objects have been rendered possibily in a sorted fashion
		// i am not considering sorting for themoment becasue all overlays will be rendered in the final stage

		class OverlayHandler
		{
			private:

				//------------------------------------------------------------------

				static const uint32_t MaximumNumberOfOverlays = 32678;

				// ----------------------------------------------------

				std::vector<EffectOverlay*> Overlays;			// Array
				bool					    Initialized;		// Initilized flag

				// ----------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Releasing" });

					// release views array
					for (auto it = Overlays.begin(); it != Overlays.end(); ++it)
					{
						vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Releasing :'" + (*it)->GetScreenName() + "'" });
						vml::os::SafeDelete(*it);
					}

					// clear views array
					Overlays.clear();

					vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Released" });
				}

			public:

				// ---------------------------------------------------
				// Initialize handler

				void Init()
				{
					if (Initialized)
						vml::os::Message::Error("OverlayHandler : (Init) , Handler is already initialized");
					Overlays.reserve(MaximumNumberOfOverlays);
					Initialized = true;
				}

				// ----------------------------------------------------
				// Add view
				
				[[nodiscard]] vml::overlays::EffectOverlay* Add(const uint32_t meshtype,
																const std::string& screenname,
																const std::string& texturepath,
																const glm::vec3& pos,
																const glm::vec3& rot,
																const glm::vec3& scaling)
				{
					
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (Add) , Handler is not initialized");

					if (Overlays.size() >= MaximumNumberOfOverlays)
						vml::os::Message::Error("OverlayHandler : Exceeded maximum number of views (", MaximumNumberOfOverlays, ")");

					// duplicates names for object aren't allowed

					for (auto it = Overlays.begin(); it != Overlays.end(); ++it)
						if ((*it)->GetScreenName() == screenname)
							vml::os::Message::Error("OverlayHandler : (Add) , Duplicated names for overlays are not allowed");

					vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Adding :'" + screenname + "'" });

					// if we get here, we can store the object in the pointer vector
					
					vml::overlays::EffectOverlay *overlay = new vml::overlays::EffectOverlay(meshtype,screenname,texturepath,pos, rot,scaling);

					// store this view
					Overlays.emplace_back(overlay);
					
					// return newly allocated view
					return overlay;
				}
				
				// -----------------------------------------------------------------
				// removes an overlay
				// * Remove effect overlay from attached mesh before attempting to 
				//   remove an effec t overlay *

				void Remove(const size_t pos)
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (RemoveOverlay) , Handler is not initialized");

					if (pos >= Overlays.size())
						vml::os::Message::Error("OverlayHandler : (RemoveOverlay) , View index is out of range");

					// release object memenory
					vml::os::SafeDelete(Overlays[pos]);

					// remove from array
					Overlays.erase(Overlays.begin() + pos);
				}

				// -----------------------------------------------------------------
				// remove all overlays

				void RemoveAll()
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (RemoveAllOverlays) , Handler is not initialized");

					ReleaseAll();
					vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Resetting Overlay Handler" });
				}

				// -----------------------------------------------------------------
				// retrieves objects given its name

				vml::overlays::EffectOverlay* Get(const size_t pos) const
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (GetViewAt) , Handler is not initialized");
					return Overlays[pos];
				}

				// -----------------------------------------------------------------
				// get overlay by screenanme

				vml::overlays::EffectOverlay* GetByName(const std::string& screenname) const
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (GetViewByName) , Handler is not initialized");
					for (auto it = Overlays.begin(); it != Overlays.end(); ++it)
						if ((*it)->GetScreenName() == screenname) return *it;
					return nullptr;
				}

				// -----------------------------------------------------------------
				// get overlays array

				const std::vector<vml::overlays::EffectOverlay*>* GetArray() const
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (GetOverlaysArray) , Handler is not initialized");
					return &Overlays;
				}

				// ---------------------------------------------------
				//

				void Transform(vml::views::View *view)
				{
					for (size_t i = 0; i < Overlays.size(); ++i)
						Overlays[i]->Transform(view);
				}

				// -----------------------------------------------------------------
				// gets number of objects

				size_t GetCount() const
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (GetOverlayCount) , Handler is not initialized");
					return Overlays.size();
				}

				// -----------------------------------------------------------------
				// return finalized flag

				bool IsInitialized() const
				{
					return Initialized;
				}

				// -----------------------------------------------------------------
				// dump objects

				void Dump()
				{
					if (!Initialized)
						vml::os::Message::Error("OverlayHandler : (Dump) ,  Handler is not initialized (11)");

					for (size_t i = 0; i < Overlays.size(); ++i)
						std::cout << i << " : " << Overlays[i]->GetScreenName() << std::endl;
				}

				// ---------------------------------------------------

				void Draw(vml::views::View* view)
				{
					for (size_t i = 0; i < Overlays.size(); ++i)
							Overlays[i]->Draw(view);
				}

				// ---------------------------------------------------
				// ctor / dtor

				OverlayHandler()
				{
					vml::logger::Logger2::GetInstance()->Info({ "OverlayHandler","Initting" });
					Initialized = false;
				}

				~OverlayHandler()
				{
					ReleaseAll();
				}

		};

	}

}


