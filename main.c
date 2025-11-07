#define SDL_MAIN_HANDLED

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "stk.h"


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
SDL_FRect tex_rect = {0.0f, 0.0f, 300.0f, 300.0f};

char *getCfg(const char *name) {
	struct STK_Result *result = STK_GetData();
	char *ret = "";

	for (int i = 0; i < result->nrLines; i++) {
		if (strcmp(name, result->lines[i].tokens[0].data) == 0) {
			ret = result->lines[i].tokens[2].data;
		}
	}

	return ret;
}

SDL_Texture* loadTexture(char *path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == nullptr)
	{
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == nullptr)
		{
			SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		}
		SDL_DestroySurface(loadedSurface);
	}

	return newTexture;
}

bool start()
{
    bool success = true;

	STK_Create("settings.ini");
	STK_Parse();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL3 could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		char *convResult = nullptr;
		const int WIDTH = (int)strtol(getCfg("Width"), &convResult, 10);
		const int HEIGHT = (int)strtol(getCfg("Height"), &convResult, 10);

		gWindow = SDL_CreateWindow(getCfg("Title"), WIDTH , HEIGHT, 0);
		if( gWindow == nullptr )
		{
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, nullptr);
			if(gRenderer == nullptr)
			{
				SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

				constexpr int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if( TTF_Init() == -1 )
				{
					SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool load() {
	gTexture = loadTexture("boom.png");
	if(gTexture == NULL)
	{
		SDL_Log("Failed to load texture image!\n");
		return false;
	}

	return true;
}

void end()
{
	STK_Destroy();

	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, const char *argv[])
{
	bool loop = true;

    start();
	load();

	SDL_Event e;
	while(loop)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_EVENT_QUIT)
			{
				loop = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0, 0, 30, 255);
		SDL_RenderClear(gRenderer);

		SDL_RenderTexture(gRenderer, gTexture, nullptr, &tex_rect);

		SDL_RenderPresent(gRenderer);
	}

    end();

    return 0;
}