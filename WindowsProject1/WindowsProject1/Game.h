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

#pragma region cubeVerts
	//{
	//			  //x		y			z			r			g			b			a		u		v
	//			  // Front Face
	//	MY_VERTEX(-1.0f,	-1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	1.0f),
	//	MY_VERTEX(-1.0f,	1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	0.0f),
	//	MY_VERTEX(1.0f,		1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	0.0f),
	//	MY_VERTEX(1.0f,		-1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	1.0f),

	//	// Back Face
	//	MY_VERTEX(-1.0f,	-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	1.0f),
	//	MY_VERTEX(1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	1.0f),
	//	MY_VERTEX(1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	0.0f),
	//	MY_VERTEX(-1.0f,	1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	0.0f),

	//	// Top Face
	//	MY_VERTEX(-1.0f,	1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	1.0f),
	//	MY_VERTEX(-1.0f, 1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 0.0f),
	//	MY_VERTEX(1.0f, 1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 0.0f),
	//	MY_VERTEX(1.0f, 1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 1.0f),

	//	// Bottom Face
	//	MY_VERTEX(-1.0f, -1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 1.0f),
	//	MY_VERTEX(1.0f, -1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 1.0f),
	//	MY_VERTEX(1.0f, -1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 0.0f),
	//	MY_VERTEX(-1.0f, -1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 0.0f),

	//	// Left Face
	//	MY_VERTEX(-1.0f, -1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 1.0f),
	//	MY_VERTEX(-1.0f,  1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 0.0f),
	//	MY_VERTEX(-1.0f,  1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 0.0f),
	//	MY_VERTEX(-1.0f, -1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 1.0f),

	//	// Right Face
	//	MY_VERTEX(1.0f, -1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 1.0f),
	//	MY_VERTEX(1.0f,  1.0f, -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 0.0f, 0.0f),
	//	MY_VERTEX(1.0f,  1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 0.0f),
	//	MY_VERTEX(1.0f, -1.0f,  1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	 1.0f, 1.0f)
	//};
#pragma endregion

#pragma region cubeIndices
	//{
	//	// Front Face
	//	0,  1,  2,
	//	0,  2,  3,

	//	// Back Face
	//	4,  5,  6,
	//	4,  6,  7,

	//	// Top Face
	//	8,  9, 10,
	//	8, 10, 11,

	//	// Bottom Face
	//	12, 13, 14,
	//	12, 14, 15,

	//	// Left Face
	//	16, 17, 18,
	//	16, 18, 19,

	//	// Right Face
	//	20, 21, 22,
	//	20, 22, 23
	//};
#pragma endregion

	bool wireFrame = false;

	vector<geometry> geometries;

	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	///////////////////////////////////

	ID3D11RasterizerState *rasterizerState;

	//Shaders//////////////////////////
	ID3D11InputLayout *layout;
	ID3D11Buffer *shaderBuffer;

	//3D magic shit/////////////////////
	ID3D11Buffer *cbPerObjectBuffer;

	cbPerObject cbPerObj;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMMATRIX camRotation;
	XMVECTOR defForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR defRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	float xPos = 0.5f		,yPos = 0.0f,		zPos = -3.0f;
	float xTarget = 0.0f	,yTarget = 0.0f,	zTarget = 0.0f;
	float xCam = 0.0f,		yCam = 1.0f,		zCam = 0.0f;

	float moveSpeed = 10.0f;

	float aspectRatio = ((float)BACKBUFFER_WIDTH / (float)BACKBUFFER_HEIGHT);

	float verticalFOV = XMConvertToRadians(90.0f);
	float zNear = 0.1f;
	float zFar = 1000.0f;
	////////////////////////////////////

	//Textures!!/////////////////////////////
	ID3D11Texture2D *envTexture;
	ID3D11ShaderResourceView * deWeySRV;

	ID3D11Texture2D *ironTexture;
	ID3D11ShaderResourceView *ironSRV;

	ID3D11Texture2D *trippyTexture;
	ID3D11ShaderResourceView *trippySRV;

	ID3D11Texture2D *knuckleTexture;
	ID3D11ShaderResourceView *knuckleSRV;

	ID3D11Texture2D *groundTexture;
	ID3D11ShaderResourceView *groundSRV;

	//one sampler for all shaders for now
	ID3D11SamplerState *sampler;

	/////////////////////////////////////////

	//Lighting///////////////////////////////
	XMVECTOR lightDir;
	XMVECTOR lightColor;

	/////////////////////////////////////////

	//Skybox/////////////////////////////////
	XMMATRIX skybox;
	ID3D11Texture2D *skyboxTexture;
	ID3D11Buffer* skyboxIndexBuffer;
	ID3D11Buffer* skyboxVertexBuffer;
	ID3D11VertexShader* Skybox_VShader;
	ID3D11PixelShader* Skybox_PShader;

	ID3D11ShaderResourceView* skyboxSRV;

	ID3D11DepthStencilState* skyboxDSS;
	ID3D11RasterizerState* skyboxRS;

	unsigned int numSphereVerts;
	unsigned int numSphereFaces;

	XMMATRIX sphere;

	/////////////////////////////////////////

	void setBaseColor(float r, float g, float b, float a);
	void initializeWindow(HWND hwnd);

public:

	void Initialize(HWND hwnd);
	void Update(float delta);
	void Render();
	void Shutdown();
	void setDeltaTime(float t);
	//DEPRACATED
	//void loadPyramid(const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse);
	//DEPRACATED
	void loadIOModel(char* file, const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse);
	void CreateSphere(unsigned int latLines, unsigned int longLines, const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse);

};