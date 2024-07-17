#include "defs.h"

SDL_Texture *wallTexture = NULL;

void loadTextures(SDL_Renderer *renderer)
{
    const char *textureFile = "textures/mossy.png";

    SDL_Surface *surface = IMG_Load(textureFile);
    if (surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", textureFile, IMG_GetError());
        return;
    }

    wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (wallTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", textureFile, SDL_GetError());
    }
}

void freeTextures()
{
    if (wallTexture != NULL)
    {
        SDL_DestroyTexture(wallTexture);
        wallTexture = NULL;
    }
}