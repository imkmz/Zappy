/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** egg
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"

static void set_next_egg(egg_t *egg, team_t *team)
{
    if (team) {
        egg->next = team->eggs;
        team->eggs = egg;
        team->egg_count++;
    }
}

egg_t *create_egg(game_t *game, const char *team_name,
    position_t pos, int is_initial)
{
    egg_t *egg = malloc(sizeof(egg_t));
    team_t *team;

    if (!egg)
        return NULL;
    egg->id = game->next_egg_id;
    game->next_egg_id++;
    egg->pos = pos;
    strncpy(egg->team_name, team_name, MAX_TEAM_NAME - 1);
    egg->team_name[MAX_TEAM_NAME - 1] = '\0';
    if (is_initial)
        egg->hatching_time = 0;
    else
        egg->hatching_time = game->time_unit + (42 * game->freq);
    egg->next = NULL;
    team = get_team_by_name(game, team_name);
    set_next_egg(egg, team);
    if (!is_initial && team)
        team->available_slots++;
    return egg;
}

void remove_egg(game_t *game, int egg_id, const char *team_name)
{
    team_t *team = get_team_by_name(game, team_name);
    egg_t **current;
    egg_t *to_remove;

    if (!team)
        return;
    current = &team->eggs;
    while (*current) {
        if ((*current)->id == egg_id) {
            to_remove = *current;
            *current = (*current)->next;
            handle_gui_edi(game, game->gui_client, to_remove);
            free(to_remove);
            team->egg_count--;
            printf("Egg %d removed\n", egg_id);
            return;
        }
        current = &(*current)->next;
    }
}

egg_t *get_random_egg(team_t *team)
{
    int target;
    egg_t *current;

    if (!team->eggs || team->egg_count == 0)
        return NULL;
    target = rand() % team->egg_count;
    current = team->eggs;
    for (int i = 0; i < target && current; i++)
        current = current->next;
    return current;
}

static int find_free_player_slot(game_t *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id == -1)
            return i;
    }
    return -1;
}

static void init_hatched_player(player_t *player, game_t *game,
    egg_t *egg, int socket_fd)
{
    player->id = game->next_player_id;
    game->next_player_id++;
    player->socket_fd = socket_fd;
    player->pos = egg->pos;
    player->orientation = random_orientation();
    player->level = 1;
    player->last_eat = INITIAL_LIFE_UNITS;
    player->dead = 0;
    player->is_in_incantation = 0;
    strncpy(player->team_name, egg->team_name, sizeof(player->team_name) - 1);
    player->team_name[sizeof(player->team_name) - 1] = '\0';
    memset(&player->inventory, 0, sizeof(inventory_t));
    player->inventory.food = INITIAL_LIFE_UNITS;
}

int hatch_egg(game_t *game, const char *team_name, int socket_fd)
{
    team_t *team = get_team_by_name(game, team_name);
    player_t *player;
    int slot;
    egg_t *egg;

    if (!team || !team->eggs || team->egg_count == 0)
        return -1;
    egg = get_random_egg(team);
    if (!egg)
        return -1;
    slot = find_free_player_slot(game);
    if (slot == -1)
        return -1;
    player = &game->players[slot];
    init_hatched_player(player, game, egg, socket_fd);
    team->connected_clients++;
    game->player_count++;
    remove_egg(game, egg->id, team_name);
    return player->id;
}
