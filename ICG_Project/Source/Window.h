#pragma once

#include "Lib.h"
#include "Common.h"
#include "LdrFrame.h"

#include <string>

namespace core
{

class Window
{
public:
	Window(const std::string &title, LdrFrame *m_ldrFrame);
	~Window();

	boolean generate();
	void terminate();
	void refresh();

private:
	std::string m_title;

	SDL_Window *m_sdlWindow = nullptr;
	SDL_Renderer *m_sdlRenderer = nullptr;
	SDL_Texture *m_sdlTexture = nullptr;
	LdrFrame *m_ldrFrame = nullptr;
};

}

