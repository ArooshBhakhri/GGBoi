#include "stdafx.h"

void Game::Initialize(HWND hwnd)
{

	time.Restart();

	initializeWindow(hwnd);


	lightDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//yellow
	lightColor = XMVectorSet(0.956f, 0.921f, 0.258f, 1.0f);
	
	//describing input layout for our position to GPU
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	//creating actual input layout
	device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &layout);

	//Load in textures
	CreateDDSTextureFromFile(device, L"bricks.dds", (ID3D11Resource**)&envTexture, &deWeySRV);
	
	CreateDDSTextureFromFile(device, L"Skybox.dds", (ID3D11Resource**)&skyboxTexture, &skyboxSRV);

	CreateDDSTextureFromFile(device, L"iron.dds", (ID3D11Resource**)&ironTexture, &ironSRV);

	CreateDDSTextureFromFile(device, L"trippy.dds", (ID3D11Resource**)&trippyTexture, &trippySRV);

	CreateDDSTextureFromFile(device, L"knuckle.dds", (ID3D11Resource**)&knuckleTexture, &knuckleSRV);

	CreateDDSTextureFromFile(device, L"knuckle.dds", (ID3D11Resource**)&groundTexture, &groundSRV);

	//Create models//////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////MAKING SKYBOX/////////////////////////////////////////////////////////
	geometry cube;

	//assigning cube verts
	MY_VERTEX verts[] = 
	{
		//			x		y			z			r			g			b			a		u		v		nX		nY		nZ
		// Front Face = bottom-left, top-left, top-right, bottom-right 
		MY_VERTEX(-1.0f,	-1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,	1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.75f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		-1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.75f,	0.66f,	0.0f,	0.0f,	0.0f),

		// Back Face
		MY_VERTEX(-1.0f,	-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,	0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f,	0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,	1.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,	0.33f,	0.0f,	0.0f,	0.0f),

		// Top Face
		MY_VERTEX(-1.0f,	1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,    1.0f,	    1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,	0.0f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,     1.0f,       1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.5017f,0.0f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,     1.0f,      -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.5017f,0.33f,	0.0f,	0.0f,	0.0f),

		// Bottom Face
		MY_VERTEX(-1.0f,   -1.0f,	   -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f, 1.0f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,    -1.0f,	   -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f, 0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,    -1.0f,	    1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f, 0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,   -1.0f,	    1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f, 1.0f,	0.0f,	0.0f,	0.0f),

		// Left Face 
		MY_VERTEX(-1.0f,   -1.0f,       1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,  0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,    1.0f,       1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.25f,  0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,    1.0f,      -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,   0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(-1.0f,   -1.0f,      -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,   0.66f,	0.0f,	0.0f,	0.0f),

		// Right Face
		MY_VERTEX(1.0f,		-1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.75f,	0.66f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		 1.0f,		-1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.75f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		 1.0f,		 1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f,	0.33f,	0.0f,	0.0f,	0.0f),
		MY_VERTEX(1.0f,		-1.0f,		 1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.50f,	0.66f,	0.0f,	0.0f,	0.0f)
	};

	for (unsigned int i = 0; i < ARRAYSIZE(verts); i++)
	{
		MY_VERTEX v;
		v = verts[i];
		cube.vertices.push_back(v);
	}

	//assigning cube indexes
	unsigned int indexes[] = 
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	for (int i = ARRAYSIZE(indexes) - 1; i > -1 ; i--)
	{
		cube.indices.push_back(indexes[i]);
	}

	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &cube.vertexShader);
	device->CreatePixelShader(Skybox_PS, sizeof(Skybox_PS), NULL, &cube.pixelShader);

	cube.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	cube.textureSRV = skyboxSRV;

	geometries.push_back(cube);
	/////////////////////////////////////////////////////////////////////////////////////////////

