#pragma once
#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class RigidBodyComponent : public Component
{
public:
	void SetVelocity(const XMFLOAT3& velocity) { m_Velocity = velocity; }
	void SetUseGravity(bool useGravity) { m_useGravity = useGravity; }

	bool IsUsingGravity() const { return m_useGravity; }
	const XMFLOAT3& GetVelocity() const { return m_Velocity; }

private:
	XMFLOAT3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	bool m_useGravity = true;
};

