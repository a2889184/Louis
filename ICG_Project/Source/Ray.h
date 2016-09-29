#pragma once

#include "CoreMath.h"

namespace core
{

class Ray
{
public:
	Ray(const math::Vector3f &origin, const math::Vector3f &dir)
		: m_origin(origin), m_dir(dir)
	{

	}

	Ray()
		: m_origin(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, -1.0f)
	{

	}

	inline math::Vector3f getOrigin() const
	{
		return m_origin;
	}

	inline void getOrigin(math::Vector3f *result) const
	{
		result->set(m_origin);
	}

	inline math::Vector3f getDir() const
	{
		return m_dir;
	}

	inline void getDir(math::Vector3f *result) const
	{
		result->set(m_dir);
	}

	inline void setOrigin(const math::Vector3f &origin)
	{
		m_origin.set(origin);
	}

	inline void setDir(const math::Vector3f &dir)
	{
		m_dir.set(dir);
	}

private:
	math::Vector3f m_origin;
	math::Vector3f m_dir;
};

}