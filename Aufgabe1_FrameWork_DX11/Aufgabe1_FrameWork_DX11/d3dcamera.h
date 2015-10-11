#ifndef _D3DCAMERA_H_
#define _D3DCAMERA_H_

#include "gameobject.h"
using namespace DirectX;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1F;

class D3DCamera: public GameObject
{

public:
	D3DCamera();
	D3DCamera(const D3DCamera&);
	~D3DCamera();

	void Init(int, int, ID3D11DeviceContext*);
	void Render(XMVECTOR, XMVECTOR,bool);
	void GetViewMatrix(XMMATRIX&);
	void GetOrthomatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
public:
	float speedMovement = 10;
	float speedRotation = 1;

private:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
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
