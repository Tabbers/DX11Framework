#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Vertice
{
public:
	Vertice();
	Vertice(DirectX::XMFLOAT3, DirectX::XMFLOAT4, DirectX::XMFLOAT3);
	~Vertice();

public:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_color;
	DirectX::XMFLOAT3 m_normal;

};

