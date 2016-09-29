#pragma once

#include "CoreMath.h"

namespace mdl
{

class Transform
{
public:
	Transform();

	inline void setPos(const float32 x, const float32 y, const float32 z)
	{
		m_translationMatrix.initTranslation(x, y, z);
		update();
	}

	inline void update()
	{
		m_translationMatrix.mul(m_rotationMatrix.mul(m_scaleMatrix), &m_modelMatrix);
		m_modelMatrix.inverse(&m_inverseModelMatrix);
	}

	inline math::Matrix4f getModelMatrix()
	{
		return m_modelMatrix;
	}

	inline math::Matrix4f getInverseModelMatrix()
	{
		return m_inverseModelMatrix;
	}

private:
	math::Matrix4f m_modelMatrix;
	math::Matrix4f m_inverseModelMatrix;

	math::Matrix4f m_translationMatrix;
	math::Matrix4f m_rotationMatrix;
	math::Matrix4f m_scaleMatrix;
};

}