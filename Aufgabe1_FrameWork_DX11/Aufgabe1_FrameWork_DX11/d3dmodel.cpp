#include "d3dmodel.h"
#include "modelcube.h"


D3Dmodel::D3Dmodel()
{
	vertexBuffer = 0;
	indexBuffer = 0;
}

D3Dmodel::D3Dmodel(const D3Dmodel &other)
{
}


D3Dmodel::~D3Dmodel()
{
}

bool D3Dmodel::Init(ID3D11Device * device)
{
	bool result;

	result = InitBuffers(device);
	if (!result) return false;

	return true;
}

void D3Dmodel::Shutdown()
{
	ShutdownBuffers();
	return;
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

bool D3Dmodel::InitBuffers(ID3D11Device * device)
{
	HRESULT result;
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	ModelCube* cube = new ModelCube;


	vertexCount = cube->vertexcount;
	indexCount = cube->indexcount;

	// Array for vertices
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = cube->vertices[i];	
	}

	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	vertices[4].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	vertices[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[6].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[7].color = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);


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
	indexData.pSysMem = cube->indices;
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

	//Activate vert and index buffer in the input assembler
	// Set the vertex buffer to active
	devCon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	// Set the index buffer ot active
	devCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set it to render Triangles
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;

}
