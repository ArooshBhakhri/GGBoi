#pragma once

#include "stdafx.h"

class Game
{
	
private:
	float deltaTime = 0;

	ID3D11Device * device;
	IDXGISwapChain *swapChain;
	ID3D11DeviceContext *context;
	D3D11_VIEWPORT viewport;
	DXGI_SWAP_CHAIN_DESC scd;
	ID3D11Resource *buffer;
	ID3D11RenderTargetView *view;
	
	ID3D11DepthStencilView *dStencilView;
	ID3D11Texture2D *depthStencilTexture;

	float baseColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//Shapes//////////////////
	vector<Triangle> triangles;
	vector<Line> lines;
	///////////////////////////////////

	//Shaders//////////////////////////
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11InputLayout *layout;
	ID3D11Buffer *shaderBuffer;

	//3D magic shit/////////////////////
	ID3D11Buffer * cbPerObjectBuffer;

	cbPerObject cbPerObj;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	float xPos = 0.0f		,yPos = 0.0f,		zPos = -0.5f;
	float xTarget = 0.0f	,yTarget = 0.0f,	zTarget = 0.0f;
	float xCam = 0.0f,		yCam = 0.0f,		zCam = 0.0f;

	float aspectRatio = ((float)BACKBUFFER_WIDTH / (float)BACKBUFFER_HEIGHT);

	float verticalFOV = 90.0f;
	float zNear = 0.1f;
	float zFar = 1.0f;
	////////////////////////////////////

	void setBaseColor(float r, float g, float b, float a);
	void initializeWindow(HWND hwnd);
	void initializeTriangle(Triangle triangle);
	void initializeLine(Line line);

public:

	void Initialize(HWND hwnd);
	void Update(float delta);
	void Render();
	void Shutdown();
	void setDeltaTime(float t);

	//XMMATRIX buildProjectionMatrix();
};