#pragma region ground
	///////////////////MAKING GROUND PLANE///////////////////////////////////////////////////////
	//geometry ground;

	//MY_VERTEX gVerts[] =
	//{
	//	//			x		y			z			r			g			b			a		u		v		nX		nY		nZ
	//	MY_VERTEX(-1.0f,	0.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f),
	//	MY_VERTEX(1.0f,		0.0f,		1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f),
	//	MY_VERTEX(-1.0f,	0.0f,	   -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	0.0f,	1.0f,	0.0f,	0.0f,	0.0f),
	//	MY_VERTEX(1.0f,		0.0f,	   -1.0f,		1.0f,		1.0f,		1.0f,		1.0f,	1.0f,	1.0f,	0.0f,	0.0f,	0.0f)
	//};

	//unsigned int gIndexes[] =
	//{
	//	0,	1,	2,
	//	2,	1,	3
	//};

	//for (unsigned int i = 0; i < ARRAYSIZE(gVerts); i++)
	//{
	//	ground.vertices.push_back(gVerts[i]);
	//}

	//for (unsigned int i = 0; i < ARRAYSIZE(gIndexes); i++)
	//{
	//	ground.indices.push_back(gIndexes[i]);
	//}

	//device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &ground.vertexShader);
	//device->CreatePixelShader(Skybox_PS, sizeof(Skybox_PS), NULL, &ground.pixelShader);

	//ground.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//ground.textureSRV = groundSRV;

	//geometries.push_back(ground);
	////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

	//Load models
	loadIOModel("test pyramid.obj", Trivial_PS, sizeof(Trivial_PS), Trivial_VS, sizeof(Trivial_VS), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ironSRV);

	loadIOModel("test pyramid.obj", Trivial_PS, sizeof(Trivial_PS), Trivial_VS, sizeof(Trivial_VS), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, trippySRV);

	CreateSphere(100, 100, Basic_PS, sizeof(Basic_PS), Trivial_VS, sizeof(Trivial_VS), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, trippySRV);

	//loadIOModel("knuckle.obj", Trivial_PS, sizeof(Trivial_PS), Trivial_VS, sizeof(Trivial_VS), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, knuckleSRV);


}

