/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** broadcast
*/

#include "../../../../include/common.h"
#include "../../../../include/protocol.h"
#include "../../../../include/command.h"

void cmd_connect_nbr(player_t *player, int available_slots)
{
    char response[32];

    if (!player)
        return;
    sprintf(response, "%d\n", available_slots);
    send_responsefd(player->socket_fd, response);
}

char *broadcast_message(const char *text, int direction)
{
    char *message = malloc(strlen(text) + 32);

    if (!message)
        return NULL;
    sprintf(message, "message %d, %s\n", direction, text);
    return message;
}

static int calculate_last_directions(int dx, int dy)
{
    if (dx > 0 && dy < 0)
        return 2;
    if (dx > 0 && dy > 0)
        return 4;
    if (dx < 0 && dy > 0)
        return 6;
    return 8;
}

static int calculate_direction(position_t from, position_t to, map_t *map)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    if (abs(dx) > map->width / 2)
        dx = dx > 0 ? dx - map->width : dx + map->width;
    if (abs(dy) > map->height / 2)
        dy = dy > 0 ? dy - map->height : dy + map->height;
    if (dx == 0 && dy == 0)
        return 0;
    if (dy < 0 && abs(dy) >= abs(dx))
        return 1;
    if (dx > 0 && abs(dx) >= abs(dy))
        return 3;
    if (dy > 0 && abs(dy) >= abs(dx))
        return 5;
    if (dx < 0 && abs(dx) >= abs(dy))
        return 7;
    return (calculate_last_directions(dx, dy));
}

static void is_msg_formatted(char *formatted_msg, player_t *receiver)
{
    if (formatted_msg) {
        send_responsefd(receiver->socket_fd, formatted_msg);
        free(formatted_msg);
    }
}

void cmd_broadcast(game_t *game, player_t *sender, const char *text)
{
    player_t *receiver;
    int direction = 0;
    char *formatted_msg;

    if (!game || !sender || !text) {
        send_responsefd(sender->socket_fd, RESPONSE_KO);
        return;
    }
    for (int i = 0; i < game->player_count; i++) {
        receiver = &game->players[i];
        if (receiver && receiver->id != sender->id) {
            direction = calculate_direction(receiver->pos,
                sender->pos, &game->map);
            formatted_msg = broadcast_message(text, direction);
            is_msg_formatted(formatted_msg, receiver);
        }
    }
    handle_gui_pbc(game, game->gui_client, sender->id, text);
    send_responsefd(sender->socket_fd, RESPONSE_OK);
}

void send_responsefd(int socket_fd, const char *message)
{
    size_t len;
    ssize_t sent;

    if (socket_fd < 0 || !message)
        return;
    len = strlen(message);
    sent = write(socket_fd, message, len);
    if (sent < 0)
        perror("send_response: write failed");
}
