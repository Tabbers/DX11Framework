#ifndef _GRAPHICSCORE_H_
#define _GRAPHICSCORE_H_

#include "d3dc.h"
#include "d3dmodel.h"
#include "d3dcamera.h"
#include "d3dmath.h"
#include "colorshader.h"
#include "path.h"
#include "input.h"
#include <chrono>
#include <string>

const bool VSYNC = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1F;
const float DEG_TO_RAD = 0.0174532925F;

class GraphicsCore
{
public:
	GraphicsCore();
	GraphicsCore(const GraphicsCore&);
	~GraphicsCore();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Frame(float,Input*,bool);
private:
	bool Render(float,Input*,bool);       

private:
	D3Dc* Direct3DWrapper;
	D3DCamera* Camera;
	D3Dmodel* Model;
	D3Dmodel* Model1;
	D3Dmodel* Model2;
	ColorShader* colShader;
	Path* path;

	// reference to parent window
	HWND hwndin;

	int currentPoint = 0;
	float partialTime = 0;
	float sublength = 0;
	float elapsedTime = 0;
	bool playback = false;
};

#endif // !_GRAPHICSCORE_H_

