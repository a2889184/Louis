#pragma once

#include "Frame.h"

namespace core
{
	
class HdrFrame : public Frame
{
public:
	HdrFrame(const uint32 widthPx, const uint32 heightPx);
	~HdrFrame();

	virtual void getPixelRgb(math::Vector3f *rgb, uint32 x, uint32 y) const override;
	virtual void setPixelRgb(uint32 x, uint32 y, float32 r, float32 g, float32 b) override;

	inline virtual void* getBuffer() const override
	{
		return m_buffer;
	}

private:
	float32 *m_buffer;
};

}