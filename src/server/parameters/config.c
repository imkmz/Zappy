/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** config
*/

#include "../../../include/network.h"
#include "../../../include/config.h"
#include <stdio.h>

void init_config(server_config_t *config)
{
    config->port = 0;
    config->width = 0;
    config->height = 0;
    config->team_count = 0;
    config->clients_per_team = 0;
    config->freq = DEFAULT_FREQ;
    for (int i = 0; i < MAX_TEAMS; i++)
        memset(config->teams[i], 0, MAX_TEAM_NAME);
}

int validate_config(server_config_t *config)
{
    if (config->port == 0) {
        fprintf(stderr, "Error: Port is required\n");
        return -1;
    }
    if (config->width == 0) {
        fprintf(stderr, "Error: Width is required\n");
        return -1;
    }
    if (config->height == 0) {
        fprintf(stderr, "Error: Height is required\n");
        return -1;
    }
    if (config->team_count == 0)
        return -1;
    if (config->clients_per_team == 0) {
        fprintf(stderr, "Error: Number of clients per team is required\n");
        return -1;
    }
    return 0;
}
