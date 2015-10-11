#include "vertice.h"



Vertice::Vertice()
{
}

Vertice::Vertice(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 normal)
{
	m_normal = normal;
	m_color	 = color;
	m_position = position;
}


Vertice::~Vertice()
{
}
