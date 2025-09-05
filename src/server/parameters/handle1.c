/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** handle1
*/

#include "../../../include/network.h"
#include <string.h>

const option_map_t option_handlers[] = {
    {'p', handle_port},
    {'x', handle_width},
    {'y', handle_height},
    {'n', handle_teams},
    {'c', handle_clients},
    {'f', handle_freq},
    {0, NULL}
};

option_handler_t get_option_handler(char opt)
{
    for (int i = 0; option_handlers[i].handler; i++) {
        if (option_handlers[i].opt == opt)
            return option_handlers[i].handler;
    }
    return NULL;
}

int handle_port(server_config_t *config, char *arg, char **argv)
{
    int port;

    (void)argv;
    if (!arg || !valid_number(arg)) {
        fprintf(stderr, "Error: Invalid port number\n");
        return -1;
    }
    port = atoi(arg);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Error: Port must be between 1 and 65535\n");
        return -1;
    }
    config->port = port;
    return 1;
}

int handle_width(server_config_t *config, char *arg, char **argv)
{
    int width;

    (void)argv;
    if (!arg || !valid_number(arg)) {
        fprintf(stderr, "Error: Invalid width\n");
        return -1;
    }
    width = atoi(arg);
    if (width <= 0) {
        fprintf(stderr, "Error: Width must be positive\n");
        return -1;
    }
    config->width = width;
    return 1;
}

int handle_height(server_config_t *config, char *arg, char **argv)
{
    int height;

    (void)argv;
    if (!arg || !valid_number(arg)) {
        fprintf(stderr, "Error: Invalid height\n");
        return -1;
    }
    height = atoi(arg);
    if (height <= 0) {
        fprintf(stderr, "Error: Height must be positive\n");
        return -1;
    }
    config->height = height;
    return 1;
}

int handle_teams(server_config_t *config, char *arg, char **argv)
{
    int consumed = 1;

    if (!arg) {
        fprintf(stderr, "Error: No team names provided\n");
        return -1;
    }
    strncpy(config->teams[config->team_count], arg, MAX_TEAM_NAME - 1);
    config->teams[config->team_count][MAX_TEAM_NAME - 1] = '\0';
    config->team_count++;
    while (argv && *argv && **argv != '-' && config->team_count < MAX_TEAMS) {
        strncpy(config->teams[config->team_count], *argv, MAX_TEAM_NAME - 1);
        config->teams[config->team_count][MAX_TEAM_NAME - 1] = '\0';
        config->team_count++;
        consumed++;
        argv++;
    }
    if (config->team_count == 0)
        return -1;
    return consumed;
}
