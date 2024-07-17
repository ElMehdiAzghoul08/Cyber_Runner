#include "defs.h"
#include <math.h>

void castRays(SDL_Renderer *renderer, const Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;

        int mapX = (int)player->x;
        int mapY = (int)player->y;

        double sideDistX, sideDistY;
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (player->x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player->y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->y) * deltaDistY;
        }

        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (map[mapY][mapX] > 0)
                hit = 1;
        }

        if (side == 0)
            perpWallDist = (mapX - player->x + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - player->y + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        // Set color based on wall orientation
        if (side == 0)
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Lighter for N/S walls
        else
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Darker for E/W walls

        // Draw the wall slice
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

void renderGroundAndCeiling(SDL_Renderer *renderer)
{
    SDL_Rect ceiling = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_Rect floor = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Blue for ceiling
    SDL_RenderFillRect(renderer, &ceiling);

    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255); // Green for floor
    SDL_RenderFillRect(renderer, &floor);
}

void renderFrame(SDL_Renderer *renderer, const Player *player)
{
    // Render the ground and ceiling
    renderGroundAndCeiling(renderer);

    // Cast rays and render walls
    castRays(renderer, player);

    // Draw minimap if enabled
    if (showMinimap)
    {
        drawMinimap(renderer, player);
    }
}