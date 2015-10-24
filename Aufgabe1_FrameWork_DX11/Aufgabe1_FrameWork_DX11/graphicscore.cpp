#include "graphicscore.h"
#include "d3dmodel.h"
#include "d3dmath.h"
#include "d3dcamera.h"
#include "d3drendertotexture.h"
#include "input.h"
#include "path.h"
#include "gameobject.h"
#include "light.h"
#include "colorshader.h"
#include "depthShader.h"

GraphicsCore::GraphicsCore() :
	m_path(nullptr), m_Direct3DWrapper(nullptr), m_Camera(nullptr), m_Model(nullptr), 
	m_Model1(nullptr), m_Model2(nullptr),m_Model3(nullptr), m_Model4(nullptr), 
	m_colShader(nullptr),m_depthShader(nullptr), m_Light(nullptr)
{
}

GraphicsCore::~GraphicsCore()
{
	delete m_colShader;
	delete m_depthShader;
	delete m_path;
	delete m_Model;
	delete m_Model1;
	delete m_Model2;
	delete m_Model3;
	delete m_Model4;
	delete m_Camera;
	delete m_Light;
	delete m_Direct3DWrapper;
	return;
}

bool GraphicsCore::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	m_hwndin = hwnd;
	XMVECTOR tempPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR tempRot = XMQuaternionRotationRollPitchYaw(0, 0, 0);

	m_Direct3DWrapper = new D3Dc;
	if (!m_Direct3DWrapper) return false;

	result = m_Direct3DWrapper->Init(screenWidth, screenHeight, VSYNC, hwnd, false);
	if (!result) return false;

	m_Camera = new D3DCamera;
	if (!m_Camera) return false;
	m_Camera->Init(screenWidth, screenHeight, m_Direct3DWrapper->GetDeviceContext(),tempPos,tempRot);

	m_Model = new D3Dmodel;
	if (!m_Model) return false;
	result = m_Model->Init("Data/cube.txt",m_Direct3DWrapper->GetDevice(), tempPos, tempRot, {1,0,0,1});
	if (!result) return false;

	tempPos = XMVectorSet(5.0f, 0.0f, 0.0f, 0.0f);
	m_Model1 = new D3Dmodel;
	if (!m_Model1) return false;
	result = m_Model1->Init("Data/cube.txt",m_Direct3DWrapper->GetDevice(),tempPos,tempRot, { 0,1,0,1 });
	if (!result) return false;

	tempPos = XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f);
	m_Model2 = new D3Dmodel;
	if (!m_Model2) return false;
	result = m_Model2->Init("Data/cube.txt",m_Direct3DWrapper->GetDevice(),tempPos,tempRot, { 0,0,1,1 });
	if (!result) return false;

	tempPos = XMVectorSet(0.0f, -5.0f, 0.0f, 0.0f);
	m_Model3 = new D3Dmodel;
	if (!m_Model3) return false;
	result = m_Model3->Init("Data/plane01.txt", m_Direct3DWrapper->GetDevice(), tempPos, tempRot, { 1,1,1,1 });
	if (!result) return false;

	tempPos = XMVectorSet(0.0f, 10.0f, -50.0f, 0.0f);
	tempRot = XMQuaternionRotationRollPitchYaw(0,0,0);

	m_Light = new Light;
	if (!m_Light) return false;
	m_Light->Init(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, m_Direct3DWrapper->GetDeviceContext(),tempPos,tempRot);
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_Model4 = new D3Dmodel;
	if (!m_Model4) return false;
	result = m_Model4->Init("Data/cube.txt", m_Direct3DWrapper->GetDevice(), m_Light->GetPosition(), m_Light->GetRotation(), { 1,1,0,1 });
	m_Model4->SetScale(0.5);
	if (!result) return false;

	m_RenderTexture = new D3DRenderToTexture;
	if (!m_RenderTexture) return false;
	result = m_RenderTexture->Init(m_Direct3DWrapper->GetDevice(),SHADOWMAP_WIDTH,SHADOWMAP_HEIGHT);
	if (!result) return false;

	//Shader Initialisation
	m_depthShader = new DepthShader;
	if (!m_depthShader) return false;
	result = m_depthShader->Init(m_Direct3DWrapper->GetDevice(), hwnd);
	if (!result) return false;

	m_colShader = new ColorShader;
	if (!m_colShader) return false;
	result = m_colShader->Init(m_Direct3DWrapper->GetDevice(), hwnd);
	if (!result) return false;

	m_path = new Path;
	if (!m_path) return false;

	return true;
}

bool GraphicsCore::Frame(float delta_time, Input* inKey, bool Editmode)
{
	bool result;

	result = Render(delta_time, inKey, Editmode);
	if (!result) return false;

	return true;
}

