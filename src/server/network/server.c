/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server
*/

#include "../../../include/network.h"
#include "../../../include/protocol.h"

int create_socket(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket failed");
        return -1;
    }
    return server_fd;
}

static int set_socket_options(int server_fd)
{
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }
    return 0;
}

static int bind_socket(int server_fd, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }
    return 0;
}

static int listen_socket(int server_fd)
{
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("listen failedd");
        return -1;
    }
    return 0;
}

int create_server(int port)
{
    int server_fd = create_socket();

    if (server_fd < 0)
        return -1;
    if (set_socket_options(server_fd) < 0) {
        close(server_fd);
        return -1;
    }
    if (bind_socket(server_fd, port) < 0) {
        close(server_fd);
        return -1;
    }
    if (listen_socket(server_fd) < 0) {
        close(server_fd);
        return -1;
    }
    printf("Server listening on port %d\n", port);
    return server_fd;
}

static void init_poll_structure(server_t *server)
{
    server->fds[0].fd = server->server_fd;
    server->fds[0].events = POLLIN;
    server->fds[0].revents = 0;
    for (int i = 1; i < MAX_CONNECTIONS; i++) {
        server->fds[i].fd = -1;
        server->fds[i].events = POLLIN;
        server->fds[i].revents = 0;
    }
}

static void init_clients(server_t *server)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        server->clients[i].socket_fd = -1;
        server->clients[i].buffer_pos = 0;
        server->clients[i].is_gui = 0;
        server->clients[i].player = NULL;
        memset(server->clients[i].buffer, 0, BUFFER_SIZE);
    }
}

int init_server(server_t *server, server_config_t *config)
{
    server->server_fd = create_server(config->port);
    if (server->server_fd < 0)
        return -1;
    server->port = config->port;
    server->client_count = 0;
    init_poll_structure(server);
    init_clients(server);
    printf("Server listening on port %d\n", config->port);
    return 0;
}
