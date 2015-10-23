#include "graphicscore.h"
#include "d3dmath.h"


GraphicsCore::GraphicsCore() :
	m_path(nullptr), m_Direct3DWrapper(nullptr), m_Camera(nullptr), m_Model(nullptr), 
	m_Model1(nullptr), m_Model2(nullptr),m_Model3(nullptr), m_Model4(nullptr), 
	m_colShader(nullptr), m_Light(nullptr)
{
}

GraphicsCore::~GraphicsCore()
{
	delete m_colShader;
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

	tempPos = XMVectorSet(-5.0f, 10.0f, -5.0f, 0.0f);
	tempRot = XMQuaternionRotationRollPitchYaw(0, D3DMath::DegreeToRadians(20), 0);

	m_Light = new Light;
	if (!m_Light) return false;
	m_Light->Init(screenWidth, screenHeight, m_Direct3DWrapper->GetDeviceContext(),tempPos,tempRot);
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_Model4 = new D3Dmodel;
	if (!m_Model4) return false;
	result = m_Model4->Init("Data/cube.txt", m_Direct3DWrapper->GetDevice(), m_Light->GetPosition(), m_Light->GetRotation(), { 1,1,0,1 });
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
	// check if Rendering was successfull
	if (!result) return false;

	return true;
}

bool GraphicsCore::Render(float delta_time, Input* inKey, bool Editmode)
{
	XMVECTOR translateC = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotateC = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR translateL = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotateL = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	if (Editmode)
	{
		if (inKey->Keystate('W'))	rotateC = XMVectorSetX(rotateC, m_Camera->speedRotation*delta_time);
		if (inKey->Keystate('S'))	rotateC = XMVectorSetX(rotateC,-1 * m_Camera->speedRotation*delta_time);
		if (inKey->Keystate('A'))	rotateC = XMVectorSetY(rotateC, -1 * m_Camera->speedRotation*delta_time);
		if (inKey->Keystate('D'))	rotateC = XMVectorSetY(rotateC, m_Camera->speedRotation*delta_time);
		if (inKey->Keystate('Q'))	rotateC = XMVectorSetZ(rotateC, m_Camera->speedRotation*delta_time);
		if (inKey->Keystate('E'))	rotateC = XMVectorSetZ(rotateC, -1*m_Camera->speedRotation*delta_time);

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

	m_Direct3DWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render(translateC, rotateC, Editmode);
	m_Light->Render(translateL, rotateL, Editmode);

	m_Direct3DWrapper->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->adjustWorldmatrix(worldMatrix), viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(),m_Light->GetXMFLOAT3Position(),m_Light->GetDiffuseColor(),m_Light->GetAmbientColor());
	m_Model1->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model1->GetIndexCount(), m_Model1->adjustWorldmatrix(worldMatrix), viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(), m_Light->GetXMFLOAT3Position(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
	m_Model2->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model2->GetIndexCount(), m_Model2->adjustWorldmatrix(worldMatrix), viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(), m_Light->GetXMFLOAT3Position(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
	m_Model3->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model3->GetIndexCount(), m_Model3->adjustWorldmatrix(worldMatrix), viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(), m_Light->GetXMFLOAT3Position(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
	
	//m_Model4->SetTransform(m_Light->GetPosition(), m_Light->GetRotation());
	m_Model4->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model4->GetIndexCount(), m_Model4->adjustWorldmatrix(worldMatrix), viewMatrix, projectionMatrix, m_Light->GetViewMatrix(), m_Light->GetProjectionMatrix(), m_Light->GetXMFLOAT3Position(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
	

	if (!result) return false;

	m_Direct3DWrapper->EndScene();

	return true;
}
