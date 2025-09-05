/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** msz
*/

#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_gui_tna(client_t *client, game_t *game)
{
    char response[64];

    if (!client)
        return -1;
    for (int i = 0; i < game->team_count; i++) {
        snprintf(response, sizeof(response), "tna %s\n", game->teams[i].name);
        add_gui_command_to_queue(game->gui_q, response);
    }
    return 0;
}

int handle_gui_pnw(client_t *client, game_t *game, player_t *ply)
{
    char response[256];

    if (!client || !ply)
        return -1;
    printf("Sending player %d to GUI\n", ply->id);
    snprintf(response, sizeof(response), "pnw %d %d %d %d %d %s\n",
        ply->id, ply->pos.x, ply->pos.y,
        ply->orientation, ply->level, ply->team_name);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_ppo(client_t *client, game_t *game, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "ppo %d %d %d %d\n",
        ply->id, ply->pos.x, ply->pos.y, ply->orientation);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_plv(client_t *client, game_t *game, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "plv %d %d\n",
        ply->id, ply->level);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pin(client_t *client, game_t *game, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response),
        "pin %d %d %d %d %d %d %d %d %d %d\n",
        ply->id, ply->pos.x, ply->pos.y,
        ply->inventory.food, ply->inventory.linemate,
        ply->inventory.deraumere, ply->inventory.sibur,
        ply->inventory.mendiane, ply->inventory.phiras,
        ply->inventory.thystame);
    return add_gui_command_to_queue(game->gui_q, response);
}
