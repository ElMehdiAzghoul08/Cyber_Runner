#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Map-related function declarations and structures
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Defining the map using integers for simplicity
extern int map[MAP_HEIGHT][MAP_WIDTH];

// Player structure
typedef struct
{
    double x;
    double y;
    double angle;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} Player;

// Function prototypes
SDL_Window *initializeWindow(void);
SDL_Renderer *createRenderer(SDL_Window *window);
void setupPlayer(Player *player);
void castRays(SDL_Renderer *renderer, const Player *player);
void loadMap(void);
void drawMinimap(SDL_Renderer *renderer, const Player *player);

#endif // DEFS_H