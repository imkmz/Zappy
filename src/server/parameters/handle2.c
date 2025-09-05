/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** handle2
*/

#include "../../../include/network.h"
#include <string.h>

int handle_clients(server_config_t *config, char *arg, char **argv)
{
    int clients;

    (void)argv;
    if (!arg || !valid_number(arg)) {
        fprintf(stderr, "Error: Invalid number of clients\n");
        return -1;
    }
    clients = atoi(arg);
    if (clients <= 0 || clients > MAX_CLIENTS_PER_TEAM) {
        fprintf(stderr, "Error: Number of clients must be "
            "between 1 and %d\n", MAX_CLIENTS_PER_TEAM);
        return -1;
    }
    config->clients_per_team = clients;
    return 1;
}

int handle_freq(server_config_t *config, char *arg, char **argv)
{
    int freq;

    (void)argv;
    if (!arg || !valid_number(arg)) {
        fprintf(stderr, "Error: Invalid frequency\n");
        return -1;
    }
    freq = atoi(arg);
    if (freq <= 0) {
        fprintf(stderr, "Error: Frequency must be positive\n");
        return -1;
    }
    config->freq = freq;
    return 1;
}
