#include "stdafx.h"

void Game::Initialize(HWND hwnd)
{

	time.Restart();

	initializeWindow(hwnd);

	//loadPyramid();

	loadIOModel();

	lightDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	lightColor = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	//creating pixel and vertex shader to use
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);

	//describing input layout for our position to GPU
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//creating actual input layout
	HRESULT result1 = device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &layout);

	//Load in textures
	HRESULT result = CreateDDSTextureFromFile(device, L"gg.dds", (ID3D11Resource**)&envTexture, &envView);

}

void Game::Update(float delta)
{

	if (delta != 0 && delta < deltaTime)
		return;

	time.Signal();

#pragma region camControls

	if (GetAsyncKeyState('W'))					//Move forward
	{
		zPos += time.SmoothDelta() * 25;
		zTarget += time.SmoothDelta() * 25;
	}
	else if (GetAsyncKeyState('S'))				//Move back
	{
		zPos -= time.SmoothDelta() * 25;
		zTarget -= time.SmoothDelta() * 25;
	}

	if (GetAsyncKeyState('A'))					//Move left
	{
		xPos -= time.SmoothDelta() * 25;
		xTarget -= time.SmoothDelta() * 25;
	}
	else if (GetAsyncKeyState('D'))				//Move right
	{
		xPos += time.SmoothDelta() * 25;
		xTarget += time.SmoothDelta() * 25;
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
		if(verticalFOV >= 0.1f)
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
		0,									0,			5.0f,															1
	};

	WVP = World * camView * camProjection;
	
	XMStoreFloat4x4(&cbPerObj.WVP, WVP);
	
	XMVECTOR timeVar;
	timeVar = XMVectorSet(time.Delta(), time.SmoothDelta(), time.TotalTime(), 0.0f);

	XMStoreFloat3(&cbPerObj.time, timeVar);
	XMStoreFloat3(&cbPerObj.lightDir, lightDir);
	XMStoreFloat4(&cbPerObj.lightColor, lightColor);

	D3D11_BUFFER_DESC vsBuffer;

	ZeroMemory(&vsBuffer, sizeof(vsBuffer));

	vsBuffer.Usage = D3D11_USAGE_DEFAULT;
	vsBuffer.ByteWidth = sizeof(cbPerObj) + 8;
	vsBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsBuffer.CPUAccessFlags = 0;
	vsBuffer.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA vsSubData;

	ZeroMemory(&vsSubData, sizeof(vsSubData));

	vsSubData.pSysMem = &cbPerObj;
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

	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	safeRelease(sampler);
	device->CreateSamplerState(&samplerDesc, &sampler);

	context->PSSetSamplers(0, 1, &sampler);

	//Drawing shapes///////////////////////////////////////////////////////

	//Triangles-

	context->VSSetShader(vertexShader, NULL, 0);
	context->PSSetShader(pixelShader, NULL, 0);

	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;

	ZeroMemory(&indexBufferData, sizeof(indexBufferData));

	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	safeRelease(indexBuffer);

	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC drawingBufferDesc;
	
	ZeroMemory(&drawingBufferDesc, sizeof(drawingBufferDesc));

	drawingBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	drawingBufferDesc.ByteWidth = sizeof(MY_VERTEX) * vertices.size();
	drawingBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	drawingBufferDesc.CPUAccessFlags = 0;
	drawingBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA drawingBufferSubresource;

	ZeroMemory(&drawingBufferSubresource, sizeof(drawingBufferSubresource));

	drawingBufferSubresource.pSysMem = vertices.data();
	drawingBufferSubresource.SysMemPitch = 0;
	drawingBufferSubresource.SysMemSlicePitch = 0;

	safeRelease(drawingBuffer);

	device->CreateBuffer(&drawingBufferDesc, &drawingBufferSubresource, &drawingBuffer);

	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &drawingBuffer, &stride, &offset);
	
	context->IASetInputLayout(layout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(indices.size(), 0, 0);

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

	safeRelease(envTexture);
	safeRelease(envView);
	safeRelease(sampler);

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

	//setting up 3D shit

	//D3D11_BUFFER_DESC cbbd;

	//ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	//cbbd.Usage = D3D11_USAGE_DEFAULT;
	//cbbd.ByteWidth = sizeof(cbPerObject);
	//cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbbd.CPUAccessFlags = 0;
	//cbbd.MiscFlags = 0;

}

//input 1 for pyramid, 2 for house
void Game::loadPyramid()
{

	vertices.resize(numPyramidVerts);

	for (unsigned int i = 0; i < numPyramidVerts; i++)
	{
		//assigning vert pos
		vertices[i].pos.x = pyramidData[i].pos[0];
		vertices[i].pos.y = pyramidData[i].pos[1];
		vertices[i].pos.z = pyramidData[i].pos[2];

		//assigning rgba
		vertices[i].rgba = { 1.0f, 1.0f, 1.0f, 1.0f };

		//assigning uvs
		vertices[i].texPos.x = pyramidData[i].uvw[0];
		vertices[i].texPos.y = pyramidData[i].uvw[1];

		//load normals
		vertices[i].normals.x = pyramidData[i].nrm[0];
		vertices[i].normals.y = pyramidData[i].nrm[1];
		vertices[i].normals.z = pyramidData[i].nrm[2];
	}

	indices.resize(numPyramidIndices);

	for (unsigned int i = 0; i < numPyramidIndices; i++)
	{
		indices[i] = pyramidIndices[i];
	}

}

void Game::loadIOModel()
{
	ifstream objFile;

	objFile.open("House2.obj");

	if (objFile.is_open())
	{

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

			/*		bool match = false;
					for (unsigned int i = 0; i < triangles.size(); i++)
					{
						if (triangles[i] == pushThis)
						{
							match = true;
							break;
						}
					}
					if (!match)*/
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

					/*match = false;
					for (unsigned int i = 0; i < triangles.size(); i++)
					{
						if (triangles[i] == pushThis1)
						{
							match = true;
							break;
						}
					}
					if (!match)*/
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

					/*match = false;
					for (unsigned int i = 0; i < triangles.size(); i++)
					{
						if (triangles[i] == pushThis2)
						{
							match = true;
							break;
						}
					}
					if (!match)*/
						triangles.push_back(pushThis2);

				}
				break;

			default:
				break;
			}

		}

		//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		for (unsigned int i = 0; i < position.size(); i++)
		{
			MY_VERTEX pushThis;

			//assign position
			pushThis.pos = position[i];

			//assign normals
			//pushThis.normals = normals[triangles[i].indices.y - 1];

			//assign uvs
			pushThis.texPos = texturePos[triangles[i].indices.z - 1];
			
			//assign random color
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			pushThis.rgba = { r, g, b, 1.0f };

			vertices.push_back(pushThis);
		}

		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			bool match = false;
			unsigned int matchIndex = 0;
			for (unsigned int j = 0; j < indices.size(); j++)
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
				indices.push_back(matchIndex);
			}
			else
				indices.push_back(i);
		}

	}
}