/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** state
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static void print_teams(game_t *game)
{
    printf("\n=== TEAMS ===\n");
    for (int i = 0; i < game->team_count; i++) {
        printf("Team %s: %d/%d clients, %d slots, %d eggs\n",
            game->teams[i].name,
            game->teams[i].connected_clients,
            game->teams[i].max_clients,
            game->teams[i].available_slots,
            game->teams[i].egg_count);
    }
}

static void print_players(game_t *game)
{
    printf("\n=== PLAYERS ===\n");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id != -1) {
            printf("Player %d (team %s): pos(%d,%d), level %d, "
                "orientation %d\n",
                game->players[i].id,
                game->players[i].team_name,
                game->players[i].pos.x,
                game->players[i].pos.y,
                game->players[i].level,
                game->players[i].orientation);
        }
    }
}

static void print_incantations(game_t *game)
{
    incantation_t *inc = game->incantations;
    int inc_count = 0;

    printf("\n=== INCANTATIONS ===\n");
    while (inc) {
        printf("Incantation %d: pos(%d,%d), level %d, %d players, "
            "active: %s\n",
            inc_count++, inc->pos.x, inc->pos.y,
            inc->level, inc->player_count,
            inc->is_active ? "Yes" : "No");
        inc = inc->next;
    }
    if (inc_count == 0)
        printf("No active incantations\n");
}

void print_game_state(game_t *game)
{
    printf("=== GAME STATE ===\n");
    printf("Time unit: %d\n", game->time_unit);
    printf("Frequency: %d\n", game->freq);
    printf("Total players: %d\n", game->player_count);
    printf("Game ended: %s\n", game->game_ended ? "Yes" : "No");
    if (game->game_ended)
        printf("Winning team: %s\n", game->winning_team);
    print_teams(game);
    print_players(game);
    print_incantations(game);
    printf("==================\n\n");
}

void print_map_resources(game_t *game)
{
    inventory_t *tile;
    int total_resources[7] = {0};
    const char *resource_names[] = {"food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame"};

    printf("=== MAP RESOURCES ===\n");
    for (int y = 0; y < game->map.height; y++)
        for (int x = 0; x < game->map.width; x++) {
            tile = &game->map.tiles[y][x];
            total_resources[0] += tile->food;
            total_resources[1] += tile->linemate;
            total_resources[2] += tile->deraumere;
            total_resources[3] += tile->sibur;
            total_resources[4] += tile->mendiane;
            total_resources[5] += tile->phiras;
            total_resources[6] += tile->thystame;
        }
    for (int i = 0; i < 7; i++)
        printf("%s: %d\n", resource_names[i], total_resources[i]);
    printf("=====================\n\n");
}

int get_level_8_count(game_t *game, const char *team_name)
{
    int count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id != -1 &&
            strcmp(game->players[i].team_name, team_name) == 0 &&
            game->players[i].level >= 8) {
            count++;
        }
    }
    return count;
}

void announce_winner(game_t *game)
{
    if (game->game_ended) {
        handle_gui_seg(game, game->gui_client, game->winning_team);
        printf("\n🎉 GAME OVER! 🎉\n");
        printf("Winner: Team %s\n", game->winning_team);
        printf("They have %d players at level 8!\n",
            get_level_8_count(game, game->winning_team));
        printf("Game duration: %d time units\n", game->time_unit);
    }
}

void process_player_life(game_t *game, player_t *player)
{
    if (game->time_unit >= player->last_eat + LIFE_UNIT_DURATION) {
        if (player->inventory.food > 0) {
            player->inventory.food--;
            player->last_eat = game->time_unit;
            handle_gui_pin(game->gui_client, game, player);
        } else
            handle_player_death(game, player->id);
    }
}
