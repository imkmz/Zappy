/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** parser
*/

#include "../../../include/network.h"
#include "../../../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height -n name1 name2..."
        " -c clientsNb -f freq\n");
}

int valid_number(const char *str)
{
    if (!str || *str == '\0')
        return 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

static int handle_argument(int i, int argc, char **argv, server_config_t *conf)
{
    int consumed;
    char *next_arg;
    char **remaining_args;
    char opt = argv[i][1];
    option_handler_t handler = get_option_handler(opt);

    if (!handler) {
        fprintf(stderr, "Error: Unknown option '-%c'\n", opt);
        return -1;
    }
    next_arg = (i + 1 < argc) ? argv[i + 1] : NULL;
    remaining_args = (i + 2 < argc) ? &argv[i + 2] : NULL;
    consumed = handler(conf, next_arg, remaining_args);
    if (consumed < 0)
        return -1;
    return consumed;
}

static int argument_loop(int argc, char **argv, server_config_t *config)
{
    int consumed;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || strlen(argv[i]) != 2) {
            fprintf(stderr, "Error: Invalid option format '%s'\n", argv[i]);
            return -1;
        }
        consumed = handle_argument(i, argc, argv, config);
        if (consumed < 0)
            return -1;
        i += consumed;
    }
    return 0;
}

int parse_arguments(int argc, char **argv, server_config_t *config)
{
    if (argc < 2) {
        print_usage();
        return -1;
    }
    if (argc == 2 && strcmp(argv[1], "help") == 0) {
        print_usage();
        return 0;
    }
    init_config(config);
    if (argument_loop(argc, argv, config) < 0)
        return -1;
    return validate_config(config);
}
