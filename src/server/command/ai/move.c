/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** move
*/

#include "../../../../include/common.h"
#include "../../../../include/protocol.h"
#include "../../../../include/config.h"
#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_forward(game_t *game, player_t *pla, const char *ag, client_t *cli)
{
    (void)ag;
    if (!game || !pla || !cli)
        return send_response(cli, RESPONSE_KO);
    move_player_forward(game, pla);
    handle_gui_ppo(game->gui_client, game, pla);
    return send_response(cli, RESPONSE_OK);
}

int handle_right(game_t *game, player_t *pla, const char *args, client_t *cli)
{
    (void)game;
    (void)args;
    if (!pla || !cli)
        return send_response(cli, RESPONSE_KO);
    turn_player_right(pla);
    handle_gui_ppo(game->gui_client, game, pla);
    return send_response(cli, RESPONSE_OK);
}

int handle_left(game_t *game, player_t *pla, const char *args, client_t *cli)
{
    (void)game;
    (void)args;
    if (!pla || !cli)
        return send_response(cli, RESPONSE_KO);
    turn_player_left(pla);
    handle_gui_ppo(game->gui_client, game, pla);
    return send_response(cli, RESPONSE_OK);
}
