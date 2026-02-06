#pragma once
#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class CameraComponent : public Component
{
public:
	void SetFOV(float fov) { m_fov = fov; }
	void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
	void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
	void SetIsMainCamera(bool isMain) { m_isMainCamera = isMain; }
	void SetViewMatrix(const XMFLOAT4X4& viewMatrix) { m_viewMatrix = viewMatrix; }
	void SetProjectionMatrix(const XMFLOAT4X4& projectionMatrix) { m_projectionMatrix = projectionMatrix; }
	
	const XMFLOAT4X4& GetViewMatrix() const { return m_viewMatrix; }
	const XMFLOAT4X4& GetProjectionMatrix() const { return m_projectionMatrix; }
	float GetFOV() const { return m_fov; }
	float GetNearPlane() const { return m_nearPlane; }
	float GetFarPlane() const { return m_farPlane; }
	bool IsMainCamera() const { return m_isMainCamera; }

	void Update(float dt) override;

private:
	float m_fov = 45.0f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 100.0f;

	bool m_isMainCamera = false;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;

};

