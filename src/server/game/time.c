/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** time
*/

/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** time
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/game.h"

int should_update_time(game_t *game)
{
    struct timeval current_time;
    long elapsed;
    long time_unit_duration;

    gettimeofday(&current_time, NULL);
    elapsed = (current_time.tv_sec - game->time_update.tv_sec) * 1000000 +
        (current_time.tv_usec - game->time_update.tv_usec);
    time_unit_duration = 1000000 / game->freq;
    return elapsed >= time_unit_duration;
}

void update_game_time(server_t *server, game_t *game)
{
    game->time_unit++;
    gettimeofday(&game->time_update, NULL);
    update_player_life(game);
    update_incantations(server, game);
    if (game->time_unit % 20 == 0)
        spawn_resources(game);
}

long get_action_duration(int action_time, int freq)
{
    return (action_time * 1000000) / freq;
}

int is_action_completed(struct timeval *start_time, int action_time, int freq)
{
    struct timeval current_time;
    long elapsed;
    long action_duration;

    gettimeofday(&current_time, NULL);
    elapsed = (current_time.tv_sec - (*start_time).tv_sec) * 1000000 +
        (current_time.tv_usec - (*start_time).tv_usec);
    action_duration = get_action_duration(action_time, freq);
    return elapsed >= action_duration;
}
