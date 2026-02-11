#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_scale = { 1, 1, 1 };

	m_forward = { 0.0f, 0.0f, 1.0f };
	m_up = { 0.0f, 1.0f, 0.0f };
	m_right = { 1.0f, 0.0f, 0.0f };
	m_quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };

	IdentityMatrix(m_matrix);
}

void Entity::IdentityMatrix(XMFLOAT4X4& matrix)
{
	// Set a given 4x4 matrix values to 0;
	matrix._11 = 1; matrix._12 = 0; matrix._13 = 0; matrix._14 = 0;
	matrix._21 = 0; matrix._22 = 1; matrix._23 = 0; matrix._24 = 0;
	matrix._31 = 0; matrix._32 = 0; matrix._33 = 1; matrix._34 = 0;
	matrix._41 = 0; matrix._42 = 0; matrix._34 = 0; matrix._44 = 1;
}

void Entity::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Entity::MoveForward(float dist)
{
	m_position.x += m_forward.x * dist;
	m_position.y += m_forward.y * dist;
	m_position.z += m_forward.z * dist;
}

void Entity::MoveUp(float dist)
{
	m_position.x += m_up.x * dist;
	m_position.y += m_up.y * dist;
	m_position.z += m_up.z * dist;
}

void Entity::MoveRight(float dist)
{
	m_position.x += m_right.x * dist;
	m_position.y += m_right.y * dist;
	m_position.z += m_right.z * dist;
}

void Entity::CoutPosition()
{
	std::cout << "Position : [" << m_position.x << "][" << m_position.y << "][" << m_position.z << "]\n" << std::endl;
}

// SCALING

void Entity::SetScale(float scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
}

void Entity::Scale(float scale)
{
	m_scale.x *= scale;
	m_scale.y *= scale;
	m_scale.z *= scale;
}

void Entity::SetScaleVector(XMFLOAT3 vectorScale)
{
	m_scale.x = vectorScale.x;
	m_scale.y = vectorScale.y;
	m_scale.z = vectorScale.z;
}

void Entity::ScaleVector(XMFLOAT3 vectorScale)
{
	m_scale.x *= vectorScale.x;
	m_scale.y *= vectorScale.y;
	m_scale.z *= vectorScale.z;
}

void Entity::CoutScale()
{
	std::cout << "Scaling : [" << m_scale.x << "][" << m_scale.y << "][" << m_scale.z << "]\n" << std::endl;
}

// ROTATION

void Entity::SetRotationX(float angle)
{
	XMMATRIX world = XMLoadFloat4x4(&m_matrix);
	world *= XMMatrixRotationX(angle);
	XMStoreFloat4x4(&m_matrix, world);
}

void Entity::SetRotationY(float angle)
{
	XMMATRIX world = XMLoadFloat4x4(&m_matrix);
	world *= XMMatrixRotationY(angle);
	XMStoreFloat4x4(&m_matrix, world);
}

void Entity::SetRotationZ(float angle)
{
	XMMATRIX world = XMLoadFloat4x4(&m_matrix);
	world *= XMMatrixRotationZ(angle);
	XMStoreFloat4x4(&m_matrix, world);
}

void Entity::LookAt(float x, float y, float z)
{

}

void Entity::SetMatrix(float matrixPos, float value)
{
	matrixPos = value;
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