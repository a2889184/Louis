#include "Camera.h"
#include "Ray.h"

core::Camera::Camera()
: m_pos(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, -1.0f), m_fov(70.0f / 180.0f * FLOAT32_PI)
{

}

void core::Camera::calcRayThroughPixel(core::Ray *ray, uint32 xResPx, uint32 yResPx, uint32 xPx, uint32 yPx) const
{
	// FIXME: this will fail when the camera is facing directly on y-axis

	math::Vector3f rightDir(-m_dir.z, 0.0f, m_dir.x);
	rightDir.normalizeLocal();
	math::Vector3f upDir;
	rightDir.cross(m_dir, &upDir);
	rightDir.normalizeLocal();

	float32 halfWidth = tan(m_fov / 2.0f);
	float32 halfHeight = halfWidth * (float32)yResPx / (float32)xResPx;

	float32 pixelPosX = ((xPx + 0.5f) / (xResPx / 2.0f) - 1.0f) * halfWidth;
	float32 pixelPosY = ((yPx + 0.5f) / (yResPx / 2.0f) - 1.0f) * halfHeight;

	rightDir.mulLocal(pixelPosX);
	upDir.mulLocal(pixelPosY);

	ray->setDir(m_dir.add(rightDir.addLocal(upDir)).normalizeLocal());
	ray->setOrigin(m_pos);
}