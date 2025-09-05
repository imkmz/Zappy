/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** vision_plus
*/

#include "../../../include/game.h"

static void append_to_buffer(char *buffer, const char *content)
{
    size_t buffer_len = strlen(buffer);
    size_t available_space = 256 - buffer_len - 1;

    if (available_space > 0) {
        strncat(buffer, content, available_space);
    }
}

static void append_resources_on_tile(inventory_t *tile, char *buffer)
{
    const char *resources[] = {"food ", "linemate ", "deraumere ", "sibur ",
        "mendiane ", "phiras ", "thystame "};
    int quantities[] = {tile->food, tile->linemate, tile->deraumere,
        tile->sibur, tile->mendiane, tile->phiras, tile->thystame};

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < quantities[i]; j++)
            append_to_buffer(buffer, resources[i]);
}

static void get_tile_objects(game_t *game, int x, int y, char *buffer)
{
    inventory_t tile = game->map.tiles[y][x];
    size_t len;

    buffer[0] = '\0';
    for (int i = 0; i < game->player_count; i++)
        if (game->players[i].pos.x == x && game->players[i].pos.y == y)
            append_to_buffer(buffer, "player ");
    append_resources_on_tile(&tile, buffer);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == ' ')
        buffer[len - 1] = '\0';
}

static void get_tile_coords_ne(vision_context_t *vp, int *tx, int *ty)
{
    player_t *p = vp->player;
    game_t *game = vp->game;
    int row = vp->row;
    int offset = vp->offset;

    if (p->orientation == 1) {
        *tx = (p->pos.x + offset + game->map.width) % game->map.width;
        *ty = (p->pos.y - row + game->map.height) % game->map.height;
    }
    if (p->orientation == 2) {
        *tx = (p->pos.x + row + game->map.width) % game->map.width;
        *ty = (p->pos.y + offset + game->map.height) % game->map.height;
    }
}

static void get_tile_coords_sw(vision_context_t *vp, int *tx, int *ty)
{
    player_t *p = vp->player;
    game_t *game = vp->game;
    int row = vp->row;
    int offset = vp->offset;

    if (p->orientation == 3) {
        *tx = (p->pos.x - offset + game->map.width) % game->map.width;
        *ty = (p->pos.y + row + game->map.height) % game->map.height;
    }
    if (p->orientation == 4) {
        *tx = (p->pos.x - row + game->map.width) % game->map.width;
        *ty = (p->pos.y - offset + game->map.height) % game->map.height;
    }
}

static void append_tile_content(result_context_t *ctx, const char *content)
{
    size_t len = strlen(content);

    if (*(ctx->pos) > 1 && *(ctx->pos) < ctx->max_size - 1) {
        ctx->result[*ctx->pos] = ',';
        (*(ctx->pos))++;
    }
    if (*(ctx->pos) + len < ctx->max_size - 1) {
        strcpy(ctx->result + *(ctx->pos), content);
        *(ctx->pos) += len;
    }
}

static void get_tile_content(vision_context_t *vp, int tx, int ty,
    char *buffer)
{
    buffer[0] = '\0';
    get_tile_objects(vp->game, tx, ty, buffer);
}

static char *initialize_result(void)
{
    char *result = malloc(266);

    result[0] = '[';
    result[1] = '\0';
    return result;
}

static void process_row(vision_context_t *vp, result_context_t *ctx,
    char *buffer)
{
    int tx;
    int ty;

    for (int offset = -vp->row; offset <= vp->row; ++offset) {
        vp->offset = offset;
        get_tile_coords_ne(vp, &tx, &ty);
        get_tile_coords_sw(vp, &tx, &ty);
        get_tile_content(vp, tx, ty, buffer);
        append_tile_content(ctx, buffer);
    }
}

char *get_player_vision(game_t *game, player_t *p)
{
    char buffer[256];
    char *result = initialize_result();
    size_t pos = 1;
    vision_context_t vp = { .player = p, .game = game };
    result_context_t ctx = { .result = result, .pos = &pos, .max_size = 256 };

    for (int row = 0; row <= p->level; ++row) {
        vp.row = row;
        process_row(&vp, &ctx, buffer);
    }
    result[pos] = ']';
    pos++;
    result[pos] = '\n';
    pos++;
    result[pos] = '\0';
    return result;
}
