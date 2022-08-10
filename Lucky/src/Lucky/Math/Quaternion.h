#pragma once

#include <DirectXMath.h>

#include "../../LuckyMacros.h"

using namespace DirectX;

NAMESPACE_LUCKY_MATH_START


class Quaternion
{
public:
	Quaternion() { m_Vector = XMQuaternionIdentity(); }
	Quaternion(float pitch, float yaw, float roll) { m_Vector = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }

	explicit Quaternion(const DirectX::XMMATRIX& matrix) { m_Vector = XMQuaternionRotationMatrix(matrix); }
	explicit Quaternion(FXMVECTOR vec) { m_Vector = vec; }

	operator XMVECTOR() const { return m_Vector; }

	Quaternion operator~ (void) const { return Quaternion(XMQuaternionConjugate(m_Vector)); }
	Quaternion operator- (void) const { return Quaternion(XMVectorNegate(m_Vector)); }

	Quaternion operator* (Quaternion rhs) const { return Quaternion(XMQuaternionMultiply(rhs, m_Vector)); }
	

private:
	XMVECTOR m_Vector;
};

NAMESPACE_LUCKY_MATH_END