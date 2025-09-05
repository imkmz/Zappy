/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** actions
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static int take_object_one(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "food") == 0 && tile->food > 0) {
        tile->food--;
        player->inventory.food++;
        printf("Player %d took food\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "linemate") == 0 && tile->linemate > 0) {
        tile->linemate--;
        player->inventory.linemate++;
        printf("Player %d took linemate\n", player->id);
        return 1;
    }
    return 0;
}

static int take_object_two(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "deraumere") == 0 && tile->deraumere > 0) {
        tile->deraumere--;
        player->inventory.deraumere++;
        printf("Player %d took deraumere\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "sibur") == 0 && tile->sibur > 0) {
        tile->sibur--;
        player->inventory.sibur++;
        printf("Player %d took sibur\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "mendiane") == 0 && tile->mendiane > 0) {
        tile->mendiane--;
        player->inventory.mendiane++;
        printf("Player %d took mendiane\n", player->id);
        return 1;
    }
    return 0;
}

static int take_object_three(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "phiras") == 0 && tile->phiras > 0) {
        tile->phiras--;
        player->inventory.phiras++;
        printf("Player %d took phiras\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "thystame") == 0 && tile->thystame > 0) {
        tile->thystame--;
        player->inventory.thystame++;
        printf("Player %d took thystame\n", player->id);
        return 1;
    }
    printf("Player %d failed to take %s\n", player->id, object_name);
    return 0;
}

int take_object(game_t *game, player_t *player, const char *object_name)
{
    inventory_t *tile;
    int return_value = 0;

    if (!player || !object_name)
        return 0;
    tile = &game->map.tiles[player->pos.y][player->pos.x];
    return_value = take_object_one(object_name, tile, player);
    if (return_value == 1)
        return return_value;
    return_value = take_object_two(object_name, tile, player);
    if (return_value == 1)
        return return_value;
    return (take_object_three(object_name, tile, player));
}

static int set_object_one(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "food") == 0 && player->inventory.food > 0) {
        player->inventory.food--;
        tile->food++;
        printf("Player %d set food\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "linemate") == 0 &&
        player->inventory.linemate > 0) {
        player->inventory.linemate--;
        tile->linemate++;
        printf("Player %d set linemate\n", player->id);
        return 1;
    }
    return 0;
}

static int set_object_two(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "deraumere") == 0 &&
        player->inventory.deraumere > 0) {
        player->inventory.deraumere--;
        tile->deraumere++;
        printf("Player %d set deraumere\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "sibur") == 0 && player->inventory.sibur > 0) {
        player->inventory.sibur--;
        tile->sibur++;
        printf("Player %d set sibur\n", player->id);
        return 1;
    } else if (strcmp(object_name, "mendiane") == 0 &&
        player->inventory.mendiane > 0) {
        player->inventory.mendiane--;
        tile->mendiane++;
        printf("Player %d set mendiane\n", player->id);
        return 1;
    }
    return 0;
}

static int set_object_three(const char *object_name, inventory_t *tile,
    player_t *player)
{
    if (strcmp(object_name, "phiras") == 0 && player->inventory.phiras > 0) {
        player->inventory.phiras--;
        tile->phiras++;
        printf("Player %d set phiras\n", player->id);
        return 1;
    }
    if (strcmp(object_name, "thystame") == 0 &&
        player->inventory.thystame > 0) {
        player->inventory.thystame--;
        tile->thystame++;
        printf("Player %d set thystame\n", player->id);
        return 1;
    }
    printf("Player %d failed to set %s\n", player->id, object_name);
    return 0;
}

int set_object(game_t *game, player_t *player, const char *object_name)
{
    inventory_t *tile;
    int return_value = 0;

    if (!player || !object_name)
        return 0;
    tile = &game->map.tiles[player->pos.y][player->pos.x];
    return_value = set_object_one(object_name, tile, player);
    if (return_value == 1)
        return 1;
    return_value = set_object_two(object_name, tile, player);
    if (return_value == 1)
        return 1;
    return (set_object_three(object_name, tile, player));
}

static void eject_players_from_tile(game_t *game, player_t *ejector)
{
    position_t ejector_pos = ejector->pos;
    position_t new_pos;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id != -1 &&
            game->players[i].id != ejector->id &&
            game->players[i].pos.x == ejector_pos.x &&
            game->players[i].pos.y == ejector_pos.y) {
            new_pos = get_position_in_direction(
                ejector_pos,
                ejector->orientation,
                game->map.width,
                game->map.height
            );
            game->players[i].pos = new_pos;
            handle_gui_ppo(game->gui_client, game, &game->players[i]);
            printf("Player %d ejected player %d to (%d, %d)\n",
                ejector->id, game->players[i].id, new_pos.x, new_pos.y);
        }
    }
}

void eject_players(game_t *game, player_t *ejector)
{
    if (!ejector)
        return;
    eject_players_from_tile(game, ejector);
    destroy_eggs_on_tile(game, ejector->pos);
}
