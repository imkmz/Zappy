/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** protocol
*/

#ifndef PROTOCOL_H_
    #define PROTOCOL_H_

    // la reponse (serveur -> ai)
    #define RESPONSE_OK "ok\n"
    #define RESPONSE_KO "ko\n"
    #define RESPONSE_DEAD "dead\n"

    // gui
    #define GUI_TEAM_NAME "GRAPHIC"

    // server
    #define WELCOME_MSG "WELCOME\n"

// cmd (ai -> serveur)
typedef enum {
    CMD_FORWARD,
    CMD_RIGHT,
    CMD_LEFT,
    CMD_LOOK,
    CMD_INVENTORY,
    CMD_CONNECT_NBR,
    CMD_BROADCAST,
    CMD_FORK,
    CMD_EJECT,
    CMD_TAKE,
    CMD_SET,
    CMD_INCANTATION,
    CMD_UNKNOWN
} command_type_t;

typedef struct command_s {
    command_type_t type;
    char *args;
    int execution_time;
    struct command_s *next;
} command_t;


#endif /* !PROTOCOL_H_ */
