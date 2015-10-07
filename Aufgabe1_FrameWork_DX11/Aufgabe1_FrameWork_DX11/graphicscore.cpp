#include "graphicscore.h"



GraphicsCore::GraphicsCore()
{
	Direct3DWrapper = 0;
	Camera = 0;
	Model = 0;
	colShader = 0;
}

GraphicsCore::GraphicsCore(const GraphicsCore &other)
{
}


GraphicsCore::~GraphicsCore()
{
}

bool GraphicsCore::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	hwndin = hwnd;

	Direct3DWrapper = new D3Dc;
	if (!Direct3DWrapper) return false;

	result = Direct3DWrapper->Init(screenWidth, screenHeight, VSYNC, hwnd, false, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) return false;

	Camera = new D3DCamera;
	if (!Camera) return false;
	Camera->SetPosition(0.0f, 0.0f, -2.5f);
	Camera->SetRotation(0.0f, 0.0f, 0.0f);

	Model = new D3Dmodel;
	if (!Model) return false;
	result = Model->Init(Direct3DWrapper->GetDevice());
	if (!result) return false;

	Model1 = new D3Dmodel;
	if (!Model1) return false;
	result = Model1->Init(Direct3DWrapper->GetDevice());
	if (!result) return false;

	Model2 = new D3Dmodel;
	if (!Model2) return false;
	result = Model2->Init(Direct3DWrapper->GetDevice());
	if (!result) return false;

	colShader = new ColorShader;
	if (!colShader) return false;

	result = colShader->Init(Direct3DWrapper->GetDevice(), hwnd);
	if (!result) return false;

	path = new Path;
	if (!path) return false;

	return true;
}

void GraphicsCore::Shutdown()
{
	if (colShader)
	{
		colShader->Shutdown();
		delete colShader;
		colShader = 0;
	}
	if (Model)
	{
		Model->Shutdown();
		delete Model;
		Model = 0;
	}
	if (Model1)
	{
		Model1->Shutdown();
		delete Model1;
		Model1 = 0;
	}
	if (Model2)
	{
		Model2->Shutdown();
		delete Model2;
		Model2 = 0;
	}
	if (path)
	{
		delete path;
		path = 0;
	}
	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}
	if (Direct3DWrapper)
	{
		Direct3DWrapper->Shutdown();
		delete Direct3DWrapper;
		Direct3DWrapper = 0;
	}
	return;
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
		if (inKey->Keystate(VK_UP))		rotate = XMVectorSetX(rotate, -1 * Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_DOWN))	rotate = XMVectorSetX(rotate, Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_LEFT))	rotate = XMVectorSetY(rotate, -1 * Camera->speedRotation*delta_time);
		if (inKey->Keystate(VK_RIGHT))	rotate = XMVectorSetY(rotate, Camera->speedRotation*delta_time);

		// Up
		if (inKey->Keystate('W'))		translate = XMVectorSetZ(translate, Camera->speedMovement*delta_time);
		// Down
		if (inKey->Keystate('S'))		translate = XMVectorSetZ(translate, -1 * Camera->speedMovement*delta_time);
		//Left
		if (inKey->Keystate('A'))		translate = XMVectorSetX(translate, -1 * Camera->speedMovement*delta_time);
		//Right
		if (inKey->Keystate('D'))		translate = XMVectorSetX(translate, Camera->speedMovement*delta_time);

		// Set point using camera Rotation and Position;
		if (inKey->Keystate(VK_SPACE) && !inKey->KeystateOld(VK_SPACE)) path->AddPoint(Camera->GetPosition(), Camera->GetRotation());
	}
	else
	{
		if (inKey->Keystate(VK_SPACE) && !inKey->KeystateOld(VK_SPACE))
		{
			if (path->GetPathSize() < 3)
			{
				MessageBox(hwndin, L"You have to enter at least 4 Points", L"Error", MB_OK);
				return false;
			}
			if (!playback)
			{
				// INterpolation setup
				playback = true;
				Camera->speedMovement = 500.0f;
				elapsedTime = 0;
				currentPoint = 0;
				sublength = XMVector4Length(path->GetPositionOfPoint(0) + path->GetPositionOfPoint(1)).m128_f32[0];
				partialTime = sublength / (Camera->speedMovement*delta_time);
			}
			else
			{
				playback = false;
			}

		}
		if (playback)
		{
			float t;
			if (elapsedTime > partialTime)
			{
				if (currentPoint < path->GetPathSize() - 2)
				{
					// Setting up for interpolation to the next point in the path
					currentPoint++;
					elapsedTime = 0;
					sublength = XMVector4Length(path->GetPositionOfPoint(currentPoint) + path->GetPositionOfPoint(currentPoint + 1)).m128_f32[0];
					partialTime = sublength / (Camera->speedMovement*delta_time);
				}
				else
				{
					playback = false;
				}
			}
			t = elapsedTime / partialTime;
			// If currentPoint is the starting point correct the interpolation and set currentPoint -1 to the currentPoint
			if (currentPoint == 0)
			{
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, path->GetPositionOfPoint(currentPoint), path->GetPositionOfPoint(currentPoint), path->GetPositionOfPoint(currentPoint + 1), path->GetPositionOfPoint(currentPoint + 2));
				rotate = D3DMath::Squad(t, path->GetRotationOfPoint(currentPoint), path->GetRotationOfPoint(currentPoint), path->GetRotationOfPoint(currentPoint + 1), path->GetRotationOfPoint(currentPoint + 2));
			}
			else if (currentPoint == path->GetPathSize() - 2)
			{
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, path->GetPositionOfPoint(currentPoint - 1), path->GetPositionOfPoint(currentPoint), path->GetPositionOfPoint(currentPoint + 1), path->GetPositionOfPoint(currentPoint + 1));
				rotate = D3DMath::Squad(t, path->GetRotationOfPoint(currentPoint - 1), path->GetRotationOfPoint(currentPoint), path->GetRotationOfPoint(currentPoint + 1), path->GetRotationOfPoint(currentPoint + 1));
			}
			else
			{
				translate = D3DMath::KochanekBartels(t, 0, 0, 0, path->GetPositionOfPoint(currentPoint - 1), path->GetPositionOfPoint(currentPoint), path->GetPositionOfPoint(currentPoint + 1), path->GetPositionOfPoint(currentPoint + 2));
				rotate = D3DMath::Squad(t, path->GetRotationOfPoint(currentPoint - 1), path->GetRotationOfPoint(currentPoint), path->GetRotationOfPoint(currentPoint + 1), path->GetRotationOfPoint(currentPoint + 2));
			}
		}
		elapsedTime += delta_time;
	}

	Direct3DWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	Camera->Render(translate, rotate, Editmode);

	Direct3DWrapper->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	Direct3DWrapper->GetProjectionMatrix(projectionMatrix);

	Model->Render(Direct3DWrapper->GetDeviceContext());
	translate = XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f);
	result = colShader->Render(Direct3DWrapper->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	worldMatrix = XMMatrixTranslationFromVector(translate);
	Model1->Render(Direct3DWrapper->GetDeviceContext());
	result = colShader->Render(Direct3DWrapper->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	translate = XMVectorSet(0.0f, 5.0f, 10.0f, 0.0f);
	worldMatrix = XMMatrixTranslationFromVector(translate);
	Model2->Render(Direct3DWrapper->GetDeviceContext());
	result = colShader->Render(Direct3DWrapper->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result) return false;

	Direct3DWrapper->EndScene();

	return true;
}
