/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** loop_handle_gui_cmd
*/

#include "../../../include/network.h"
#include "../../../include/protocol.h"
#include "../../../include/config.h"
#include "../../../include/game.h"
#include "../../../include/command.h"

static void notify_gui_of_death(server_t *server, int player_id)
{
    char death_msg[256];

    snprintf(death_msg, sizeof(death_msg), "pdi %d\n", player_id);
    for (int k = 1; k < MAX_CONNECTIONS; k++) {
        if (server->clients[k].socket_fd >= 0 && server->clients[k].is_gui)
            send_message_to_client(&server->clients[k], death_msg);
    }
}

static void handle_client_disconnection(server_t *server, int player_id)
{
    for (int j = 1; j < MAX_CONNECTIONS; j++) {
        if (server->clients[j].player &&
            server->clients[j].player->id == player_id) {
            notify_gui_of_death(server, player_id);
            send_message_to_client(&server->clients[j], RESPONSE_DEAD);
            server->clients[j].player = NULL;
            break;
        }
    }
}

void handle_dead_players(server_t *server, game_t *game)
{
    for (int i = 0; i < game->player_count; i++) {
        if (game->players[i].dead) {
            handle_client_disconnection(server, game->players[i].id);
            handle_player_death(game, game->players[i].id);
        }
    }
}
