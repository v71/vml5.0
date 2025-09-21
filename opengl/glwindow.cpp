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

#include <vml5.0/opengl/glwindow.h>

/*
// ---------------------------------------------------------------------------
// detect if key is presed repeadetly 

bool glfwKeyRepeat(GLFWwindow* glwindow, unsigned int key, unsigned int limit)
{
	if (!glfwKeyPressed)
	{
		glfwKeyStartClock[key] = std::chrono::system_clock::now();
		glfwKeyPressed[key] = 1;
	}

	if (glfwGetKey(glwindow, key))
	{
		unsigned int diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - glfwKeyStartClock[key]).count();

		if (diff > limit)
		{
			glfwKeyStartClock[key] = std::chrono::system_clock::now();
			glfwKeyPressed[key] = 0;
			return true;
		}
	}
	else
	{
		glfwKeyPressed[key] = 0;
	}

	return false;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////
// Opengl window


namespace vml
{
		char   OpenGLContextWindow::GlfwKeyPressed[GLFW_KEY_LAST + 1];
		char   OpenGLContextWindow::GlfKeyOnce[GLFW_KEY_LAST + 1];
		double OpenGLContextWindow::MouseX;
		double OpenGLContextWindow::MouseY;
		int	   OpenGLContextWindow::LeftButtonState;
		int	   OpenGLContextWindow::RightButtonState;
		int	   OpenGLContextWindow::MiddleButtonState;

		// ---------------------------------------------------------------------------
		// This initializes our window and creates the OpenGL context

		void OpenGLContextWindow::Init(int width, int height, const std::string& strTitle, const vml::utils::Flags& preferencesflags)
		{
			// check if class has been initialised
			if (Initialized)
				vml::os::Message::Error("OpenglApi : Cannot restart context before it has not been closed");

			Initialized = false;

			// get flags
			PreferencesFlags = preferencesflags;

			// validate preferences flags

		//	if (IsVerbose() && IsQuiet())
		//		vml::os::Message::Error("OpenGlAPI : Verbose mode mismatch/not set");

			// get logger singleton

			vml::logger::Logger2* logger = vml::logger::Logger2::GetInstance();

			// log out verbose mode
			logger->Info({ "OpenGlAPI","Initializing" });

			if (IsVerbose())
				logger->Info({ "OpenGlAPI","Verbose flag set to 'True'" });
			else
				logger->Info({ "OpenGlAPI", "Verbose flag set to 'False'" });

			if (IsVerbose())
			{
				if (IsMouseVisible())
					logger->Info({ "OpenGlAPI","HideCursor flag is set to 'True'" });
				else
					logger->Info({ "OpenGlAPI","HideCursor flag set to 'False'" });
				if (IsFullscreen())
					logger->Info({ "OpenGlAPI","Window mode is set to 'FullScreen'" });
				if (IsWindowed())
					logger->Info({ "OpenGlAPI","Window mode is set to 'Windowed'" });
				if (IsMaxmized())
					logger->Info({ "OpenGlAPI","Window is 'Maximized'" });
				if (IsSwapScreenLocked())
					logger->Info({ "OpenGlAPI","LockSwap flag is set to 'True'" });
				else
					logger->Info({ "OpenGlAPI","LockSwap flag is set to 'False'" });
				if (IsExitOnEsc())
					logger->Info({ "OpenGlAPI","ExitOnEsc flag is set to 'True'" });
				else
					logger->Info({ "OpenGlAPI","ExitOnEsc flag is set to 'False'" });
			}

			// sts window's dimensions

			ScreenWidth  = width;
			ScreenHeight = height;

			// check if window is at fullscreen or not

			bool windowed   = PreferencesFlags.Get(vml::flags::Preferences::WINDOWED);
			bool fullscreen = PreferencesFlags.Get(vml::flags::Preferences::FULLSCREEN);
		
			// This tries to first init the GLFW library and make sure it is available

			if (!glfwInit())
			{
				glfwTerminate();
				vml::os::Message::Error("GlWindow : Couldn't init glfw library");
			}

			// ask for an opengl 4.0 context

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

			// antialiasing

			if (PreferencesFlags.Get(vml::flags::Preferences::ANTIALIASING))
			{
				glfwWindowHint(GLFW_SAMPLES, 4);
				glEnable(GL_MULTISAMPLE);
			}

			// validate fullscreen or windowed

			if ((windowed && fullscreen) || (!windowed && !fullscreen))
			{
				glfwTerminate();
				vml::os::Message::Error("GlWindow : Couldn't set up screen mode choose fullscreen or windowed");
			}

			// Create a window either in full screen 

			if (fullscreen && !windowed)
			{
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				Window = glfwCreateWindow(mode->width, mode->height, strTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);

				if (IsMaxmized())
					vml::os::Message::Error("GlWindow : Screen mode is fullscreen but you requested a maximized Windowed");

				// exit with error if context cannot be created

				if (!Window)
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow :Couldn't init glfw window context");
				}
			}

			// create a windowed window

			if (windowed && !fullscreen)
			{
				Window = glfwCreateWindow(ScreenWidth, ScreenHeight, strTitle.c_str(), nullptr, nullptr);

				// exit with error if context cannot be created

				if (!Window)
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow : Can't init glfw window context");
				}

				// windows specific
				// window is maximized only if mode is windowed

				if (IsMaxmized())
					ShowWindow(glfwGetWin32Window(Window), SW_SHOWMAXIMIZED);

				// gets new window extents

				glfwGetFramebufferSize(Window, &ScreenWidth, &ScreenHeight);
			}

			// Create the OpenGL context from the window and settings specified

			glfwMakeContextCurrent(Window);

			// get version info

			RendererString = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
			VersionString = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

			// This turns off the vertical sync to your monitor so it renders as fast as possible

			if (IsSwapScreenLocked())
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);

			 // Register key callback
			glfwSetKeyCallback(Window, GLFWKeyCallback);
			glfwSetCursorPosCallback(Window, GLFWCursorPositionCallback);
			glfwSetMouseButtonCallback(Window, GLFWMouseButtonsCallback);

			// We want to hide the mouse since it will be used to move the camera's view around
			// and don't want to see it being pushed up into the top left corner.

			if (IsMouseVisible())
				glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Initialize the GLEW library and attach all the OpenGL functions and extensions
			// Tell GLEW to grab all the OpenGL functions and extensions even if "experimental"

			glewExperimental = GL_TRUE;

			GLenum err = glewInit();

			// If we had an error, return -1.  Be sure to see if glewExperimental isn't true, this worked for me.

			if (err != GLEW_OK)
			{
				glfwTerminate();
				vml::os::Message::Error("GlWindow : Failed to initialize glew");
			}

			logger->Info({ "OpenGlAPI","Initialized" });

			// set context as started
			Initialized = true;

			// set running flag as true
			Running = true;

			// cache exitonesc flag for later use
			ExitOnEsc = PreferencesFlags.Get(vml::flags::Preferences::EXITONESC);

		}

		// ---------------------------------------------------------------------------
		// Closes glfw

		void OpenGLContextWindow::Close()
		{
			if (!Initialized)
				vml::os::Message::Error("OpenGlAPI : Cannot close context before it has not been initialized");
			vml::logger::Logger2::GetInstance()->Info({ "OpenGlAPI","Shutting Down" });
			// destroy glf window	
			glfwDestroyWindow(Window);
			glfwTerminate();
		}

		// ---------------------------------------------------------------------------
		// This function processes all the application's input 
		// and returns a bool to tell us if we should continue

		void OpenGLContextWindow::InitEvents()
		{
			if (!Initialized)
				vml::os::Message::Error("OpenGlAPI : Context not initialized");

			// Use the GLFW function to check for the user pressing the Escape button, as well as a window close event.

			if (ExitOnEsc)
			{
				// Close window on Escape press
				if ( GlfwKeyPressed[GLFW_KEY_ESCAPE] != GLFW_RELEASE || glfwWindowShouldClose(Window) != 0)
					Running = false;
			}
			else
			{
				if (glfwWindowShouldClose(Window) != 0)
					Running = false;
			}

			// detect if window resizes and changes screen dimensions accordingly
			glfwGetFramebufferSize(Window, &ScreenWidth, &ScreenHeight);

			// The current mouse position from our window from the mouse callback

			// compute mouse deltas
			MouseXDelta -= MouseX;
			MouseYDelta -= MouseY;
		}

		// ---------------------------------------------------------------------------
		// Set window title

		void OpenGLContextWindow::SetWindowTitle(const std::string& str)
		{
			if (IsWindowed())
				glfwSetWindowTitle(Window, str.c_str());
		}

		// ---------------------------------------------------------------------------
		// Swap opengl buffer

		void OpenGLContextWindow::SwapBuffers()
		{
			glfwSwapBuffers(Window);
			glFinish();
		}

		// ---------------------------------------------------------------------------
		// Poll events

		void OpenGLContextWindow::PollEvents()
		{
			// reset mouse deltas for the next cycle
			MouseXDelta = MouseX;
			MouseYDelta = MouseY;

			// Poll the input events to see if the user quit or closes the window
			glfwPollEvents();
		}

		// ---------------------------------------------------------------------------
		// Callback to handle key input

		void OpenGLContextWindow::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			GlfwKeyPressed[key] = action;
		}

		// ---------------------------------------------------------------------------
		// Callback to handle mouse position

		void OpenGLContextWindow::GLFWCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
		{
			MouseX = xpos;
			MouseY = ypos;
		}

		// ---------------------------------------------------------------------------
		// Callback to handle mouse input

		void OpenGLContextWindow::GLFWMouseButtonsCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT) 
			{
				if (GLFW_PRESS == action)
					LeftButtonState = true;
				else if (GLFW_RELEASE == action)
					LeftButtonState = false;
			}

			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				if (GLFW_PRESS == action)
					RightButtonState = true;
				else if (GLFW_RELEASE == action)
					RightButtonState = false;
			}

			if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				if (GLFW_PRESS == action)
					MiddleButtonState = true;
				else if (GLFW_RELEASE == action)
					MiddleButtonState = false;
			}
		}

		//------------------------------------------------------------------------------
		// getters

		[[nodiscard]] bool					   OpenGLContextWindow::IsMouseVisible()	   const { return PreferencesFlags.Get(vml::flags::Preferences::HIDECURSOR); }
		[[nodiscard]] bool					   OpenGLContextWindow::IsFullscreen()		   const { return PreferencesFlags.Get(vml::flags::Preferences::FULLSCREEN); }
		[[nodiscard]] bool				       OpenGLContextWindow::IsWindowed()		   const { return PreferencesFlags.Get(vml::flags::Preferences::WINDOWED); }
		[[nodiscard]] bool	   				   OpenGLContextWindow::IsMaxmized()		   const { return PreferencesFlags.Get(vml::flags::Preferences::MAXIMIZED); }
		[[nodiscard]] bool	   				   OpenGLContextWindow::IsSwapScreenLocked()   const { return PreferencesFlags.Get(vml::flags::Preferences::LOCKSWAP); }
		[[nodiscard]] bool	   				   OpenGLContextWindow::IsExitOnEsc()		   const { return PreferencesFlags.Get(vml::flags::Preferences::EXITONESC); }
		[[nodiscard]] bool					   OpenGLContextWindow::IsVerbose()			   const { return vml::logger::Logger2::GetInstance()->IsVerbose(); }
		[[nodiscard]] bool					   OpenGLContextWindow::IsQuiet()			   const { return vml::logger::Logger2::GetInstance()->IsQuiet(); }
		[[nodiscard]] bool					   OpenGLContextWindow::IsInitialized()		   const { return Initialized; }
		[[nodiscard]] GLFWwindow*			   OpenGLContextWindow::GetWindow()			   const { return Window; }
		[[nodiscard]] const vml::utils::Flags& OpenGLContextWindow::GetPreferencesFlags()  const { return PreferencesFlags; }
		[[nodiscard]] int					   OpenGLContextWindow::GetScreenWidth()	   const { return ScreenWidth; }
		[[nodiscard]] int					   OpenGLContextWindow::GetScreenHeight()	   const { return ScreenHeight; }
		[[nodiscard]] double				   OpenGLContextWindow::GetMouseX()			   const { return MouseX; }
		[[nodiscard]] double				   OpenGLContextWindow::GetMouseY()			   const { return MouseY; }
		[[nodiscard]] double				   OpenGLContextWindow::GetMouseXDelta()	   const { return MouseXDelta; };
		[[nodiscard]] double				   OpenGLContextWindow::GetMouseYDelta()	   const { return MouseYDelta; }
		[[nodiscard]] int					   OpenGLContextWindow::GetLeftButtonState()   const { return LeftButtonState; }
		[[nodiscard]] int					   OpenGLContextWindow::GetRightButtonState()  const { return RightButtonState; }
		[[nodiscard]] int					   OpenGLContextWindow::GetMiddleButtonState() const { return MiddleButtonState; }
		[[nodiscard]] bool				       OpenGLContextWindow::IsRunning()			   const { return Running; }
		[[nodiscard]] const std::string&	   OpenGLContextWindow::GetRenderString()	   const { return RendererString; }
		[[nodiscard]] const std::string&	   OpenGLContextWindow::GetVersionString()	   const { return VersionString; }
		[[nodiscard]] bool					   OpenGLContextWindow::IsKeyPressed(int key)  const { return GlfwKeyPressed[key] != GLFW_RELEASE; }

		// ---------------------------------------------------------------------------
		// detec t a key opressed only once

		[[nodiscard]] bool OpenGLContextWindow::IsKeyPressedOnce(int key) const
		{
			if ( GlfwKeyPressed[key] != GLFW_RELEASE)
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

		//------------------------------------------------------------------------------
		// ctor / dtor

		OpenGLContextWindow::OpenGLContextWindow()
		{
			Window			  = nullptr;
			ScreenWidth		  = 0;
			ScreenHeight	  = 0;
			ExitOnEsc		  = false;
			Running			  = false;
			Initialized		  = false;
			MouseXDelta		  = 0;
			MouseYDelta		  = 0;
			PreferencesFlags  = vml::utils::Flags();
			
			// static variables
			memset(GlfwKeyPressed, 0, sizeof(char) * (GLFW_KEY_LAST + 1));
			memset(GlfKeyOnce, 0, sizeof(char) * (GLFW_KEY_LAST + 1));
			MouseX			  = 0.0;
			MouseY			  = 0.0;
			LeftButtonState   = 0;
			RightButtonState  = 0;
			MiddleButtonState = 0;
		}

		OpenGLContextWindow::~OpenGLContextWindow()
		{
			Close();
		}

}