/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** incantation_update
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static void notify_player_incantation_complete(server_t *server,
    game_t *game, player_t *ply)
{
    char response[128];
    client_t *client = get_client_by_player_id(server, ply->id);

    if (!game || !ply)
        return;
    ply->is_in_incantation = 0;
    snprintf(response, sizeof(response),
        "Current level: %d\n", ply->level);
    send_response(client, response);
}

static void complete_incantation(server_t *server, game_t *game,
    incantation_t *incantation)
{
    player_t *player;

    if (!can_start_incantation(game, incantation->pos, incantation->level)) {
        printf("Incantation failed: conditions not met at completion\n");
        return;
    }
    consume_resources(game, incantation->pos, incantation->level);
    for (int i = 0; i < incantation->player_count; i++) {
        player = get_player_by_id(game, incantation->player_ids[i]);
        if (player && player->pos.x == incantation->pos.x &&
            player->pos.y == incantation->pos.y &&
            player->level == incantation->level) {
            player->is_in_incantation = 0;
            player->level++;
            notify_player_incantation_complete(server, game, player);
            handle_gui_plv(game->gui_client, game, player);
            handle_gui_pie(game, game->gui_client, incantation);
            printf("Player %d elevated to level %d\n", player->id,
                player->level);
        }
    }
}

static void update_incantation_free(incantation_t *incantation)
{
    if (incantation->player_ids)
        free(incantation->player_ids);
}

void update_incantations(server_t *server, game_t *game)
{
    incantation_t **current = &game->incantations;
    incantation_t *incantation;

    while (*current) {
        incantation = *current;
        if (game->time_unit >= incantation->start_time + TIME_INCANTATION) {
            complete_incantation(server, game, incantation);
            *current = incantation->next;
            update_incantation_free(incantation);
            free(incantation);
        } else
            current = &(*current)->next;
    }
}
