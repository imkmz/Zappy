/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** msz
*/

#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_gui_pex(game_t *game, client_t *client, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "pex %d\n", ply->id);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pic(game_t *game, client_t *client, player_t **players)
{
    char response[64];
    char *cur = response;

    if (!client || !players || !players[0])
        return -1;
    cur += snprintf(cur, sizeof(response) - (cur - response), "pic %d %d %d",
        players[0]->pos.x, players[0]->pos.y, players[0]->level);
    for (int i = 0; players[i]; i++)
        cur += snprintf(cur, sizeof(response) - (cur - response), " %d",
            players[i]->id);
    cur += snprintf(cur, sizeof(response) - (cur - response), "\n");
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pie(game_t *game, client_t *client, incantation_t *incantation)
{
    char response[64];

    if (!client || !incantation)
        return -1;
    snprintf(response, sizeof(response), "pie %d %d %d\n",
        incantation->pos.x, incantation->pos.y, 1);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pfk(game_t *game, client_t *client, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "pfk %d\n", ply->id);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pdr(game_t *game, client_t *client, player_t *ply, int i)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "pdr %d %d\n", ply->id, i);
    return add_gui_command_to_queue(game->gui_q, response);
}
