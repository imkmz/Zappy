/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** parser_get_object
*/

#include "../../../../include/command.h"

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

const char *get_object_name_by_index(int index)
{
    if (index < 0 || index >= 7)
        return NULL;
    return object_names[index];
}

int get_object_index_by_name(const char *object_name)
{
    if (!object_name)
        return -1;
    for (int i = 0; object_names[i] != NULL; i++) {
        if (strcmp(object_name, object_names[i]) == 0)
            return i;
    }
    return -1;
}
