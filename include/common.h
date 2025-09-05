/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** common
*/

#ifndef COMMON_H_
    #define COMMON_H_
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct {
    inventory_t inventory;
    position_t pos;
    int id;
    int last_eat;
    char team_name[50];
    int level;
    int socket_fd;
    int orientation;
    int is_in_incantation;
    int dead;
} player_t;

typedef struct {
    int width;
    int height;
    inventory_t **tiles;
} map_t;

#endif /* !COMMON_H_ */
