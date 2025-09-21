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

class OpenglApp : public vml::Core
{
	private:

		// -------------------------------------------------------------------
		// fixed time handler

		double CurrentTime;
		double Accumulator;
		bool   TimerStarted;
		float  Dt;

	public:

		// ----------------------------------------------

		vml::OpenGLContextWindow* OpenGLContextWindow;
		vml::OpenglDebugRender* OpenglDebugRenderer;
		vml::views::ViewHandler* ViewHandler;
		vml::models::ModelHandler* ModelHandler;
		vml::overlays::OverlayHandler* OverlayHandler;

		//	vml::handlers::Scene*     Scene;


		// ----------------------------------------------
		// init opengl api

		void InitOpenGlAPI(const vml::utils::Flags& openglflags)
		{
			if (!IsInitialized())
				vml::os::Message::Error("Core : Not initted when trying to init graphics api");

			// allocate opengl context

			OpenGLContextWindow = new vml::OpenGLContextWindow();
			OpenGLContextWindow->Init(1024, 768, "CleoEngine3.5", openglflags);

			// init stores which will store graphics context
			// shaders, textures, and meshes are 
			// collected through stores

			if (GetProjectPath().empty())
				vml::os::Message::Error("Core : Project path is missing");
			if (GetProjectName().empty())
				vml::os::Message::Error("Core : Project Name is missing");

			// init stores

			vml::logger::Logger2::GetInstance()->Info({ "Core","Initializing Stores" });

			vml::stores::ShaderStore = new vml::utils::SharedResourceStore("ShaderStore");
			vml::stores::TextureStore = new vml::utils::SharedResourceStore("TextureStore");
			vml::stores::MeshStore = new vml::utils::SharedResourceStore("MeshStore");

			vml::logger::Logger2::GetInstance()->Info({ "Core","Stores initialized" });

			// create opengl debug renderer

			OpenglDebugRenderer = new vml::OpenglDebugRender();
			OpenglDebugRenderer->Init();
			OpenglDebugRenderer->SetClearColor(vml::colors::Black);
			OpenglDebugRenderer->CheckeredPlaneMesh->FinalColor = OpenglDebugRenderer->GetClearColor();
		}

		// -----------------------------------------------------------
		// clear stores, we need to delete stores
		// the store might address some, rendering classes such as
		// shaders and textures which need, an active in the 
		// rendering context to be properly deleted

		void DisposeResources()
		{
			if (!IsInitialized())
				vml::os::Message::Error("Core : Not initted when disposing resources");

			// release model handler
			vml::os::SafeDelete(ModelHandler);

			// release view handler
			vml::os::SafeDelete(ViewHandler);

			// release model handler
			vml::os::SafeDelete(OverlayHandler);

			// release opengl renderer *first*
			vml::os::SafeDelete(OpenglDebugRenderer);

			// log out message
			vml::logger::Logger2::GetInstance()->Info({ "Core","Shutting Down Stores" });

			// delete stores
			vml::os::SafeDelete(vml::stores::MeshStore);
			vml::os::SafeDelete(vml::stores::ShaderStore);
			vml::os::SafeDelete(vml::stores::TextureStore);

			// close opengl context
			vml::os::SafeDelete(OpenGLContextWindow);
		}

		// -------------------------------------------------------------
		// returns time in seconds

		double GetTimeAsDouble() 
		{
			using secondsFP = std::chrono::duration<double>;
			return std::chrono::duration_cast<secondsFP>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		}

		// -------------------------------------------------------------
		// fixed time controller

		void FixedTimeController(vml::views::View* view)
		{
			if (!TimerStarted)
			{
				CurrentTime = glfwGetTime();
		//		CurrentTime = GetTimeAsDouble();
				Accumulator = 0;
				TimerStarted = true;
			}
			else
			{
				double newTime = glfwGetTime();
		//		double newTime = GetTimeAsDouble();
				double frameTime = newTime - CurrentTime;
				CurrentTime = newTime;
				Accumulator += frameTime;

				while (Accumulator >= Dt)
				{
		
					// call objects controllers
					Controller();

		//			Scene->GetObjectHandler()->CallObjectsController(view);

					// increazse acclumulator

					Accumulator -= Dt;

				}
			}

			// call keybindings
			KeyBindings();

		}

		// -----------------------------------------------------------
		// rendering context functions

		__forceinline void SwapBuffers()								 const { OpenGLContextWindow->SwapBuffers(); }
		__forceinline void PollEvents()									 const { OpenGLContextWindow->PollEvents(); }
		__forceinline void SetWindowTitle(const std::string& str)	     const { OpenGLContextWindow->SetWindowTitle(str); }
		__forceinline void InitEvents()									 const { OpenGLContextWindow->InitEvents(); }
		__forceinline bool IsWindowRunning()							 const { return OpenGLContextWindow->IsRunning(); }
		__forceinline GLFWwindow* GetGLFWindow()						 const { return OpenGLContextWindow->GetWindow(); }
		__forceinline vml::OpenGLContextWindow* GetOpenGLContextWindow() const { return OpenGLContextWindow; }

		// -----------------------------------------------------------
		// key bindings

		virtual void KeyBindings() = 0;

		// -----------------------------------------------------------
		// Resource loading

		virtual void LoadResources() = 0;

		// -----------------------------------------------------------
		// Controller

		virtual void Controller() = 0;

		// ----------------------------------------------
		// ctor / dtor

		OpenglApp()
		{
			CurrentTime  = 0.0;
			Accumulator  = 0.0;
			TimerStarted = false;
			Dt			 = 1.0f / 60.0f;

			OpenGLContextWindow = nullptr;
			OpenglDebugRenderer = nullptr;
			ViewHandler			= nullptr;
			ModelHandler		= nullptr;
			OverlayHandler		= nullptr;
		}

		virtual ~OpenglApp()
		{

		}

};
