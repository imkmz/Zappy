/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** queue
*/

/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** command_queue
*/

#include "../../../../include/command.h"

command_queue_t *init_command_queue(void)
{
    command_queue_t *queue = malloc(sizeof(command_queue_t));

    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    return queue;
}

static void queue_command_management(pending_command_t *new_cmd,
    command_queue_t *queue)
{
    if (queue->tail) {
        queue->tail->next = new_cmd;
        queue->tail = new_cmd;
    } else {
        queue->head = new_cmd;
        queue->tail = new_cmd;
    }
    queue->count++;
}

int add_command_to_queue(command_queue_t *queue, command_type_t type,
    const char *args, int player_id)
{
    pending_command_t *new_cmd;
    command_info_t *cmd_info;

    if (!queue || is_queue_full(queue))
        return -1;
    cmd_info = get_command_info(type);
    if (!cmd_info)
        return -1;
    new_cmd = malloc(sizeof(pending_command_t));
    if (!new_cmd)
        return -1;
    new_cmd->type = type;
    new_cmd->args = args ? strdup(args) : NULL;
    new_cmd->execution_time = cmd_info->execution_time;
    new_cmd->player_id = player_id;
    new_cmd->next = NULL;
    gettimeofday(&new_cmd->start_time, NULL);
    queue_command_management(new_cmd, queue);
    return 0;
}

pending_command_t *get_next_ready_command(command_queue_t *queue, int freq)
{
    pending_command_t *current;
    struct timeval current_time;
    long elapsed_time;
    long required_time;

    if (!queue || !queue->head)
        return NULL;
    current = queue->head;
    gettimeofday(&current_time, NULL);
    elapsed_time = (current_time.tv_sec - current->start_time.tv_sec) * 1000 +
        (current_time.tv_usec - current->start_time.tv_usec) / 1000;
    required_time = (current->execution_time * 1000) / freq;
    if (elapsed_time >= required_time)
        return current;
    return NULL;
}

static pending_command_t *find_command(command_queue_t *queue,
    pending_command_t *cmd, pending_command_t **prev)
{
    pending_command_t *current = queue->head;

    *prev = NULL;
    while (current && current != cmd) {
        *prev = current;
        current = current->next;
    }
    return current;
}

void remove_command_from_queue(command_queue_t *queue, pending_command_t *cmd)
{
    pending_command_t *prev = NULL;
    pending_command_t *current;

    if (!queue || !cmd)
        return;
    current = find_command(queue, cmd, &prev);
    if (!current)
        return;
    if (prev)
        prev->next = current->next;
    else
        queue->head = current->next;
    if (current == queue->tail)
        queue->tail = prev;
    if (current->args)
        free(current->args);
    free(current);
    queue->count--;
}

void cleanup_command_queue(command_queue_t *queue)
{
    pending_command_t *current;
    pending_command_t *next;

    if (!queue)
        return;
    current = queue->head;
    while (current) {
        next = current->next;
        if (current->args)
            free(current->args);
        free(current);
        current = next;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
}
