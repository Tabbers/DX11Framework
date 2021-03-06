#include "Light.h"
#include "globaldefinitions.h"


Light::Light()
{
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotation = XMQuaternionIdentity();
	SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
}


Light::~Light()
{
}

void Light::Init(int screenWidth, int screenHeight, ID3D11DeviceContext *devCon,XMVECTOR position,XMVECTOR rotation)
{
	// Setup the viewport
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	m_position = position;
	m_rotation = rotation;

	devCon->RSSetViewports(1, &m_viewport);

	// Set up fov to 90�
	m_fieldOfView = PI/2.0f;
	//get Apsect ratio
	m_screenAspect = (float)screenWidth / (float)screenHeight;

	//Set the projection matrix
	m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fieldOfView, m_screenAspect, SCREEN_NEARL, SCREEN_DEPTHL);

	//OrthoMatrix
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, SCREEN_NEARL, SCREEN_DEPTHL);


}

void Light::Render(XMVECTOR translate, XMVECTOR rotate, bool move)
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
		m_rotation = XMQuaternionMultiply(quaternion, m_rotation);
	}

	rotationMatrix = XMMatrixRotationQuaternion(m_rotation);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	m_lookAtVector.x = lookAtVector.m128_f32[0];
	m_lookAtVector.y = lookAtVector.m128_f32[1];
	m_lookAtVector.z = lookAtVector.m128_f32[2];
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
	rightVector = XMVector3TransformCoord(upVector, rotationMatrix);
	if (move)
	{
		translate = XMVector3TransformCoord(translate, rotationMatrix);
		translate = XMVectorSetW(translate, 0);
		m_position += translate;
	}

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(m_position, lookAtVector);


	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(m_position, lookAtVector, upVector);
}
void Light::ResetViewport(ID3D11DeviceContext* devCon)
{
	devCon->RSSetViewports(1, &m_viewport);
}
