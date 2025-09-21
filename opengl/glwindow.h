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

//////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL and GLEW Header Files and Libraries

#include <GL\glew.h>
#include <GL\wglew.h>
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>

#ifdef _WIN32 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <vml5.0/os/errormsg.h>		// error/warning/trace messaging , platform dependent
#include <vml5.0/os/cpuinfo2.h>		// system info detection class ,platform dependent
#include <vml5.0/os/timer.h>		// timer class ,platform dependent
#endif	

#include <vml5.0/os/common.h>
#include <vml5.0/os/globalflags.h>
#include <vml5.0/utils/flags.h>
#include <vml5.0/utils/logger2.h>

//------------------------------------------------------------------------------
// key handling data
/*
static std::chrono::time_point<std::chrono::system_clock> glfwKeyStartClock[GLFW_KEY_LAST + 1] = { std::chrono::system_clock::now() };
static char glfwKeyPressed[GLFW_KEY_LAST + 1] = { 0 };
//------------------------------------------------------------------------------
// key handling
bool glfwKeyRepeat(GLFWwindow* glwindow, unsigned int key, unsigned int limit = 250);
*/

//////////////////////////////////////////////////////////////////////////////////////////////////
// Opengl window

namespace vml
{
	
	class OpenGLContextWindow
	{

		private:

			//------------------------------------------------------------------------------
			// private data

			GLFWwindow*		  Window;				// Glfw windw
			vml::utils::Flags PreferencesFlags;		// Preferences flags
			int				  ScreenWidth;			// Screen width
			int				  ScreenHeight;			// Screen height
			bool			  ExitOnEsc;			// Exit if esc is pressed
			bool			  Running;				// is rendeirng loop running
			double			  MouseXDelta;
			double			  MouseYDelta;
			std::string		  RendererString;
			std::string		  VersionString;
			bool			  Initialized;
			static char		  GlfwKeyPressed[GLFW_KEY_LAST + 1];
			static char		  GlfKeyOnce[GLFW_KEY_LAST + 1];
			static double	  MouseX;
			static double	  MouseY;
			static int		  LeftButtonState;
			static int		  RightButtonState;
			static int		  MiddleButtonState;

			// ---------------------------------------------------------------------------
			// static callback for key handling

			static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void GLFWCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
			static void GLFWMouseButtonsCallback(GLFWwindow* window, int button, int action, int mods);

		public:

			// ---------------------------------------------------------------------------
			// This initializes our window and creates the OpenGL context

			void Init(int width, int height, const std::string& strTitle, const vml::utils::Flags& preferencesflags);

			// ---------------------------------------------------------------------------
			// Closes glfw

			void Close();

			// ---------------------------------------------------------------------------
			// This function processes all the application's input 
			// and returns a bool to tell us if we should continue

			void InitEvents();

			// ---------------------------------------------------------------------------
			// Set window title

			void SetWindowTitle(const std::string& str);

			// ---------------------------------------------------------------------------
			// Swap opengl buffer

			void SwapBuffers();

			// ---------------------------------------------------------------------------
			// Poll events

			void PollEvents();

			//------------------------------------------------------------------------------
			// getters

			[[nodiscard]] bool					   IsMouseVisible()		  const;
			[[nodiscard]] bool					   IsFullscreen()		  const;
			[[nodiscard]] bool				       IsWindowed()			  const;
			[[nodiscard]] bool	   				   IsMaxmized()			  const;
			[[nodiscard]] bool	   				   IsSwapScreenLocked()	  const;
			[[nodiscard]] bool	   				   IsExitOnEsc()		  const;
			[[nodiscard]] bool					   IsVerbose()			  const; 
			[[nodiscard]] bool					   IsQuiet()			  const; 
			[[nodiscard]] bool					   IsInitialized()		  const; 
			[[nodiscard]] GLFWwindow*			   GetWindow()			  const; 
			[[nodiscard]] const vml::utils::Flags& GetPreferencesFlags()  const; 
			[[nodiscard]] int					   GetScreenWidth()		  const; 
			[[nodiscard]] int					   GetScreenHeight()	  const; 
			[[nodiscard]] double				   GetMouseX()			  const;
			[[nodiscard]] double				   GetMouseY()			  const;
			[[nodiscard]] double				   GetMouseXDelta()		  const;
			[[nodiscard]] double				   GetMouseYDelta()		  const;
			[[nodiscard]] int					   GetLeftButtonState()	  const;
			[[nodiscard]] int					   GetRightButtonState()  const;
			[[nodiscard]] int					   GetMiddleButtonState() const;
			[[nodiscard]] bool					   IsRunning()			  const;
			[[nodiscard]] const std::string&	   GetRenderString()	  const;
			[[nodiscard]] const std::string&	   GetVersionString()	  const;
			[[nodiscard]] bool					   IsKeyPressed(int key)  const;
			[[nodiscard]] bool					   IsKeyPressedOnce(int key) const;

			// -----------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed since classes
			// are referenced

			OpenGLContextWindow(const OpenGLContextWindow& glwindow) = delete;

			// -----------------------------------------------------------------------
			// overload operator = is private
			// no copies allowed since classes
			// are referenced

			OpenGLContextWindow& operator = (const OpenGLContextWindow& glwindow) = delete;

			//-----------------------------------------------------------------------------------
			// class is non mmovable

			OpenGLContextWindow(OpenGLContextWindow&& other) = delete;
			OpenGLContextWindow& operator=(OpenGLContextWindow&& other) = delete;

			//------------------------------------------------------------------------------
			// ctor / dtor

			OpenGLContextWindow();
			~OpenGLContextWindow();

	};

} // end of vml namespace

