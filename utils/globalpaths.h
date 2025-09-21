#pragma once

////////////////////////////////////////////////////////////////////////////
// Session class 

namespace vml
{
	namespace utils
	{
		
		///////////////////////////////////////////////////////////////////////////////////////
		// Thread safe logger

		class GlobalPaths
		{

			private:

				// ---------------------------------------------
				// singleton design pattern
				// A singlton is a class instantiated only once.
				// Usually it is globally accessiblae,
				// constructor and destructors are kept private to the 
				// final user. User has the responsability to delete
				// memory associated with the class explicitly at
				// main entry point closure

				static GlobalPaths*							 Singleton;			// Singleton pointer
				std::string								     UserSignature;		// user name
				std::string									 ProjectPath;		// project path
				std::string								     ProjectName;		// project name
				std::unordered_map<std::string, std::string> ProjectDirs;		// store generated directories	
				bool										 Intialized;

				// ---------------------------------------------
				// private ctor / dtor

				GlobalPaths()
				{
					Singleton = nullptr;
					Intialized = false;
				}
			
			public:
			
				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				GlobalPaths(GlobalPaths& other) = delete;

				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				GlobalPaths operator=(const GlobalPaths&) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				GlobalPaths(GlobalPaths&& other) = delete;
				GlobalPaths& operator=(GlobalPaths&& other) = delete;

				//---------------------------------------------------------------------
				// get instance of singleton , if singleton is null , which is mandatory at
				// application startup, it will be allocated only once
				// Eventually, to reallocate , user must call the delete member function

				static GlobalPaths *GetInstance()
				{
					if (!Singleton)
						Singleton = new GlobalPaths();
					return Singleton;
				}

				// -----------------------------------------------------------------------------

				void Init()
				{
					// check if class has been initialised
					if (IsInitialized())
						vml::os::Message::Error("GlobalPaths : Cannot restart GlobalPaths before it has not been closed");
					Intialized = true;
				}
				
				// -----------------------------------------------------------------------------
				// sets name for current user

				void SetCurrentUserSignature(const std::string& name)
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : GlobalPaths not initted(1)");
					if (name.empty())
						vml::os::Message::Error("GlobalPaths : UserName is empty");
					if (!UserSignature.empty())
						vml::os::Message::Error("GlobalPaths : UserName is already set");

					// set user name
					UserSignature = name;
				}
				
				// -----------------------------------------------------------------------------
				// set wrking directory and paths

				void SetProjectPath(const std::string& mainpath)
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : GlobalPaths not initted(2)");
					if (mainpath.empty())
						vml::os::Message::Error("GlobalPaths : Project Path is empty");
					if (!ProjectPath.empty())
						vml::os::Message::Error("GlobalPaths : Project Path is already set");

					// setting main path
					ProjectPath = std::string(std::filesystem::path({ mainpath }).string());

				}
				
				// -----------------------------------------------------------------------------
				// set project name

				void SetProjectName(const std::string& projectname)
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : Paths not initted(3)");
					if (projectname.empty())
						vml::os::Message::Error("GlobalPaths : Project name is empty");
					if (!ProjectName.empty())
						vml::os::Message::Error("GlobalPaths : Project Name is already set");
		
					// set project name
					ProjectName = projectname;
				}
				
				// -----------------------------------------------------------------------------
				// create projectdir

				void CreateProjectDirs(const std::vector<std::string>& paths)
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : Paths not initted(4)");
					if (paths.empty())
						vml::os::Message::Error("GlobalPaths : Dirs list is empty");
					if (GetProjectPath().empty())
						vml::os::Message::Error("GlobalPaths : Project path is missing");
					if (GetProjectName().empty())
						vml::os::Message::Error("GlobalPaths : Project Name is missing");

					// create directories for the project
					for (auto it = paths.begin(); it != paths.end(); ++it)
					{
						std::string fulldir = GetProjectPath() + "\\" + GetProjectName() + "\\" + vml::strings::StringUtils::MakeLower((*it)+"\\");

						// create the pair and insert into map
						ProjectDirs.insert(std::make_pair((*it), fulldir));

						// create physiucal dir
						std::filesystem::create_directories(fulldir);
					}
				}
				
				// -----------------------------------------------------------------------------
				// getters

				const std::string& GetUserSignature()	const { return UserSignature; }
				const std::string& GetProjectPath()		const { return ProjectPath; }
				const std::string& GetProjectName()		const { return ProjectName; }
				const std::string  GetFullProjectPath()	const { return ProjectPath + "/" + ProjectName; }
				const std::string  GetFullDebugPath()	const { return ProjectPath + "/debug"; }

				// ---------------------------------------------------------------------------------
				// returns true if globalpaths is initialized  

				bool IsInitialized() const
				{
					return Intialized;
				}

				// -----------------------------------------------------------------------------
				// list recursively all subdirs given a path

				std::vector<std::string> GetProjectDirs() const
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : GlobalPaths not initted(5)");
					if (GetProjectPath().empty())
						vml::os::Message::Error("GlobalPaths : Project Path is missing");
					if (GetProjectName().empty())
						vml::os::Message::Error("GlobalPaths : Project Name is missing");

					// enumerate project directories and store them in a vector
					std::filesystem::path path = vml::utils::GlobalPaths::GetInstance()->GetProjectPath() + "\\" + GetProjectName()+"\\";
					std::vector<std::string> dirs;
					for (const auto& p : std::filesystem::recursive_directory_iterator(path))
						if (std::filesystem::is_directory(p))
							dirs.emplace_back(p.path().generic_string());
					return dirs;
				}

				// ------------------------------------------------------------
				// this functions return the required dir
			
				const std::string GetProjectDir(const std::string& dir)
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : Paths not initted(6)");
					auto projdir = ProjectDirs.find(dir);
					if (projdir != ProjectDirs.end())
						return projdir->second;
					return std::string();
				}
				
				// ------------------------------------------------------------
				// closes stream

				void Close()
				{
					if (!IsInitialized())
						vml::os::Message::Error("GlobalPaths : Paths not initted(7)");
					Intialized=false;
					vml::os::SafeDelete(Singleton);
				}
				
				// ------------------------------------------------------------
				// ctor / dtor

				~GlobalPaths() {};

		};
	}
}

