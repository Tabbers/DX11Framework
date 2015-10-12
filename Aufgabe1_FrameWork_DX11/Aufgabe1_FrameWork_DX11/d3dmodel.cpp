#include "d3dmodel.h"
#include "model.h"
#include "vertice.h"


D3Dmodel::D3Dmodel()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	model = 0;
}

D3Dmodel::D3Dmodel(const D3Dmodel &other)
{
}


D3Dmodel::~D3Dmodel()
{
	ShutdownBuffers();
	model->ReleaseModel();
}

bool D3Dmodel::Init(ID3D11Device * device, XMVECTOR position, XMVECTOR rotation)
{
	bool result;

	model = new Model();
	model->LoadModel("../Aufgabe1_FrameWork_DX11/Data/cube.txt");

	result = InitBuffers(device, position, rotation);
	if (!result) return false;

	return true;
}

void D3Dmodel::Render(ID3D11DeviceContext * devCon)
{
	RenderBuffers(devCon);
	return;
}

int D3Dmodel::GetIndexCount()
{
	return indexCount;
}

XMMATRIX D3Dmodel::adjustWorldmatrix(XMMATRIX worldMatrix)
{
	m_worldmatrix = XMMatrixTranslationFromVector(m_position);
	worldMatrix = XMMatrixMultiply(worldMatrix,m_worldmatrix);
	return worldMatrix;
}

bool D3Dmodel::InitBuffers(ID3D11Device * device, XMVECTOR position, XMVECTOR rotation)
{
	this->m_position = position;
	this->m_rotation = rotation;
	HRESULT result;
	VertexType* vertices;
	XMFLOAT3 positions;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = model->m_vertexcount;
	indexCount = model->m_indexcount;

	// Array for vertices
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = (*model).vertices[i].m_position;
		vertices[i].color = (*model).vertices[i].m_color;
		vertices[i].normal = (*model).vertices[i].m_normal;
	}

	// Setup dec vor vert buffer with size of vertextype*count
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// pointer to vertiues
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) return false;


	// Set up desc for static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// pointer to index Data
	indexData.pSysMem = (*model).indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) return false;

	// Release arrays
	delete[] vertices;
	vertices = 0;

	return true;
}

void D3Dmodel::ShutdownBuffers()
{
	// Release the index and vert buffer
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
	return;
}

void D3Dmodel::RenderBuffers(ID3D11DeviceContext * devCon)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	devCon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	devCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
