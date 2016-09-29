#pragma once

#include "Common.h"

namespace core
{
	class Ray;
	class Intersection;
}

namespace mdl
{

class Model;
	
class Primitive
{
public:
	virtual ~Primitive();

	virtual boolean isIntersect(const core::Ray &ray, core::Intersection *intersection) const = 0;

	inline Model* getModel() const
	{
		return m_model;
	}

	inline void setModel(Model *model)
	{
		m_model = model;
	}

private:
	Model *m_model = nullptr;
};

}