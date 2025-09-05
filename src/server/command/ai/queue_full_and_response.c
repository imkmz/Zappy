/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** queue_full_and_response
*/

#include "../../../../include/command.h"

int is_queue_full(command_queue_t *queue)
{
    if (!queue)
        return 1;
    return queue->count >= MAX_PENDING_COMMANDS;
}

client_t *get_client_by_player_id(server_t *server, int player_id)
{
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        if (server->clients[i].player &&
            server->clients[i].player->id == player_id) {
            return &server->clients[i];
        }
    }
    return NULL;
}

int send_response(client_t *client, const char *response)
{
    if (!client || !response)
        return -1;
    return send_message_to_client(client, response);
}
