/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** loop_run_server
*/

#include "../../../include/network.h"
#include "../../../include/protocol.h"
#include "../../../include/config.h"
#include "../../../include/game.h"
#include "../../../include/command.h"
#include <stdbool.h>

static void handle_client_event(server_t *server, int i, game_t *game)
{
    if (server->fds[i].fd < 0)
        return;
    if (server->fds[i].revents & POLLIN)
        handle_client_data(server, i, game);
    if (server->fds[i].revents & (POLLHUP | POLLERR)) {
        disconnect_client(server, i, game);
        remove_player(game, server->clients[i].player->id);
    }
}

static client_t *find_client_by_player_id(server_t *server, int player_id)
{
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        if (server->clients[i].player &&
            server->clients[i].player->id == player_id)
            return &server->clients[i];
    }
    return NULL;
}

static void execute_command(server_t *server, pending_command_t *cmd,
    game_t *game)
{
    player_t *player = get_player_by_id(game, cmd->player_id);
    client_t *client = find_client_by_player_id(server, cmd->player_id);
    command_info_t *info;

    if (!player || !client)
        return;
    info = get_command_info(cmd->type);
    if (info && info->handler)
        info->handler(game, player, cmd->args, client);
}

static void process_ready_commands(server_t *server, game_t *game)
{
    pending_command_t *cmd = get_next_ready_command(game->cmd_q,
        game->freq);

    while (cmd) {
        execute_command(server, cmd, game);
        remove_command_from_queue(game->cmd_q, cmd);
        cmd = get_next_ready_command(game->cmd_q, game->freq);
    }
}

static void process_gui_commands(server_t *server, game_t *game)
{
    gui_command_t *gui_cmd = get_next_gui_command(game->gui_q);

    while (gui_cmd) {
        if (game->gui_client)
            send_message_to_client(game->gui_client, gui_cmd->command);
        remove_gui_command_from_queue(game->gui_q, gui_cmd);
        gui_cmd = get_next_gui_command(game->gui_q);
    }
}

static void send_game_state_msg(server_t *server, game_t *game)
{
    char end_msg[256];

    snprintf(end_msg, sizeof(end_msg), "seg %s\n", game->winning_team);
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        if (server->clients[i].socket_fd >= 0 && server->clients[i].is_gui)
            send_message_to_client(&server->clients[i], end_msg);
    }
}

static void update_game_state(server_t *server, game_t *game)
{
    if (should_update_time(game))
        update_game_time(server, game);
    if (check_winning_condition(game)) {
        game->game_ended = 1;
        announce_winner(game);
        send_game_state_msg(server, game);
    }
}

static void handle_server_loop(server_t *server, int timeout, game_t *game)
{
    int poll_result = poll(server->fds, MAX_CONNECTIONS, timeout);

    if (poll_result < 0) {
        perror("poll failed");
        return;
    }
    if (server->fds[0].revents & POLLIN)
        accept_new_client(server);
    for (int i = 1; i < MAX_CONNECTIONS; i++)
        handle_client_event(server, i, game);
    process_ready_commands(server, game);
    process_gui_commands(server, game);
    update_game_state(server, game);
    handle_dead_players(server, game);
}

int run_server(server_t *server, server_config_t *config)
{
    int timeout = 1000 / config->freq;
    game_t game;
    sig_atomic_t *running = get_or_set_running(NULL);

    if (init_game(&game, server, config) < 0) {
        fprintf(stderr, "Failed to initialize game\n");
        return -1;
    }
    printf("Server running on port %d...\n", config->port);
    while (!game.game_ended && *running) {
        handle_server_loop(server, timeout, &game);
        running = get_or_set_running(NULL);
    }
    cleanup_command_queue(game.cmd_q);
    cleanup_game(&game);
    return 0;
}
