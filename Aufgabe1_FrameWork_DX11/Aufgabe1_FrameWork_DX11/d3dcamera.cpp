#include "d3dcamera.h"



D3DCamera::D3DCamera()
{
	position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMQuaternionIdentity();
}

D3DCamera::D3DCamera(const D3DCamera &other)
{
}

D3DCamera::~D3DCamera()
{
}

void D3DCamera::SetPosition(float x, float y, float z)
{
	position = XMVectorSet(x, y, z, 0.0f);
	return;
}

void D3DCamera::SetRotation(float x, float y, float z)
{
	rotation = XMVectorSet(x, y, z, rotation.m128_f32[3]);
	return;
}

XMVECTOR D3DCamera::GetPosition()
{
	return position;
}

XMVECTOR D3DCamera::GetRotation()
{
	return rotation;
}

void D3DCamera::Render(XMVECTOR translate, XMVECTOR rotate, bool move)
{
	XMVECTOR upVector, lookAtVector, rightVector;
	XMMATRIX rotationMatrix;

	rightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookAtVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	if (move)
	{
		XMVECTOR quaternion = XMQuaternionRotationRollPitchYawFromVector(rotate);
		quaternion = XMQuaternionNormalize(quaternion);
		rotation = XMQuaternionMultiply(quaternion, rotation);
	}
	else rotation = rotate;

	rotationMatrix = XMMatrixRotationQuaternion(rotation);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
	rightVector = XMVector3TransformCoord(upVector, rotationMatrix);
	if (move)
	{
		translate = XMVector3TransformCoord(translate, rotationMatrix);
		translate = XMVectorSetW(translate, 0);
		position += translate;
	}
	else position = translate;

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(position, lookAtVector);

	
	// Finally create the view matrix from the three updated vectors.
	viewMatrix = XMMatrixLookAtLH(position, lookAtVector, upVector);

	return;
}

void D3DCamera::GetViewMatrix(XMMATRIX & viewMatrixEXT)
{
	viewMatrixEXT = viewMatrix;
	return;
}
