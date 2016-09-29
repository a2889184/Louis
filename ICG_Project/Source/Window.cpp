#include "Window.h"

#include <iostream>


core::Window::Window(const std::string &title, LdrFrame *ldrFrame)
: m_title(title), m_ldrFrame(ldrFrame)
{
	
}

core::Window::~Window()
{
	
}

boolean core::Window::generate()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init() Error: " << SDL_GetError() << std::endl;
		return FALSE;
	}

	m_sdlWindow = SDL_CreateWindow(m_title.c_str(),
								   30, 30, m_ldrFrame->getWidthPx(), m_ldrFrame->getHeightPx(),
								   SDL_WINDOW_SHOWN);
	if(m_sdlWindow == nullptr)
	{
		std::cerr << "SDL_CreateWindow() Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return FALSE;
	}

	m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_SOFTWARE);
	if(m_sdlRenderer == nullptr)
	{
		std::cout << "SDL_CreateRenderer() Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_sdlWindow);
		SDL_Quit();
		return FALSE;
	}


	m_sdlTexture = SDL_CreateTexture(m_sdlRenderer,
								     SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
									 m_ldrFrame->getWidthPx(), m_ldrFrame->getHeightPx());
	if(m_sdlTexture == nullptr)
	{
		SDL_DestroyRenderer(m_sdlRenderer);
		SDL_DestroyWindow(m_sdlWindow);
		std::cout << "SDL_CreateTexture() Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return FALSE;
	}

	return TRUE;
}

void core::Window::terminate()
{
	SDL_DestroyWindow(m_sdlWindow);
	SDL_Quit();
}

void core::Window::refresh()
{
	SDL_RenderClear(m_sdlRenderer);
	SDL_UpdateTexture(m_sdlTexture, NULL, m_ldrFrame->getBuffer(), 3 * m_ldrFrame->getWidthPx() * sizeof(uint8));
	SDL_RenderCopyEx(m_sdlRenderer, m_sdlTexture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
	SDL_RenderPresent(m_sdlRenderer);
}