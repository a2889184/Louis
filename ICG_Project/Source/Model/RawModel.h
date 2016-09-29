#pragma once

#include "Model.h"
#include "Transform.h"
#include "Primitive.h"

namespace mdl
{

class RawModel : public Model
{
public:
	RawModel(mdl::Primitive &primitive);
	virtual ~RawModel() override;

	inline virtual Transform* getTransform() override
	{
		return m_transform;
	}

	inline virtual Primitive* getPrimitive() override
	{
		return m_primitive;
	}

private:
	Primitive *m_primitive;
	Transform *m_transform;
};

}