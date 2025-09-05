/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** resources
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"
#include "../../../include/command.h"

static int calculate_resource_quantity(int width, int height, double density)
{
    int quantity = (int)(width * height * density);

    return quantity > 0 ? quantity : 1;
}

static int count_total_quantity(int r_type, inventory_t *tile)
{
    int total = 0;

    if (r_type == 0)
        total += tile->food;
    if (r_type == 1)
        total += tile->linemate;
    if (r_type == 2)
        total += tile->deraumere;
    if (r_type == 3)
        total += tile->sibur;
    if (r_type == 4)
        total += tile->mendiane;
    if (r_type == 5)
        total += tile->phiras;
    if (r_type == 6)
        total += tile->thystame;
    return total;
}

static int count_resource_on_map(game_t *game, int resource_type)
{
    int total = 0;
    inventory_t *tile;

    for (int y = 0; y < game->map.height; y++) {
        for (int x = 0; x < game->map.width; x++) {
            tile = &game->map.tiles[y][x];
            total += count_total_quantity(resource_type, tile);
        }
    }
    return total;
}

static void increment_resource(inventory_t *tile, int resource_type)
{
    if (resource_type == 0)
        tile->food++;
    if (resource_type == 1)
        tile->linemate++;
    if (resource_type == 2)
        tile->deraumere++;
    if (resource_type == 3)
        tile->sibur++;
    if (resource_type == 4)
        tile->mendiane++;
    if (resource_type == 5)
        tile->phiras++;
    if (resource_type == 6)
        tile->thystame++;
}

static void spawn_resource_on_map(game_t *game, int resource_type,
    int quantity)
{
    int x;
    int y;

    for (int i = 0; i < quantity; i++) {
        x = rand() % game->map.width;
        y = rand() % game->map.height;
        increment_resource(&game->map.tiles[y][x], resource_type);
        handle_gui_bct(game->gui_client, game, x, y);
    }
}

static void resources_to_spawn(int i, int current_quantity,
    int required_quantity, game_t *game)
{
    int to_spawn = 0;

    if (current_quantity < required_quantity) {
        to_spawn = required_quantity - current_quantity;
        spawn_resource_on_map(game, i, to_spawn);
        printf("Spawned %d resources of type %d (had %d, needed %d)\n",
            to_spawn, i, current_quantity, required_quantity);
    }
}

void spawn_resources(game_t *game)
{
    int required_quantity;
    int current_quantity;
    double densities[] = {
        DENSITY_FOOD, DENSITY_LINEMATE, DENSITY_DERAUMERE,
        DENSITY_SIBUR, DENSITY_MENDIANE, DENSITY_PHIRAS, DENSITY_THYSTAME
    };

    if (!game || !game->map.tiles) {
        fprintf(stderr, "Error: Invalid game or map in spawn_resources\n");
        return;
    }
    for (int i = 0; i < 7; i++) {
        required_quantity = calculate_resource_quantity(game->map.width,
            game->map.height, densities[i]);
        current_quantity = count_resource_on_map(game, i);
        resources_to_spawn(i, current_quantity, required_quantity, game);
    }
}

void initial_spawn_resources(game_t *game)
{
    int width;
    int height;
    int quantity;
    double densities[] = {
        DENSITY_FOOD, DENSITY_LINEMATE, DENSITY_DERAUMERE,
        DENSITY_SIBUR, DENSITY_MENDIANE, DENSITY_PHIRAS, DENSITY_THYSTAME
    };

    if (!game || !game->map.tiles) {
        fprintf(stderr,
            "Error: Invalid game or map in initial_spawn_resources\n");
        return;
    }
    width = game->map.width;
    height = game->map.height;
    for (int i = 0; i < 7; i++) {
        quantity = calculate_resource_quantity(width, height, densities[i]);
        spawn_resource_on_map(game, i, quantity);
    }
    printf("Initial resources spawned on map\n");
}

int should_spawn_resources(game_t *game)
{
    if (!game)
        return 0;
    return (game->time_unit > 0) && (game->time_unit % 20) == 0;
}

void update_resources(game_t *game)
{
    if (should_spawn_resources(game)) {
        spawn_resources(game);
    }
}
