#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
using namespace DirectX;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMVECTOR GetPosition();
	XMVECTOR GetRotation();

	virtual bool Init(ID3D11Device*,XMVECTOR, XMVECTOR);
	virtual void Render(ID3D11DeviceContext*);
protected:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
};

