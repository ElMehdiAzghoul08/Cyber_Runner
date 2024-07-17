#include "defs.h"

SDL_bool showMinimap = SDL_TRUE;

void toggleMinimap(void)
{
    showMinimap = !showMinimap;
}

void drawMinimap(SDL_Renderer *renderer, const Player *player)
{
    if (!showMinimap)
        return;

    // Draw minimap background
    SDL_Rect minimapRect = {MINIMAP_POSITION_X, MINIMAP_POSITION_Y, MINIMAP_WIDTH, MINIMAP_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Semi-transparent black
    SDL_RenderFillRect(renderer, &minimapRect);

    int mapX, mapY, screenX, screenY;
    SDL_Rect mapRect;

    for (mapY = 0; mapY < MAP_HEIGHT; mapY++)
    {
        for (mapX = 0; mapX < MAP_WIDTH; mapX++)
        {
            screenX = MINIMAP_POSITION_X + mapX * MINIMAP_SCALE;
            screenY = MINIMAP_POSITION_Y + mapY * MINIMAP_SCALE;
            mapRect.x = screenX;
            mapRect.y = screenY;
            mapRect.w = MINIMAP_SCALE;
            mapRect.h = MINIMAP_SCALE;

            if (map[mapY][mapX] == 0)
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            else
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

            SDL_RenderFillRect(renderer, &mapRect);
        }
    }

    // Draw player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    mapRect.x = MINIMAP_POSITION_X + player->x * MINIMAP_SCALE - 2;
    mapRect.y = MINIMAP_POSITION_Y + player->y * MINIMAP_SCALE - 2;
    mapRect.w = 4;
    mapRect.h = 4;
    SDL_RenderFillRect(renderer, &mapRect);

    // Drawing player's line of sight
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer,
                       MINIMAP_POSITION_X + player->x * MINIMAP_SCALE,
                       MINIMAP_POSITION_Y + player->y * MINIMAP_SCALE,
                       MINIMAP_POSITION_X + (player->x + player->dirX * 2) * MINIMAP_SCALE,
                       MINIMAP_POSITION_Y + (player->y + player->dirY * 2) * MINIMAP_SCALE);

    // Draw minimap border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
    SDL_RenderDrawRect(renderer, &minimapRect);
}