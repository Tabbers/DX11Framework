#ifndef _D3DMODEL_H_
#define _D3DMODEL_H_

#include "gameobject.h"
class Model;
using namespace DirectX;

class D3Dmodel : public GameObject
{

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};
public:
	D3Dmodel();
	D3Dmodel(const D3Dmodel&);
	~D3Dmodel();

	bool Init(ID3D11Device*,XMVECTOR,XMVECTOR);
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	XMMATRIX adjustWorldmatrix(XMMATRIX);

private:
	bool InitBuffers(ID3D11Device*,XMVECTOR,XMVECTOR);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount;
	int indexCount;
	Model* model;

	XMMATRIX m_worldmatrix;
};

#endif // !_D3DMODEL_H_