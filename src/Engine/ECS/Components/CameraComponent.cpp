#include "CameraComponent.h"
#include "Engine/Utils/Frustum.h"

void CameraComponent::SetAll(float nearPlane, float farPlane, float height, float width, bool perspective)
{
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_height = height;
	m_width = width;
	m_aspectRatio = m_width / m_height;
	m_isPerspective = perspective;

	m_frustum = new Frustum();
}

void CameraComponent::SetWindowSize(float width, float height)
{
	m_width = width; 
	m_height = height; 
	m_aspectRatio = width / height; 
}

void CameraComponent::Update()
{
	XMMATRIX view = XMLoadFloat4x4(&m_viewMatrix);
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixInverse(nullptr, view));
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
