#include "Entity.h"
#include <iostream>

using namespace DirectX;

Entity::Entity()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_scale = { 1, 1, 1 };

	m_quaternion = { 0.0f, 0.0f, 0.0f, 1.0f }; // identité

	UpdateWorldMatrix();
}

void Entity::IdentityMatrix(XMFLOAT4X4& matrix)
{
	matrix._11 = 1; matrix._12 = 0; matrix._13 = 0; matrix._14 = 0;
	matrix._21 = 0; matrix._22 = 1; matrix._23 = 0; matrix._24 = 0;
	matrix._31 = 0; matrix._32 = 0; matrix._33 = 1; matrix._34 = 0;
	matrix._41 = 0; matrix._42 = 0; matrix._43 = 0; matrix._44 = 1;
}

void Entity::SetPosition(float x, float y, float z)
{
	m_position = { x,y,z };
	UpdateWorldMatrix();
}

void Entity::SetPosition(XMFLOAT3 vector)
{
	m_position.x = vector.x;
	m_position.y = vector.y;
	m_position.z = vector.z;
	UpdateWorldMatrix();
}

void Entity::SetPositionX(float x)
{
	m_position.x = x;
	UpdateWorldMatrix();
}

void Entity::SetPositionY(float y)
{
	m_position.y = y;
	UpdateWorldMatrix();
}

void Entity::SetPositionZ(float z)
{
	m_position.z = z;
	UpdateWorldMatrix();
}

void Entity::MoveForward(float dist)
{
	m_position.x += m_forward.x * dist;
	m_position.y += m_forward.y * dist;
	m_position.z += m_forward.z * dist;
	UpdateWorldMatrix();
}

void Entity::MoveUp(float dist)
{
	m_position.x += m_up.x * dist;
	m_position.y += m_up.y * dist;
	m_position.z += m_up.z * dist;
	UpdateWorldMatrix();
}

void Entity::MoveRight(float dist)
{
	m_position.x += m_right.x * dist;
	m_position.y += m_right.y * dist;
	m_position.z += m_right.z * dist;
	UpdateWorldMatrix();
}

void Entity::Translate(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	UpdateWorldMatrix();
}

void Entity::CoutPosition()
{
	std::cout << "Position : [" << m_position.x << "][" << m_position.y << "][" << m_position.z << "]\n" << std::endl;
}

// SCALING

void Entity::SetScale(float scale)
{
	m_scale = { scale, scale, scale };
	UpdateWorldMatrix();
}

void Entity::Scale(float scale)
{
	m_scale.x *= scale;
	m_scale.y *= scale;
	m_scale.z *= scale;
	UpdateWorldMatrix();
}

void Entity::SetScale(XMFLOAT3 vectorScale)
{
	m_scale = vectorScale;
	UpdateWorldMatrix();
}

void Entity::Scale(XMFLOAT3 vectorScale)
{
	m_scale.x *= vectorScale.x;
	m_scale.y *= vectorScale.y;
	m_scale.z *= vectorScale.z;
	UpdateWorldMatrix();
}

void Entity::CoutScale()
{
	std::cout << "Scaling : [" << m_scale.x << "][" << m_scale.y << "][" << m_scale.z << "]\n" << std::endl;
}

// ROTATION

void Entity::RotateX(float angle)
{
    XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
    XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(1,0,0,0), angle);
    q = XMQuaternionNormalize(XMQuaternionMultiply(q, dq));
    XMStoreFloat4(&m_quaternion, q);
    UpdateWorldMatrix();
}

void Entity::RotateLocalX(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), angle);
	q = XMQuaternionNormalize(XMQuaternionMultiply(dq, q));
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::RotateY(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), angle);
	q = XMQuaternionNormalize(XMQuaternionMultiply(q, dq));
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::RotateLocalY(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), angle);
	q = XMQuaternionNormalize(XMQuaternionMultiply(dq, q));
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::RotateZ(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), angle);
	q = XMQuaternionNormalize(XMQuaternionMultiply(q, dq));
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::RotateLocalZ(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMVECTOR dq = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), angle);
	q = XMQuaternionNormalize(XMQuaternionMultiply(dq, q));
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::SetRotation(XMFLOAT4 quaternion)
{
	XMVECTOR q = XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::SetRotationX(float angle)
{
	XMVECTOR q = XMVectorSet(angle, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::SetRotationY(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, angle, m_quaternion.z, m_quaternion.w);
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}

void Entity::SetRotationZ(float angle)
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y,  angle, m_quaternion.w);
	XMStoreFloat4(&m_quaternion, q);
	UpdateWorldMatrix();
}



void Entity::LookAt(float x, float y, float z)
{

}

void Entity::OrbitAround(XMFLOAT3 pivot, XMFLOAT3 axis, float angle, float radius)
{
	// Charger position et pivot
	XMVECTOR pos = XMLoadFloat3(&m_position);
	XMVECTOR piv = XMLoadFloat3(&pivot);
	XMVECTOR ax = XMVector3Normalize(XMLoadFloat3(&axis));

	// Direction actuelle depuis le pivot
	XMVECTOR dir = pos - piv;

	// Si l'objet est exactement au pivot, on force une direction par défaut
	if (XMVector3LengthSq(dir).m128_f32[0] < 0.00001f)
	{
		dir = XMVectorSet(1, 0, 0, 0); // direction X par défaut
	}

	// Normalisation puis application du rayon
	dir = XMVector3Normalize(dir);
	dir *= radius;

	// Quaternion de rotation
	XMVECTOR q = XMQuaternionRotationAxis(ax, angle);

	// Rotation du vecteur
	dir = XMVector3Rotate(dir, q);

	// Nouvelle position
	pos = piv + dir;

	XMStoreFloat3(&m_position, pos);

	UpdateWorldMatrix();
}

XMFLOAT4 Entity::GetRotation()
{
	return XMFLOAT4();
}



void Entity::CoutRotation()
{
	std::cout << "Rotation : [" << m_quaternion.x << "][" << m_quaternion.y << "][" << m_quaternion.z << "][" << m_quaternion.w << "]\n" << std::endl;
}

void Entity::CoutMatrix()
{
	std::cout << "Matrix :";
	for (int i = 0; i < 4; i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < 4; j++)
		{
			std::cout << " [" << m_matrix.m[i][j] << "] ";
		}
	}
	std::cout << std::endl;
}

void Entity::UpdateBasisFromQuaternion()
{
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	q = XMQuaternionNormalize(q);

	XMMATRIX R = XMMatrixRotationQuaternion(q);

	// convention: forward = +Z, up = +Y, right = +X en local
	XMVECTOR right = XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), R);
	XMVECTOR up = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), R);
	XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), R);

	XMStoreFloat3(&m_right, right);
	XMStoreFloat3(&m_up, up);
	XMStoreFloat3(&m_forward, forward);

	XMStoreFloat4(&m_quaternion, q);
}

void Entity::UpdateWorldMatrix()
{
	// Rotation
	XMVECTOR q = XMVectorSet(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	q = XMQuaternionNormalize(q);

	XMMATRIX S = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX R = XMMatrixRotationQuaternion(q);
	XMMATRIX T = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	// World = S * R * T (convention DirectX classique)
	XMMATRIX W = S * R * T;
	XMStoreFloat4x4(&m_matrix, W);

	// Mets aussi à jour forward/up/right pour MoveForward/Up/Right
	UpdateBasisFromQuaternion();
}
