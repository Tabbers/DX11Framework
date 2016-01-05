#ifndef _D3DMODEL_H_
#define _D3DMODEL_H_

#include "gameobject.h"
#include "texture.h"
#include "globaldefinitions.h"
#include <vector>
class Model;
using namespace DirectX;

class D3Dmodel : public GameObject
{

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};
	struct ModelType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct TempVertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
public:
	D3Dmodel();
	D3Dmodel(const D3Dmodel&);
	~D3Dmodel();

	bool Init(char*,wchar_t*,wchar_t*,ID3D11Device*,ID3D11DeviceContext*,XMVECTOR,XMVECTOR);
	void Render(ID3D11DeviceContext*);
	std::vector<Triangle> ModelAsTriangles(XMMATRIX world);

	int GetIndexCount();
	inline Texture* GetTexture() { return m_Text; };
	inline Texture* GetNormalMap() { return m_NormalMap; };
	XMMATRIX adjustWorldmatrix(XMMATRIX);
	bool collision = true;
private:
	bool InitBuffers(ID3D11Device*,XMVECTOR,XMVECTOR);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, XMFLOAT3 & tangent, XMFLOAT3 & binormal);
	void CalculateNormal(XMFLOAT3 tangent, XMFLOAT3 binormal, XMFLOAT3 & normal);
private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	Model* m_model;
	Texture* m_Text;
	Texture* m_NormalMap;

	XMMATRIX m_worldmatrix;

	
};

#endif // !_D3DMODEL_H_