void Game::Update(float delta)
{

	if (delta != 0 && delta < deltaTime)
		return;

	time.Signal();

#pragma region camControls

	if (GetAsyncKeyState('W'))					//Move forward
	{
		zPos += (float)time.SmoothDelta() * moveSpeed;
		//zTarget += time.SmoothDelta() * moveSpeed;
	}
	else if (GetAsyncKeyState('S'))				//Move back
	{
		zPos -= (float)time.SmoothDelta() * moveSpeed;
		//zTarget -= time.SmoothDelta() * moveSpeed;
	}

	if (GetAsyncKeyState('A'))					//Move left
	{
		xPos -= (float)time.SmoothDelta() * moveSpeed;
	}
	else if (GetAsyncKeyState('D'))				//Move right
	{
		xPos += (float)time.SmoothDelta() * moveSpeed;
	}

	if (GetAsyncKeyState('Q'))					//Move up
	{
		yPos += (float)time.SmoothDelta() * moveSpeed;
	}
	else if (GetAsyncKeyState('E'))				//Move down
	{
		yPos -= (float)time.SmoothDelta() * moveSpeed;
	}

	if (GetAsyncKeyState('J'))					//Rotate left
	{
		yTarget -= (float)time.SmoothDelta() * moveSpeed;
	}
	else if (GetAsyncKeyState('L'))				//Rotate right
	{
		yTarget += (float)time.SmoothDelta() * moveSpeed;
	}

	if (GetAsyncKeyState('I'))					//Rotate up
	{
		xTarget -= (float)time.SmoothDelta() * moveSpeed;
	}
	else if (GetAsyncKeyState('K'))				//Rotate down
	{
		xTarget += (float)time.SmoothDelta() * moveSpeed;
	}

	if (GetAsyncKeyState('Z'))
	{
		zNear -= (float)time.SmoothDelta();
	}
	else if (GetAsyncKeyState('X'))
	{
		zNear += (float)time.SmoothDelta();
	}

	if (GetAsyncKeyState('C'))
	{
		zFar -= (float)time.SmoothDelta();
	}
	else if (GetAsyncKeyState('V'))
	{
		zFar += (float)time.SmoothDelta();
	}

	if (GetAsyncKeyState('B'))
	{
		if(verticalFOV >= 0.1f)
			verticalFOV -= (float)time.SmoothDelta();
	}
	else if (GetAsyncKeyState('N'))
	{
		verticalFOV += (float)time.SmoothDelta();
	}

#pragma endregion

#pragma region cameraStuff

	camPosition = XMVectorSet(xPos, yPos, zPos, 0.0f);

	camRotation = XMMatrixRotationRollPitchYaw(xTarget, yTarget, 0);
	camTarget = XMVector3TransformCoord(defForward, camRotation);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX rotateY;
	rotateY = XMMatrixRotationY(yTarget);

	camRight = XMVector2TransformCoord(defRight, rotateY);
	camUp = XMVectorSet(xCam, yCam, zCam, 0.0f);
	camForward = XMVector3TransformCoord(defForward, rotateY);

	camPosition += xCam * camRight;
	camPosition += zCam * camForward;

	xCam = 0.0f;
	zCam = 0.0f;

	camTarget = camPosition + camTarget;

	FLOAT yScale = 1.0f / tanf(0.5f * verticalFOV);
	camProjection = XMMatrixPerspectiveFovLH(verticalFOV, aspectRatio, zNear, zFar);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

#pragma endregion

#pragma region constantBuffer
	XMVECTOR timeVar;
	timeVar = XMVectorSet((float)time.Delta(), (float)time.SmoothDelta(), (float)time.TotalTime(), 0.0f);

	XMStoreFloat3(&cbPerObj.time, timeVar);
	XMStoreFloat3(&cbPerObj.lightDir, lightDir);
	XMStoreFloat4(&cbPerObj.lightColor, lightColor);
#pragma endregion

	
	//change wvp for each object in render to set it's position/behaviour////////////////////////////////////////////////
	//skybox
	geometries[0].matrix = XMMatrixScaling(500.0f, 500.0f, 500.0f);
	geometries[0].matrix = geometries[0].matrix * XMMatrixTranslation(xPos, yPos, zPos);

	//geometries[1].matrix = XMMatrixIdentity() * XMMatrixScaling(10.0f, 10.0f, 1.0f);

	//left most
	geometries[1].matrix = XMMatrixIdentity();
	geometries[1].matrix = geometries[1].matrix * XMMatrixRotationY((float)time.TotalTime()) *  XMMatrixTranslation(sinf((float)time.TotalTime()), -2 * cosf((float)time.TotalTime()), -0.5f * sin((float)time.TotalTime()));

	//right most
	geometries[2].matrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	geometries[2].matrix = geometries[2].matrix * XMMatrixRotationZ((float)time.TotalTimeExact());

	geometries[3].matrix = XMMatrixIdentity();

	/*geometries[3].matrix = XMMatrixTranslation(1.0f, 2 * sinf((float)time.TotalTime()), 1.0f);
	geometries[3].matrix = geometries[3].matrix * XMMatrixRotationY(time.TotalTime());*/

	//geometries[3].matrix = XMMatrixTranslation(0.0f, 5.0f, 10.0f) * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixRotationY(155);

}

