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

struct geometry
{
	XMMATRIX matrix;
	//vertex shader stuff//////////////////////////
	D3D11_BUFFER_DESC vertexBuffer;
	D3D11_SUBRESOURCE_DATA vertexBufferSubData;
	ID3D11VertexShader *vertexShader = nullptr;
	///////////////////////////////////////////////

	//pixel shader stuff////////////////////////////
	ID3D11ShaderResourceView *textureSRV = nullptr;
	ID3D11ShaderResourceView *texture1SRV = nullptr;
	ID3D11PixelShader *pixelShader = nullptr;
	////////////////////////////////////////////////

	//index buffer stuff///////////////////////////
	D3D11_BUFFER_DESC indexBuffer;
	D3D11_SUBRESOURCE_DATA indexBufferSubData;
	////////////////////////////////////////////////

	//for tesselation////////////////////////////////
	ID3D11HullShader *hullShader = nullptr;
	ID3D11DomainShader *domainShader = nullptr;
	ID3D11GeometryShader *geometryShader = nullptr;
	/////////////////////////////////////////////////

	const BYTE *PixelShader = nullptr;
	const BYTE *VertexShader = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY topology;

	vector<MY_VERTEX> vertices;
	vector<unsigned int> indices;

	void operator=(geometry rhs)
	{
		this->matrix = rhs.matrix;
		this->vertexBuffer = rhs.vertexBuffer;
		this->vertexBufferSubData = rhs.vertexBufferSubData;
		this->vertexShader = rhs.vertexShader;
		this->textureSRV = rhs.textureSRV;
		this->pixelShader = rhs.pixelShader;
		this->indexBuffer = rhs.indexBuffer;
		this->indexBufferSubData = rhs.indexBufferSubData;
		this->hullShader = rhs.hullShader;
		this->domainShader = rhs.domainShader;
		this->geometryShader = rhs.geometryShader;
		this->PixelShader = rhs.PixelShader;
		this->VertexShader = rhs.VertexShader;
		this->topology = rhs.topology;
		this->vertices = rhs.vertices;
		this->indices = rhs.indices;
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
};

struct cbTrivial_PS
{
	XMFLOAT3 dir;
	XMFLOAT4 color;
};

struct cbGrid_GS
{
	XMFLOAT4X4 VP;
};

struct cbGrid_HS
{
	float x;
	float y;
	float z;
};