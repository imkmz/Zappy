/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** main server
*/

#include "../../include/network.h"
#include "../../include/protocol.h"
#include "../../include/config.h"
#include "../../include/game.h"
#include <signal.h>

static void print_usage(const char *program_name)
{
    printf("USAGE: %s -p port -x width -y height -n name1 name2 ... "
        "-c clientsNb -f freq\n", program_name);
    printf("    -p port      port number\n");
    printf("    -x width     width of the world\n");
    printf("    -y height    height of the world\n");
    printf("    -n name1 name2 ...  name of the team\n");
    printf("    -c clientsNb number of authorized clients per team\n");
    printf("    -f freq      reciprocal of time unit for "
        "execution of actions\n");
}

static int is_help_request(int argc, char **argv)
{
    if (argc == 2) {
        if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
            strcmp(argv[1], "--help") == 0) {
            return 1;
        }
    }
    return 0;
}

sig_atomic_t *get_or_set_running(sig_atomic_t *new_value)
{
    static sig_atomic_t running = 0;

    if (new_value != NULL)
        running = *new_value;
    return &running;
}

void signal_handler(int sig)
{
    sig_atomic_t new_value = 0;

    if (sig == SIGINT || sig == SIGTERM)
        get_or_set_running(&new_value);
}

static void setup_signal_handlers(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);
}

static void print_server_config(const server_config_t *config)
{
    printf("Zappy Server Configuration:\n");
    printf("  Port: %d\n", config->port);
    printf("  World size: %dx%d\n", config->width, config->height);
    printf("  Teams: ");
    for (int i = 0; i < config->team_count; i++)
        printf("%s%s", config->teams[i],
            (i < config->team_count - 1) ? ", " : "\n");
    printf("  Clients per team: %d\n", config->clients_per_team);
    printf("  Frequency: %d\n\n", config->freq);
}

static int error_gestion(server_config_t *config, int argc, char **argv)
{
    init_config(config);
    if (parse_arguments(argc, argv, config) != 0) {
        fprintf(stderr, "Error: Invalid arguments\n");
        print_usage(argv[0]);
        return 84;
    }
    if (validate_config(config) != 0) {
        fprintf(stderr, "Error: Invalid configuration\n");
        return 84;
    }
    return 0;
}

int prepare_server_config(int argc, char **argv, server_t *server,
    server_config_t *config)
{
    sig_atomic_t running = 1;

    if (argc < 11) {
        fprintf(stderr, "Error: Not enough arguments\n");
        print_usage(argv[0]);
        return 84;
    }
    if (error_gestion(config, argc, argv) == 84)
        return 84;
    memset(server, 0, sizeof(server_t));
    if (init_server(server, config) < 0) {
        fprintf(stderr, "Error: Failed to initialize server\n");
        return 84;
    }
    setup_signal_handlers();
    get_or_set_running(&running);
    return 0;
}

static int server_down(int result, server_t *server)
{
    cleanup_server(server);
    free(server);
    if (result < 0) {
        fprintf(stderr, "Server encountered an error\n");
        return 84;
    }
    printf("Server shutdown complete\n");
    return 0;
}

int main(int argc, char **argv)
{
    server_config_t config;
    server_t *server;
    int result;

    if (is_help_request(argc, argv)) {
        print_usage(argv[0]);
        return 0;
    }
    server = malloc(sizeof(server_t));
    if (!server) {
        fprintf(stderr, "Error: Failed to allocate memory for server\n");
        return 84;
    }
    if (prepare_server_config(argc, argv, server, &config) == 84) {
        free(server);
        return 84;
    }
    print_server_config(&config);
    result = run_server(server, &config);
    return server_down(result, server);
}
