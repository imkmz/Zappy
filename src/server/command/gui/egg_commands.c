/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** msz
*/

#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_gui_enw(game_t *game, client_t *client, player_t *ply, egg_t *egg)
{
    char response[64];
    int id = 0;

    if (!client || !egg)
        return -1;
    if (ply != NULL)
        id = ply->id;
    snprintf(response, sizeof(response), "enw %d %d %d %d\n",
        egg->id, id, egg->pos.x, egg->pos.y);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_ebo(game_t *game, client_t *client, egg_t *egg)
{
    char response[64];

    if (!client || !egg)
        return -1;
    snprintf(response, sizeof(response), "ebo %d\n", egg->id);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_edi(game_t *game, client_t *client, egg_t *egg)
{
    char response[64];

    if (!client || !egg)
        return -1;
    snprintf(response, sizeof(response), "edi %d\n", egg->id);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pbc(game_t *game, client_t *client, int id, const char *msg)
{
    char response[64];

    if (!client || id < 0)
        return -1;
    snprintf(response, sizeof(response), "pbc %d %s\n", id, msg);
    return add_gui_command_to_queue(game->gui_q, response);
}
