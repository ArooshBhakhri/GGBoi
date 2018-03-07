#pragma once

#include "stdafx.h"

#define BACKBUFFER_HEIGHT 800
#define BACKBUFFER_WIDTH 800

#define safeRelease(finished) if(finished != nullptr) if(finished->Release()) { finished = nullptr; }

using namespace DirectX;
using namespace std;

struct MY_VERTEX 
{
	XMFLOAT3 pos;
	XMFLOAT4 rgba;
	XMFLOAT2 texPos;
	XMFLOAT3 normals;

	MY_VERTEX() {}
	MY_VERTEX(float x, float y, float z, float r, float g, float b, float a, float u, float v, float nX, float nY, float nZ)
		: pos(x, y, z), rgba(r, g, b, a), texPos(u, v), normals(nX, nY, nZ) {}
};

struct cbPerObject
{
	XMFLOAT4X4 WVP;
	XMFLOAT3 time;
	XMFLOAT3 lightDir;
	XMFLOAT4 lightColor;
};