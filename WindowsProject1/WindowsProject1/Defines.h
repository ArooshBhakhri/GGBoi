#pragma once

#include "stdafx.h"

#define BACKBUFFER_HEIGHT 800
#define BACKBUFFER_WIDTH 800

#define safeRelease(finished) if(finished->Release()) { finished = nullptr; }

using namespace DirectX;

struct MY_VERTEX
{
	XMFLOAT2 pos;
	XMFLOAT4 rgba;
};

struct SEND_TO_RAM
{
	XMFLOAT4 constantColor;
	XMFLOAT2 constantOffset;
	XMFLOAT2 padding;
};

