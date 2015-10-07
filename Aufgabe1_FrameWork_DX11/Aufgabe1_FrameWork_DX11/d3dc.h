#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3Dc
{
public:
	D3Dc();
	D3Dc(const D3Dc&);
	~D3Dc();

	bool Init(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	void BeginScene(float ,float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthomatrix(XMMATRIX&);

	void GetVideoCard(char*,int&);
private:
	bool vsync_on;
	int videoCardMemory;
	char vCardDesc[128];
	ID3D11Device* device;
	ID3D11DeviceContext* devCon;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* targetView;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;

	//Matrix housing projection positions
	XMMATRIX projectionMatrix;
	//Matrix housing world positions
	XMMATRIX worldMatrix;
	//Matrix for orhtigraphic projection
	XMMATRIX orthoMatrix;
};
#endif

