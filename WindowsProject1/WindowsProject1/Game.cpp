#include "stdafx.h"

void Game::Initialize(HWND hwnd)
{

	time.Restart();

	initializeWindow(hwnd);

	//creating pixel and vertex shader to use
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);

	//describing input layout for our position to GPU
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//creating actual input layout
	device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &layout);

	//Load in textures

#pragma region oldCode

	HRESULT result = CreateDDSTextureFromFile(device, L"gg.dds", (ID3D11Resource**)&envTexture, &envView);


#pragma endregion



}

void Game::Update(float delta)
{

	if (delta != 0 && delta < deltaTime)
		return;

	time.Signal();

#pragma region camControls

	if (GetAsyncKeyState('W'))					//Move forward
	{
		zPos += time.SmoothDelta();
		zTarget += time.SmoothDelta();
	}
	else if (GetAsyncKeyState('S'))				//Move back
	{
		zPos -= time.SmoothDelta();
		zTarget -= time.SmoothDelta();
	}

	if (GetAsyncKeyState('A'))					//Move left
	{
		xPos -= time.SmoothDelta();
		xTarget -= time.SmoothDelta();
	}
	else if (GetAsyncKeyState('D'))				//Move right
	{
		xPos += time.SmoothDelta();
		xTarget += time.SmoothDelta();
	}

	if (GetAsyncKeyState('Q'))					//Move up
	{
		yPos += time.SmoothDelta();
		yTarget += time.SmoothDelta();
	}
	else if (GetAsyncKeyState('E'))				//Move down
	{
		yPos -= time.SmoothDelta();
		yTarget -= time.SmoothDelta();
	}

	if (GetAsyncKeyState('J'))					//Rotate left
	{
		xTarget -= time.SmoothDelta();
	}
	else if (GetAsyncKeyState('L'))				//Rotate right
	{
		xTarget += time.SmoothDelta();
	}

	if (GetAsyncKeyState('I'))					//Rotate up
	{
		yTarget += time.SmoothDelta();
	}
	else if (GetAsyncKeyState('K'))				//Rotate down
	{
		yTarget -= time.SmoothDelta();
	}

	if (GetAsyncKeyState('Z'))
	{
		zNear -= time.SmoothDelta();
	}
	else if (GetAsyncKeyState('X'))
	{
		zNear += time.SmoothDelta();
	}

	if (GetAsyncKeyState('C'))
	{
		zFar -= time.SmoothDelta();
	}
	else if (GetAsyncKeyState('V'))
	{
		zFar += time.SmoothDelta();
	}

	if (GetAsyncKeyState('B'))
	{
		verticalFOV -= time.SmoothDelta();
	}
	else if (GetAsyncKeyState('N'))
	{
		verticalFOV += time.SmoothDelta();
	}

#pragma endregion

	camPosition = XMVectorSet(xPos, yPos, zPos, 0.0f);
	camTarget = XMVectorSet(xTarget, yTarget, zTarget, 0.0f);
	camUp = XMVectorSet(xCam, yCam, zCam, 0.0f);

	FLOAT yScale = 1.0f / tanf(0.5f * verticalFOV);
	camProjection = XMMatrixPerspectiveFovLH(verticalFOV, aspectRatio, zNear, zFar);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	World = 
	{
		cosf(XMConvertToRadians((float)time.TotalTime() * 40.0f)),		0,			sinf(XMConvertToRadians((float)time.TotalTime() * 40.0f)),		0,
		0,									1,			0,															0,
		-sinf(XMConvertToRadians((float)time.TotalTime() * 40.0f)),	0,			cosf(XMConvertToRadians((float)time.TotalTime() * 40.0f)),		0,
		0,									0,		1.0f,															1
	};

	WVP = World * camView * camProjection;

	XMStoreFloat4x4(&cbPerObj.WVP, WVP);

	D3D11_BUFFER_DESC vsBuffer;

	ZeroMemory(&vsBuffer, sizeof(vsBuffer));

	vsBuffer.Usage = D3D11_USAGE_DEFAULT;
	vsBuffer.ByteWidth = sizeof(cbPerObj);
	vsBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsBuffer.CPUAccessFlags = 0;
	vsBuffer.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vsSubData;

	ZeroMemory(&vsSubData, sizeof(vsSubData));

	vsSubData.pSysMem = &cbPerObj.WVP;
	vsSubData.SysMemPitch = 0;
	vsSubData.SysMemSlicePitch = 0;

	safeRelease(cbPerObjectBuffer);
	device->CreateBuffer(&vsBuffer, &vsSubData, &cbPerObjectBuffer);

	context->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

}

void Game::Render()
{
	
	setBaseColor(0.0f, 0.0f, 0.0f, 1.0f);
	context->ClearDepthStencilView(dStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11ShaderResourceView* texSRV[] = { envView };

	context->PSSetShaderResources(0, 1, texSRV);

	//Drawing shapes///////////////////////////////////////////////////////

	//Triangles-

	context->VSSetShader(vertexShader, NULL, 0);
	context->PSSetShader(pixelShader, NULL, 0);

	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;

	ZeroMemory(&indexBufferData, sizeof(indexBufferData));

	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	safeRelease(indexBuffer);

	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC drawingBufferDesc;
	
	ZeroMemory(&drawingBufferDesc, sizeof(drawingBufferDesc));

	drawingBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	drawingBufferDesc.ByteWidth = sizeof(MY_VERTEX) * ARRAYSIZE(vertices);
	drawingBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	drawingBufferDesc.CPUAccessFlags = 0;
	drawingBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA drawingBufferSubresource;

	ZeroMemory(&drawingBufferSubresource, sizeof(drawingBufferSubresource));

	drawingBufferSubresource.pSysMem = vertices;
	drawingBufferSubresource.SysMemPitch = 0;
	drawingBufferSubresource.SysMemSlicePitch = 0;

	safeRelease(drawingBuffer);

	device->CreateBuffer(&drawingBufferDesc, &drawingBufferSubresource, &drawingBuffer);

	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &drawingBuffer, &stride, &offset);
	
	context->IASetInputLayout(layout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(ARRAYSIZE(indices), 0, 0);

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

	safeRelease(drawingBuffer);
	safeRelease(indexBuffer);

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

}