#include "d3dc.h"



D3Dc::D3Dc()
{
	device		= 0;
	devCon		= 0;
	swapChain	= 0;
	targetView	= 0;
	depthBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
}

D3Dc::D3Dc(const D3Dc &other)
{
}


D3Dc::~D3Dc()
{
	if (rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}
	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}
	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}
	if (depthBuffer)
	{
		depthBuffer->Release();
		depthBuffer = 0;
	}
	if (targetView)
	{
		targetView->Release();
		targetView = 0;
	}
	if (devCon)
	{
		devCon->Release();
		devCon = 0;
	}
	if (device)
	{
		device->Release();
		device = 0;
	}
	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}
}

bool D3Dc::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	// Win Result code
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterout;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	// Store the vsync setting.
	vsync_on = vsync;

	// graphics facotry interface
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;
	//Create adapter using factory
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;
	//Primary monitor enumeration
	result = adapter->EnumOutputs(0, &adapterout);
	if (FAILED(result)) return false;
	// Get A alist of Display modes fitting the DXGI_FORMAT norm for 32 colors
	result = adapterout->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) return false;
	// get posiible display combinations for videoCard/Monitor
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;
	// Fill the  mode list intop the adapteroutput
	result = adapterout->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	//search throug the modes and get widthheight enumerator and denominator for refreshrate, needed to vsync
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// get video Card name
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// Store the dedicated video card memory in megabytes.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// convert Name to char array of length 128
	error = wcstombs_s(&stringLength, vCardDesc, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// release unneeded values
	delete[] displayModeList;
	displayModeList = 0;
	adapterout->Release();
	adapterout = 0;
	adapter->Release();
	adapter = 0;
	factory->Release();
	factory = 0;

	// Initialize the swapchain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	// set the format to 32 bit surface
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//vsync settings
	if (vsync_on)
	{
		//Adapt render speed to the refresh of the Monitor
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// Render as fast as possible
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// Set the Backbuffer as the standard output for rendering
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the window for rendering
	swapChainDesc.OutputWindow = hwnd;

	// Disable Mulitsampling
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	// Senable windowd mode
	swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set the direct X version to be used
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//create the device and swapchain 
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &devCon);
	if (FAILED(result)) return false;
	
	// Get a Back buffer pointer
	result = swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D),(LPVOID*)&backBufferPtr);
	if (FAILED(result)) return false;
	
	// create rendertargetView
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &targetView);
	if (FAILED(result)) return false;
	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
	if (FAILED(result)) return false;

	/*
		Stencilbuffer used to mask out rendered content and or implement shadows
	*/

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if(FAILED(result)) return false;
	// Set the stencils state to be active
	devCon->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result)) return false;

	// Bind the render target and stencil buffer to the render pipeline
	devCon->OMSetRenderTargets(1, &targetView, depthStencilView);

	// Set up rasterization
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create rasterized state
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result)) return false;

	devCon->RSSetState(rasterState);
	//Initialize the World Matrix
	worldMatrix = XMMatrixIdentity();
	return true;
}

void D3Dc::BeginScene(float red, float green, float blue, float alpha)
{

	float color[4] {red,green,blue,alpha};
	devCon->ClearRenderTargetView(targetView, color);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3Dc::EndScene()
{
	if (vsync_on) swapChain->Present(1, 0);
	else swapChain->Present(0, 0);
	return;
}

ID3D11Device* D3Dc::GetDevice()
{
	return device;
}

ID3D11DeviceContext* D3Dc::GetDeviceContext()
{
	return devCon;
}

void D3Dc::GetWorldMatrix(XMMATRIX& worldMatrixExt)
{
	worldMatrixExt = worldMatrix;
	return;
}

void D3Dc::GetVideoCard(char * cardName, int& memory)
{
	strcpy_s(cardName, 129, vCardDesc);
	memory = videoCardMemory;
	return;
}

void D3Dc::getScreenDimensions(int & screenWidth, int &screenHeight)
{
	screenWidth  = this->screenWidth;
	screenHeight = this->screenHeight;
}
