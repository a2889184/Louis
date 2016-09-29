#include "Transform.h"

mdl::Transform::Transform()
{
	m_modelMatrix.initIdentity();
	m_inverseModelMatrix.initIdentity();

	m_translationMatrix.initIdentity();
	m_rotationMatrix.initIdentity();
	m_scaleMatrix.initIdentity();
}

