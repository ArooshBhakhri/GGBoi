#pragma once

#include "stdafx.h"

#include "XTime.h"

class Game
{
	
private:
	float deltaTime = 0;

	XTime time;

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
	MY_VERTEX vertices[8] =
	{
				  //x		y			z			r			g			b			a
		MY_VERTEX(-0.25f,	0.25f,		0.0f,		1.0f,		0.0f,		0.0f,		1.0f),		//top left				0
		MY_VERTEX(0.25f,	0.25f,		0.0f,		0.0f,		1.0f,		0.0f,		1.0f),		//top right				1
		MY_VERTEX(-0.25f,	-0.25f,		0.0f,		0.0f,		0.0f,		1.0f,		1.0f),		//bottom left			2
		MY_VERTEX(0.25f,	-0.25f,		0.0f,		0.0f,		0.0f,		0.0f,		1.0f),		//bottom right			3
		//////////////////////////////////////////////////////////////////////////////////////////////back verts below
		MY_VERTEX(-0.25f,	0.25f,		0.25f,		1.0f,		1.0f,		0.0f,		1.0f),		//top left				4
		MY_VERTEX(0.25f,	0.25f,		0.25f,		0.0f,		1.0f,		1.0f,		1.0f),		//top right				5
		MY_VERTEX(-0.25f,	-0.25f,		0.25f,		1.0f,		0.0f,		1.0f,		1.0f),		//bottom left			6
		MY_VERTEX(0.25f,	-0.25f,		0.25f,		1.0f,		1.0f,		1.0f,		1.0f),		//bottom right			7
	};

	DWORD indices[36] =
	{
		0, 1, 2,	//front - top
		2, 1, 3,	//front - bottom
		3, 1, 5,	//right - top
		3, 5, 7,	//right - bottom
		7, 5, 4,	//back	- top
		7, 4, 6,	//back  - bottom
		4, 0, 2,	//left  - top
		6, 4, 2,	//left  - bottom
		1, 4, 5,	//top   - top
		1, 0, 4,	//top	- bottom
		7, 6, 2,	//bottom - bottom
		7, 2, 3
	};

	ID3D11Buffer *drawingBuffer;
	ID3D11Buffer *indexBuffer;
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
	float xCam = 0.0f,		yCam = 1.0f,		zCam = 0.0f;

	float aspectRatio = ((float)BACKBUFFER_WIDTH / (float)BACKBUFFER_HEIGHT);

	float verticalFOV = XMConvertToRadians(60.0f);
	float zNear = 0.1f;
	float zFar = 1000.0f;
	////////////////////////////////////

	void setBaseColor(float r, float g, float b, float a);
	void initializeWindow(HWND hwnd);

public:

	void Initialize(HWND hwnd);
	void Update(float delta);
	void Render();
	void Shutdown();
	void setDeltaTime(float t);

	//XMMATRIX buildProjectionMatrix();
};