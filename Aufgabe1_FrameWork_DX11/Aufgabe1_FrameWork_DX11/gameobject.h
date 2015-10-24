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

	//Getter
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	//setter
	inline void SetTransform(XMVECTOR position, XMVECTOR rotation) {
		m_position = position;
		m_rotation = rotation;
	};
	inline void SetScale(float scale) { m_scale = XMVectorSet(scale, scale, scale, 0); };
	inline void SetPosition(XMVECTOR position) { m_position = position; };
	inline void SetRotation(XMVECTOR rotation) { m_rotation = rotation; };

	virtual bool Init(ID3D11Device*,XMVECTOR, XMVECTOR);
	virtual void Render(ID3D11DeviceContext*);
protected:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
	XMVECTOR m_scale;
};

