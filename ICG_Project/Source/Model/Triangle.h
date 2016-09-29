#pragma once

#include "Primitive.h"
#include "CoreMath.h"

namespace mdl
{

class Triangle : public Primitive
{
public:
	Triangle(const math::Vector3f &vA, const math::Vector3f &vB, const math::Vector3f &vC);
	virtual ~Triangle();

	virtual boolean isIntersect(const core::Ray &ray, core::Intersection *intersection) const override;

private:
	// vertex positions
	math::Vector3f m_vA;
	math::Vector3f m_vB;
	math::Vector3f m_vC;

	// face normal
	math::Vector3f m_nFace;

	// normal for each vertices
	math::Vector3f m_nA;
	math::Vector3f m_nB;
	math::Vector3f m_nC;

	// edge vector
	math::Vector3f m_eAB;
	math::Vector3f m_eAC;
};

}