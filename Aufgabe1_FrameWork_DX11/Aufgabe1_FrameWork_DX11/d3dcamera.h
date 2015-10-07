#ifndef _D3DCAMERA_H_
#define _D3DCAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class D3DCamera
{

public:
	D3DCamera();
	D3DCamera(const D3DCamera&);
	~D3DCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMVECTOR GetPosition();
	XMVECTOR GetRotation();

	void Render(XMVECTOR, XMVECTOR,bool);
	void GetViewMatrix(XMMATRIX&);
public:
	float speedMovement = 10;
	float speedRotation = 1;

private:
	XMVECTOR position;
	XMVECTOR rotation;
	XMMATRIX viewMatrix;
};

#endif
