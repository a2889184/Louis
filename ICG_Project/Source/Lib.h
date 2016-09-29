#pragma once

#if _WIN32 || _WIN64
	#pragma comment(lib, "./Library/SDL2-2.0.4/lib/x64/SDL2.lib")
	#pragma comment(lib, "./Library/SDL2-2.0.4/lib/x64/SDL2main.lib")
#endif

#include "SDL.h"

#ifdef main
#undef main
#endif
