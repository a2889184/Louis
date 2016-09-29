#include "Lib.h"
#include "Window.h"
#include "ToneMapper.h"
#include "RayTracer.h"
#include "HdrFrame.h"

//#include "Model/Triangle.h"

#include <iostream>
#include <random>
#include <ctime>

#define SCREEN_WIDTH_PX 600
#define SCREEN_HEIGHT_PX 400

int main(int argc, char **argv)
{

	srand(time(nullptr));

	core::LdrFrame ldrFrame(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX);
	core::HdrFrame hdrFrame(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX);
	core::RayTracer rayTracer;

	core::Window window("ICG 2016", &ldrFrame);

	if(!window.generate())
	{
		return EXIT_FAILURE;
	}

	uint32 numFrames = 0;

	SDL_Event e;
	bool quit = false;
	while(!quit)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}

			/*if(e.type == SDL_KEYDOWN)
			{
				quit = true;
			}*/

			/*if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				float r = (float32)rand() / (float32)RAND_MAX;
				float g = (float32)rand() / (float32)RAND_MAX;
				float b = (float32)rand() / (float32)RAND_MAX;

				for(uint32 x = 0; x < 200; x++)
				{
					for(uint32 y = 0; y < 200; y++)
					{
						ldrFrame.setPixelRgb(x, y, r, g, b);
					}
				}
			}*/
		}

		rayTracer.trace(&hdrFrame);
		core::reinhardToneMapping(hdrFrame, ldrFrame);

		window.refresh();

		std::cout << "frame rendered " << ++numFrames << std::endl;
	}

	window.terminate();

	return EXIT_SUCCESS;
}