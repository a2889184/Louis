#pragma once

#include "CoreMath.h"

namespace core
{

class Ray;

class Camera
{
public:
	Camera();

	void calcRayThroughPixel(core::Ray *ray, uint32 xResPx, uint32 yResPx, uint32 xPx, uint32 yPx) const;

	inline void getPos(math::Vector3f *result) const
	{
		result->set(m_pos);
	}

	inline void getDir(math::Vector3f *result) const
	{
		result->set(m_dir);
	}

	inline float32 getFov() const
	{
		return m_fov;
	}

	inline void setPos(const math::Vector3f &pos)
	{
		m_pos.set(pos);
	}

	inline void setPos(const float32 x, const float32 y, const float32 z)
	{
		m_pos.set(x, y, z);
	}

	inline void setDir(const math::Vector3f &dir)
	{
		m_pos.set(m_dir);
	}

	inline void setDir(const float32 x, const float32 y, const float32 z)
	{
		m_dir.set(x, y, z);
	}

	inline void setFov(const float32 fov)
	{
		m_fov = fov;
	}

	inline void setFovDeg(const float32 fovDeg)
	{
		m_fov = fovDeg / 180.0f * FLOAT32_PI;
	}

private:
	math::Vector3f m_pos;
	math::Vector3f m_dir;

	// field of view, in radians
	float32 m_fov;
};

}