bool GraphicsCore::Render(float delta_time, Input* inKey, bool Editmode)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	XMVECTOR translateC = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotateC = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR translateL = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotateL = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	if (inKey->Keystate('1') && !inKey->KeystateOld('1')) m = CAMERA;
	if (inKey->Keystate('2') && !inKey->KeystateOld('2')) m = LIGHT;
	if (Editmode)
	{
		if (m == CAMERA)
		{
			if (inKey->Keystate('W'))	rotateC = XMVectorSetX(rotateC, m_Camera->speedRotation*delta_time);
			if (inKey->Keystate('S'))	rotateC = XMVectorSetX(rotateC, -1 * m_Camera->speedRotation*delta_time);
			if (inKey->Keystate('A'))	rotateC = XMVectorSetY(rotateC, -1 * m_Camera->speedRotation*delta_time);
			if (inKey->Keystate('D'))	rotateC = XMVectorSetY(rotateC, m_Camera->speedRotation*delta_time);
			if (inKey->Keystate('Q'))	rotateC = XMVectorSetZ(rotateC, m_Camera->speedRotation*delta_time);
			if (inKey->Keystate('E'))	rotateC = XMVectorSetZ(rotateC, -1 * m_Camera->speedRotation*delta_time);

			// Up
			if (inKey->Keystate(VK_UP))		translateC = XMVectorSetZ(translateC, m_Camera->speedMovement*delta_time);
			// Down
			if (inKey->Keystate(VK_DOWN))		translateC = XMVectorSetZ(translateC, -1 * m_Camera->speedMovement*delta_time);
			//Left
			if (inKey->Keystate(VK_LEFT))		translateC = XMVectorSetX(translateC, -1 * m_Camera->speedMovement*delta_time);
			//Right
			if (inKey->Keystate(VK_RIGHT))		translateC = XMVectorSetX(translateC, m_Camera->speedMovement*delta_time);

			// Set point using camera Rotation and Position;
			if (inKey->Keystate(VK_SPACE) && !inKey->KeystateOld(VK_SPACE)) m_path->AddPoint(m_Camera->GetPosition(), m_Camera->GetRotation());
		}
		if (m == LIGHT)
		{
			if (inKey->Keystate('W'))	rotateL = XMVectorSetX(rotateL, m_Light->speedRotation*delta_time);
			if (inKey->Keystate('S'))	rotateL = XMVectorSetX(rotateL, -1 * m_Light->speedRotation*delta_time);
			if (inKey->Keystate('A'))	rotateL = XMVectorSetY(rotateL, -1 * m_Light->speedRotation*delta_time);
			if (inKey->Keystate('D'))	rotateL = XMVectorSetY(rotateL, m_Light->speedRotation*delta_time);
			if (inKey->Keystate('Q'))	rotateL = XMVectorSetZ(rotateL, m_Light->speedRotation*delta_time);
			if (inKey->Keystate('E'))	rotateL = XMVectorSetZ(rotateL, -1 * m_Light->speedRotation*delta_time);

			// Forwards
			if (inKey->Keystate(VK_UP))		translateL = XMVectorSetZ(translateL, m_Light->speedMovement*delta_time);
			// Backwards									 
			if (inKey->Keystate(VK_DOWN))	translateL = XMVectorSetZ(translateL, -1 * m_Light->speedMovement*delta_time);
			//Left									 
			if (inKey->Keystate(VK_LEFT))	translateL = XMVectorSetX(translateL, -1 * m_Light->speedMovement*delta_time);
			//Right									 
			if (inKey->Keystate(VK_RIGHT))	translateL = XMVectorSetX(translateL, m_Light->speedMovement*delta_time);
			//Up
			if (inKey->Keystate(VK_SHIFT))	translateL = XMVectorSetY(translateL,  m_Light->speedMovement*delta_time);
			//Down									 
			if (inKey->Keystate(VK_CONTROL))	translateL = XMVectorSetY(translateL, -1 * m_Light->speedMovement*delta_time);
		}
	}
	else
	{
		if (inKey->Keystate(VK_SPACE) && !inKey->KeystateOld(VK_SPACE))
		{
			if (m_path->GetPathSize() < 3)
			{
				MessageBox(m_hwndin, L"You have to enter at least 4 Points", L"Error", MB_OK);
				return false;
			}
			if (!m_playback)
			{
				// INterpolation setup
				m_playback = true;
				m_Camera->speedMovement = 500.0f;
				m_elapsedTime = 0;
				m_currentPoint = 0;
				m_sublength = XMVector4Length(m_path->GetPositionOfPoint(0) + m_path->GetPositionOfPoint(1)).m128_f32[0];
				m_partialTime = m_sublength / (m_Camera->speedMovement*delta_time);
			}
			else
			{
				m_playback = false;
			}

		}
		if (m_playback)
		{
			float t;
			if (m_elapsedTime > m_partialTime)
			{
				if (m_currentPoint < m_path->GetPathSize() - 2)
				{
					// Setting up for interpolation to the next point in the path
					m_currentPoint++;
					m_elapsedTime = 0;
					m_sublength = XMVector4Length(m_path->GetPositionOfPoint(m_currentPoint) + m_path->GetPositionOfPoint(m_currentPoint + 1)).m128_f32[0];
					m_partialTime = m_sublength / (m_Camera->speedMovement*delta_time);
				}
				else
				{
					m_playback = false;
				}
			}
			t = m_elapsedTime / m_partialTime;
			// If currentPoint is the starting point correct the interpolation and set currentPoint -1 to the currentPoint
			if (m_currentPoint == 0)
			{
				translateC = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 2));
				rotateC = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 2));
			}
			else if (m_currentPoint == m_path->GetPathSize() - 2)
			{
				translateC = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint - 1), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 1));
				rotateC = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint - 1), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 1));
			}
			else
			{
				translateC = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint - 1), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 2));
				rotateC = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint - 1), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 2));
			}
		}
		m_elapsedTime += delta_time;
	}

	result = RenderTexture(Editmode, translateL, rotateL);
	if (!result) return false;

	m_Camera->ResetViewport(m_Direct3DWrapper->GetDeviceContext());

	m_Direct3DWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render(translateC, rotateC, Editmode);
	m_Light->Render(translateL, rotateL, Editmode);

	m_Direct3DWrapper->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->adjustWorldmatrix(worldMatrix),
								viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),
								m_RenderTexture->GetShaderRessourceView(), m_Light->GetXMFLOAT3Position(), m_Light->GetAmbientColor(), 
								m_Light->GetDiffuseColor());
	if (!result) return false;

	m_Model1->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model1->GetIndexCount(), m_Model1->adjustWorldmatrix(worldMatrix),
								viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),
								m_RenderTexture->GetShaderRessourceView(), m_Light->GetXMFLOAT3Position(), m_Light->GetAmbientColor(),
								m_Light->GetDiffuseColor());
	if (!result) return false;

	m_Model2->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model2->GetIndexCount(), m_Model2->adjustWorldmatrix(worldMatrix),
								viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),
								m_RenderTexture->GetShaderRessourceView(), m_Light->GetXMFLOAT3Position(), m_Light->GetAmbientColor(),
								m_Light->GetDiffuseColor());
	if (!result) return false;

	m_Model3->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model3->GetIndexCount(), m_Model3->adjustWorldmatrix(worldMatrix),
								viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),
								m_RenderTexture->GetShaderRessourceView(), m_Light->GetXMFLOAT3Position(), m_Light->GetAmbientColor(),
								m_Light->GetDiffuseColor());
	if (!result) return false;
	m_Model4->SetPosition(m_Light->GetPosition());
	m_Model4->SetRotation(m_Light->GetRotation());
	m_Model4->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model4->GetIndexCount(), m_Model4->adjustWorldmatrix(worldMatrix),
								viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),
								m_RenderTexture->GetShaderRessourceView(), m_Light->GetXMFLOAT3Position(), m_Light->GetAmbientColor(),
								m_Light->GetDiffuseColor());
	if (!result) return false;

	m_Direct3DWrapper->EndScene();

	return true;
}

