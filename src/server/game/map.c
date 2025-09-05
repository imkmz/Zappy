/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** map
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/game.h"

void init_map_failed(int y, map_t *map)
{
    for (int i = 0; i < y; i++)
        free(map->tiles[i]);
}

int init_map(map_t *map, int width, int height)
{
    map->width = width;
    map->height = height;
    map->tiles = malloc(height * sizeof(inventory_t *));
    if (!map->tiles)
        return -1;
    for (int y = 0; y < height; y++) {
        map->tiles[y] = malloc(width * sizeof(inventory_t));
        if (!map->tiles[y]) {
            init_map_failed(y, map);
            free(map->tiles);
            return -1;
        }
        for (int x = 0; x < width; x++)
            memset(&map->tiles[y][x], 0, sizeof(inventory_t));
    }
    return 0;
}

void clean_map_tiles(map_t *map)
{
    for (int y = 0; y < map->height; y++) {
        if (map->tiles[y])
            free(map->tiles[y]);
    }
}

static void free_map_tiles(int y, map_t *map)
{
    if (map->tiles[y]) {
        free(map->tiles[y]);
        map->tiles[y] = NULL;
    }
}

void cleanup_map(map_t *map)
{
    if (!map)
        return;
    if (map->tiles) {
        for (int y = 0; y < map->height; y++) {
            free_map_tiles(y, map);
        }
        free(map->tiles);
        map->tiles = NULL;
    }
    map->width = 0;
    map->height = 0;
}
