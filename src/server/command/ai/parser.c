/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** parser
*/

#include "../../../../include/command.h"

static const command_info_t command_table[] = {
    {"Forward", CMD_FORWARD, TIME_FORWARD, handle_forward},
    {"Right", CMD_RIGHT, TIME_RIGHT, handle_right},
    {"Left", CMD_LEFT, TIME_LEFT, handle_left},
    {"Look", CMD_LOOK, TIME_LOOK, handle_look},
    {"Inventory", CMD_INVENTORY, TIME_INVENTORY, handle_inventory},
    {"Connect_nbr", CMD_CONNECT_NBR, 0, handle_connect_nbr},
    {"Broadcast", CMD_BROADCAST, TIME_BROADCAST, handle_broadcast},
    {"Fork", CMD_FORK, TIME_FORK, handle_fork},
    {"Eject", CMD_EJECT, TIME_EJECT, handle_eject},
    {"Take", CMD_TAKE, TIME_TAKE, handle_take},
    {"Set", CMD_SET, TIME_SET, handle_set},
    {"Incantation", CMD_INCANTATION, 0, handle_incantation},
    {NULL, CMD_UNKNOWN, 0, NULL}
};

static const char *object_names[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame",
    NULL
};

static command_type_t valid_command(char *first_word)
{
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (strcasecmp(first_word, command_table[i].name) == 0) {
            return command_table[i].type;
        }
    }
    return CMD_UNKNOWN;
}

command_type_t parse_command(const char *command_str)
{
    char *command_copy;
    char *first_word;
    char *space_pos;
    command_type_t cmd_type;

    if (!command_str || strlen(command_str) == 0)
        return CMD_UNKNOWN;
    command_copy = strdup(command_str);
    if (!command_copy)
        return CMD_UNKNOWN;
    space_pos = strchr(command_copy, ' ');
    if (space_pos)
        *space_pos = '\0';
    first_word = trim_whitespace(command_copy);
    cmd_type = valid_command(first_word);
    free(command_copy);
    return cmd_type;
}

static void clean_args(int *len, char **args)
{
    if (args && *args) {
        *len = (int)strlen(*args);
        while (*len > 0 && ((*args)[*len - 1] == ' ' ||
                (*args)[*len - 1] == '\t' || (*args)[*len - 1] == '\n')) {
            (*args)[*len - 1] = '\0';
            (*len)--;
        }
    }
}

char *extract_command_args(const char *command_str)
{
    const char *space_pos;
    char *args = NULL;
    int len = 0;

    if (!command_str)
        return NULL;
    space_pos = strchr(command_str, ' ');
    if (!space_pos)
        return strdup("");
    space_pos++;
    while (*space_pos == ' ' || *space_pos == '\t')
        space_pos++;
    args = strdup(space_pos);
    if (!args)
        return NULL;
    clean_args(&len, &args);
    return args;
}

command_info_t *get_command_info(command_type_t type)
{
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (command_table[i].type == type)
            return (command_info_t *)&command_table[i];
    }
    return NULL;
}

char *trim_whitespace(char *str)
{
    char *end;

    if (!str)
        return str;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

int is_valid(const char *object_name)
{
    if (!object_name)
        return 0;
    for (int i = 0; object_names[i] != NULL; i++) {
        if (strcmp(object_name, object_names[i]) == 0)
            return 1;
    }
    return 0;
}
