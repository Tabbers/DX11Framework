#ifndef _GRAPHICSCORE_H_
#define _GRAPHICSCORE_H_

#include "d3dc.h"
#include "d3dmodel.h"
#include "d3dcamera.h"
#include "d3dmath.h"
#include "colorshader.h"
#include "path.h"
#include "input.h"
#include "gameobject.h"
#include "light.h"
#include <chrono>
#include <string>
#include <vector>

const bool VSYNC = true;
const float DEG_TO_RAD = 0.0174532925F;

class GraphicsCore
{
public:
	GraphicsCore();
	~GraphicsCore();

	bool Init(int, int, HWND);
	bool Frame(float,Input*,bool);
private:
	bool Render(float,Input*,bool);       

private:
	D3Dc* m_Direct3DWrapper;
	D3DCamera* m_Camera;
	D3Dmodel* m_Model;
	D3Dmodel* m_Model1;
	D3Dmodel* m_Model2;
	D3Dmodel* m_Model3;
	D3Dmodel* m_Model4;
	Light*	m_Light;
	ColorShader* m_colShader;
	Path* m_path;

	// reference to parent window
	HWND m_hwndin;

	int   m_currentPoint = 0;
	float m_partialTime = 0;
	float m_sublength = 0;
	float m_elapsedTime = 0;
	bool  m_playback = false;

	vector<GameObject*> RenderableObjects;
};

#endif // !_GRAPHICSCORE_H_

