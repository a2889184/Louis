#pragma once

#include "Common.h"

namespace core
{

// The coordinate system of Frame is ordinary Cartesian coordinate system. Which
// means the origin is at the lower-left corner.

class Frame
{
public:
	Frame(const uint32 widthPx, const uint32 heightPx);
	~Frame();

	virtual void getPixelRgb(math::Vector3f *rgb, uint32 x, uint32 y) const = 0;
	virtual void setPixelRgb(uint32 x, uint32 y, float32 r, float32 g, float32 b) = 0;
	inline virtual void* getBuffer() const = 0;

	inline uint32 getWidthPx() const
	{
		return m_widthPx;
	}

	inline uint32 getHeightPx() const
	{
		return m_heightPx;
	}

private:
	uint32 m_widthPx;
	uint32 m_heightPx;
};

}