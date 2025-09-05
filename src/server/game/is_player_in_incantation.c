/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** incantation
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/game.h"

static int check_all_player_id(incantation_t *current, int player_id)
{
    for (int i = 0; i < current->player_count; i++) {
        if (current->player_ids[i] == player_id)
            return 1;
    }
    return 0;
}

int is_player_in_incantation(game_t *game, int player_id)
{
    incantation_t *current = game->incantations;

    while (current) {
        if (check_all_player_id(current, player_id) == 1)
            return 1;
        current = current->next;
    }
    return 0;
}
