#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int start()
{
    int success = 1;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL3 could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	}
	else
	{
		const int WIDTH = 1024;
		const int HEIGHT = 768;

		gWindow = SDL_CreateWindow("SDL3_template", WIDTH, HEIGHT, 0);
		if( gWindow == NULL )
		{
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = 0;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, NULL);
			if(gRenderer == NULL)
			{
				SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = 0;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			}
		}
	}

	return success;
}


void end()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

int main(int argc, const char *argv[])
{
	int loop = 0;

    start();

	SDL_Event e;
	while(!loop)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_EVENT_QUIT)
			{
				return 0;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0, 50, 0, 255);
		SDL_RenderClear(gRenderer);

		SDL_RenderPresent(gRenderer);
	}

    end();

    return 0;
}