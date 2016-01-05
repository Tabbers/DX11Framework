#ifndef _GRAPHICSCORE_H_
#define _GRAPHICSCORE_H_

#include "d3dc.h"
#include "globaldefinitions.h"
#include <chrono>
#include <string>

class D3Dmodel;
class D3DCamera;
class D3DRenderToTexture;
class Input;
class GameObject;
class Path;
class Light;
class ColorShader;
class DepthShader;
class LineShader;
class DebugWindow;
class Collision;
class Line;
class KdTree;

class GraphicsCore
{
public:
	GraphicsCore();
	~GraphicsCore();

	bool Init(int, int, HWND);
	bool Frame(float,Input*,bool);
private:
	bool Render(float, Input*, bool);
	bool RenderTexture(bool, XMVECTOR, XMVECTOR);
	void SetWindowNameOnFilterChange();
private:
	D3Dc* m_Direct3DWrapper;
	D3DCamera* m_Camera;
	D3DRenderToTexture* m_RenderTexture;
	D3DRenderToTexture* m_msaaText;
	std::vector<D3Dmodel*> renderable;
	std::vector<Line*> renderableLines;
	std::vector<XMVECTOR> m_collPoints;
	std::vector<Line*> conLines;
	Light*	m_Light;
		
	ColorShader* m_colShader;
	DepthShader* m_depthShader;
	LineShader* m_lineShader;
	Path* m_path;
	KdTree* m_kdtree;
	DebugWindow* m_dwind;

	// reference to parent window
	HWND m_hwndin;

	int   m_currentPoint = 0;
	float m_partialTime = 0;
	float m_sublength = 0;
	float m_elapsedTime = 0;
	bool  m_playback = false;
	bool  m_displayKd = false;

	int m_screenWidth, m_screenHeight;
	Modes m = Modes::CAMERA;
};

#endif // !_GRAPHICSCORE_H_

