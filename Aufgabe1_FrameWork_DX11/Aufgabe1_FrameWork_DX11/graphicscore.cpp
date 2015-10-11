#include "graphicscore.h"



GraphicsCore::GraphicsCore()
{
	m_Direct3DWrapper = 0;
	m_Camera = 0;
	m_Model = 0;
	m_colShader = 0;
}

GraphicsCore::GraphicsCore(const GraphicsCore &other)
{
}


GraphicsCore::~GraphicsCore()
{
	if (m_colShader)
	{
		delete m_colShader;
		m_colShader = 0;
	}
	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}
	if (m_Model1)
	{
		delete m_Model1;
		m_Model1 = 0;
	}
	if (m_Model2)
	{
		delete m_Model2;
		m_Model2 = 0;
	}
	if (m_path)
	{
		delete m_path;
		m_path = 0;
	}
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	if (m_Direct3DWrapper)
	{
		delete m_Direct3DWrapper;
		m_Direct3DWrapper = 0;
	}
	return;
}

bool GraphicsCore::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	m_hwndin = hwnd;

	m_Direct3DWrapper = new D3Dc;
	if (!m_Direct3DWrapper) return false;

	result = m_Direct3DWrapper->Init(screenWidth, screenHeight, VSYNC, hwnd, false);
	if (!result) return false;

	m_Camera = new D3DCamera;
	if (!m_Camera) return false;
	m_Camera->Init(screenWidth, screenHeight, m_Direct3DWrapper->GetDeviceContext());
	m_Camera->SetPosition(0.0f, 0.0f, -2.5f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);


	XMVECTOR tempPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR tempRot = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	m_Model = new D3Dmodel;
	if (!m_Model) return false;
	result = m_Model->Init(m_Direct3DWrapper->GetDevice(),tempPos,tempRot);
	if (!result) return false;

	tempPos = XMVectorSet(5.0f, 0.0f, 0.0f, 0.0f);
	m_Model1 = new D3Dmodel;
	if (!m_Model1) return false;
	result = m_Model1->Init(m_Direct3DWrapper->GetDevice(),tempPos,tempRot);
	if (!result) return false;

	tempPos = XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f);
	m_Model2 = new D3Dmodel;
	if (!m_Model2) return false;
	result = m_Model2->Init(m_Direct3DWrapper->GetDevice(),tempPos,tempRot);
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
	XMVECTOR translate = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR rotate = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	if (Editmode)
	{
		if (inKey->Keystate(VK_UP))		rotate = XMVectorSetX(rotate, -1 * m_Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_DOWN))	rotate = XMVectorSetX(rotate, m_Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_LEFT))	rotate = XMVectorSetY(rotate, -1 * m_Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_RIGHT))	rotate = XMVectorSetY(rotate, m_Camera->speedRotation*delta_time);

		// Up
		if (inKey->Keystate('W'))		translate = XMVectorSetZ(translate, m_Camera->speedMovement*delta_time);
		// Down
		if (inKey->Keystate('S'))		translate = XMVectorSetZ(translate, -1 * m_Camera->speedMovement*delta_time);
		//Left
		if (inKey->Keystate('A'))		translate = XMVectorSetX(translate, -1 * m_Camera->speedMovement*delta_time);
		//Right
		if (inKey->Keystate('D'))		translate = XMVectorSetX(translate, m_Camera->speedMovement*delta_time);

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
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 2));
				rotate = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 2));
			}
			else if (m_currentPoint == m_path->GetPathSize() - 2)
			{
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint - 1), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 1));
				rotate = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint - 1), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 1));
			}
			else
			{
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, m_path->GetPositionOfPoint(m_currentPoint - 1), m_path->GetPositionOfPoint(m_currentPoint), m_path->GetPositionOfPoint(m_currentPoint + 1), m_path->GetPositionOfPoint(m_currentPoint + 2));
				rotate = D3DMath::Squad(t, m_path->GetRotationOfPoint(m_currentPoint - 1), m_path->GetRotationOfPoint(m_currentPoint), m_path->GetRotationOfPoint(m_currentPoint + 1), m_path->GetRotationOfPoint(m_currentPoint + 2));
			}
		}
		m_elapsedTime += delta_time;
	}

	m_Direct3DWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render(translate, rotate, Editmode);

	m_Direct3DWrapper->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);



	m_Model->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	worldMatrix = XMMatrixTranslationFromVector(translate);
	m_Model1->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	worldMatrix = XMMatrixTranslationFromVector(translate);
	m_Model2->Render(m_Direct3DWrapper->GetDeviceContext());
	result = m_colShader->Render(m_Direct3DWrapper->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result) return false;

	m_Direct3DWrapper->EndScene();

	return true;
}
