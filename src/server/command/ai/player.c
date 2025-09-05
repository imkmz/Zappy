/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player
*/

#include "../../../../include/common.h"
#include "../../../../include/protocol.h"
#include "../../../../include/config.h"
#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_look(game_t *game, player_t *player, const char *args,
    client_t *client)
{
    char *vision_result;
    int result;

    (void)args;
    if (!game || !player || !client)
        return send_response(client, RESPONSE_KO);
    vision_result = get_player_vision(game, player);
    if (!vision_result)
        return send_response(client, RESPONSE_KO);
    result = send_response(client, vision_result);
    free(vision_result);
    return result;
}

int handle_inventory(game_t *game, player_t *player, const char *args,
    client_t *client)
{
    char *inventory_str;
    int result;

    (void)game;
    (void)args;
    if (!player || !client)
        return send_response(client, RESPONSE_KO);
    inventory_str = format_inventory(player);
    if (!inventory_str)
        return send_response(client, RESPONSE_KO);
    result = send_response(client, inventory_str);
    free(inventory_str);
    return result;
}

int handle_connect_nbr(game_t *game, player_t *player,
    const char *args, client_t *client)
{
    team_t *team;
    char response[64];

    (void)args;
    if (!game || !player || !client)
        return send_response(client, RESPONSE_KO);
    team = get_team_by_name(game, player->team_name);
    if (!team)
        return send_response(client, RESPONSE_KO);
    snprintf(response, sizeof(response), "%d\n", team->available_slots);
    return send_response(client, response);
}

int handle_broadcast(game_t *game, player_t *player,
    const char *args, client_t *client)
{
    if (!game || !player || !client || !args)
        return send_response(client, RESPONSE_KO);
    if (strlen(args) == 0)
        return send_response(client, RESPONSE_KO);
    cmd_broadcast(game, player, args);
    return 0;
}

int handle_fork(game_t *game, player_t *player,
    const char *args, client_t *client)
{
    egg_t *new_egg;

    (void)args;
    if (!game || !player || !client)
        return send_response(client, RESPONSE_KO);
    new_egg = create_egg(game, player->team_name, player->pos, 0);
    if (!new_egg)
        return send_response(client, RESPONSE_KO);
    handle_gui_pfk(game, game->gui_client, player);
    handle_gui_enw(game, game->gui_client, player, new_egg);
    return send_response(client, RESPONSE_OK);
}