void Game::Render()
{
	
	setBaseColor(0.0f, 0.0f, 0.0f, 1.0f);
	context->ClearDepthStencilView(dStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->IASetInputLayout(layout);

	HRESULT result;

	//setting sampler for all pixel shaders//////////////////////////////////////////////
#pragma region sampler
	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	safeRelease(sampler);
	result = device->CreateSamplerState(&samplerDesc, &sampler);

	context->PSSetSamplers(0, 1, &sampler);
#pragma endregion
	///////////////////////////////////////////////////////////////////////////////////

//drawing loop
for (unsigned int i = 0; i < geometries.size(); i++)
	{

		context->VSSetShader(geometries[i].vertexShader, 0, 0);
		context->PSSetShader(geometries[i].pixelShader, 0, 0);

		WVP = geometries[i].matrix * camView * camProjection;
		XMStoreFloat4x4(&cbPerObj.WVP, WVP);
		XMStoreFloat4x4(&cbPerObj.World, geometries[i].matrix);

		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		cBufferDesc.ByteWidth = sizeof(cbPerObj) + 8;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = 0;
		cBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA cBufferSubData;
		cBufferSubData.pSysMem = &cbPerObj;
		cBufferSubData.SysMemPitch = 0;
		cBufferSubData.SysMemSlicePitch = 0;

		//setting up constant buffer
		safeRelease(cbPerObjectBuffer);
		result = device->CreateBuffer(&cBufferDesc, &cBufferSubData, &cbPerObjectBuffer);
		context->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

		ID3D11ShaderResourceView* textureSRV[] = { geometries[i].textureSRV };
		context->PSSetShaderResources(0, 1, textureSRV);

		//creating vertex buffer
		ZeroMemory(&geometries[i].vertexBuffer, sizeof(geometries[i].vertexBuffer));

		geometries[i].vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
		geometries[i].vertexBuffer.ByteWidth = sizeof(MY_VERTEX) * geometries[i].vertices.size();
		geometries[i].vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		geometries[i].vertexBuffer.CPUAccessFlags = 0;
		geometries[i].vertexBuffer.MiscFlags = 0;

		ZeroMemory(&geometries[i].vertexBufferSubData, sizeof(geometries[i].vertexBufferSubData));
		geometries[i].vertexBufferSubData.pSysMem = geometries[i].vertices.data();
		geometries[i].vertexBufferSubData.SysMemPitch = 0;
		geometries[i].vertexBufferSubData.SysMemSlicePitch = 0;

		UINT stride1 = sizeof(MY_VERTEX);
		UINT offset1 = 0;

		safeRelease(vertexBuffer);
		result = device->CreateBuffer(&geometries[i].vertexBuffer, &geometries[i].vertexBufferSubData, &vertexBuffer);
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride1, &offset1);

		//setting up index buffer
		ZeroMemory(&geometries[i].indexBuffer, sizeof(geometries[i].indexBuffer));
		geometries[i].indexBuffer.Usage = D3D11_USAGE_DEFAULT;
		geometries[i].indexBuffer.ByteWidth = sizeof(unsigned int) * geometries[i].indices.size();
		geometries[i].indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
		geometries[i].indexBuffer.CPUAccessFlags = 0;
		geometries[i].indexBuffer.MiscFlags = 0;

		ZeroMemory(&geometries[i].indexBufferSubData, sizeof(geometries[i].indexBufferSubData));
		geometries[i].indexBufferSubData.pSysMem = geometries[i].indices.data();
		geometries[i].indexBufferSubData.SysMemPitch = 0;
		geometries[i].indexBufferSubData.SysMemSlicePitch = 0;

		safeRelease(indexBuffer);
		device->CreateBuffer(&geometries[i].indexBuffer, &geometries[i].indexBufferSubData, &indexBuffer);

		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//setting final drawing stuff
		context->IASetPrimitiveTopology(geometries[i].topology);

		context->DrawIndexed(geometries[i].indices.size(), 0, 0);
	}

	//////////////////////////////////
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

	for (unsigned int i = 0; i < geometries.size(); i++)
	{
		safeRelease(geometries[i].vertexShader);
		safeRelease(geometries[i].pixelShader);
		
	}

	safeRelease(vertexBuffer);
	safeRelease(indexBuffer);

	safeRelease(layout);
	safeRelease(shaderBuffer);

	safeRelease(cbPerObjectBuffer);

	safeRelease(envTexture);
	safeRelease(deWeySRV);

	safeRelease(ironTexture);
	safeRelease(ironSRV);

	safeRelease(trippyTexture);
	safeRelease(trippySRV);

	safeRelease(knuckleTexture);
	safeRelease(knuckleSRV);

	safeRelease(groundTexture);
	safeRelease(groundSRV);

	safeRelease(sampler);

	safeRelease(skyboxTexture);
	safeRelease(skyboxIndexBuffer);
	safeRelease(skyboxVertexBuffer);
	safeRelease(Skybox_VShader);
	safeRelease(Skybox_PShader);
	safeRelease(skyboxSRV);
	safeRelease(skyboxDSS);
	safeRelease(skyboxRS);

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
	scd.SampleDesc.Count = 4;
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
	dStencilTexDesc.SampleDesc.Count = 4;
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

}

