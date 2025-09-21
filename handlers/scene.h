#pragma once

namespace vml
{
	namespace handlers
	{
		class Scene
		{

			private:
				
				// -------------------------------------------------------------------
				// business logic

				std::string  Name;
				
				// -------------------------------------------------------------------
				// scene handlers

				vml::scenes::Level* Level;

				// -------------------------------------------------------------------
				//

				void ReleaseAll()
				{
					vml::logger::Logger2::GetInstance()->Info({ "Scene","Releasing Scene : '" + Name + "'"});
					vml::os::SafeDelete(Level);
				}
				
			public:
					
				// -------------------------------------------------------------------
				//

				vml::scenes::Level* GetLevel() const
				{
					if (Level) {
						return Level;
					}
					else {
						vml::os::Message::Error("Scene : ", "Level is not loaded");
					}
					return nullptr;
				}
				
				// -------------------------------------------------------------------
				//

				vml::octree::OctTree* GetLevelOctTree() const
				{
					if (Level)
					{
						if (Level->GetOctTree()) {
							return Level->GetOctTree();
						}
						else {
							vml::os::Message::Error("Scene : ", "Octree is not valid");
						}
					}
					else {
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					}
					return nullptr;
				}

				/*
				// -------------------------------------------------------------------
				//

				vml::meshes::Mesh3d* GetLevelNavMesh() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level->GetNavMesh();
				}
				*/
				// -------------------------------------------------------------------
				//
				/*
				vml::meshes::Mesh3d* GetLevelCollisionMesh() const
				{
					if (!vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					return Level->GetCollisionMesh();
				}
				*/
				// -------------------------------------------------------------------
				//
				
				void Clear()
				{
					// release memory
					vml::os::SafeDelete(Level);

					// reallocate classes
					Level  = new vml::scenes::Level();

					// log message
					vml::logger::Logger2::GetInstance()->Info({ "Scene","Clearing Scene : '" + Name + "'" });
				}
				
				// -------------------------------------------------------------------
				// ctor / dtor
				
				Scene(const std::string &name)
				{
					if ( name.empty())
						vml::os::Message::Error("Scene : Name cannot be null");

					// set name
					Name = vml::strings::StringUtils::TrimAndLower(name);

					// allocate level class
					Level = new vml::scenes::Level();

					// log out
					vml::logger::Logger2::GetInstance()->Info({ "Scene","Initting Scene : '" + Name + "'"});
				}

				virtual ~Scene()
				{
					ReleaseAll();
				}
		};

	}
}
