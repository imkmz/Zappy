/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** msz
*/

#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_gui_msz(game_t *game, client_t *client)
{
    char response[64];

    if (!game || !client)
        return -1;
    snprintf(response, sizeof(response), "msz %d %d\n",
        game->map.width, game->map.height);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_bct(client_t *client, game_t *game, int x, int y)
{
    char response[128];
    inventory_t *tile;

    if (!client || x < 0 || x >= game->map.width || y < 0 ||
        y >= game->map.height)
        return 0;
    tile = &game->map.tiles[y][x];
    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile->food, tile->linemate, tile->deraumere,
        tile->sibur, tile->mendiane, tile->phiras, tile->thystame);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_mct(client_t *client, game_t *game)
{
    if (!client || !game)
        return -1;
    for (int y = 0; y < game->map.height; y++)
        for (int x = 0; x < game->map.width; x++) {
            handle_gui_bct(client, game, x, y);
        }
    return 0;
}
