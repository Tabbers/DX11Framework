#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Vertice
{
public:
	Vertice();
	Vertice(DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT3);
	~Vertice();

public:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_textcord;
	DirectX::XMFLOAT3 m_normal;
	DirectX::XMFLOAT3 m_tangent;
	DirectX::XMFLOAT3 m_binormal;
};

