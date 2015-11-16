#include "vertice.h"



Vertice::Vertice()
{
}

Vertice::Vertice(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 text, DirectX::XMFLOAT3 normal)
{
	m_normal   = normal;
	m_textcord = text;
	m_position = position;
}


Vertice::~Vertice()
{
}
