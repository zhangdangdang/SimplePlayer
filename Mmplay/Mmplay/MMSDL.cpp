#include"Mmplay.h"
MMSDL::MMSDL(int _screen_w, int _screen_h)
{
	screen_w = _screen_w;
	screen_h = _screen_h;
}

MMSDL::~MMSDL()
{
	printf("sdl Îö¹¹\n");
	if (sdlRenderer != nullptr)
	{
		delete sdlRenderer;
		sdlRenderer = nullptr;
	}
	if (sdlTexture != nullptr)
	{
		delete sdlTexture;
		sdlTexture = nullptr;
	}
}
int MMSDL::MMSDLInit()
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}
	SDL_Window* screen;
	//SDL 2.0 Support for multiple windows
	
	screen = SDL_CreateWindow(" Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!screen) {
		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
		return -1;
	}
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	Uint32 pixformat = 0;
	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	pixformat = SDL_PIXELFORMAT_IYUV;

	sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
	
}
int MMSDL::MMSdlUpdateTexture(MMAVFrame* frame,int x,int y,int w,int h)
{
	SDL_UpdateTexture(sdlTexture, NULL, frame->imp->frame->data[0], frame->imp->frame->linesize[0]);
	sdlRect.x = x;
	sdlRect.y = y;
	sdlRect.w = w;
	sdlRect.h = h;
	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
	SDL_RenderPresent(sdlRenderer);
	return 0;
}