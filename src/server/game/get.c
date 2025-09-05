/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** get
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/game.h"

int get_team_index(game_t *game, const char *team_name)
{
    for (int i = 0; i < game->team_count; i++) {
        if (strcmp(game->teams[i].name, team_name) == 0)
            return i;
    }
    return -1;
}

team_t *get_team_by_name(game_t *game, const char *team_name)
{
    int index = get_team_index(game, team_name);

    return (index >= 0) ? &game->teams[index] : NULL;
}

int get_available_tiles(game_t *game, const char *team_name)
{
    team_t *team = get_team_by_name(game, team_name);

    return team ? team->available_slots : 0;
}

player_t *get_player_by_id(game_t *game, int player_id)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id == player_id)
            return &game->players[i];
    }
    return NULL;
}

player_t *get_player_by_socket(game_t *game, int sock_fd)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].socket_fd == sock_fd && game->players[i].id != -1)
            return &game->players[i];
    }
    return NULL;
}
