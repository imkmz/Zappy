/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** actions_destro_egg
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static void destroy_eggs_in_list(game_t *game, egg_t **egg_list,
    int *egg_count, position_t pos)
{
    egg_t *to_remove;

    while (*egg_list) {
        if ((*egg_list)->pos.x == pos.x && (*egg_list)->pos.y == pos.y) {
            to_remove = *egg_list;
            *egg_list = (*egg_list)->next;
            (*egg_count)--;
            handle_gui_edi(game, game->gui_client, to_remove);
            free(to_remove);
            printf("Egg destroyed by ejection at (%d, %d)\n", pos.x, pos.y);
        } else {
            egg_list = &(*egg_list)->next;
        }
    }
}

void destroy_eggs_on_tile(game_t *game, position_t pos)
{
    for (int t = 0; t < game->team_count; t++) {
        destroy_eggs_in_list(game, &game->teams[t].eggs,
            &game->teams[t].egg_count, pos);
    }
}
