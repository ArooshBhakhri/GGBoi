#include "stdafx.h"

void Game::Initialize(HWND hwnd)
{

	initializeWindow(hwnd);

	///////////////////////////////////////////////////////////INITIALIZING SHAPES////////////////////////////////////////////////

	//Shape1- Triangle/////////////////////////////////////////////////////////////////
	Triangle tri1;
	tri1.verts[0].pos.x = -0.25f;	tri1.verts[0].pos.y = 0.25f;	tri1.verts[0].pos.z = 0.0f;
	tri1.verts[1].pos.x = 0.25f;	tri1.verts[1].pos.y = 0.25f;	tri1.verts[1].pos.z = 0.0f;
	tri1.verts[2].pos.x = -0.25f;	tri1.verts[2].pos.y = -0.25f;	tri1.verts[2].pos.z = 0.0f;

	tri1.triangleShader.constantColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	tri1.triangleShader.constantOffset = { 0.0f, 0.0f, 0.0f };
	//tri1.triangleShader.padding = { 6.0f, 6.0f };

	initializeTriangle(tri1);

	Triangle tri2;
	tri2.verts[0].pos.x = -0.25f;	tri2.verts[0].pos.y = -0.25f;	tri2.verts[0].pos.z = 0.0f;
	tri2.verts[1].pos.x = 0.25f;	tri2.verts[1].pos.y = 0.25f;	tri2.verts[1].pos.z = 0.0f;
	tri2.verts[2].pos.x = 0.25f;	tri2.verts[2].pos.y = -0.25f;	tri2.verts[2].pos.z = 0.0f;

	tri2.triangleShader.constantColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	tri2.triangleShader.constantOffset = { 0.0f, 0.0f, 0.0f };
	//tri2.triangleShader.padding = { 6.0f, 6.0f };

	initializeTriangle(tri2);

	Triangle tri3;
	tri3.verts[0].pos.x = 0.25f;	tri3.verts[0].pos.y = 0.25f;	tri3.verts[0].pos.z = 0.0f;
	tri3.verts[1].pos.x = 0.0f;		tri3.verts[1].pos.y = 0.25f;	tri3.verts[1].pos.z = 0.25f;
	tri3.verts[2].pos.x = 0.25f;	tri3.verts[2].pos.y = -0.25f;	tri3.verts[2].pos.z = 0.0f;

	tri3.triangleShader.constantColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	tri3.triangleShader.constantOffset = { 0.0f, 0.0f, 0.0f };
	//tri3.triangleShader.padding = { 6.0f, 6.0f };

	initializeTriangle(tri3);

	//Triangle tri4;
	//tri4.verts[0].pos.x = 0.50f;	tri4.verts[0].pos.y = -0.25f;	tri4.verts[0].pos.z = 0.5f;
	//tri4.verts[1].pos.x = 0.0f;		tri4.verts[1].pos.y = -0.25f;	tri4.verts[1].pos.z = 0.5f;
	//tri4.verts[2].pos.x = 0.25f;	tri4.verts[2].pos.y = 0.25f;	tri4.verts[2].pos.z = 0.5f;

	//tri4.triangleShader.constantColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//tri4.triangleShader.constantOffset = { 0.0f, 0.0f, 0.0f };

	//initializeTriangle(tri4);

	//////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////INITIALIZING SHAPES - END////////////////////////////////////////////////

	//creating pixel and vertex shader to use
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);

	//describing input layout for our position to GPU
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//creating actual input layout
	device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &layout);

}

void Game::Update(float delta)
{

	if (delta != 0 && delta < deltaTime)
		return;

	camPosition = XMVectorSet(xPos, yPos, zPos, 0.0f);
	camTarget = XMVectorSet(xTarget, yTarget, zTarget, 0.0f);
	camUp = XMVectorSet(xCam, yCam, zCam, 0.0f);

	FLOAT yScale = 1.0f / tanf(0.5f * verticalFOV);
	camProjection = XMMatrixPerspectiveFovLH(verticalFOV, aspectRatio, zNear, zFar);

	World = XMMatrixIdentity();

	WVP = World * camView * camProjection;

	cbPerObj.WVP = XMMatrixTranspose(WVP);

	context->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);

	context->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

}

