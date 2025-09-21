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
	class SDLOpenGlApp : public vml::Core
	{

		private:

			// -------------------------------------------------------------------

			std::string       Title;
			int			      ScreenWidth;
			int			      ScreenHeight;
			vml::utils::Flags PreferencesFlags;
			SDL_Window*		  GlWindow;
			SDL_GLContext     GlContext;
			bool		      Running;
			bool		      ExitOnEsc;
			bool			  IsWindowed;
			bool			  IsFullScreen;
			bool			  IsMouseHidden;
			bool			  IsMaximized;
			bool			  IsSwapScreenLocked;
			bool			  IsExitOnEsc;
			bool			  IsAntialiased;
			int				  LeftButtonState;
			int				  MidButtonState;
			int				  RightButtonState;
			float			  MouseX;
			float			  MouseY;
			float			  MouseDx;
			float			  MouseDy;
			float			  MouseXRel;
			float			  MouseYRel;
			const bool*		  KeyboardKeys;
			int				  NumKeys;
			char			  GlfwKeyPressed[512];
			char			  GlfKeyOnce[512];

			// -------------------------------------------------------------------
			// Handlers

			vml::views::ViewHandler*	   ViewHandler;
			vml::overlays::OverlayHandler* OverlayHandler;
			vml::models::ModelHandler*	   ModelHandler;
			vml::OpenglDebugRender*		   OpenglDebugRenderer;

			// -------------------------------------------------------------------
			// fixed time handler

			double CurrentTime;
			double Accumulator;
			bool   TimerStarted;
			float  Dt;

			// -------------------------------------------------------------
			// returns time in seconds

			// --------------------------------------------------------------------------------
			//

			void DisposeResources()
			{
				vml::logger::Logger2::GetInstance()->Info({ "Core","Releasing Handlers" });

				// release handlers

				vml::os::SafeDelete(ViewHandler);
				vml::os::SafeDelete(OverlayHandler);
				vml::os::SafeDelete(ModelHandler);

				vml::logger::Logger2::GetInstance()->Info({ "Core","Releasing OpenglDebugger" });

				// release opengl renderer *first*

				vml::os::SafeDelete(OpenglDebugRenderer);

				vml::logger::Logger2::GetInstance()->Info({ "Core","Releasing Stoires" });

				// delete stores

				vml::os::SafeDelete(vml::stores::MeshStore);
				vml::os::SafeDelete(vml::stores::ShaderStore);
				vml::os::SafeDelete(vml::stores::TextureStore);
			}

			// --------------------------------------------------------------------------------
			//

			bool InitGLResources()
			{
				// init stores which will store graphics context
				// shaders, textures, and meshes are 
				// collected through stores

				vml::logger::Logger2::GetInstance()->Info({ "Core","Initializing Stores" });

				vml::stores::ShaderStore = new vml::utils::SharedResourceStore("ShaderStore");
				vml::stores::TextureStore = new vml::utils::SharedResourceStore("TextureStore");
				vml::stores::MeshStore = new vml::utils::SharedResourceStore("MeshStore");

				vml::logger::Logger2::GetInstance()->Info({ "Core","Stores initialized" });

				// create opengl debug renderer

				vml::logger::Logger2::GetInstance()->Info({ "Core","Initializing Debug Renderer" });

				// allocate opengl debug renderer

				OpenglDebugRenderer = new vml::OpenglDebugRender();
				OpenglDebugRenderer->Init();
				OpenglDebugRenderer->SetClearColor(vml::colors::Black);
				OpenglDebugRenderer->CheckeredPlaneMesh->FinalColor = OpenglDebugRenderer->GetClearColor();

				// allocate handlers

				vml::logger::Logger2::GetInstance()->Info({ "Core","Initializing Handlers" });

				ViewHandler = new vml::views::ViewHandler();
				OverlayHandler = new vml::overlays::OverlayHandler();
				ModelHandler = new vml::models::ModelHandler();

				// init handlers

				ViewHandler->Init();
				OverlayHandler->Init();
				ModelHandler->Init();

				return true;
			}

			// --------------------------------------------------------------------------------
			//

			void CloseSDL() const
			{
				//Destroy window	
				SDL_DestroyWindow(GlWindow);

				//Quit SDL subsystems
				SDL_Quit();
			}

			// -------------------------------------------------------------
			// returns time in seconds

			double GetTimeAsDouble()
			{
				using secondsFP = std::chrono::duration<double>;
				return std::chrono::duration_cast<secondsFP>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			}

		public:

			// --------------------------------------------------------------------------------
			//

			virtual void UpdateInputs() = 0;
			virtual void UpdateControllers() = 0;

			// --------------------------------------------------------------------------------
			//

			void UpdateFixed()
			{
				// fixed time updates

				if (!TimerStarted)
				{
					CurrentTime = GetTimeAsDouble();
					Accumulator = 0;
					TimerStarted = true;
				}
				else
				{
					double newTime = GetTimeAsDouble();
					double frameTime = newTime - CurrentTime;
					CurrentTime = newTime;
					Accumulator += frameTime;

					while (Accumulator >= Dt)
					{
						// handle controllers

						UpdateControllers();

						// increase accumulator

						Accumulator -= Dt;
					}
				}
			}

			// --------------------------------------------------------------------------------
			//

			void PollEvents()
			{

				// get mouse window coordinates
				// and compute mouse deltas

				SDL_MouseButtonFlags mousestate = SDL_GetMouseState(&MouseX, &MouseY);

				MouseXRel -= MouseX;
				MouseYRel -= MouseY;
				MouseDx = MouseXRel;
				MouseDy = MouseYRel;
				MouseXRel = MouseX;
				MouseYRel = MouseY;
				LeftButtonState = 0;
				MidButtonState = 0;
				RightButtonState = 0;

				if (mousestate == SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
				{
					LeftButtonState = 1;
				}

				if (mousestate == SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE))
				{
					MidButtonState = 1;
				}

				if (mousestate == SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))
				{
					RightButtonState = 1;
				}

				if (mousestate == (SDL_BUTTON_MASK(SDL_BUTTON_LEFT) | SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)))
				{
					LeftButtonState = 1;
					RightButtonState = 1;
				}

				// get keyboard pointer

				KeyboardKeys = SDL_GetKeyboardState(&NumKeys);
				if (ExitOnEsc)
					if (KeyboardKeys[SDL_SCANCODE_ESCAPE]) Running = false;

				// Handle events on queue

				SDL_Event e;

				while (SDL_PollEvent(&e))
				{

					switch (e.type)
					{
					case SDL_EVENT_QUIT: Running = false; break;

					case SDL_EVENT_WINDOW_RESIZED:

						ScreenWidth = e.window.data1;
						ScreenHeight = e.window.data2;

						break;

					}

				}

				// update inputs

				UpdateInputs();

			}

			// --------------------------------------------------------------------------------
			//

			void SetWindowTitle()
			{
				if (!IsFullScreen)
					SDL_SetWindowTitle(GlWindow, GetFpsString().c_str());
			}

			// --------------------------------------------------------------------------------
			// Initialize SDL

			void InitSDL(const std::string& title, const vml::utils::Flags& openglflags)
			{

				if (title.empty())
					vml::os::Message::Error("SDL missing title string");
				
				// Initialize SDL

				if (!SDL_Init(SDL_INIT_VIDEO))
				{
					vml::os::Message::Error("SDL could not initialize! SDL Error : ", SDL_GetError());
				}
				else
				{
					
					// set title

					Title = title;

					// get flags

					PreferencesFlags = openglflags;

					// validate preferences flags

					vml::logger::Logger2* logger = vml::logger::Logger2::GetInstance();

					if (IsVerbose())
						logger->Info({ "OpenGlAPI","Verbose flag set to 'True'" });
					else
						logger->Info({ "OpenGlAPI", "Verbose flag set to 'False'" });

					logger->Info({ "SDL API", "Initting SDL" });

					// check if window is at fullscreen or not

					IsWindowed = PreferencesFlags.Get(vml::flags::Preferences::WINDOWED);
					IsFullScreen = PreferencesFlags.Get(vml::flags::Preferences::FULLSCREEN);
					IsMouseHidden = PreferencesFlags.Get(vml::flags::Preferences::HIDECURSOR);
					IsMaximized = PreferencesFlags.Get(vml::flags::Preferences::MAXIMIZED);
					IsSwapScreenLocked = PreferencesFlags.Get(vml::flags::Preferences::LOCKSWAP);
					IsExitOnEsc = PreferencesFlags.Get(vml::flags::Preferences::EXITONESC);
					IsAntialiased = PreferencesFlags.Get(vml::flags::Preferences::ANTIALIASING);

					// validate fullscreen or windowed

					if ((IsWindowed && IsFullScreen) || (!IsWindowed && !IsFullScreen))
					{
						SDL_Quit();
						vml::os::Message::Error("GlWindow : Couldn't set up screen mode choose fullscreen or windowed");
					}

					if (IsVerbose())
					{
						if (IsMouseHidden)
							logger->Info({ "OpenGlAPI","HideCursor flag is set to 'True'" });
						else
							logger->Info({ "OpenGlAPI","HideCursor flag set to 'False'" });
						if (IsFullScreen)
							logger->Info({ "OpenGlAPI","Window mode is set to 'FullScreen'" });
						if (IsWindowed)
							logger->Info({ "OpenGlAPI","Window mode is set to 'Windowed'" });
						if (IsMaximized)
							logger->Info({ "OpenGlAPI","Window is 'Maximized'" });
						if (IsSwapScreenLocked)
							logger->Info({ "OpenGlAPI","LockSwap flag is set to 'True'" });
						else
							logger->Info({ "OpenGlAPI","LockSwap flag is set to 'False'" });
						if (IsExitOnEsc)
							logger->Info({ "OpenGlAPI","ExitOnEsc flag is set to 'True'" });
						else
							logger->Info({ "OpenGlAPI","ExitOnEsc flag is set to 'False'" });
					}
					
					// Use OpenGL 3.1 core
					//	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
					
					// antialiasing

					if (IsAntialiased)
					{
						if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1))
							vml::os::Message::Error("SDL : ", "couldn't set up multisample buffers");
						if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
							vml::os::Message::Error("SDL : ", "couldn't set up multi samples");
						glEnable(GL_MULTISAMPLE);
					}

					// Also request a depth buffer
					
					SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					
					// Create window

					GlWindow = SDL_CreateWindow(Title.c_str(), ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL);
					
					if (!GlWindow)
						vml::os::Message::Error("SDL : ", "couldn't set up video mode");
					
					// check if window is at fullscreen or not
					
					if (IsMaximized && !IsFullScreen)
					{
						SDL_SetWindowResizable(GlWindow, true);
						SDL_MaximizeWindow(GlWindow);
						SDL_SetWindowRelativeMouseMode(GlWindow, true);
					}

					if (IsFullScreen && !IsMaximized)
					{
						SDL_SetWindowResizable(GlWindow, false);
						SDL_SetWindowFullscreen(GlWindow, true);
						SDL_SetWindowRelativeMouseMode(GlWindow, false);
					}
					
					// exit on esc pressed ?

					ExitOnEsc = PreferencesFlags.Get(vml::flags::Preferences::EXITONESC);

					// retrieve window size, this is necessary for fullscreen

					SDL_GetWindowSizeInPixels(GlWindow, &ScreenWidth, &ScreenHeight);

					// set relative mouse motion

					if (IsMouseHidden)
						SDL_HideCursor();
					else
						SDL_ShowCursor();

					// create opengl window

					GlContext = SDL_GL_CreateContext(GlWindow);

					if (!GlContext)
					{
						vml::os::Message::Error("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
					}
					else
					{
						// Initialize GLEW

						glewExperimental = GL_TRUE;

						GLenum glewError = glewInit();

						if (glewError != GLEW_OK)
							vml::os::Message::Error("Error initializing GLEW! %s\n", glewGetErrorString(glewError));

						// Use Vsync

						if (IsSwapScreenLocked)
						{
							if (!SDL_GL_SetSwapInterval(1))
								vml::os::Message::Error("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
						}
						else
						{
							if (!SDL_GL_SetSwapInterval(0))
								vml::os::Message::Error("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
						}

						// Initialize OpenGL

						if (!InitGLResources())
							vml::os::Message::Error("Unable to initialize OpenGL!\n");

					}
					
					vml::logger::Logger2::GetInstance()->Info({ "SDL API", "SDL Inittialized" });

				}
			}

			// ---------------------------------------------------------------
			//

			[[nodiscard]] bool					         IsVerbose()            const { return vml::logger::Logger2::GetInstance()->IsVerbose(); }
			[[nodiscard]] bool					         IsQuiet()              const { return vml::logger::Logger2::GetInstance()->IsQuiet(); }
			[[nodiscard]] int					         GetScreenWidth()       const { return ScreenWidth; }
			[[nodiscard]] int					         GetScreenHeight()      const { return ScreenHeight; }
			[[nodiscard]] SDL_Window*					 GetGlWindow()          const { return GlWindow; };
			[[nodiscard]] bool						     IsRunning()            const { return Running; }
			[[nodiscard]] int						     GetLeftButtonState()   const { return LeftButtonState; }
			[[nodiscard]] int						     GetRightButtonState()  const { return RightButtonState; }
			[[nodiscard]] float						     GetMouseX()			const { return MouseX; }
			[[nodiscard]] float						     GetMouseY()			const { return MouseY; }
			[[nodiscard]] float						     GetMouseDx()			const { return MouseDx; }
			[[nodiscard]] float							 GetMouseDy()			const { return MouseDy; }
			[[nodiscard]] const bool*					 GetKeyboardKeys()		const { return KeyboardKeys; }
			[[nodiscard]] vml::views::ViewHandler*		 GetViewHandler()			  { return ViewHandler; }
			[[nodiscard]] vml::overlays::OverlayHandler* GetOverlayHandler()		  { return OverlayHandler; }
			[[nodiscard]] vml::models::ModelHandler*	 GetModelHandler()			  { return ModelHandler; }
			[[nodiscard]] vml::OpenglDebugRender*		 GetOpenglDebugRenderer()	  { return OpenglDebugRenderer; }

			// ---------------------------------------------------------------------------
			// detect a key press 

			[[nodiscard]] const bool IsKeyPressed(int key) const
			{
				return KeyboardKeys[key];
			}

			// ---------------------------------------------------------------------------
			// detec t a key opressed only once

			[[nodiscard]] bool IsKeyPressedOnce(int key)
			{
				if (KeyboardKeys[key])
				{
					if (!GlfKeyOnce[key])
					{
						GlfKeyOnce[key] = true;
						return true;
					}
					return false;
				}
				GlfKeyOnce[key] = false;
				return false;
			}

			// ---------------------------------------------------------------
			//

			SDLOpenGlApp()
			{
				CurrentTime		    = 0.0;
				Accumulator		    = 0.0;
				TimerStarted		= false;
				Dt					= 1.0f / 60.0f;

				ScreenWidth         = 800;
				ScreenHeight        = 600;
				GlWindow            = nullptr;
				GlContext           = nullptr;
				Running             = true;
				ExitOnEsc           = true;
				ViewHandler         = nullptr;
				OverlayHandler      = nullptr;
				ModelHandler        = nullptr;
				OpenglDebugRenderer = nullptr;
				KeyboardKeys		= nullptr;
				NumKeys				= 0;
				MouseX				= 0;
				MouseY				= 0;
				MouseDx				= 0;
				MouseDy				= 0;
				MouseXRel			= 0;
				MouseYRel			= 0;
				IsWindowed			= 0;
				IsFullScreen		= 0;
				IsMouseHidden		= 0;
				IsMaximized			= 0;
				IsSwapScreenLocked	= 0;
				IsExitOnEsc			= 0;
				IsAntialiased		= 0;
				LeftButtonState		= 0;
				MidButtonState		= 0;
				RightButtonState	= 0;
				PreferencesFlags	= vml::utils::Flags();

				memset(GlfwKeyPressed, 0, sizeof(char) * 512);
				memset(GlfKeyOnce, 0, sizeof(char) * 512);

			}

			virtual ~SDLOpenGlApp()
			{
				// release opengl resources
				DisposeResources();

				// close sdl
				CloseSDL();
			}

		};
}
