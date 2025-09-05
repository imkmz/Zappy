/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** client
*/

#include "../../../include/network.h"
#include "../../../include/command.h"
#include "../../../include/protocol.h"
#include <stdbool.h>

static int accept_client_socket(int server_fd)
{
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    if (new_socket < 0) {
        perror("accept failed");
        return -1;
    }
    return new_socket;
}

static int init_client(server_t *server, int socket_fd)
{
    client_t *client;

    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        if (server->clients[i].socket_fd == -1) {
            client = &server->clients[i];
            client->socket_fd = socket_fd;
            client->buffer_pos = 0;
            client->is_gui = 0;
            client->player = NULL;
            client->index = i;
            memset(client->buffer, 0, BUFFER_SIZE);
            server->fds[i].fd = socket_fd;
            server->fds[i].events = POLLIN;
            server->fds[i].revents = 0;
            return i;
        }
    }
    return -1;
}

bool should_continue_on_send_error(void)
{
    if (errno == EAGAIN || errno == EWOULDBLOCK)
        return true;
    perror("send failed");
    return false;
}

int send_message_to_client(client_t *client, const char *message)
{
    int len = strlen(message);
    int sent = 0;
    int result;

    if (client->socket_fd < 0)
        return -1;
    while (sent < len) {
        result = send(client->socket_fd, message + sent,
            len - sent, MSG_NOSIGNAL);
        if (result >= 0) {
            sent += result;
            continue;
        }
        if (!should_continue_on_send_error())
            return -1;
    }
    return sent;
}

int accept_new_client(server_t *server)
{
    int new_socket = accept_client_socket(server->server_fd);
    int client_index;

    if (new_socket < 0)
        return -1;
    client_index = init_client(server, new_socket);
    if (client_index == -1) {
        printf("Maximum connections reached\n");
        close(new_socket);
        return -1;
    }
    server->client_count++;
    send_message_to_client(&server->clients[client_index], WELCOME_MSG);
    printf("New client connected (socket %d)\n", new_socket);
    return client_index;
}

void disconnect_client(server_t *server, int index, game_t *game)
{
    client_t *client = &server->clients[index];

    if (client->socket_fd >= 0) {
        if (client->is_gui)
            game->gui_client = NULL;
        printf("Client disconnected (socket %d)\n", client->socket_fd);
        close(client->socket_fd);
        client->socket_fd = -1;
        client->buffer_pos = 0;
        client->is_gui = 0;
        client->player = NULL;
        memset(client->buffer, 0, BUFFER_SIZE);
        server->fds[index].fd = -1;
        server->fds[index].events = 0;
        server->fds[index].revents = 0;
        server->client_count--;
    }
}

static int read_client_data(client_t *client)
{
    int available_space = BUFFER_SIZE - client->buffer_pos - 1;
    int bytes_read;

    if (available_space <= 0) {
        printf("Buffer full for client %d\n", client->socket_fd);
        return -1;
    }
    bytes_read = recv(client->socket_fd, client->buffer + client->buffer_pos,
        available_space, 0);
    if (bytes_read <= 0) {
        if (bytes_read == 0)
            printf("Client %d disconnected\n", client->socket_fd);
        else
            perror("recv failed");
        return -1;
    }
    client->buffer_pos += bytes_read;
    client->buffer[client->buffer_pos] = '\0';
    return 0;
}

static int process_commands_in_buffer(server_t *server, int client_index,
    game_t *game)
{
    client_t *client = &server->clients[client_index];
    char *newline_pos;
    char command[BUFFER_SIZE];
    int command_len = 0;

    newline_pos = strchr(client->buffer, '\n');
    while (newline_pos != NULL) {
        command_len = newline_pos - client->buffer;
        strncpy(command, client->buffer, command_len);
        command[command_len] = '\0';
        memmove(client->buffer, newline_pos + 1,
            client->buffer_pos - command_len - 1);
        client->buffer_pos -= (command_len + 1);
        client->buffer[client->buffer_pos] = '\0';
        if (process_client_command(server, client_index, command, game) < 0)
            return -1;
        newline_pos = strchr(client->buffer, '\n');
    }
    return 0;
}

int handle_client_data(server_t *server, int client_index, game_t *game)
{
    client_t *client = &server->clients[client_index];

    if (read_client_data(client) < 0) {
        if (client->player)
            remove_player(game, client->player->id);
        if (!client->player)
            disconnect_client(server, client_index, game);
        return -1;
    }
    if (process_commands_in_buffer(server, client_index, game) < 0) {
        if (client->player)
            remove_player(game, client->player->id);
        if (!client->player)
            disconnect_client(server, client_index, game);
        return -1;
    }
    return 0;
}
