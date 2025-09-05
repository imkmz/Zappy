/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** inventory
*/

#include "../../../../include/common.h"
#include "../../../../include/protocol.h"
#include "../../../../include/command.h"


char *format_inventory(player_t *player)
{
    char *result = malloc(512);
    inventory_t *inv;

    if (!result || !player)
        return NULL;
    inv = &player->inventory;
    snprintf(result, 512, "[food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        inv->food, inv->linemate, inv->deraumere, inv->sibur,
        inv->mendiane, inv->phiras, inv->thystame);
    return result;
}
