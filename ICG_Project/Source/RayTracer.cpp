#include "RayTracer.h"
#include "Model/Sphere.h"
#include "Camera.h"
#include "Intersection.h"
#include "HdrFrame.h"
#include "Ray.h"

#include <limits>
#include <vector>

// returns whether the intersection is found or not
boolean findClosestIntersection(const std::vector<mdl::Primitive*> &scene, const core::Ray &ray, core::Intersection *intersection)
{
	float32 closestSquaredHitDistance = std::numeric_limits<float32>::max();
	math::Vector3f closestHitPoint;
	math::Vector3f closestHitNormal;

	for(mdl::Primitive* pPrimitive : scene)
	{
		if(pPrimitive->isIntersect(ray, intersection))
		{
			float32 squaredHitDist = intersection->hitPoint.sub(ray.getOrigin()).squareLength();

			if(squaredHitDist < closestSquaredHitDistance)
			{
				closestSquaredHitDistance = squaredHitDist;
				closestHitPoint = intersection->hitPoint;
				closestHitNormal = intersection->hitNormal;
			}
		}
	}

	return closestSquaredHitDistance != std::numeric_limits<float32>::max();
}

void core::RayTracer::trace(core::HdrFrame *hdrFrame) const
{
	// a scene consists many primitives
	std::vector<mdl::Primitive*> scene;
	scene.push_back(new mdl::Sphere(0.0f, 0.0f, -10.0f, 0.5f));
	scene.push_back(new mdl::Sphere(2.0f, 0.0f, -10.0f, 0.5f));

	// default camera origin is at (0, 0, 0) looking in the -z direction
	// default field of view is 70 degrees
	core::Camera camera;

	// a storage place for intersection data
	core::Intersection intersection;

	// a storage place for ray attributes
	core::Ray ray;

	// for each pixel, shoot a ray and find the closest intersection, if found, add
	// color white in the frame
	for(uint32 x = 0; x < hdrFrame->getWidthPx(); x++)
	{
		for(uint32 y = 0; y < hdrFrame->getHeightPx(); y++)
		{
			camera.calcRayThroughPixel(&ray, hdrFrame->getWidthPx(), hdrFrame->getHeightPx(), x, y);

			if(findClosestIntersection(scene, ray, &intersection))
			{
				hdrFrame->setPixelRgb(x, y, 1.0f, 1.0f, 1.0f);
			}
		}
	}

	// don't forget to free them up
	for(mdl::Primitive* pPrimitive : scene)
	{
		delete pPrimitive;
	}
}
