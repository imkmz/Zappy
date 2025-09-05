/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** loop
*/

#include "../../../include/network.h"
#include "../../../include/protocol.h"
#include "../../../include/config.h"
#include "../../../include/game.h"
#include "../../../include/command.h"
#include <stdbool.h>

static bool is_team_valid(team_t *team, client_t *client)
{
    if (!team) {
        send_message_to_client(client, RESPONSE_KO);
        return false;
    }
    if (team->available_slots <= 0) {
        send_message_to_client(client, "0\n");
        return false;
    }
    return true;
}

static int handle_team_authentication(server_t *server, int client_index,
    const char *team_name, game_t *game)
{
    client_t *client = &server->clients[client_index];
    team_t *team = get_team_by_name(game, team_name);
    char response[256];
    int player_id = 0;

    if (!is_team_valid(team, client))
        return -1;
    player_id = create_player(game, team_name, client->socket_fd);
    if (player_id < 0) {
        send_message_to_client(client, RESPONSE_KO);
        return -1;
    }
    client->player = get_player_by_id(game, player_id);
    snprintf(response, sizeof(response), "%d\n", team->available_slots);
    send_message_to_client(client, response);
    snprintf(response, sizeof(response), "%d %d\n", game->map.width,
        game->map.height);
    send_message_to_client(client, response);
    printf("Player %d joined team %s\n", player_id, team_name);
    return 0;
}

static int cannot_handle_player_cmd(player_t *player, client_t *client,
    game_t *game)
{
    if (!player) {
        send_message_to_client(client, RESPONSE_KO);
        return -1;
    }
    if (player->is_in_incantation)
        return 0;
    if (is_queue_full(game->cmd_q))
        return 0;
    return 2;
}

static player_t **get_players_on_pos(game_t *game, position_t pos)
{
    player_t **players = malloc(MAX_PLAYERS * sizeof(player_t *));
    int index = 0;

    if (!players)
        return NULL;
    for (int i = 0; i < game->player_count; i++) {
        if (game->players[i].id < 0)
            continue;
        if (game->players[i].pos.x == pos.x &&
            game->players[i].pos.y == pos.y) {
            players[index] = &game->players[i];
            index++;
        }
    }
    players[index] = NULL;
    return players;
}

static void send_anim_data(command_type_t cmd, game_t *game, player_t *ply,
    const char *args)
{
    player_t **ply_on_pos = get_players_on_pos(game, ply->pos);

    switch (cmd) {
        case CMD_EJECT:
            handle_gui_pex(game, game->gui_client, ply);
            break;
        case CMD_INCANTATION:
            handle_gui_pic(game, game->gui_client, ply_on_pos);
            break;
        default:
            break;
    }
    free(ply_on_pos);
}

static int handle_player_command(server_t *server, int client_index,
    const char *command_str, game_t *game)
{
    client_t *client = &server->clients[client_index];
    player_t *player = client->player;
    command_type_t cmd_type;
    char *args;
    int value = 0;

    value = cannot_handle_player_cmd(player, client, game);
    if (value != 2)
        return value;
    cmd_type = parse_command(command_str);
    args = extract_command_args(command_str);
    if (cmd_type == CMD_UNKNOWN)
        return send_message_to_client(client, RESPONSE_KO);
    send_anim_data(cmd_type, game, player, args);
    if (add_command_to_queue(game->cmd_q, cmd_type, args, player->id) < 0) {
        send_message_to_client(client, RESPONSE_KO);
        return -1;
    }
    free(args);
    return 0;
}

static void init_players(game_t *game, client_t *client)
{
    for (int i = 0; i < game->player_count; i++) {
        if (game->players[i].id < 0)
            continue;
        handle_gui_pnw(client, game, &game->players[i]);
        handle_gui_pin(client, game, &game->players[i]);
    }
}

static void handle_gui_connection(game_t *game, client_t *client)
{
    egg_t *egg = NULL;

    game->gui_client = client;
    handle_gui_msz(game, client);
    handle_gui_mct(client, game);
    handle_gui_tna(client, game);
    handle_gui_sgt(game, client, game->freq);
    init_players(game, client);
    for (int i = 0; i < game->team_count; i++) {
        egg = game->teams[i].eggs;
        while (egg) {
            handle_gui_enw(game, client, NULL, egg);
            egg = egg->next;
        }
    }
}

int process_client_command(server_t *server, int client_index,
    const char *command, game_t *game)
{
    client_t *client = &server->clients[client_index];

    if (!client->is_gui && !client->player) {
        if (strcmp(command, GUI_TEAM_NAME) == 0 && !game->gui_client) {
            client->is_gui = 1;
            printf("GUI client authenticated\n");
            handle_gui_connection(game, client);
            return 0;
        } else {
            return handle_team_authentication(server, client_index, command,
                game);
        }
    }
    if (client->is_gui)
        return handle_gui_command(server, client_index, command, game);
    if (client->player)
        return handle_player_command(server, client_index, command, game);
    return 0;
}

void cleanup_server(server_t *server)
{
    if (!server)
        return;
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        if (server->clients[i].socket_fd >= 0) {
            close(server->clients[i].socket_fd);
            server->clients[i].socket_fd = -1;
            server->clients[i].player = NULL;
        }
        server->fds[i].fd = -1;
        server->fds[i].events = 0;
        server->fds[i].revents = 0;
    }
    if (server->server_fd >= 0) {
        close(server->server_fd);
        server->server_fd = -1;
    }
    server->client_count = 0;
    printf("Server cleaned up\n");
}
