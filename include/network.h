/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** network
*/

#ifndef NETWORK_H_
    #define NETWORK_H_
    #define MAX_CONNECTIONS 1024
    #define BUFFER_SIZE 4096
    #include <poll.h>
    #include <ctype.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <errno.h>
    #include "common.h"
    #include "config.h"
    #include "game.h"
    #include <signal.h>


typedef struct client_s {
    int socket_fd;
    char buffer[BUFFER_SIZE];
    int buffer_pos;
    int is_gui;
    int index;
    player_t *player;
} client_t;

typedef struct server_s {
    int server_fd;
    struct pollfd fds[MAX_CONNECTIONS];
    client_t clients[MAX_CONNECTIONS];
    int client_count;
    int port;
} server_t;

typedef struct {
    int port;
    int width;
    int height;
    char teams[MAX_TEAMS][MAX_TEAM_NAME];
    int team_count;
    int clients_per_team;
    int freq;
} server_config_t;

typedef int (*option_handler_t)(server_config_t *, char *, char **);

typedef struct {
    char opt;
    option_handler_t handler;
} option_map_t;

int init_game(game_t *game, server_t *server, server_config_t *cf);
int valid_number(const char *str);
void cleanup_server(server_t *server);
int accept_new_client(server_t *server);
void init_config(server_config_t *config);
int validate_config(server_config_t *config);
option_handler_t get_option_handler(char opt);
void handle_dead_players(server_t *server, game_t *game);
int run_server(server_t *server, server_config_t *config);
int init_server(server_t *server, server_config_t *config);
int handle_client_data(server_t *server, int client_index, game_t *game);
int handle_gui_command(server_t *server, int client_index,
    const char *command, game_t *game);
void disconnect_client(server_t *server, int client_index, game_t *game);
int parse_arguments(int argc, char **argv, server_config_t *config);
int handle_clients(server_config_t *config, char *arg, char **argv);
int handle_freq(server_config_t *config, char *arg, char **argv);
int handle_port(server_config_t *config, char *arg, char **argv);
int handle_width(server_config_t *config, char *arg, char **argv);
int handle_height(server_config_t *config, char *arg, char **argv);
int handle_teams(server_config_t *config, char *arg, char **argv);
int process_client_command(server_t *server, int client_index,
    const char *command, game_t *game);
void signal_handler(int sig);
sig_atomic_t *get_or_set_running(sig_atomic_t *new_value);

#endif /* !NETWORK_H_ */
