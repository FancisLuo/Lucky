#pragma once

#include <DirectXMath.h>

#include "../../LuckyMacros.h"


using namespace DirectX;

NAMESPACE_LUCKY_MATH_START

class Vector3
{
public:
	Vector3() = default;
	Vector3(float x, float y) { m_Vector = XMVectorSet(x, y, 0.0f, 0.0f); }
	Vector3(float x, float y, float z) { m_Vector = XMVectorSet(x, y, z, 0.0f); }
	Vector3(const XMFLOAT3& vf) { m_Vector = XMLoadFloat3(&vf); }
	Vector3(const Vector3& other) { m_Vector = other.m_Vector; }
	Vector3(const XMVECTOR& v) { m_Vector = v; }

	operator XMVECTOR() const { return m_Vector; }

	Vector3 operator+ (Vector3 other) const { return Vector3(XMVectorAdd(m_Vector, other)); }
	Vector3 operator- (Vector3 other) const { return Vector3(XMVectorSubtract(m_Vector, other)); }
	Vector3 operator* (Vector3 other) const { return Vector3(XMVectorMultiply(m_Vector, other)); }
	Vector3 operator/ (Vector3 other) const { return Vector3(XMVectorDivide(m_Vector, other)); }


private:
	XMVECTOR m_Vector;
};

NAMESPACE_LUCKY_MATH_END