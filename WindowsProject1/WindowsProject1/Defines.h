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

	MY_VERTEX() {}
	MY_VERTEX(float x, float y, float z, float r, float g, float b, float a, float u, float v)
		: pos(x, y, z), rgba(r, g, b, a), texPos(u, v) {}
};

struct SEND_TO_RAM
{
	XMFLOAT4 constantColor;
	XMFLOAT3 constantOffset;
	XMFLOAT2 padding;
};

struct Line
{
	MY_VERTEX verts[2];
	ID3D11Buffer* lineBuffer;
	SEND_TO_RAM lineShader;
};

struct cbPerObject
{
	XMFLOAT4X4 WVP;
};