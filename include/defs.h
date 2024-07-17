#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

// Screen dimensions
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Map dimensions
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Raycasting constants
#define FOV 60
#define HALF_FOV 30
#define ANGLE_STEP (FOV / SCREEN_WIDTH)
#define MAX_DEPTH 10

// Minimap constants
#define MINIMAP_SCALE 4
#define MINIMAP_WIDTH (MAP_WIDTH * MINIMAP_SCALE)
#define MINIMAP_HEIGHT (MAP_HEIGHT * MINIMAP_SCALE)
#define MINIMAP_POSITION_X 10
#define MINIMAP_POSITION_Y 10

// #define TEXTURE_WIDTH 64
// #define TEXTURE_HEIGHT 64

// Map data
extern int map[MAP_HEIGHT][MAP_WIDTH];

// Minimap toggle
extern SDL_bool showMinimap;
// texture
// extern SDL_Texture *wallTexture;
// Player structure
typedef struct
{
    double x;
    double y;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} Player;

// Function prototypes

// Main
SDL_Window *initializeWindow(void);
SDL_Renderer *createRenderer(SDL_Window *window);

// Player
void setupPlayer(Player *player);
void movePlayer(Player *player, double moveX, double moveY);

// Render
void castRays(SDL_Renderer *renderer, const Player *player);
void renderFrame(SDL_Renderer *renderer, const Player *player);
void renderGroundAndCeiling(SDL_Renderer *renderer);

// Map
void loadMap(void);

// Minimap
void toggleMinimap(void);
void drawMinimap(SDL_Renderer *renderer, const Player *player);

// Texture
// void loadTextures(SDL_Renderer *renderer);
// void freeTextures(void);
#endif // DEFS_H