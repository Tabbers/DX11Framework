#ifndef _MODEL_H_
#define _MODEL_H_

#include <DirectXMath.h>
#include "vertice.h"

class Model
{
public:
	Model();
	~Model();

	bool LoadModel(char*);
	void ReleaseModel();
	//setter
	void setObjectColor(DirectX::XMFLOAT4);

public:
	Vertice* vertices;

	unsigned long* indices;
	int m_vertexcount;
	int m_indexcount;
};
#endif

