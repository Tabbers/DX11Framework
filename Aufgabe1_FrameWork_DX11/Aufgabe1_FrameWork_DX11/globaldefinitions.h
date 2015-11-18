#pragma once

const bool VSYNC = true;
const float DEG_TO_RAD = 0.0174532925F;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SCREEN_DEPTHL = 5000.0f;
const float SCREEN_NEARL = 0.1f;
const float PI = 3.1415926535898f;

enum Modes 
{ 
	CAMERA, 
	LIGHT, 
};

struct Matrices
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
	bool DrawNormal;

	inline Matrices(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
	{
		this->worldMatrix = worldMatrix;
		this->viewMatrix = viewMatrix;
		this->projectionMatrix = projectionMatrix;
	};
};
struct LightData
{
	DirectX::XMMATRIX lightViewMatrix;
	DirectX::XMMATRIX lightProjectionMatrix;
	
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;

	DirectX::XMFLOAT3 lightPosition;

	inline LightData(DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjectionMatrix, 
		DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, 
		DirectX::XMFLOAT3 lightPosition)
	{
		this->lightViewMatrix = lightViewMatrix;
		this->lightProjectionMatrix = lightProjectionMatrix;
		this->ambientColor = ambientColor;
		this->diffuseColor = diffuseColor;
		this->lightPosition = lightPosition;
	}
};