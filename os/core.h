#pragma once

namespace vml
{

	class Core
	{
		
		private:
			
			// -----------------------------------------------------------------------------
			// private data
			
			vml::utils::Flags PreferencesFlags;	// preferences flags
			std::clock_t	  ClockStart;		// session time
			vml::os::Timer    Timer;			// timer class
			bool			  Initialized;		// Initialisation flag

			// -----------------------------------------------------------------------------
			// closes Core

			void Close() const
			{
				// get logger singleton
				vml::logger::Logger2* logger = vml::logger::Logger2::GetInstance();

				// close global paths singleton
				vml::utils::GlobalPaths::GetInstance()->Close();

				// log out results
				logger->Info({ "Core","Shutting Down" });

				// log out logger closing
				logger->Info({ "Core","Shutting Down Logger" });

				// close logger 
				logger->Close();
			}

		public:
			
			//-----------------------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed 

			Core(Core& other) = delete;

			//-----------------------------------------------------------------------------------
			// overload operator is private,
			// no copies allowed since classes
			// are referenced

			Core &operator=(const Core& other) = delete;
						
			//-----------------------------------------------------------------------------------
			// class is non mmovable

			Core(Core&& other) = delete;
			Core& operator=(Core&& other) = delete;

			// -----------------------------------------------------------------------------
			// getters

			[[nodiscard]] bool					   IsInitialized()					     const { return Initialized; }
			[[nodiscard]] bool					   IsLogVerbose()					     const { return vml::logger::Logger2::GetInstance()->IsVerbose(); }
			[[nodiscard]] bool					   IsLogQuiet()						     const { return vml::logger::Logger2::GetInstance()->IsQuiet(); }
			[[nodiscard]] bool					   IsDebugLogToFile()				     const { return vml::logger::Logger2::GetInstance()->IsLogModeFile(); }
			[[nodiscard]] bool					   IsDebugLogToStdout()				     const { return vml::logger::Logger2::GetInstance()->IsLogModeStd(); }
			[[nodiscard]] bool					   IsDebugLogToMem()				     const { return vml::logger::Logger2::GetInstance()->IsLogModeMem(); }
			[[nodiscard]] bool					   IsUserSignatureValid()		         const { return !vml::utils::GlobalPaths::GetInstance()->GetUserSignature().empty(); }
			[[nodiscard]] bool					   IsProjectPathValid()				     const { return !vml::utils::GlobalPaths::GetInstance()->GetProjectPath().empty(); }
			[[nodiscard]] bool					   IsProjectNameValid()				     const { return !vml::utils::GlobalPaths::GetInstance()->GetProjectName().empty(); }
			[[nodiscard]] bool					   IsFullProjectPathValid()			     const { return !vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath().empty(); }
			[[nodiscard]] bool				 	   AreProjectDirsValid()			     const { return !vml::utils::GlobalPaths::GetInstance()->GetProjectDirs().empty(); }
			[[nodiscard]] const std::string&	   GetUserSignature()				     const { return vml::utils::GlobalPaths::GetInstance()->GetUserSignature(); }
			[[nodiscard]] const std::string&	   GetProjectName()					     const { return vml::utils::GlobalPaths::GetInstance()->GetProjectName(); }
			[[nodiscard]] const std::string&	   GetProjectPath()					     const { return vml::utils::GlobalPaths::GetInstance()->GetProjectPath(); }
			[[nodiscard]] const std::string 	   GetFullProjectPath()				     const { return vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath(); }
			[[nodiscard]] const std::string 	   GetFullDebugPath()				     const { return vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath(); }
			[[nodiscard]] float					   GetElapsedTime()					     const { return std::clock() - ClockStart; }
			[[nodiscard]] float					   GetTime()						     const { return std::clock(); }
			[[nodiscard]] float					   GetFps()								       { return Timer.GetFPS(); }
			[[nodiscard]] const std::string		   GetFpsString()							   { return std::to_string(Timer.GetFPS()); }
			[[nodiscard]] const std::string&	   GetProjectDir(const std::string& dir) const { return vml::utils::GlobalPaths::GetInstance()->GetProjectDir(dir); }
			[[nodiscard]] std::vector<std::string> GetProjectDirs()					     const { return vml::utils::GlobalPaths::GetInstance()->GetProjectDirs();}
			[[nodiscard]] const vml::utils::Flags& GetPreferencesFlags()				 const { return PreferencesFlags; }

