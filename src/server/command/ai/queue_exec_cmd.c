/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** main server
*/

#include "../../../../include/command.h"
#include <stdbool.h>

static bool is_command_invalid(command_queue_t *queue,
    pending_command_t *cmd, player_t *player, client_t *client)
{
    command_info_t *cmd_info = get_command_info(cmd->type);

    if (!cmd_info || !cmd_info->handler) {
        send_response(client, RESPONSE_KO);
        remove_command_from_queue(queue, cmd);
        return true;
    }
    if (player->dead) {
        send_response(client, RESPONSE_DEAD);
        remove_command_from_queue(queue, cmd);
        return true;
    }
    return false;
}

static int loop_on_command(game_t *game, command_queue_t *queue,
    pending_command_t *cmd, server_t *server)
{
    player_t *player;
    client_t *client;

    player = get_player_by_id(game, cmd->player_id);
    client = get_client_by_player_id(server, cmd->player_id);
    if (!player || !client) {
        remove_command_from_queue(queue, cmd);
        return 1;
    }
    if (is_player_in_incantation(game, player->id) ||
        player->is_in_incantation)
        return 1;
    if (is_command_invalid(queue, cmd, player, client))
        return 1;
    get_command_info(cmd->type)->handler(game, player, cmd->args, client);
    remove_command_from_queue(queue, cmd);
    return 0;
}

void execute_ready_commands(game_t *game, command_queue_t *queue,
    server_t *server)
{
    pending_command_t *cmd;

    if (!game || !queue || !server)
        return;
    cmd = get_next_ready_command(queue, game->freq);
    while (cmd != NULL) {
        loop_on_command(game, queue, cmd, server);
        cmd = get_next_ready_command(queue, game->freq);
    }
}