//depracated shit
/*
void Game::loadPyramid(const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse)
{
	geometry geometryToPush;

	geometryToPush.topology = topologyToUse;
	geometryToPush.textureSRV = textureToUse;

	HRESULT result;

	result = device->CreatePixelShader(pixelShaderData, psSize, NULL, &geometryToPush.pixelShader);

	result = device->CreateVertexShader(vertexShaderData, vsSize, NULL, &geometryToPush.vertexShader);

	geometryToPush.vertices.resize(numPyramidVerts);

	for (unsigned int i = 0; i < numPyramidVerts; i++)
	{
		//assigning vert pos
		geometryToPush.vertices[i].pos.x = pyramidData[i].pos[0];
		geometryToPush.vertices[i].pos.y = pyramidData[i].pos[1];
		geometryToPush.vertices[i].pos.z = pyramidData[i].pos[2];

		//assigning rgba
		//assign random color
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		geometryToPush.vertices[i].rgba = { r, g, b, 1.0f };

		//assigning uvs
		geometryToPush.vertices[i].texPos.x = pyramidData[i].uvw[0];
		geometryToPush.vertices[i].texPos.y = pyramidData[i].uvw[1];

		//load normals
		geometryToPush.vertices[i].normals.x = pyramidData[i].nrm[0];
		geometryToPush.vertices[i].normals.y = pyramidData[i].nrm[1];
		geometryToPush.vertices[i].normals.z = pyramidData[i].nrm[2];
	}

	geometryToPush.indices.resize(numPyramidIndices);

	for (unsigned int i = 0; i < numPyramidIndices; i++)
	{
		geometryToPush.indices[i] = pyramidIndices[i];
	}

	geometries.push_back(geometryToPush);

}
*/

