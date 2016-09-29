#include "LdrFrame.h"
#include "CoreMath.h"

core::LdrFrame::LdrFrame(const uint32 widthPx, const uint32 heightPx)
: Frame(widthPx, heightPx)
{
	m_buffer = new uint8[widthPx * heightPx * 3]();
}

core::LdrFrame::~LdrFrame()
{
	delete[] m_buffer;
}

void core::LdrFrame::getPixelRgb(math::Vector3f *rgb, uint32 x, uint32 y) const
{
	uint32 index = 3 * (y * getWidthPx() + x);
	rgb->x = (float32)(m_buffer[index + 0]) / 255.0f;
	rgb->y = (float32)(m_buffer[index + 1]) / 255.0f;
	rgb->z = (float32)(m_buffer[index + 2]) / 255.0f;
}

void core::LdrFrame::setPixelRgb(uint32 x, uint32 y, float32 r, float32 g, float32 b)
{
	uint32 index = 3 * (y * getWidthPx() + x);
	m_buffer[index + 0] = (uint8)(255.0f * r + 0.5f);
	m_buffer[index + 1] = (uint8)(255.0f * g + 0.5f);
	m_buffer[index + 2] = (uint8)(255.0f * b + 0.5f);
}