			// -----------------------------------------------------------------------------
			// init core
			
			void Init(const std::string& username, const std::string& projectpath, const std::string& projectname, const vml::utils::Flags &flags)
			{
				if (Initialized)
					vml::os::Message::Error("Core : Cannot restart context before it has not been closed");

				if (username.empty())
					vml::os::Message::Error("Core : UserName is missing");
				if (projectpath.empty())
					vml::os::Message::Error("Core : Project path is missing");
				if (projectname.empty())
					vml::os::Message::Error("Core : Project Name is missing");

				// set preferences flags
				
				PreferencesFlags = flags;

				// extract flags

				bool verbose     = PreferencesFlags.Get(vml::flags::Preferences::VERBOSE);
				bool logtostdout = PreferencesFlags.Get(vml::flags::Preferences::LOG_TO_STDOUT);
				bool logtofile   = PreferencesFlags.Get(vml::flags::Preferences::LOG_TO_FILE);
				bool logtomem    = PreferencesFlags.Get(vml::flags::Preferences::LOG_TO_MEM);
		
				// check if debug flag is set

				if (logtostdout && logtofile)
					vml::os::Message::Error("Core : Debug mode mismatch/not set");

				// get logger singleton

				vml::logger::Logger2 *logger = vml::logger::Logger2::GetInstance();

				// Inits logger accoridng to log mode

				if (logtostdout) logger->Init(vml::logger::LogModes::TO_STD , verbose ? vml::logger::VerboseModes::VERBOSE : vml::logger::VerboseModes::QUIET);
				if (logtomem)	 logger->Init(vml::logger::LogModes::TO_MEM , verbose ? vml::logger::VerboseModes::VERBOSE : vml::logger::VerboseModes::QUIET);
				if (logtofile)	 logger->Init(vml::logger::LogModes::TO_FILE, verbose ? vml::logger::VerboseModes::VERBOSE : vml::logger::VerboseModes::QUIET, "log.txt");

				// add two clomuns

				logger->AddColumn("Column1", 20);
				logger->AddColumn("Column2", 0);

				// log out that core has been initted correctly

				logger->Info({"Core","Initializing"});

				// log out verbose mode

				logger->Info({ "Core", logger->IsVerbose() ? "Verbose flag set to \"True\"" : "Verbose flag set to \"False\"" });

				// log logger mode

				if (logger->IsLogModeStd() ) logger->Info({ "Core", "Logger output set to 'Stdout'"});
				if (logger->IsLogModeFile()) logger->Info({ "Core", "Logger output set to 'File'" });
				if (logger->IsLogModeMem())  logger->Info({ "Core", "Logger output set to 'Memory'" });

				// init global paths

				logger->Info({ "Core","Initializing GlobalPaths" });
					vml::utils::GlobalPaths::GetInstance()->Init();
				logger->Info({ "Core","GlobalPaths initialized" });

				// init timer

				logger->Info({ "Core","Initializing Performance Timer" });
					Timer.Init();
				logger->Info({ "Core","Performance Timer initialized" });

				//start measuring if verbose mode is set to true
		
				vml::os::SystemInfo SystemInfo;
				SystemInfo.Measure();
				logger->Info({ "SystemInfo","Retrieving System Info : In progress" });
				logger->Info({ "SystemInfo","Vendor: " + SystemInfo.GetCpuVendorString() });
				logger->Info({ "SystemInfo","CodeName: " + SystemInfo.GetCpuCodeNameString() });
				logger->Info({ "SystemInfo","Brand: " + SystemInfo.GetCpuBrandString() });
				logger->Info({ "SystemInfo","VendorId: " + std::to_string(SystemInfo.GetVendorId()) });
				logger->Info({ "SystemInfo","CodeNameId: " + std::to_string(SystemInfo.GetCodeNameId()) });
				logger->Info({ "SystemInfo","Stepping: " + std::to_string(SystemInfo.GetStepping()) });
				logger->Info({ "SystemInfo","Family: " + std::to_string(SystemInfo.GetFamily()) });
				logger->Info({ "SystemInfo","Model: " + std::to_string(SystemInfo.GetModel()) });
				logger->Info({ "SystemInfo","CPUMaxFunction: " + std::to_string(SystemInfo.GetCPUIdMaxFunction()) });
				logger->Info({ "SystemInfo","Processr Type : " + std::to_string(SystemInfo.GetProcessorType()) });
				logger->Info({ "SystemInfo","Extended Model : " + std::to_string(SystemInfo.GetExtendedModel()) });
				logger->Info({ "SystemInfo","Extended Family : " + std::to_string(SystemInfo.GetExtendedFamily()) });
				logger->Info({ "SystemInfo","Brand Index : " + std::to_string(SystemInfo.GetBrandIndex()) });
				logger->Info({ "SystemInfo","CLFLUSH Cache line size : " + std::to_string(SystemInfo.GetCLFLUSHCacheLineSize()) });
				logger->Info({ "SystemInfo","API Physical ID : " + std::to_string(SystemInfo.GetAPICPhysicalID()) });
				logger->Info({ "SystemInfo","Cache line size : " + std::to_string(SystemInfo.GetCacheLineSize()) });
				logger->Info({ "SystemInfo","L2 Associativity : " + std::to_string(SystemInfo.GetL2Associativity()) });
				logger->Info({ "SystemInfo","Cache size : " + std::to_string(SystemInfo.GetCacheSize()) });
				logger->Info({ "SystemInfo","Physical address : " + std::to_string(SystemInfo.GetPhysicalAddress()) });
				logger->Info({ "SystemInfo","Virtual address : " + std::to_string(SystemInfo.GetVirtualAddress()) });
				logger->Info({ "SystemInfo","Logical processor(s) number : " + std::to_string(SystemInfo.GetLogicalProcessorsNumber()) });
				logger->Info({ "SystemInfo","Retrieving System Info : Done" });
				
				// log out that core has been initted correctly
				
				logger->Info({ "Core","Initialized" });

				// Set project details

				vml::utils::GlobalPaths::GetInstance()->SetCurrentUserSignature(username);
				vml::utils::GlobalPaths::GetInstance()->SetProjectPath(projectpath);
				vml::utils::GlobalPaths::GetInstance()->SetProjectName(projectname);
				vml::utils::GlobalPaths::GetInstance()->CreateProjectDirs({ "icons","fonts","levels","meshes","textures","shaders","sounds","source" });

				// get logger singleton

				logger->Info({ "UserDetails", Initialized ? "Project : Initialized" : "Project : Not initialized" });
				logger->Info({ "UserDetails", IsUserSignatureValid() ? "User Signature : '" + GetUserSignature() + "'" : "User Signature not set" });
				logger->Info({ "UserDetails", IsProjectPathValid() ? "UserDetails","Project Path : '" + GetProjectPath() + "'" : "Project Path not set" });
				logger->Info({ "UserDetails", IsProjectNameValid() ? "UserDetails","Project Name : '" + GetProjectName() + "'" : "Project Name not set" });
				logger->Info({ "UserDetails", IsFullProjectPathValid() ? "UserDetails","Full Project Path : '" + GetFullProjectPath() + "'" : "Full Project Path : Not set" });

				if (AreProjectDirsValid())
				{
					std::vector<std::string> dirs = GetProjectDirs();
					for (size_t i = 0; i < dirs.size(); ++i)
						logger->Info({ "UserDetails","Project Dir(" + std::to_string(i) + ") : '" + dirs[i] + "'" });
				}
				else
				{
					logger->Info({ "UserDetails","Project Dirs not set" });
				}

				// set context as started
				
				Initialized = true;
			}

			// -----------------------------------------------------------------------------
			// ctor / dtor

			Core()
			{
				ClockStart		 = std::clock();
				Initialized      = false;
				PreferencesFlags = vml::utils::Flags();
			}

			virtual ~Core()
			{
				Close();
			}

	};

} // end of vml namespace


