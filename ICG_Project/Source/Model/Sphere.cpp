#include "Sphere.h"
#include "Ray.h"
#include "Intersection.h"

mdl::Sphere::Sphere(const math::Vector3f &center, const float32 radius)
: m_center(center), m_radius(radius)
{

}

mdl::Sphere::Sphere(float32 centerX, float32 centerY, float32 centerZ, float32 radius)
: m_center(centerX, centerY, centerZ), m_radius(radius)
{

}

mdl::Sphere::~Sphere()
{

}

boolean mdl::Sphere::isIntersect(const core::Ray &ray, core::Intersection *intersection) const
{
	// ray origin:         o
	// ray direction:      d
	// sphere center:      c
	// sphere radius:      r
	// intersection point: p
	// vector dot:         *
	// ray equation:       o + td (t is a scalar variable)
	// To find the intersection point, the length of vector (td - oc) must equals r.
	// equation: t^2(d*d) - 2t(d*op) + (oc*oc) - r^2 = 0
	// (d*d is in fact 1)

	// vector from ray origin to sphere center
	math::Vector3f oc;
	ray.getOrigin(&oc);
	oc.set(m_center.x - oc.x, m_center.y - oc.y, m_center.z - oc.z);

	math::Vector3f rayDir;
	ray.getDir(&rayDir);

	// b in the quadratic equation above (-2 can be cancelled out while solving t)
	float32 b = rayDir.dot(oc);
	float32 D = b*b - oc.dot(oc) + m_radius*m_radius;

	if(D < 0.0f)
	{
		return FALSE;
	}
	else
	{
		float32 t;

		D = sqrt(D);

		// pick closest point in front of ray origin
		t = (t = b - D) > FLOAT32_EPSILON ? t : ((t = b + D) > FLOAT32_EPSILON ? t : 0.0f);

		if(t > 0.0f)
		{
			math::Vector3f rayOrigin;
			ray.getOrigin(&rayOrigin);

			//intersection.setHitAtomicPrimitive(this);
			intersection->hitPoint = rayDir.mul(t).addLocal(rayOrigin);
			intersection->hitNormal = intersection->hitPoint.sub(m_center).divLocal(m_radius);

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}