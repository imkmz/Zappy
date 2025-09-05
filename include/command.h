/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command
*/

#ifndef COMMAND_H_
    #define COMMAND_H_
    #include "common.h"
    #include "config.h"
    #include "protocol.h"
    #include "game.h"
    #include "network.h"
    #include <sys/time.h>

typedef struct pending_command_s {
    command_type_t type;
    char *args;
    int execution_time;
    struct timeval start_time;
    int player_id;
    struct pending_command_s *next;
} pending_command_t;

typedef struct command_queue_s {
    pending_command_t *head;
    pending_command_t *tail;
    int count;
} command_queue_t;

typedef struct {
    const char *name;
    command_type_t type;
    int execution_time;
    int (*handler)(game_t *game, player_t *player,
        const char *args, client_t *client);
} command_info_t;


// Command parsing and identification
command_type_t parse_command(const char *command_str);
char *extract_command_args(const char *command_str);
command_info_t *get_command_info(command_type_t type);
client_t *get_client_by_player_id(server_t *server, int player_id);

// Command queue management
command_queue_t *init_command_queue(void);
int add_command_to_queue(command_queue_t *queue, command_type_t type,
    const char *args, int player_id);
pending_command_t *get_next_ready_command(command_queue_t *queue, int freq);
void remove_command_from_queue(command_queue_t *queue, pending_command_t *cmd);
void cleanup_command_queue(command_queue_t *queue);
int is_queue_full(command_queue_t *queue);

int handle_forward(game_t *game, player_t *pla, const char *ag, client_t *cli);
int handle_right(game_t *game, player_t *pla, const char *args, client_t *cli);
int handle_left(game_t *game, player_t *pla, const char *args, client_t *cli);
int handle_look(game_t *game, player_t *player, const char *args,
    client_t *client);
int handle_inventory(game_t *game, player_t *player, const char *args,
    client_t *client);
int handle_connect_nbr(game_t *game, player_t *player, const char *args,
    client_t *client);
int handle_broadcast(game_t *game, player_t *player, const char *args,
    client_t *client);
int handle_fork(game_t *game, player_t *player, const char *args,
    client_t *client);
int handle_eject(game_t *ga, player_t *pla, const char *ag, client_t *cli);
int handle_take(game_t *ga, player_t *pla, const char *ag, client_t *cli);
int handle_set(game_t *ga, player_t *pla, const char *ag, client_t *cli);
int handle_incantation(game_t *ga, player_t *pla, const char *ag,
    client_t *cli);

int handle_gui_pex(game_t *game, client_t *client, player_t *ply);
int handle_gui_pic(game_t *game, client_t *client, player_t **players);
int handle_gui_pie(game_t *game, client_t *client, incantation_t *incantation);
int handle_gui_pfk(game_t *game, client_t *client, player_t *ply);
int handle_gui_pdr(game_t *game, client_t *client, player_t *ply, int i);
int handle_gui_enw(game_t *game, client_t *client, player_t *ply, egg_t *egg);
int handle_gui_ebo(game_t *game, client_t *client, egg_t *egg);
int handle_gui_edi(game_t *game, client_t *client, egg_t *egg);
int handle_gui_pbc(game_t *game, client_t *client, int id, const char *msg);
int handle_gui_tna(client_t *client, game_t *game);
int handle_gui_pnw(client_t *client, game_t *game, player_t *ply);
int handle_gui_ppo(client_t *client, game_t *game, player_t *ply);
int handle_gui_plv(client_t *client, game_t *game, player_t *ply);
int handle_gui_pin(client_t *client, game_t *game, player_t *ply);
int handle_gui_sgt(game_t *game, client_t *client, int time_unit);
int handle_gui_seg(game_t *game, client_t *client, char *winner);
int handle_gui_smg(game_t *game, client_t *client, char *msg);
int handle_gui_pgt(game_t *game, client_t *client, player_t *ply, int i);
int handle_gui_pdi(game_t *game, client_t *client, player_t *ply);
int handle_gui_msz(game_t *game, client_t *client);
int handle_gui_bct(client_t *client, game_t *game, int x, int y);
int handle_gui_mct(client_t *client, game_t *game);

int send_response(client_t *client, const char *response);
void send_responsefd(int socket_fd, const char *message);
char *trim_whitespace(char *str);
char *format_inventory(player_t *player);
void cmd_broadcast(game_t *game, player_t *sender, const char *text);
int send_message_to_client(client_t *client, const char *message);

typedef struct gui_command_s {
    char *command;
    struct gui_command_s *next;
} gui_command_t;

typedef struct gui_command_queue_s {
    gui_command_t *head;
    gui_command_t *tail;
    int count;
} gui_command_queue_t;

// GUI Command queue management
gui_command_queue_t *init_gui_command_queue(void);
int add_gui_command_to_queue(gui_command_queue_t *queue, const char *command);
gui_command_t *get_next_gui_command(gui_command_queue_t *queue);
void remove_gui_command_from_queue(gui_command_queue_t *queue,
    gui_command_t *cmd);
void cleanup_gui_command_queue(gui_command_queue_t *queue);

#endif /* !COMMAND_H_ */
