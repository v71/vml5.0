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
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER7
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:6386)
#pragma warning(disable:26451)

#define _CRT_SECURE_NO_WARNINGS

////////////////////////////////////////////////////////////////////////////////////
// include directories

#include <vml5.0/os/common.h>

////////////////////////////////////////////////////////////////////////////////////
// host system

#ifdef _WIN32 
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <vml5.0/os/errormsg.h>		// error/warning/trace messaging , platform dependent
#include <vml5.0/os/cpuinfo2.h>		// system info detection class ,platform dependent
#include <vml5.0/os/timer.h>		// timer class ,platform dependent
#endif	

////////////////////////////////////////////////////////////////////////////////////
// bits utils

#include <vml5.0/utils/uniqueid.h>

////////////////////////////////////////////////////////////////////////////////////
// flags

#include <vml5.0/utils/flags.h>

////////////////////////////////////////////////////////////////////////////////////
// globasl flags

#include <vml5.0/os/globalflags.h>

////////////////////////////////////////////////////////////////////////////////////
// string utils

#include <vml5.0/strings/stringconverter.h>
#include <vml5.0/strings/stringutils.h>
#include <vml5.0/strings/stringpath.h>

////////////////////////////////////////////////////////////////////////////////////
// logger

#include <vml5.0/utils/logger2.h>

////////////////////////////////////////////////////////////////////////////////////
// session

#include <vml5.0/utils/globalpaths.h>

////////////////////////////////////////////////////////////////////////////////////
// assetstore

#include <vml5.0/utils/assetstore.h>

////////////////////////////////////////////////////////////////////////////////////
// json

#include <vml5.0/utils/json.hpp>

////////////////////////////////////////////////////////////////////////////////////
// math

#include <vml5.0/math/mathutils.h>

////////////////////////////////////////////////////////////////////////////////////
// opengl

#include <vml5.0/opengl/glwindow.h>
#include <vml5.0/opengl/shaders/shader.h>
#include <vml5.0/libs/stb/stb_image.h>
#include <vml5.0/opengl/textures/texture.h>

/////////////////////////////////////////////////////////////////////////////////////
// views / viewmanager

#include <vml5.0/opengl/view/view.h>
#include <vml5.0/opengl/view/frustum3d.h>

/////////////////////////////////////////////////////////////////////////////////////
// stores

#include <vml5.0/stores.h>

/////////////////////////////////////////////////////////////////////////////////////
// meshes

#include <vml5.0/mesh/mesh.h>
#include <vml5.0/mesh/model.h>

////////////////////////////////////////////////////////////////////////////////////
// handlers

#include <vml5.0/handlers/viewhandler.h>
#include <vml5.0/handlers/modelhandler.h>
#include <vml5.0/handlers/level.h>
#include <vml5.0/handlers/scene.h>

////////////////////////////////////////////////////////////////////////////////////
// rendering context

#include <vml5.0/opengl/debugrendering/debugrendering.h>

#include <vml5.0/opengl/effects/overlay.h>
#include <vml5.0/handlers/overlayhandler.h>

////////////////////////////////////////////////////////////////////////////////////
// importer & mesh optimizer
// in case you want to add directly in a .cpp file, use this define
//#define TINYOBJLOADER_IMPLEMENTATION

#include <vml5.0/mesh/importer.h>

/*
////////////////////////////////////////////////////////////////////////////////////
// handlers

#include <vml4.0/octree/octree.h>
#include <vml4.0/handlers/level.h>
#include <vml4.0/handlers/scene.h>

*/
////////////////////////////////////////////////////////////////////////////////////
// core

#include <vml5.0/os/core.h>
