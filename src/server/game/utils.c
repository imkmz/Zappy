/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** utils
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

int count_players_at_position(game_t *game, position_t pos)
{
    int count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id != -1 &&
            game->players[i].pos.x == pos.x &&
            game->players[i].pos.y == pos.y) {
            count++;
        }
    }
    return count;
}

int count_players_at_level(game_t *game, position_t pos, int level)
{
    int count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id != -1 &&
            game->players[i].pos.x == pos.x &&
            game->players[i].pos.y == pos.y &&
            game->players[i].level == level) {
            count++;
        }
    }
    return count;
}

int is_position_valid(game_t *game, position_t pos)
{
    return pos.x >= 0 && pos.x < game->map.width &&
        pos.y >= 0 && pos.y < game->map.height;
}

void handle_player_death(game_t *game, int player_id)
{
    if (player_id < 0 || player_id >= MAX_PLAYERS)
        return;
    printf("Player %d died\n", player_id);
    send_response(get_client_by_player_id(game->server, player_id),
        RESPONSE_DEAD);
    remove_player(game, player_id);
}

static void level_up(int *level_8, int i, int j, game_t *game)
{
    if (game->players[j].id != -1 &&
        strcmp(game->players[j].team_name, game->teams[i].name) == 0 &&
        game->players[j].level >= 8) {
        (*level_8)++;
    }
}

int check_winning_condition(game_t *game)
{
    int level_8;

    for (int i = 0; i < game->team_count; i++) {
        level_8 = 0;
        for (int j = 0; j < MAX_PLAYERS; j++) {
            level_up(&level_8, i, j, game);
        }
        if (level_8 >= 6) {
            game->game_ended = 1;
            strncpy(game->winning_team, game->teams[i].name,
                MAX_TEAM_NAME - 1);
            game->winning_team[MAX_TEAM_NAME - 1] = '\0';
            return 1;
        }
    }
    return 0;
}
