/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** loop_gui_resp_cmd
*/

#include "../../../include/network.h"
#include "../../../include/protocol.h"
#include "../../../include/config.h"
#include "../../../include/game.h"
#include "../../../include/command.h"


static int gui_resp_bct(server_t *server, int client_index,
    const char *command, game_t *game)
{
    int x;
    int y;
    char response[128];
    client_t *client = &server->clients[client_index];
    inventory_t *tile;

    if (sscanf(command, "bct %d %d", &x, &y) != 2)
        return 0;
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return 0;
    tile = &game->map.tiles[y][x];
    snprintf(response, sizeof(response),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile->food, tile->linemate, tile->deraumere,
        tile->sibur, tile->mendiane, tile->phiras, tile->thystame);
    send_message_to_client(client, response);
    return 0;
}

static int gui_resp_ppo(server_t *server, int client_index,
    const char *command, game_t *game)
{
    int id;
    char response[64];
    client_t *client = &server->clients[client_index];
    player_t *p;

    if (sscanf(command, "ppo #%d", &id) != 1)
        return 0;
    p = get_player_by_id(game, id);
    if (!p)
        return 0;
    snprintf(response, sizeof(response), "ppo #%d %d %d %d\n",
        id, p->pos.x, p->pos.y, p->orientation);
    send_message_to_client(client, response);
    return 0;
}

static int gui_resp_plv(server_t *server, int client_index,
    const char *command, game_t *game)
{
    int id;
    char response[64];
    client_t *client = &server->clients[client_index];
    player_t *p;

    if (sscanf(command, "plv #%d", &id) != 1)
        return 0;
    p = get_player_by_id(game, id);
    if (!p)
        return 0;
    snprintf(response, sizeof(response), "plv #%d %d\n", id, p->level);
    send_message_to_client(client, response);
    return 0;
}

static int gui_resp_pin(server_t *server, int client_index,
    const char *command, game_t *game)
{
    int id;
    char response[128];
    client_t *client = &server->clients[client_index];
    player_t *p;

    if (sscanf(command, "pin #%d", &id) != 1)
        return 0;
    p = get_player_by_id(game, id);
    if (!p)
        return 0;
    snprintf(response, sizeof(response),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        id, p->pos.x, p->pos.y,
        p->inventory.food, p->inventory.linemate,
        p->inventory.deraumere, p->inventory.sibur,
        p->inventory.mendiane, p->inventory.phiras,
        p->inventory.thystame);
    send_message_to_client(client, response);
    return 0;
}

static int gui_resp_sst(server_t *server, int client_index,
    const char *command, game_t *game)
{
    int new_freq;
    char response[64];

    if (sscanf(command, "sst %d", &new_freq) != 1 || new_freq <= 0)
        return 0;
    game->freq = new_freq;
    snprintf(response, sizeof(response), "sst %d\n", new_freq);
    send_message_to_client(&server->clients[client_index], response);
    return 0;
}

static int gui_resp_msz(server_t *server, int client_index, game_t *game)
{
    char response[64];

    snprintf(response, sizeof(response), "msz %d %d\n",
        game->map.width, game->map.height);
    send_message_to_client(&server->clients[client_index], response);
    return 0;
}

static int gui_resp_mct(server_t *server, int client_index, game_t *game)
{
    char response[128];
    client_t *client = &server->clients[client_index];
    inventory_t *tile;

    for (int y = 0; y < game->map.height; y++) {
        for (int x = 0; x < game->map.width; x++) {
            tile = &game->map.tiles[y][x];
            snprintf(response, sizeof(response),
                "bct %d %d %d %d %d %d %d %d %d\n",
                x, y, tile->food, tile->linemate, tile->deraumere,
                tile->sibur, tile->mendiane, tile->phiras, tile->thystame);
            send_message_to_client(client, response);
        }
    }
    return 0;
}

static int gui_resp_tna(server_t *server, int client_index, game_t *game)
{
    char response[64];
    client_t *client = &server->clients[client_index];

    for (int i = 0; i < game->team_count; i++) {
        snprintf(response, sizeof(response), "tna %s\n", game->teams[i].name);
        send_message_to_client(client, response);
    }
    return 0;
}

static int gui_resp_sgt(server_t *server, int client_index, game_t *game)
{
    char response[64];

    snprintf(response, sizeof(response), "sgt %d\n", game->freq);
    send_message_to_client(&server->clients[client_index], response);
    return 0;
}

int handle_gui_command(server_t *server, int client_index,
    const char *command, game_t *game)
{
    if (strncmp(command, "bct ", 4) == 0)
        return gui_resp_bct(server, client_index, command, game);
    if (strncmp(command, "ppo ", 4) == 0)
        return gui_resp_ppo(server, client_index, command, game);
    if (strncmp(command, "plv ", 4) == 0)
        return gui_resp_plv(server, client_index, command, game);
    if (strncmp(command, "pin ", 4) == 0)
        return gui_resp_pin(server, client_index, command, game);
    if (strncmp(command, "sst ", 4) == 0)
        return gui_resp_sst(server, client_index, command, game);
    if (strcmp(command, "msz") == 0)
        return gui_resp_msz(server, client_index, game);
    if (strcmp(command, "mct") == 0)
        return gui_resp_mct(server, client_index, game);
    if (strcmp(command, "tna") == 0)
        return gui_resp_tna(server, client_index, game);
    if (strcmp(command, "sgt") == 0)
        return gui_resp_sgt(server, client_index, game);
    send_message_to_client(&server->clients[client_index], "suc\n");
    return 0;
}
