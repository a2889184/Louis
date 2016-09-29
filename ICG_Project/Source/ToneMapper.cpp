#include "ToneMapper.h"
#include "CoreMath.h"

void core::reinhardToneMapping(const core::HdrFrame &hdrSrc, core::LdrFrame &ldrDst)
{
	// TODO: mapping with mismatched src, dst dimensions

	math::Vector3f hdrColor;

	for(uint32 x = 0; x < ldrDst.getWidthPx(); x++)
	{
		for(uint32 y = 0; y < ldrDst.getHeightPx(); y++)
		{
			hdrSrc.getPixelRgb(&hdrColor, x, y);

			float32 r = hdrColor.x / (hdrColor.x + 1.0f);
			float32 g = hdrColor.y / (hdrColor.y + 1.0f);
			float32 b = hdrColor.z / (hdrColor.z + 1.0f);

			ldrDst.setPixelRgb(x, y, r, g, b);
		}
	}
}