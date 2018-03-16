// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

// TODO: reference additional headers your program requires here
#include <iostream>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
//Shaders//////////////////////////
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "Skybox_PS.csh"
#include "Skybox_VS.csh"
#include "Basic_PS.csh"
#include "TessDomainShader.csh"
#include "TessGeometryShader.csh"
#include "TessHullShader.csh"
#include "TessVertexShader.csh"
#include "Basic_VS.csh"
///////////////////////////////////

//all .h files/////////////////////
#include "Defines.h"
#include "Game.h"
#include "DDSTextureLoader.h"
///////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>