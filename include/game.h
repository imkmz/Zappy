/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** game
*/

#ifndef GAME_H_
    #define GAME_H_
    #include "common.h"
    #include "config.h"
    #include "protocol.h"
    #include <sys/time.h>
    #include <signal.h>

    #define TILE_BUFFER_SIZE 512

typedef struct client_s client_t;
typedef struct command_queue_s command_queue_t;
typedef struct gui_command_queue_s gui_command_queue_t;
typedef struct server_s server_t;

typedef struct egg_s {
    int id;
    position_t pos;
    char team_name[MAX_TEAM_NAME];
    int hatching_time;
    struct egg_s *next;
} egg_t;

typedef struct team_s {
    char name[MAX_TEAM_NAME];
    int max_clients;
    int connected_clients;
    int available_slots;
    player_t *players;
    egg_t *eggs;
    int egg_count;
} team_t;

typedef struct incantation_s {
    position_t pos;
    int level;
    int player_count;
    int *player_ids;
    int start_time;
    int duration;
    int is_active;
    struct incantation_s *next;
} incantation_t;

typedef struct game_s {
    map_t map;
    team_t teams[MAX_TEAMS];
    int team_count;
    player_t players[MAX_PLAYERS];
    int player_count;
    incantation_t *incantations;
    int time_unit;
    int freq;
    struct timeval resource_spawn;
    struct timeval time_update;
    int next_player_id;
    int next_egg_id;
    int game_ended;
    char winning_team[MAX_TEAM_NAME];
    client_t *gui_client;
    command_queue_t *cmd_q;
    gui_command_queue_t *gui_q;
    server_t *server;
} game_t;


typedef struct {
    int nb_players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} elevation_req_t;

typedef struct {
    char *result;
    int *pos;
    int *first;
    int max_len;
} append_ctx_t;


typedef struct {
    player_t *player;
    game_t *game;
    int row;
    int offset;
} vision_context_t;

typedef struct {
    char *result;
    size_t *pos;
    size_t max_size;
} result_context_t;

typedef position_t (*vision_func_t)(position_t, int, int, map_t);

// Actions
int take_object(game_t *game, player_t *player, const char *object_name);
int set_object(game_t *game, player_t *player, const char *object_name);
void eject_players(game_t *game, player_t *ejector);

// Broadcast
char *broadcast_message(const char *message, int sender_id);

// Vision
char *get_player_vision(game_t *game, player_t *player);

//View position
position_t view_north(position_t p, int dx, int dy, map_t m);
position_t view_east(position_t p, int dx, int dy, map_t m);
position_t view_south(position_t p, int dx, int dy, map_t m);
position_t view_west(position_t p, int dx, int dy, map_t m);

// Incantation
int can_start_incantation(game_t *game, position_t pos, int level);
incantation_t *start_incantation(game_t *game, position_t pos, int level);
void update_incantations(server_t *server, game_t *game);
int is_player_in_incantation(game_t *game, int player_id);
void consume_resources(game_t *game, position_t pos, int level);

// Egg management
egg_t *create_egg(game_t *game, const char *team_name, position_t pos,
    int is_initial);
void remove_egg(game_t *game, int egg_id, const char *team_name);
egg_t *get_random_egg(team_t *team);
int hatch_egg(game_t *game, const char *team_name, int socket_fd);

// Player management
int create_player(game_t *game, const char *team_name, int socket_fd);
void remove_player(game_t *game, int player_id);
void update_player_life(game_t *game);
position_t get_position_in_direction(position_t pos, int orientation,
    int map_width, int map_height);
void move_player_forward(game_t *game, player_t *player);
void turn_player_right(player_t *player);
void turn_player_left(player_t *player);
int normalize_coordinate(int coord, int max);
position_t random_position(int width, int height);
int random_orientation(void);

// Game utilities
int count_players_at_position(game_t *game, position_t pos);
int count_players_at_level(game_t *game, position_t pos, int level);
int is_position_valid(game_t *game, position_t pos);
void handle_player_death(game_t *game, int player_id);
int check_winning_condition(game_t *game);

// Resource management
void spawn_resources(game_t *game);
int should_spawn_resources(game_t *game);
void update_resources(game_t *game);
int get_resource_nb(const char *name);

// Getters
int get_team_index(game_t *game, const char *team_name);
team_t *get_team_by_name(game_t *game, const char *team_name);
int get_available_tiles(game_t *game, const char *team_name);
player_t *get_player_by_id(game_t *game, int player_id);
player_t *get_player_by_socket(game_t *game, int sock_fd);

// Game initialization and cleanup
void cleanup_game(game_t *game);

// Map management
int init_map(map_t *map, int width, int height);
void cleanup_map(map_t *map);

// Time management
int should_update_time(game_t *game);
void update_game_time(server_t *server, game_t *game);
long get_action_duration(int action_time, int freq);
int is_action_completed(struct timeval *start_time, int action_time, int freq);

// Game state
void print_game_state(game_t *game);
void print_map_resources(game_t *game);
int get_level_8_count(game_t *game, const char *team_name);
void announce_winner(game_t *game);

int is_valid(const char *object_name);
void destroy_eggs_on_tile(game_t *game, position_t pos);

void initial_spawn_resources(game_t *game);
void process_player_life(game_t *game, player_t *player);

sig_atomic_t *get_or_set_running(sig_atomic_t *new_value);

#endif /* !GAME_H_ */
