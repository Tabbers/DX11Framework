#ifndef _D3DMODEL_H_
#define _D3DMODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class D3Dmodel
{

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
public:
	D3Dmodel();
	D3Dmodel(const D3Dmodel&);
	~D3Dmodel();

	bool Init(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};

#endif // !_D3DMODEL_H_