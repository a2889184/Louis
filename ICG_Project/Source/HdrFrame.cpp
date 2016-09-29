#include "HdrFrame.h"
#include "CoreMath.h"

core::HdrFrame::HdrFrame(const uint32 widthPx, const uint32 heightPx)
: Frame(widthPx, heightPx)
{
	m_buffer = new float32[widthPx * heightPx * 3]();
}

core::HdrFrame::~HdrFrame()
{
	delete[] m_buffer;
}

void core::HdrFrame::getPixelRgb(math::Vector3f *rgb, uint32 x, uint32 y) const
{
	uint32 index = 3 * (y * getWidthPx() + x);
	rgb->x = m_buffer[index + 0];
	rgb->y = m_buffer[index + 1];
	rgb->z = m_buffer[index + 2];
}

void core::HdrFrame::setPixelRgb(uint32 x, uint32 y, float32 r, float32 g, float32 b)
{
	uint32 index = 3 * (y * getWidthPx() + x);
	m_buffer[index + 0] = r;
	m_buffer[index + 1] = g;
	m_buffer[index + 2] = b;
}