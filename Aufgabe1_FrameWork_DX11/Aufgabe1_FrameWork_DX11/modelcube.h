#ifndef _MODELCUBE_H_
#define _MODELCUBE_H_

#include <DirectXMath.h>

class ModelCube
{
public:
	ModelCube();
	~ModelCube();
	//DirectX::XMFLOAT3 Scale(DirectX::XMFLOAT3*, int, float, float, float);

public:
	int vertexcount = 8;
	int indexcount = 36;

	DirectX::XMFLOAT3 vertices[8] = {
		// front
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
	//back
		{-1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
	};
	unsigned int indices[36] = {
		//Cube 3
		// Front face
		0,1,2,
		0,2,3,
		// Back face
		4,6,5,
		4,7,6,
		// Left face
		4,5,1,
		4,1,0,
		// Right face
		3,2,6,
		3,6,7,
		// Top face
		1,5,6,
		1,6,2,
		// Bottom face
		4,0,3,
		4,3,7
	};


	
};
#endif

