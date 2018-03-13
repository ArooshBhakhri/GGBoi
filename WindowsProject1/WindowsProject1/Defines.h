#pragma once

#include "stdafx.h"

#define BACKBUFFER_HEIGHT 1000
#define BACKBUFFER_WIDTH 1000

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

	void operator=(MY_VERTEX rhs)
	{
		this->pos = rhs.pos;
		this->rgba = rhs.rgba;
		this->texPos = rhs.texPos;
		this->normals = rhs.normals;
	}
};

struct MY_TRIANGLE
{
	XMINT3 indices;

	bool operator==(MY_TRIANGLE rhs)
	{
		if (indices.x == rhs.indices.x && indices.y == rhs.indices.y && indices.z == rhs.indices.z)
			return true;
		return false;
	}
};

struct cbPerObject
{
	XMFLOAT4X4 WVP;
	XMFLOAT4X4 World;
	XMFLOAT3 time;
	XMFLOAT3 lightDir;
	XMFLOAT4 lightColor;
};

struct geometry
{
	XMMATRIX matrix;
	//vertex shader stuff//////////////////////////
	D3D11_BUFFER_DESC vertexBuffer;
	D3D11_SUBRESOURCE_DATA vertexBufferSubData;
	ID3D11VertexShader *vertexShader;
	///////////////////////////////////////////////

	//pixel shader stuff////////////////////////////
	ID3D11ShaderResourceView* textureSRV;
	ID3D11PixelShader *pixelShader;
	////////////////////////////////////////////////

	//index buffer stuff///////////////////////////
	D3D11_BUFFER_DESC indexBuffer;
	D3D11_SUBRESOURCE_DATA indexBufferSubData;
	////////////////////////////////////////////////

	D3D11_PRIMITIVE_TOPOLOGY topology;

	vector<MY_VERTEX> vertices;
	vector<unsigned int> indices;
};