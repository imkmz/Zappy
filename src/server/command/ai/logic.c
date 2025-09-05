/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** logic
*/

#include "../../../../include/common.h"
#include "../../../../include/protocol.h"
#include "../../../../include/config.h"
#include "../../../../include/game.h"
#include "../../../../include/network.h"
#include "../../../../include/command.h"

int handle_eject(game_t *ga, player_t *pla, const char *ag, client_t *cli)
{
    int players_at_pos;

    (void)ag;
    if (!ga || !pla || !cli)
        return send_response(cli, RESPONSE_KO);
    players_at_pos = count_players_at_position(ga, pla->pos);
    if (players_at_pos <= 1)
        return send_response(cli, RESPONSE_KO);
    eject_players(ga, pla);
    return send_response(cli, RESPONSE_OK);
}

int get_resource_nb(const char *name)
{
    if (strcmp(name, "food") == 0)
        return 1;
    if (strcmp(name, "linemate") == 0)
        return 2;
    if (strcmp(name, "deraumere") == 0)
        return 3;
    if (strcmp(name, "sibur") == 0)
        return 4;
    if (strcmp(name, "mendiane") == 0)
        return 5;
    if (strcmp(name, "phiras") == 0)
        return 6;
    if (strcmp(name, "thystame") == 0)
        return 7;
    return -1;
}

int handle_take(game_t *ga, player_t *pla, const char *ag, client_t *cli)
{
    char *object_name;
    int result;

    if (!ga || !pla || !cli || !ag)
        return send_response(cli, RESPONSE_KO);
    object_name = trim_whitespace((char *)ag);
    if (strlen(object_name) == 0)
        return send_response(cli, RESPONSE_KO);
    if (!is_valid(object_name))
        return send_response(cli, RESPONSE_KO);
    result = take_object(ga, pla, object_name);
    if (result != 0) {
        handle_gui_pgt(ga, ga->gui_client, pla, get_resource_nb(object_name));
        return send_response(cli, RESPONSE_OK);
    } else
        return send_response(cli, RESPONSE_KO);
}

int handle_set(game_t *ga, player_t *pla, const char *ag, client_t *cli)
{
    char *object_name;
    int result;

    if (!ga || !pla || !cli || !ag)
        return send_response(cli, RESPONSE_KO);
    object_name = trim_whitespace((char *)ag);
    if (strlen(object_name) == 0)
        return send_response(cli, RESPONSE_KO);
    if (!is_valid(object_name))
        return send_response(cli, RESPONSE_KO);
    result = set_object(ga, pla, object_name);
    if (result != 0) {
        handle_gui_pdr(ga, ga->gui_client, pla, get_resource_nb(object_name));
        return send_response(cli, RESPONSE_OK);
    } else
        return send_response(cli, RESPONSE_KO);
}

static int notify_players_for_incantation(game_t *ga,
    incantation_t *incantation)
{
    player_t *p;
    char *response = "Elevation underway\n";

    for (int i = 0; i < incantation->player_count; i++) {
        p = get_player_by_id(ga, incantation->player_ids[i]);
        if (p)
            send_response(get_client_by_player_id(ga->server, p->id),
                response);
    }
    return sizeof(response);
}

int handle_incantation(game_t *ga, player_t *pla, const char *ag,
    client_t *cli)
{
    incantation_t *incantation;

    (void)ag;
    if (!ga || !pla || !cli)
        return send_response(cli, RESPONSE_KO);
    if (!can_start_incantation(ga, pla->pos, pla->level))
        return send_response(cli, RESPONSE_KO);
    incantation = start_incantation(ga, pla->pos, pla->level);
    if (!incantation)
        return send_response(cli, RESPONSE_KO);
    return notify_players_for_incantation(ga, incantation);
}
