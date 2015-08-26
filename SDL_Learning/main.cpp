#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// Enumerations
enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
const int SCREEN_FPS = 60;
const int STANDING_FRAME_COUNT = 8;
const int RUNNING_FRAME_COUNT = 1;

//Global Variables
SDL_Window* gWindow;
SDL_Surface* gScreenSurface;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface;
SDL_Surface* gPNGSurface;
SDL_Texture* gTexture;
SDL_Renderer* gRenderer;
Texture texture;
Texture background;
Texture stageObjects;
SDL_Rect gSpriteRects[STANDING_FRAME_COUNT];
SDL_Rect floorRect;
int floorOffset = 0;
int backgroundHorzOffset = 0;
int backgroundVertOffset = 0;
int moveSpeed = 0;
int vertSpeed = 0;

bool init();

bool loadMedia();

void close();

SDL_Surface* loadSurface(std::string path);

SDL_Texture* loadTexture(std::string path);

bool init() {
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Failed to initialize SDL: %s\n", SDL_GetError());
		success = false;
    } else {
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if (gWindow == NULL) {
			printf("Failed to create window: %s\n", SDL_GetError());
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Failed to create renderer: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	if(!texture.loadFromFile("resources/images/sonicadvance3.png", gRenderer)) {
		printf( "Failed to load texture.\n" );
		success = false;
	}

	if(!background.loadFromFile("resources/images/background.png", gRenderer)) {
		printf( "Failed to load texture.\n" );
		success = false;
	}

	if (!stageObjects.loadFromFile("resources/images/stageObjects.png", gRenderer)) {
		printf("Failed to load stage objects texture.\n");
		success = false;
	}

	if (success) {
		gSpriteRects[0].x = 6;
		gSpriteRects[0].y = 10;
		gSpriteRects[0].w = 32;
		gSpriteRects[0].h = 35;

		gSpriteRects[1].x = 42;
		gSpriteRects[1].y = 10;
		gSpriteRects[1].w = 32;
		gSpriteRects[1].h = 35;

		gSpriteRects[2].x = 78;
		gSpriteRects[2].y = 10;
		gSpriteRects[2].w = 32;
		gSpriteRects[2].h = 35;

		gSpriteRects[3].x = 114;
		gSpriteRects[3].y = 10;
		gSpriteRects[3].w = 32;
		gSpriteRects[3].h = 35;

		gSpriteRects[4].x = 150;
		gSpriteRects[4].y = 10;
		gSpriteRects[4].w = 32;
		gSpriteRects[4].h = 35;

		gSpriteRects[5].x = 186;
		gSpriteRects[5].y = 10;
		gSpriteRects[5].w = 32;
		gSpriteRects[5].h = 35;

		gSpriteRects[6].x = 222;
		gSpriteRects[6].y = 10;
		gSpriteRects[6].w = 32;
		gSpriteRects[6].h = 35;

		gSpriteRects[7].x = 258;
		gSpriteRects[7].y = 10;
		gSpriteRects[7].w = 32;
		gSpriteRects[7].h = 35;

		floorRect.x = 4;
		floorRect.y = 45;
		floorRect.w = 88;
		floorRect.h = 44;
	}

	return success;
}

void close() {
	texture.free();

	SDL_DestroyRenderer(gRenderer);
	gRenderer= NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_Surface* optimizedSurface = NULL;
	if (loadedSurface == NULL) {
		printf("Failed to load image: %s.\nError: %s\n", path.c_str(), IMG_GetError());
	} else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if (optimizedSurface == NULL) {
			printf("Failed to optimize image: %s.\nError: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Failed to load image: %s.\nError: %s\n", path.c_str(), IMG_GetError());
	} else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Failed to render texture from %s.\nError: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char* args[]) {

	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			bool quit = false;
			SDL_Event e;
			int frame = 0;
			backgroundVertOffset = ((SCREEN_HEIGHT - background.getHeight()) / 2);

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						
						if ((e.key.keysym.sym & SDLK_LEFT) == SDLK_LEFT) {
							moveSpeed = -7;
						} else if ((e.key.keysym.sym & SDLK_RIGHT) == SDLK_RIGHT) {
							moveSpeed = 7;
						} else {
							moveSpeed = 0;
						}
						
						if ((e.key.keysym.sym & SDLK_UP) == SDLK_UP) {
							vertSpeed = -1;
						} else if ((e.key.keysym.sym & SDLK_DOWN) == SDLK_DOWN) {
							vertSpeed = 1;
						} else {
							vertSpeed = 0;
						}

					} else {
						moveSpeed = 0;
						vertSpeed = 0;
					}
				}

				floorOffset -= moveSpeed;

				if (floorOffset >= floorRect.w) floorOffset -= floorRect.w;
				if (floorOffset <= -floorRect.w) floorOffset += floorRect.w;

				backgroundHorzOffset -= moveSpeed / 7;

				if (backgroundHorzOffset >= background.getWidth()) backgroundHorzOffset -= background.getWidth();
				if (backgroundHorzOffset <= -background.getWidth()) backgroundHorzOffset += background.getWidth();

				SDL_Rect* rect = &gSpriteRects[frame / STANDING_FRAME_COUNT];
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				backgroundVertOffset -= vertSpeed;

				if (backgroundVertOffset < -(background.getHeight() - SCREEN_HEIGHT)) backgroundVertOffset = -(background.getHeight() - SCREEN_HEIGHT);
				if (backgroundVertOffset > 0) backgroundVertOffset = 0;

				background.render(gRenderer, backgroundHorzOffset, backgroundVertOffset);
				background.render(gRenderer, backgroundHorzOffset + background.getWidth(), backgroundVertOffset);

				for (int floorRepeat = floorOffset - floorRect.w; floorRepeat <= SCREEN_WIDTH; floorRepeat += floorRect.w) {
					stageObjects.render(gRenderer, floorRepeat, SCREEN_HEIGHT - floorRect.h - 40, &floorRect);
				}

				SDL_RenderPresent(gRenderer);

				frame++;

				if (frame / STANDING_FRAME_COUNT >= STANDING_FRAME_COUNT) {
					frame = 0;
				}
			}
		}
	}

	close();

	return 0;
}
