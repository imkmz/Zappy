/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** game
*/

#include "../../../include/common.h"
#include "../../../include/config.h"
#include "../../../include/command.h"
#include "../../../include/game.h"
#include "../../../include/network.h"
#include <time.h>

static void init_teams(game_t *game, char teams[][MAX_TEAM_NAME],
    int team_count, int clients_per_team)
{
    game->team_count = team_count;
    for (int i = 0; i < team_count; i++) {
        strncpy(game->teams[i].name, teams[i], MAX_TEAM_NAME - 1);
        game->teams[i].name[MAX_TEAM_NAME - 1] = '\0';
        game->teams[i].max_clients = clients_per_team;
        game->teams[i].connected_clients = 0;
        game->teams[i].available_slots = 0;
        game->teams[i].players = NULL;
        game->teams[i].eggs = NULL;
        game->teams[i].egg_count = 0;
    }
}

static void init_players(game_t *game)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(&game->players[i], 0, sizeof(player_t));
        game->players[i].id = -1;
        game->players[i].socket_fd = -1;
        game->players[i].level = 1;
        game->players[i].last_eat = INITIAL_LIFE_UNITS;
        game->players[i].orientation = 1;
    }
    game->player_count = 0;
    game->next_player_id = 1;
}

static void assign_game_var(game_t *game, server_config_t *cf)
{
    game->incantations = NULL;
    game->time_unit = 0;
    game->freq = cf->freq;
    game->next_egg_id = 1;
    game->game_ended = 0;
    game->gui_client = NULL;
}

static void create_eggs_for_team(game_t *game, team_t *team,
    int clients_per_team)
{
    position_t pos;
    egg_t *egg;

    for (int j = 0; j < clients_per_team; j++) {
        pos = random_position(game->map.width, game->map.height);
        egg = create_egg(game, team->name, pos, 1);
        if (!egg) {
            fprintf(stderr, "Failed to create initial egg"
                "for team %s\n", team->name);
            continue;
        }
    }
    team->available_slots = team->egg_count;
    printf("Created %d initial eggs for team %s\n",
        team->egg_count, team->name);
}

void create_initial_eggs(game_t *game, int clients_per_team)
{
    for (int i = 0; i < game->team_count; i++)
        create_eggs_for_team(game, &game->teams[i], clients_per_team);
}

int init_game(game_t *game, server_t *server, server_config_t *cf)
{
    srand(time(NULL));
    if (init_map(&game->map, cf->width, cf->height) < 0) {
        fprintf(stderr, "Failed to initialize map\n");
        return -1;
    }
    init_teams(game, cf->teams, cf->team_count, cf->clients_per_team);
    init_players(game);
    assign_game_var(game, cf);
    create_initial_eggs(game, cf->clients_per_team);
    memset(game->winning_team, 0, MAX_TEAM_NAME);
    gettimeofday(&game->resource_spawn, NULL);
    gettimeofday(&game->time_update, NULL);
    initial_spawn_resources(game);
    printf("Game initialized: %dx%d map, %d teams, %d clients per "
        "team, freq=%d\n",
    cf->width, cf->height, cf->team_count, cf->clients_per_team, cf->freq);
    game->cmd_q = init_command_queue();
    game->gui_q = init_gui_command_queue();
    game->server = server;
    return 0;
}

static void cleanup_incantations(incantation_t *inc)
{
    incantation_t *next;

    while (inc) {
        next = inc->next;
        free(inc->player_ids);
        free(inc);
        inc = next;
    }
}

static void cleanup_eggs(egg_t **eggs)
{
    egg_t *next;

    while (*eggs) {
        next = (*eggs)->next;
        free(*eggs);
        *eggs = next;
    }
}

void cleanup_game(game_t *game)
{
    if (!game)
        return;
    cleanup_map(&game->map);
    cleanup_incantations(game->incantations);
    free(game->cmd_q);
    free(game->gui_q);
    for (int i = 0; i < game->team_count && i < MAX_TEAMS; i++) {
        cleanup_eggs(&game->teams[i].eggs);
        game->teams[i].egg_count = 0;
    }
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i].id == -1)
            continue;
        memset(&game->players[i], 0, sizeof(player_t));
        game->players[i].id = -1;
        game->players[i].socket_fd = -1;
    }
    game->player_count = 0;
    game->team_count = 0;
    game->game_ended = 1;
}
