#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////

#include <vml5.0/math/2d/pathfinder.h>
#include <vml5.0/octree/octree.h>

namespace vml
{
	namespace scenes
	{
		
		// -------------------------------------------------------------------
		// Level class, this class holds everything about 
		// the level mesh, collision and navigation data

		class Level
		{
			
			private:

				std::string			    LevelName;
				std::string			    MainPath;
				std::string			    MapMeshFileName;
				std::string			    ColMeshFileName;
				std::string			    NavMeshFileName;
				std::string			    NavMaskFileName;
				bool				    Initted;
				vml::meshes::Mesh3d*    MapMesh;
				vml::meshes::Mesh3d*    CollisionMesh;
				vml::meshes::Mesh3d*    NavMesh;
				vml::octree::OctTree*   OctTree;
				vml::geo2d::PathFinder* PathFinder;
				
				// -------------------------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					vml::os::SafeDelete(MapMesh);
					vml::os::SafeDelete(CollisionMesh);
					vml::os::SafeDelete(NavMesh);
					vml::os::SafeDelete(OctTree);
					vml::os::SafeDelete(PathFinder);
				}
				
			public:
				
				// -------------------------------------------------------------------
				// 
				
				bool Load(const std::string& levelname,const float scale=1.0f)
				{
					
					if (levelname.empty())
						vml::os::Message::Error("Level : ", "FileName is empty");
					
					// reset data

					ReleaseAll();

					// set filname

					LevelName = levelname;
					MainPath = vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/levels/" + LevelName;
					MapMeshFileName = MainPath + "/" + LevelName + ".3df";
					ColMeshFileName = MainPath + "/" + LevelName + "_col.3df";
					NavMeshFileName = MainPath + "/" + LevelName + "_nav.3df";
					NavMaskFileName = MainPath + "/" + LevelName + "_nav_mask.nvm";
					
				//	std::cout << MapMeshFileName << std::endl;

					vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Level : " + LevelName});
					vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Map Mesh : " + MapMeshFileName});
					vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Collision Mesh : " + ColMeshFileName });
					vml::logger::Logger2::GetInstance()->Info({ "Level","Loading NavMesh : " + NavMeshFileName });

					// allocate meshes
					
					MapMesh = new vml::meshes::Mesh3d(MapMeshFileName, { vml::meshes::DO_NOT_RELEASE_DATA },scale);
					
					// Collison mesh and navmesh are not mandatory
					
					// load collision mesh

					if (std::filesystem::exists(ColMeshFileName)) 
					{
						vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Collision Mesh : " + ColMeshFileName });
						CollisionMesh = new vml::meshes::Mesh3d(ColMeshFileName, { vml::meshes::DO_NOT_RELEASE_DATA },scale);
					}
					
					// load nav mesh, if nav mesh exists , create pathfinder

					if (std::filesystem::exists(NavMeshFileName)) 
					{
						vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Nav Mesh : " + NavMeshFileName });
						NavMesh = new vml::meshes::Mesh3d(NavMeshFileName, {},scale);
						// load bitmap mask for pathfinding 
						PathFinder = new vml::geo2d::PathFinder(NavMaskFileName,scale);
					}
					
					// compile level

					vml::logger::Logger2::GetInstance()->Info({ "Level","Octree Compiling : " + LevelName });
					
					// create octree

					OctTree = new vml::octree::OctTree;
					
					// compile map

					OctTree->Go(MapMesh);

					// log out
					
					vml::logger::Logger2::GetInstance()->Info({ "Level","Octree Compiling : " + LevelName + " : Done"});

					// we can release map mesh data since we don't use it anymore once the octree is computed
					
					MapMesh->ReleaseVertexBufferData();
					
					// log out

					vml::logger::Logger2::GetInstance()->Info({ "Level","Loading Level : " + LevelName + " : Done"});
	
					// set flags

					Initted = true;

					return true;
				}
				
				// -------------------------------------------------------------------
				// getters

				vml::meshes::Mesh3d*    GetCollisionMesh()     const { return CollisionMesh; }
				vml::meshes::Mesh3d*    GetNavMesh()           const { return NavMesh; }
				const std::string&      GetLevelName()	       const { return LevelName; }
				const std::string&      GetColMeshFileName()   const { return ColMeshFileName; }
				const std::string&      GetNavMeshFileName()   const { return NavMeshFileName; }
				const std::string&      GetMapMeshFileName()   const { return MapMeshFileName; }
				vml::octree::OctTree*   GetOctTree()           const { return OctTree; }
				vml::geo2d::PathFinder* GetPathFinder()        const { return PathFinder; }
				vml::meshes::Mesh3d* GetMapMesh() const { 
					if (!MapMesh->IsDataRetained())
						vml::os::Message::Error("Mesh data is not set to be retained");
					return MapMesh; 
				}

				// -------------------------------------------------------------------
				//

				bool IsInitted() const
				{
					return Initted;
				}
				
				// -------------------------------------------------------------------
				// ctor / dtor

				Level()
				{
					Initted		   = false;
					MapMesh        = nullptr;
					CollisionMesh  = nullptr;
					NavMesh		   = nullptr;
					OctTree        = nullptr;
					PathFinder	   = nullptr;
					vml::logger::Logger2::GetInstance()->Info({ "Level","Initting Level" });
				}

				~Level()
				{
					ReleaseAll();
					vml::logger::Logger2::GetInstance()->Info({ "Level","Releasing Level" });
				}

		};
		
	}
}
