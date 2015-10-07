#ifndef _D3DMATH_H_
#define _D3DMATH_H_

#include "pathpoint.h"

class D3DMath
{
public:
	D3DMath();
	~D3DMath();

	static DirectX::XMVECTOR KochanekBartels(float,float,float,float, DirectX::XMVECTOR, DirectX::XMVECTOR, DirectX::XMVECTOR, DirectX::XMVECTOR);
	static DirectX::XMVECTOR Squad(float, DirectX::XMVECTOR, DirectX::XMVECTOR, DirectX::XMVECTOR, DirectX::XMVECTOR);
};
#endif

