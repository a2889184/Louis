#pragma once

#include "LdrFrame.h"
#include "HdrFrame.h"

namespace core
{

extern void reinhardToneMapping(const core::HdrFrame &hdrSrc, core::LdrFrame &ldrDst);

}