void Game::loadIOModel(char* file, const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse)
{
	ifstream objFile;

	objFile.open(file);

	if (objFile.is_open())
	{
		geometry geometryToPush;

		geometryToPush.topology = topologyToUse;
		geometryToPush.textureSRV = textureToUse;

		HRESULT result;

		result = device->CreatePixelShader(pixelShaderData, psSize, NULL, &geometryToPush.pixelShader);

		result = device->CreateVertexShader(vertexShaderData, vsSize, NULL, &geometryToPush.vertexShader);

		char checker;

		vector<XMFLOAT3> position;
		vector<XMFLOAT2> texturePos;
		vector<XMFLOAT3> normals;
		vector<MY_TRIANGLE> triangles;

		while (objFile)
		{

			checker = objFile.get();

			switch (checker)
			{
			case '#':
				while (checker != '#')
				{
					checker = objFile.get();
				}
				break;

			case 'v':
				checker = objFile.get();
				if (checker == ' ')
				{
					float x, y, z;
					objFile >> x >> y >> z;

					XMFLOAT3 pushThis;
					pushThis = { x, y, z };
					position.push_back(pushThis);
				}
				else if (checker == 't')
				{
					float u, v;
					objFile >> u >> v;

					XMFLOAT2 pushThis;
					pushThis = { u, v };
					texturePos.push_back(pushThis);
				}
				else if(checker == 'n')
				{
					float x, y, z;
					objFile >> x >> y >> z;

					XMFLOAT3 pushThis;
					pushThis = { x, y, z };
					normals.push_back(pushThis);
				}
				break;

			case'f':
				checker = objFile.get();
				if (checker == ' ')
				{
					string line;
					getline(objFile, line);
					
					unsigned int s1Index = 0;
					unsigned int s2Index = 0;

					for (unsigned int i = 0; i < line.size(); i++)
					{
						if (line[i] == ' ' && s1Index == 0)
							s1Index = i;
						else if (line[i] == ' ')
						{
							s2Index = i;
							break;
						}
					}

					string face1;
					string face2;
					string face3;
					
					for (unsigned int i = 0; i < s1Index; i++)
					{
						face1.push_back(line[i]);
					}

					for (unsigned int i = s1Index; i < s2Index; i++)
					{
						face2.push_back(line[i]);
					}

					for (unsigned int i = s2Index; i < line.size(); i++)
					{
						face3.push_back(line[i]);
					}
					
					string value1;
					string value2;
					string value3;

					s1Index = 0;

					for (unsigned int i = 0; i < face1.size(); i++)
					{
						if (face1[i] == '/' && s1Index == 0)
							s1Index = i;
						else if (face1[i] == '/')
						{
							s2Index = i;
							break;
						}
					}

					for (unsigned int i = 0; i < s1Index; i++)
					{
						value1.push_back(face1[i]);
					}

					for (unsigned int i = s1Index + 1; i < s2Index; i++)
					{
						value2.push_back(face1[i]);
					}

					for (unsigned int i = s2Index + 1; i < face1.size(); i++)
					{
						value3.push_back(face1[i]);
					}


					float v1 = stof(value1.c_str());
					float v2 = stof(value2.c_str());
					float v3 = stof(value3.c_str());

					MY_TRIANGLE pushThis;
					pushThis.indices = { (int)v1, (int)v2, (int)v3 };

		
					triangles.push_back(pushThis);

					string value11;
					string value12;
					string value13;

					s1Index = 0;

					for (unsigned int i = 0; i < face2.size(); i++)
					{
						if (face2[i] == '/' && s1Index == 0)
							s1Index = i;
						else if (face2[i] == '/')
						{
							s2Index = i;
							break;
						}
					}

					for (unsigned int i = 0; i < s1Index; i++)
					{
						value11.push_back(face2[i]);
					}

					for (unsigned int i = s1Index + 1; i < s2Index; i++)
					{
						value12.push_back(face2[i]);
					}

					for (unsigned int i = s2Index + 1; i < face2.size(); i++)
					{
						value13.push_back(face2[i]);
					}

					float v11 = stof(value11.c_str());
					float v12 = stof(value12.c_str());
					float v13 = stof(value13.c_str());

					MY_TRIANGLE pushThis1;
					pushThis1.indices = { (int)v11, (int)v12, (int)v13 };

					triangles.push_back(pushThis1);

					string value21;
					string value22;
					string value23;

					s1Index = 0;

					for (unsigned int i = 0; i < face3.size(); i++)
					{
						if (face3[i] == '/' && s1Index == 0)
							s1Index = i;
						else if (face3[i] == '/')
						{
							s2Index = i;
							break;
						}
					}

					for (unsigned int i = 0; i < s1Index; i++)
					{
						value21.push_back(face3[i]);
					}

					for (unsigned int i = s1Index + 1; i < s2Index; i++)
					{
						value22.push_back(face3[i]);
					}

					for (unsigned int i = s2Index + 1; i < face3.size(); i++)
					{
						value23.push_back(face3[i]);
					}

					float v21 = stof(value21.c_str());
					float v22 = stof(value22.c_str());
					float v23 = stof(value23.c_str());

					MY_TRIANGLE pushThis2;
					pushThis2.indices = { (int)v21, (int)v22, (int)v23 };

					triangles.push_back(pushThis2);

				}
				break;

			default:
				break;
			}

		}

		//loading pos, uvs, normals and color
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			MY_VERTEX pushThis;

			//assign position
			pushThis.pos = position[triangles[i].indices.x - 1];

			//assign uvs
			pushThis.texPos.x = texturePos[triangles[i].indices.y - 1].x;
			pushThis.texPos.y = 1 - texturePos[triangles[i].indices.y - 1].y;

			//assign normals
			unsigned int index = (triangles[i].indices.z - 1);
			pushThis.normals = normals[index];
			
			//assign random color
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			pushThis.rgba = {1.0f, 1.0f, 1.0f, 1.0f };

			geometryToPush.vertices.push_back(pushThis);
		}

		//loading index
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			bool match = false;
			unsigned int matchIndex = 0;
			for (unsigned int j = 0; j < geometryToPush.indices.size(); j++)
			{
				if (triangles[i] == triangles[j] && i != j)
				{
					match = true;
					matchIndex = j;
					break;
				}
			}

			if (match)
			{
				geometryToPush.indices.push_back(matchIndex);
			}
			else
				geometryToPush.indices.push_back(i);
		}

		geometries.push_back(geometryToPush);

	}
}

