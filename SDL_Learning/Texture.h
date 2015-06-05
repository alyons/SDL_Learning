#pragma once
#include <SDL.h>
#include <string>

class Texture
{
	public:
		Texture();
		virtual ~Texture();
		bool loadFromFile(std::string path, SDL_Renderer* renderer);
		void free();
		void render(SDL_Renderer* renderer, int x, int y);
		void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);
		int getWidth();
		int getHeight();
	private:
		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};

