#pragma once

#include "Frame.h"

namespace core
{

// The coordinate system of Frame is ordinary Cartesian coordinate system. Which
// means the origin is at the lower-left corner.

class LdrFrame : public Frame
{
public:
	LdrFrame(const uint32 widthPx, const uint32 heightPx);
	~LdrFrame();

	virtual void getPixelRgb(math::Vector3f *rgb, uint32 x, uint32 y) const override;
	virtual void setPixelRgb(uint32 x, uint32 y, float32 r, float32 g, float32 b) override;

	inline virtual void* getBuffer() const override
	{
		return m_buffer;
	}

private:
	uint8 *m_buffer;
};

}