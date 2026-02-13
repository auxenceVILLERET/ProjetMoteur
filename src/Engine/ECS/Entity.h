#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Component;

class Entity
{
public:
	Entity();
	virtual ~Entity() = default;

	void SetId(uint32_t id) { m_Id = id; }
	uint32_t GetId() const { return m_Id; }

	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();

	void IdentityMatrix(XMFLOAT4X4& matrix); //needed ?

	//POSITION
	void SetPosition(float x, float y, float z); // set XYZ
	void SetPositionX(float x); // set XYZ
	void SetPositionY(float y); // set Y
	void SetPositionZ(float z); // set Z

	void MoveForward(float dist); // move along the "forward" of the obj
	void MoveUp(float dist); // move along the "up" of the obj
	void MoveRight(float dist); // move along the "right" of the obj

	XMFLOAT3 GetPosition() const { return m_position; } // get the position of the obj

	void CoutPosition(); // DEBUG

	//SCALING
	void SetScale(float scale); // set the scale 
	void Scale(float scale); // multiply current scale
	void SetScaleVector(XMFLOAT3 vectorScale); // set the scale with XYZ Vector
	void ScaleVector(XMFLOAT3 vectorScale); // multiply current scale with XYZ Vector
	XMFLOAT3 GetScale() const { return m_scale; } // get the scale of the obj

	void CoutScale(); // DEBUG

	//ROTATION
	void SetRotationX(float angle); // set the rotation
	void SetRotationY(float angle); // set the rotation
	void SetRotationZ(float angle); // set the rotation

	void LookAt(float x, float y, float z); // look at a specific coords

	XMFLOAT4X4 GetMatrix() const { return m_matrix; } // get the matrix of the obj

	void CoutRotation(); // DEBUG
	void CoutMatrix(); // DEBUG


public:
	void UpdateBasisFromQuaternion();
	void UpdateWorldMatrix();

	//position
	XMFLOAT3 m_position;

	//scaling
	XMFLOAT3 m_scale;

	//rotation
	XMFLOAT3 m_forward;
	XMFLOAT3 m_up;
	XMFLOAT3 m_right;

	XMFLOAT4 m_quaternion;
	XMFLOAT4X4 m_matrix;
	uint32_t m_Id = 0;
	std::vector<Component*> m_Components;
};

#include "Entity.inl"