void Game::CreateSphere(unsigned int latLines, unsigned int longLines, const BYTE* pixelShaderData, SIZE_T psSize, const BYTE* vertexShaderData, SIZE_T vsSize, D3D11_PRIMITIVE_TOPOLOGY topologyToUse, ID3D11ShaderResourceView* textureToUse)
{
	numSphereVerts = ((latLines - 2) * longLines + 2);
	numSphereFaces = ((latLines - 3) * (longLines) * 2) + (longLines * 2);

	float yaw = 0.0f;
	float pitch = 0.0f;

	geometry sphere;

	sphere.topology = topologyToUse;

	HRESULT result;

	ZeroMemory(&sphere.pixelShader, sizeof(sphere.pixelShader));
	result = device->CreatePixelShader(pixelShaderData, psSize, NULL, &sphere.pixelShader);

	ZeroMemory(&sphere.vertexShader, sizeof(sphere.vertexShader));
	result = device->CreateVertexShader(vertexShaderData, vsSize, NULL, &sphere.vertexShader);

	sphere.textureSRV = textureToUse;


	vector<MY_VERTEX> sphereVerts(numSphereVerts);

	XMVECTOR currVertexPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	sphereVerts[0].pos = { 0.0f, 0.0f, 1.0f };
	sphereVerts[0].rgba = { 1.0f, 1.0f, 1.0f, 1.0f };
	sphereVerts[0].texPos = { 0.0f, 0.0f };
	sphereVerts[0].normals = { 0.0f, 0.0f, 0.0f };

	for (unsigned int i = 0; i < latLines - 2; i++)
	{
		pitch = (i + 1) * (3.14f / (latLines - 1));
		XMMATRIX rotX = XMMatrixRotationX(pitch);

		for (unsigned int j = 0; j < longLines; j++)
		{

			yaw = j * (6.28f / (longLines));
			XMMATRIX rotZ = XMMatrixRotationZ(yaw);
			currVertexPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotX * rotZ));
			currVertexPos = XMVector3Normalize(currVertexPos);
			sphereVerts[i * longLines + j + 1].pos = { XMVectorGetX(currVertexPos), XMVectorGetY(currVertexPos), XMVectorGetZ(currVertexPos) };

		}
	}

	sphereVerts[numSphereVerts - 1].pos = { 0.0f, 0.0f, -1.0f };

	vector<unsigned int> indexes(numSphereFaces * 3);

	unsigned int index = 0;


	for (unsigned int i = 0; i < longLines - 1; i++, index += 3)
	{
		indexes[index] = 0;
		indexes[index + 1] = i + 1;
		indexes[index + 2] = i + 2;
	}

	indexes[index] = 0;
	indexes[index + 1] = longLines;
	indexes[index + 2] = 1;
	index += 3;

	for (unsigned int i = 0; i < latLines - 3; i++)
	{
		for (unsigned int j = 0; j < longLines - 1; j++)
		{
			indexes[index] = i * longLines + j + 1;
			indexes[index + 1] = i * longLines + j + 2;
			indexes[index + 2] = (i + 1) * longLines + j + 1;

			indexes[index + 3] = (i + 1) * longLines + j + 1;
			indexes[index + 4] = i * longLines + j + 2;
			indexes[index + 5] = (i + 1) * longLines + j + 2;

			index += 6;
		}

		indexes[index] = (i + longLines) + longLines;
		indexes[index + 1] = (i * longLines) + 1;
		indexes[index + 2] = ((i + 1) * longLines) + longLines;

		indexes[index + 3] = ((i + 1) * longLines) + longLines;
		indexes[index + 4] = (i * longLines) + 1;
		indexes[index + 5] = ((i + 1) * longLines) + 1;

		index += 6;
	}

	for (unsigned int i = 0; i < longLines - 1; i++)
	{
		indexes[index] = numSphereVerts - 1;
		indexes[index + 1] = (numSphereVerts - 1) - (i + 1);
		indexes[index + 2] = (numSphereVerts - 1) - (i + 2);
	}

	indexes[index] = numSphereVerts - 1;
	indexes[index + 1] = (numSphereVerts - 1) - longLines;
	indexes[index + 2] = numSphereVerts - 2;

	sphere.vertices = sphereVerts;

	sphere.indices = indexes;

	for (unsigned int i = 0; i < sphere.vertices.size(); i++)
	{
		sphere.vertices[i].rgba = { 0.0f, 0.0f, 1.0f, 1.0f };
	}

	geometries.push_back(sphere);

}