void Game::Render()
{
	
	setBaseColor(0.0f, 0.0f, 0.0f, 1.0f);
	context->ClearDepthStencilView(dStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Drawing shapes///////////////////////////////////////////////////////

	//Triangles-
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		D3D11_BUFFER_DESC describeShader;
		describeShader.Usage = D3D11_USAGE_DYNAMIC;
		describeShader.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		describeShader.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		describeShader.ByteWidth = (sizeof(SEND_TO_RAM) + 12);			///////////////////////////////////////////////////////////////////////////////////////////
		describeShader.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA shaderData;
		shaderData.pSysMem = &triangles[i].triangleShader;
		shaderData.SysMemPitch = 0;
		shaderData.SysMemSlicePitch = 0;

		safeRelease(shaderBuffer);

		device->CreateBuffer(&describeShader, &shaderData, &shaderBuffer);

		context->VSSetConstantBuffers(0, 1, &shaderBuffer);

		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;

		context->VSSetShader(vertexShader, NULL, 0);
		context->PSSetShader(pixelShader, NULL, 0);

		context->IASetInputLayout(layout);

		D3D11_MAPPED_SUBRESOURCE resource;

		context->Map(shaderBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
		memcpy(resource.pData, &triangles[i].triangleShader, sizeof(triangles[i].triangleShader));
		context->Unmap(shaderBuffer, NULL);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->IASetVertexBuffers((UINT)0, (UINT)1, &triangles[i].triangleBuffer, &stride, &offset);

		context->Draw(3, 0);
	}

	//Lines-
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		D3D11_BUFFER_DESC describeShader;
		describeShader.Usage = D3D11_USAGE_DYNAMIC;
		describeShader.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		describeShader.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		describeShader.ByteWidth = (sizeof(SEND_TO_RAM));			///////////////////////////////////////////////////////////////////////////////////////////
		describeShader.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA shaderData;
		shaderData.pSysMem = &lines[i].lineShader;
		shaderData.SysMemPitch = 0;
		shaderData.SysMemSlicePitch = 0;

		safeRelease(shaderBuffer);

		device->CreateBuffer(&describeShader, &shaderData, &shaderBuffer);

		context->VSSetConstantBuffers(0, 1, &shaderBuffer);

		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;

		context->VSSetShader(vertexShader, NULL, 0);
		context->PSSetShader(pixelShader, NULL, 0);

		context->IASetInputLayout(layout);

		D3D11_MAPPED_SUBRESOURCE resource;

		context->Map(shaderBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
		memcpy(resource.pData, &lines[i].lineShader, sizeof(lines[i].lineShader));
		context->Unmap(shaderBuffer, NULL);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

		context->IASetVertexBuffers((UINT)0, (UINT)1, &lines[i].lineBuffer, &stride, &offset);

		context->Draw(2, 0);
	}

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

	safeRelease(dStencilView);
	safeRelease(depthStencilTexture);

	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		safeRelease(triangles[i].triangleBuffer);
	}

	for (unsigned int i = 0; i < lines.size(); i++)
	{
		safeRelease(lines[i].lineBuffer);
	}

	safeRelease(vertexShader);
	safeRelease(pixelShader);
	safeRelease(layout);
	safeRelease(shaderBuffer);

	safeRelease(cbPerObjectBuffer);
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

	//setting up device and swap chain//////////////////////////////////////////////////////////////////////////
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

	//setting up depth buffer/////////////////////////////////////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC dStencilTexDesc;

	dStencilTexDesc.Width = BACKBUFFER_WIDTH;
	dStencilTexDesc.Height = BACKBUFFER_HEIGHT;
	dStencilTexDesc.MipLevels = 1;
	dStencilTexDesc.ArraySize = 1;
	dStencilTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dStencilTexDesc.SampleDesc.Count = 1;
	dStencilTexDesc.SampleDesc.Quality = 0;
	dStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dStencilTexDesc.CPUAccessFlags = 0;
	dStencilTexDesc.MiscFlags = 0;

	device->CreateTexture2D(&dStencilTexDesc, NULL, &depthStencilTexture);
	device->CreateDepthStencilView(depthStencilTexture, NULL, &dStencilView);

	context->OMSetRenderTargets(1, &view, dStencilView);

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//setting up 3D shit

	D3D11_BUFFER_DESC cbbd;

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

}

void Game::initializeTriangle(Triangle triangle)
{

	D3D11_BUFFER_DESC describeTriangle;
	describeTriangle.Usage = D3D11_USAGE_IMMUTABLE;
	describeTriangle.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	describeTriangle.CPUAccessFlags = NULL;
	describeTriangle.ByteWidth = sizeof(triangle.verts);
	describeTriangle.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA triangleData;
	triangleData.pSysMem = triangle.verts;
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;

	device->CreateBuffer(&describeTriangle, &triangleData, &triangle.triangleBuffer);

	triangles.push_back(triangle);
}

void Game::initializeLine(Line line)
{
	D3D11_BUFFER_DESC describeLine;
	describeLine.Usage = D3D11_USAGE_IMMUTABLE;
	describeLine.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	describeLine.CPUAccessFlags = NULL;
	describeLine.ByteWidth = sizeof(line.verts);
	describeLine.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA lineData;
	lineData.pSysMem = line.verts;
	lineData.SysMemPitch = 0;
	lineData.SysMemSlicePitch = 0;

	device->CreateBuffer(&describeLine, &lineData, &line.lineBuffer);
	lines.push_back(line);
}