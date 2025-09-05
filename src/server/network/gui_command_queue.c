/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** gui_command_queue
*/

#include "../../../include/command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gui_command_queue_t *init_gui_command_queue(void)
{
    gui_command_queue_t *queue = malloc(sizeof(gui_command_queue_t));

    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    return queue;
}

int add_gui_command_to_queue(gui_command_queue_t *queue, const char *command)
{
    gui_command_t *new_command = malloc(sizeof(gui_command_t));

    if (!new_command)
        return -1;
    new_command->command = strdup(command);
    if (!new_command->command) {
        free(new_command);
        return -1;
    }
    new_command->next = NULL;
    if (queue->tail)
        queue->tail->next = new_command;
    else
        queue->head = new_command;
    queue->tail = new_command;
    queue->count++;
    return 0;
}

gui_command_t *get_next_gui_command(gui_command_queue_t *queue)
{
    return queue->head;
}

void remove_gui_command_from_queue(gui_command_queue_t *queue,
    gui_command_t *cmd)
{
    if (!queue->head || !cmd)
        return;
    if (queue->head == cmd) {
        queue->head = cmd->next;
        if (!queue->head)
            queue->tail = NULL;
    }
    free(cmd->command);
    free(cmd);
    queue->count--;
}

void cleanup_gui_command_queue(gui_command_queue_t *queue)
{
    gui_command_t *current = queue->head;
    gui_command_t *next;

    while (current) {
        next = current->next;
        free(current->command);
        free(current);
        current = next;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
}
