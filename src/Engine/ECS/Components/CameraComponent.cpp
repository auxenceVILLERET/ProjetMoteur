#include "CameraComponent.h"
#include "Engine/Utils/Frustum.h"

void CameraComponent::SetAll(float nearPlane, float farPlane, float height, float width, bool perspective)
{
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_height = height;
	m_width = width;
	m_aspectRatio = m_height / m_width;
	m_isPerspective = perspective;

	m_frustum = new Frustum();
}

void CameraComponent::Update(float dt)
{
	XMMATRIX view = XMLoadFloat4x4(&m_viewMatrix);
	XMStoreFloat4x4(&m_viewMatrix, view);
	view *= XMLoadFloat4x4(&m_projectionMatrix);
	XMStoreFloat4x4(&m_projectionMatrix, view);

	m_frustum->FromViewProjection(m_projectionMatrix);
}

void CameraComponent::UpdateProjectionMatrix()
{
	if(m_isPerspective)
		XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_nearPlane, m_farPlane));
	else
		XMStoreFloat4x4(&m_projectionMatrix, XMMatrixOrthographicLH(m_width, m_height, m_nearPlane, m_farPlane));
}
