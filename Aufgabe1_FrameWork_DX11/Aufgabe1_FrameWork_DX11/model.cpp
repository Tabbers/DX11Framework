#include "model.h"
#include <fstream>



Model::Model()
{

}


Model::~Model()
{

}

bool Model::LoadModel(char * filename)
{
	using namespace std;
		ifstream fin;
		char input;
		int i;


		// Open the model file.
		fin.open(filename);

		// If it could not open the file then exit.
		if (fin.fail())
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// Read in the vertex count.
		fin >> m_vertexcount;

		// Set the number of indices to be the same as the vertex count.
		m_indexcount = m_vertexcount;

		// Create the model using the vertex count that was read in.
		vertices = new Vertice[m_vertexcount];
		indices = new unsigned long[m_indexcount];
		if (!vertices)
		{
			return false;
		}

		// Read up to the beginning of the data.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for (i = 0; i<m_vertexcount; i++)
		{
			fin >> vertices[i].m_position.x >> vertices[i].m_position.y >> vertices[i].m_position.z;
			fin >> vertices[i].m_color.x >> vertices[i].m_color.y >> vertices[i].m_color.z >> vertices[i].m_color.w;
			fin >> vertices[i].m_normal.x >> vertices[i].m_normal.y >> vertices[i].m_normal.z;
			indices[i] = i;
		}

		// Close the model file.
		fin.close();

		return true;
}

void Model::ReleaseModel()
{

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	return;
}

void Model::setObjectColor(DirectX::XMFLOAT4 color)
{
	for (int i = 0; i < m_vertexcount; i++)
	{
		vertices[i].m_color = color;
	}
}


