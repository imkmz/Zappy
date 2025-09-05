/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player - Corrected egg hatching system
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

#include <time.h>

position_t random_position(int width, int height)
{
    position_t pos;

    pos.x = rand() % width;
    pos.y = rand() % height;
    return pos;
}

int random_orientation(void)
{
    return (rand() % 4) + 1;
}

int create_player(game_t *game, const char *team_name, int socket_fd)
{
    int player_id;
    player_t *player;
    team_t *team = get_team_by_name(game, team_name);

    if (!team || !team->eggs || team->egg_count == 0)
        return -1;
    player_id = hatch_egg(game, team_name, socket_fd);
    if (player_id == -1) {
        fprintf(stderr, "Failed to hatch egg for team %s\n", team_name);
        return -1;
    }
    player = get_player_by_id(game, player_id);
    if (player) {
        handle_gui_pnw(game->gui_client, game, player);
        printf("Player %d created from egg for team %s at (%d, %d)\n",
            player->id, team_name, player->pos.x, player->pos.y);
    }
    return player_id;
}

static void get_player_inventory(inventory_t *tile,
    game_t *game, player_t *player)
{
    team_t *team;

    tile->food += player->inventory.food;
    tile->linemate += player->inventory.linemate;
    tile->deraumere += player->inventory.deraumere;
    tile->sibur += player->inventory.sibur;
    tile->mendiane += player->inventory.mendiane;
    tile->phiras += player->inventory.phiras;
    tile->thystame += player->inventory.thystame;
    handle_gui_bct(game->gui_client, game, player->pos.x, player->pos.y);
    team = get_team_by_name(game, player->team_name);
    if (team) {
        team->connected_clients--;
        team->available_slots++;
    }
}

void remove_player(game_t *game, int player_id)
{
    player_t *player = get_player_by_id(game, player_id);
    inventory_t *tile;
    client_t *client = get_client_by_player_id(game->server, player_id);

    if (!player || player->id == -1)
        return;
    printf("Player %d from team %s died\n", player_id, player->team_name);
    handle_gui_pdi(game, game->gui_client, player);
    tile = &game->map.tiles[player->pos.y][player->pos.x];
    get_player_inventory(tile, game, player);
    if (client)
        disconnect_client(game->server, client->index, game);
    memset(player, 0, sizeof(player_t));
    player->id = -1;
    player->socket_fd = -1;
    player->dead = 1;
    game->player_count--;
}

void update_player_life(game_t *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id == -1)
            continue;
        process_player_life(game, &game->players[i]);
    }
}
