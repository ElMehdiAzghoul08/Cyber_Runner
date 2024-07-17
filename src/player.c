#include "defs.h"

void setupPlayer(Player *player)
{
    player->x = 22.0;
    player->y = 12.0;
    player->dirX = -1.0;
    player->dirY = 0.0;
    player->planeX = 0.0;
    player->planeY = 0.66;
}

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