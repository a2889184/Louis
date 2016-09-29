#pragma once

#include "Primitive.h"
#include "CoreMath.h"

namespace mdl
{

class Sphere : public Primitive
{
public:
	Sphere(const math::Vector3f &center, const float32 radius);
	Sphere(float32 centerX, float32 centerY, float32 centerZ, float32 radius);
	virtual ~Sphere() override;

	virtual boolean isIntersect(const core::Ray &ray, core::Intersection *intersection) const;

private:
	math::Vector3f m_center;
	float32 m_radius;
};

}