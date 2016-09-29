#pragma once

namespace mdl
{

class Transform;
class Primitive;

class Model
{
public:
	virtual ~Model()
	{
		
	}

	inline virtual Transform* getTransform() = 0;
	inline virtual Primitive* getPrimitive() = 0;
};

}