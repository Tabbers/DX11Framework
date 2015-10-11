#include "gameobject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::Init(ID3D11Device * device, XMVECTOR position, XMVECTOR rotation)
{
	bool result = true;;
	this->m_position = position;
	this->m_rotation = rotation;
	return result;
}

void GameObject::Render(ID3D11DeviceContext *)
{
}

void GameObject::SetPosition(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 0.0f);
	return;
}

void GameObject::SetRotation(float x, float y, float z)
{
	m_rotation = XMVectorSet(x, y, z, m_rotation.m128_f32[3]);
	return;
}

XMVECTOR GameObject::GetPosition()
{
	return m_position;
}

XMVECTOR GameObject::GetRotation()
{
	return m_rotation;
}
 