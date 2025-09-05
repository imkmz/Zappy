/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** player_direction
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

#include <time.h>

int normalize_coordinate(int coord, int max)
{
    while (coord < 0)
        coord += max;
    return coord % max;
}

position_t get_position_in_direction(position_t pos, int orientation,
    int map_width, int map_height)
{
    position_t new_pos = pos;

    switch (orientation) {
        case 1:
        new_pos.y = normalize_coordinate(pos.y - 1, map_height);
        break;
        case 2:
        new_pos.x = normalize_coordinate(pos.x + 1, map_width);
        break;
        case 3:
        new_pos.y = normalize_coordinate(pos.y + 1, map_height);
        break;
        case 4:
        new_pos.x = normalize_coordinate(pos.x - 1, map_width);
        break;
    }
    return new_pos;
}

void move_player_forward(game_t *game, player_t *player)
{
    position_t new_pos;

    if (!player)
        return;
    new_pos = get_position_in_direction(player->pos,
        player->orientation, game->map.width, game->map.height);
    player->pos = new_pos;
    handle_gui_ppo(game->gui_client, game, player);
    printf("Player %d moved to (%d, %d)\n", player->id,
        player->pos.x, player->pos.y);
}

void turn_player_right(player_t *player)
{
    if (!player)
        return;
    player->orientation = (player->orientation % 4) + 1;
    printf("Player %d turned right, now facing %d\n", player->id,
        player->orientation);
}

void turn_player_left(player_t *player)
{
    if (!player)
        return;
    player->orientation = ((player->orientation - 2 + 4) % 4) + 1;
    printf("Player %d turned left, now facing %d\n", player->id,
        player->orientation);
}
