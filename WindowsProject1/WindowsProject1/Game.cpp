#include "stdafx.h"

void Game::Initialize(HWND hwnd)
{

	initializeWindow(hwnd);

	///////////////////////////////////////////////////////////INITIALIZING SHAPES////////////////////////////////////////////////

	//Shape1- Triangle/////////////////////////////////////////////////////////////////
	MY_VERTEX geometryTriangle[3];

	geometryTriangle[0].pos.x = 0.5f; geometryTriangle[0].pos.y = 0.0f;
	geometryTriangle[1].pos.x = -0.5f; geometryTriangle[1].pos.y = 0.0f;
	geometryTriangle[2].pos.x = 0.0f; geometryTriangle[2].pos.y = 0.5f;

	D3D11_BUFFER_DESC describeTriangle;
	describeTriangle.Usage = D3D11_USAGE_IMMUTABLE;
	describeTriangle.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	describeTriangle.CPUAccessFlags = NULL;
	describeTriangle.ByteWidth = sizeof(geometryTriangle);
	describeTriangle.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA triangleData;
	triangleData.pSysMem = geometryTriangle;
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;

	device->CreateBuffer(&describeTriangle, &triangleData, &triangleBuffer);
	//////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////INITIALIZING SHAPES - END////////////////////////////////////////////////

	//creating pixel and vertex shader to use
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);

	//describing input layout for our position to GPU
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//creating actual input layout
	device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &layout);

	//describing shader
	D3D11_BUFFER_DESC describeShader;
	describeShader.Usage = D3D11_USAGE_DYNAMIC;
	describeShader.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	describeShader.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	describeShader.ByteWidth = sizeof(SEND_TO_RAM);
	describeShader.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA shaderData;
	shaderData.pSysMem = &triangleShader;
	shaderData.SysMemPitch = 0;
	shaderData.SysMemSlicePitch = 0;

	triangleShader.constantColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	triangleShader.constantOffset = { 0, 0 };

	device->CreateBuffer(&describeShader, &shaderData, &shaderBuffer);

}

void Game::Update(float delta)
{

	if (delta != 0 && delta < deltaTime)
		return;


}

void Game::Render()
{
	
	setBaseColor(0.0f, 1.0f, 1.0f, 1.0f);

	//Drawing shapes///////////////////////////////////////////////////////

	//Triangle-
	triangleShader.constantColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	triangleShader.constantOffset = { 0, 0 };

	context->VSSetConstantBuffers(0, 1, &shaderBuffer);

	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;

	context->VSSetShader(vertexShader, NULL, 0);
	context->PSSetShader(pixelShader, NULL, 0);

	context->IASetInputLayout(layout);

	D3D11_MAPPED_SUBRESOURCE resource;

	context->Map(shaderBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
	memcpy(resource.pData, &triangleShader, sizeof(triangleShader));
	context->Unmap(shaderBuffer, NULL);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetVertexBuffers((UINT)0, (UINT)1, &triangleBuffer, &stride, &offset);

	context->Draw(3, 0);

	//call this at end of case WM_PAINT
	swapChain->Present(0, 0);
	//////////////////////////////////
}

void Game::Shutdown()
{

	safeRelease(device);
	safeRelease(swapChain);
	safeRelease(context);
	safeRelease(buffer);
	safeRelease(view);

	safeRelease(triangleBuffer);

	safeRelease(vertexShader);
	safeRelease(pixelShader);
	safeRelease(layout);
	safeRelease(shaderBuffer);
}

void Game::setDeltaTime(float t)
{
	deltaTime = t;
}

void Game::setBaseColor(float r, float g, float b, float a)
{

	baseColor[0] = r;
	baseColor[1] = g;
	baseColor[2] = b;
	baseColor[3] = a;

	context->OMSetRenderTargets(1, &view, 0);

	UINT var = 1;
	context->RSGetViewports(&var, &viewport);

	context->ClearRenderTargetView(view, baseColor);
}

void Game::initializeWindow(HWND hwnd)
{
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.Windowed = true;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Height = BACKBUFFER_HEIGHT;
	scd.BufferDesc.Width = BACKBUFFER_WIDTH;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.OutputWindow = hwnd;

	UINT flag = 0;
#if _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, NULL, NULL,
		D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &context);

	swapChain->GetBuffer(0, __uuidof(buffer), reinterpret_cast<void**>(&buffer));
	device->CreateRenderTargetView(buffer, NULL, &view);

	viewport.Height = BACKBUFFER_HEIGHT;
	viewport.Width = BACKBUFFER_WIDTH;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);
}

void Game::initializeTriangle(float x, float y, float z, float r, float g, float b, float a)
{
	//describing shader
	D3D11_BUFFER_DESC describeShader;
	describeShader.Usage = D3D11_USAGE_DYNAMIC;
	describeShader.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	describeShader.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	describeShader.ByteWidth = sizeof(SEND_TO_RAM);
	describeShader.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA shaderData;
	shaderData.pSysMem = &triangleShader;
	shaderData.SysMemPitch = 0;
	shaderData.SysMemSlicePitch = 0;

	triangleShader.constantColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	triangleShader.constantOffset = { 0, 0 };

	device->CreateBuffer(&describeShader, &shaderData, &shaderBuffer);
}