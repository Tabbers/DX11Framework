#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "gameobject.h"

class Light : public GameObject
{
public:
	Light();
	~Light();

	void Init(int, int, ID3D11DeviceContext*,XMVECTOR,XMVECTOR);
	void Render(XMVECTOR, XMVECTOR, bool);
	//Getter
	inline XMMATRIX GetViewMatrix() { return this->m_viewMatrix; };
	inline XMMATRIX GetProjectionMatrix() { return this->m_projectionMatrix; };
	inline XMFLOAT4 GetDiffuseColor() { return this->m_diffuseColor; };
	inline XMFLOAT4 GetAmbientColor() { return this->m_ambientColor; };
	inline XMFLOAT3 GetXMFLOAT3Position() {
		XMFLOAT3 Position;
		Position.x = m_position.m128_f32[0];
		Position.y = m_position.m128_f32[1];
		Position.z = m_position.m128_f32[2];
		return Position;
	}
	//Setter
	inline void SetAmbientColor(float r, float g, float b, float a) 
	{
		m_ambientColor.x = r;
		m_ambientColor.y = g;
		m_ambientColor.z = b;
		m_ambientColor.w = a;
	};
	inline void SetDiffuseColor(float r, float g, float b, float a)
	{
		m_diffuseColor.x = r;
		m_diffuseColor.y = g;
		m_diffuseColor.z = b;
		m_diffuseColor.w = a;
	};
public:
	float speedMovement = 10;
	float speedRotation = 1;

	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1F;
private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_ambientColor;
	XMMATRIX m_viewMatrix;

	//Matrix housing projection positions
	XMMATRIX m_projectionMatrix;
	//Matrix for orhtigraphic projection
	XMMATRIX m_orthoMatrix;

	D3D11_VIEWPORT m_viewport;
	float m_fieldOfView;
	float m_screenAspect;
};
#endif