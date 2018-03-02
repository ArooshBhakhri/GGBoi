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

	float baseColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//Shapes//////////////////
	ID3D11Buffer *triangleBuffer;
	SEND_TO_RAM triangleShader;
	///////////////////////////////////

	//Shaders//////////////////////////
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11InputLayout *layout;
	ID3D11Buffer *shaderBuffer;

	void setBaseColor(float r, float g, float b, float a);
	void initializeWindow(HWND hwnd);
	void initializeTriangle(float x, float y, float z, float r, float g, float b, float a);

public:

	void Initialize(HWND hwnd);
	void Update(float delta);
	void Render();
	void Shutdown();
	void setDeltaTime(float t);

};