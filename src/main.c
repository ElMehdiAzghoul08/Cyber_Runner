#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "defs.h"

SDL_bool showMinimap = SDL_TRUE;

void movePlayer(Player *player, double moveX, double moveY)
{
    double newX = player->x + moveX;
    double newY = player->y + moveY;

    // Check X-axis collision
    if (map[(int)(player->y)][(int)(newX)] == 0)
    {
        player->x = newX;
    }

    // Check Y-axis collision
    if (map[(int)(newY)][(int)(player->x)] == 0)
    {
        player->y = newY;
    }
}

SDL_Window *initializeWindow(void)
{
    SDL_Window *window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    window = SDL_CreateWindow(
        "Maze Project",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    return window;
}

SDL_Renderer *createRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }
    return renderer;
}

int main(int argc, char *args[])
{
    SDL_Window *window = initializeWindow();
    if (window == NULL)
    {
        return 1;
    }

    SDL_Renderer *renderer = createRenderer(window);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    loadMap(); // Load the map from map.c

    SDL_bool quit = SDL_FALSE;
    SDL_Event e;

    Player player;
    setupPlayer(&player);

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    double moveSpeed = 0.05;
    double rotSpeed = 0.03;

    Uint32 frameStart, frameTime;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while (!quit)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = SDL_TRUE;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m)
            {
                showMinimap = !showMinimap;
            }
        }

        // Keyboard input handling
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
        {
            movePlayer(&player, player.dirX * moveSpeed, player.dirY * moveSpeed);
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        {
            movePlayer(&player, -player.dirX * moveSpeed, -player.dirY * moveSpeed);
        }
        if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
        {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
            player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(-rotSpeed) - player.planeY * sin(-rotSpeed);
            player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY * cos(-rotSpeed);
        }
        if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
        {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(rotSpeed) - player.dirY * sin(rotSpeed);
            player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(rotSpeed) - player.planeY * sin(rotSpeed);
            player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY * cos(rotSpeed);
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Perform raycasting
        castRays(renderer, &player);

        // Draw minimap if enabled
        if (showMinimap)
        {
            drawMinimap(renderer, &player);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Cap the frame rate
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}