bool GraphicsCore::RenderTexture(bool Editmode, XMVECTOR translateL, XMVECTOR rotateL)
{
	using namespace DirectX;

	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;
	XMFLOAT3 pos;
	bool result = true;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3DWrapper->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3DWrapper->GetDeviceContext(), {0,0.5f,0,1});

	m_Light->Render(translateL, rotateL, Editmode);

	m_Direct3DWrapper->GetWorldMatrix(worldMatrix);
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetProjectionMatrix(lightProjectionMatrix);

	m_Model->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->adjustWorldmatrix(worldMatrix), lightViewMatrix, lightProjectionMatrix);
	if (!result) return false;

	m_Model1->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model1->GetIndexCount(), m_Model1->adjustWorldmatrix(worldMatrix), lightViewMatrix, lightProjectionMatrix);
	if (!result) return false;

	m_Model2->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model2->GetIndexCount(), m_Model2->adjustWorldmatrix(worldMatrix), lightViewMatrix, lightProjectionMatrix);
	if (!result) return false;

	m_Model3->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model3->GetIndexCount(), m_Model3->adjustWorldmatrix(worldMatrix), lightViewMatrix, lightProjectionMatrix);
	if (!result) return false;

	m_Model4->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model4->GetIndexCount(), m_Model4->adjustWorldmatrix(worldMatrix), lightViewMatrix, lightProjectionMatrix);
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3DWrapper->SetBackBufferRenderTarget();
	return result;
}
