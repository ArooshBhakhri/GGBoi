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
};

struct SEND_TO_RAM
{
	XMFLOAT4 constantColor;
	XMFLOAT3 constantOffset;
	XMFLOAT2 padding;
};

struct Triangle
{
	MY_VERTEX verts[3];
	ID3D11Buffer* triangleBuffer;
	SEND_TO_RAM triangleShader;
};

struct Line
{
	MY_VERTEX verts[2];
	ID3D11Buffer* lineBuffer;
	SEND_TO_RAM lineShader;
};

struct cbPerObject
{
	XMMATRIX WVP;
};