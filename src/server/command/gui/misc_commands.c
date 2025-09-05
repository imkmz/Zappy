/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** msz
*/

#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_gui_sgt(game_t *game, client_t *client, int time_unit)
{
    char response[64];

    if (!client || time_unit <= 0)
        return -1;
    snprintf(response, sizeof(response), "sgt %d\n", time_unit);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_seg(game_t *game, client_t *client, char *winner)
{
    char response[64];

    if (!client || !winner)
        return -1;
    snprintf(response, sizeof(response), "seg %s\n", winner);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_smg(game_t *game, client_t *client, char *msg)
{
    char response[64];

    if (!client || !msg)
        return -1;
    snprintf(response, sizeof(response), "smg %s\n", msg);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pgt(game_t *game, client_t *client, player_t *ply, int i)
{
    char response[64];

    if (!client)
        return -1;
    snprintf(response, sizeof(response), "pgt %d %d\n", ply->id, i);
    return add_gui_command_to_queue(game->gui_q, response);
}

int handle_gui_pdi(game_t *game, client_t *client, player_t *ply)
{
    char response[64];

    if (!client || !ply)
        return -1;
    snprintf(response, sizeof(response), "pdi %d\n", ply->id);
    return add_gui_command_to_queue(game->gui_q, response);
}
