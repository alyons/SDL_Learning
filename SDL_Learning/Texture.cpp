#pragma once
#include "Texture.h"
#include <stdio.h>
#include <SDL_image.h>

Texture::Texture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(std::string path, SDL_Renderer* renderer) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Failed to load texture: %s\nError: %s\n", path.c_str(), IMG_GetError());
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		} else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void Texture::free() {
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y) {
	render(renderer, x, y, NULL);
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

int Texture::getWidth() {
	return mWidth;
}

int Texture::getHeight() {
	return mHeight;
}