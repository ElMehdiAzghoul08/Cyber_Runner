#include "defs.h"
#include <math.h>

#define FOV 60
#define HALF_FOV 30
#define ANGLE_STEP (FOV / SCREEN_WIDTH)
#define MAX_DEPTH 10
#define MINIMAP_SCALE 4

void drawMinimap(SDL_Renderer *renderer, const Player *player)
{
    int mapX, mapY, screenX, screenY;
    SDL_Rect mapRect;

    for (mapY = 0; mapY < MAP_HEIGHT; mapY++)
    {
        for (mapX = 0; mapX < MAP_WIDTH; mapX++)
        {
            screenX = mapX * MINIMAP_SCALE;
            screenY = mapY * MINIMAP_SCALE;
            mapRect.x = screenX;
            mapRect.y = screenY;
            mapRect.w = MINIMAP_SCALE;
            mapRect.h = MINIMAP_SCALE;

            if (map[mapX][mapY] == 0)
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            else
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

            SDL_RenderFillRect(renderer, &mapRect);
        }
    }

    // Draw player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    mapRect.x = player->x * MINIMAP_SCALE - 2;
    mapRect.y = player->y * MINIMAP_SCALE - 2;
    mapRect.w = 4;
    mapRect.h = 4;
    SDL_RenderFillRect(renderer, &mapRect);

    // Drawing player's line of sight
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer,
                       player->x * MINIMAP_SCALE,
                       player->y * MINIMAP_SCALE,
                       (player->x + player->dirX * 5) * MINIMAP_SCALE,
                       (player->y + player->dirY * 5) * MINIMAP_SCALE);
}

void setupPlayer(Player *player)
{
    player->x = 22.0;
    player->y = 12.0;
    player->dirX = -1.0;
    player->dirY = 0.0;
    player->planeX = 0.0;
    player->planeY = 0.66;
}

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
            if (map[mapX][mapY] > 0)
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

        SDL_Color color;
        switch (map[mapX][mapY])
        {
        case 1:
            color = (SDL_Color){255, 0, 0, 255};
            break; // red
        case 2:
            color = (SDL_Color){0, 255, 0, 255};
            break; // green
        case 3:
            color = (SDL_Color){0, 0, 255, 255};
            break; // blue
        case 4:
            color = (SDL_Color){255, 255, 255, 255};
            break; // white
        default:
            color = (SDL_Color){255, 255, 0, 255};
            break; // yellow
        }

        if (side == 1)
        {
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}