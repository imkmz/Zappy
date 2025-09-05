/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** incantation
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static const elevation_req_t elevation_requirements[] = {
    {1, 1, 0, 0, 0, 0, 0}, // Level 1->2
    {2, 1, 1, 1, 0, 0, 0}, // Level 2->3
    {2, 2, 0, 1, 0, 2, 0}, // Level 3->4
    {4, 1, 1, 2, 0, 1, 0}, // Level 4->5
    {4, 1, 2, 1, 3, 0, 0}, // Level 5->6
    {6, 1, 2, 3, 0, 1, 0}, // Level 6->7
    {6, 2, 2, 2, 2, 2, 1}  // Level 7->8
};

static int check_resources(game_t *game, position_t pos, int level)
{
    inventory_t *tile;
    const elevation_req_t *req;

    if (level < 1 || level > 7)
        return 0;
    tile = &game->map.tiles[pos.y][pos.x];
    req = &elevation_requirements[level - 1];
    return (tile->linemate >= req->linemate &&
            tile->deraumere >= req->deraumere &&
            tile->sibur >= req->sibur &&
            tile->mendiane >= req->mendiane &&
            tile->phiras >= req->phiras &&
            tile->thystame >= req->thystame);
}

static int check_players(game_t *game, position_t pos, int level)
{
    int players_at_level;
    const elevation_req_t *req;

    if (level < 1 || level > 7)
        return 0;
    players_at_level = count_players_at_level(game, pos, level);
    req = &elevation_requirements[level - 1];
    return players_at_level >= req->nb_players;
}

int can_start_incantation(game_t *game, position_t pos, int level)
{
    return check_resources(game, pos, level) &&
        check_players(game, pos, level);
}

void consume_resources(game_t *game, position_t pos, int level)
{
    inventory_t *tile;
    const elevation_req_t *req;

    if (level < 1 || level > 7)
        return;
    tile = &game->map.tiles[pos.y][pos.x];
    req = &elevation_requirements[level - 1];
    tile->linemate -= req->linemate;
    tile->deraumere -= req->deraumere;
    tile->sibur -= req->sibur;
    tile->mendiane -= req->mendiane;
    tile->phiras -= req->phiras;
    tile->thystame -= req->thystame;
}

static int check_level(int level, int *count)
{
    if (level < 1 || level > 7) {
        *count = 0;
        return 1;
    }
    return 0;
}

static int player_ids_error(int *player_ids, int *count)
{
    if (!player_ids) {
        *count = 0;
        return 1;
    }
    return 0;
}

static int *get_players(game_t *game, position_t pos, int level, int *count)
{
    const elevation_req_t *req;
    int *player_ids;

    if (check_level(level, count) == 1)
        return NULL;
    req = &elevation_requirements[level - 1];
    player_ids = malloc(req->nb_players * sizeof(int));
    if (player_ids_error(player_ids, count) == 1)
        return NULL;
    *count = 0;
    for (int i = 0; i < MAX_PLAYERS && *count < req->nb_players; i++) {
        if (game->players[i].id != -1 &&
            game->players[i].pos.x == pos.x &&
            game->players[i].pos.y == pos.y &&
            game->players[i].level == level) {
            player_ids[*count] = game->players[i].id;
            (*count)++;
        }
    }
    return player_ids;
}

static void set_players_in_incantation(game_t *game,
    incantation_t *incantation)
{
    player_t *player = NULL;

    for (int i = 0; i < incantation->player_count; i++) {
        player = get_player_by_id(game, incantation->player_ids[i]);
        if (player)
            player->is_in_incantation = 1;
    }
}

incantation_t *start_incantation(game_t *game, position_t pos, int level)
{
    incantation_t *incantation;

    if (!can_start_incantation(game, pos, level))
        return NULL;
    incantation = malloc(sizeof(incantation_t));
    if (!incantation)
        return NULL;
    incantation->pos = pos;
    incantation->level = level;
    incantation->player_ids = get_players(game, pos, level,
        &incantation->player_count);
    set_players_in_incantation(game, incantation);
    incantation->start_time = game->time_unit;
    incantation->duration = TIME_INCANTATION / game->freq;
    incantation->is_active = 1;
    incantation->next = game->incantations;
    game->incantations = incantation;
    printf("Incantation started at (%d, %d) for level %d with %d players\n",
        pos.x, pos.y, level, incantation->player_count);
    return incantation;
}
