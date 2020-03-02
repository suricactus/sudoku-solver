#include "solver.h"

int is_tile_solvable(Tile * tile)
{
    int sum = 0;

    for (int i = 0; i < ROWS_COUNT; i++)
    {
        sum += tile->positions[i];

        if (sum > 1) return 0;
    }
    
    return sum == 0 ? 0 : 1;
}

int solve_tile(Tile * tile)
{
    if ( ! is_tile_solvable(tile)) return 0;

    for (int x = 0; x < ROWS_COUNT; x++)
    {
        if (tile->positions[x] == 1)
        {
            tile->number = x + 1;
            tile->positions[x] = 0;

            return 1;
        }
    }

    return 0;
}

int check_if_resolves_to(Tile ** tiles, Tile * curr_tile, int solution)
{
    if (solution <= 0 || solution > DISTINCT_NUMBERS) return 0;

    int sum = 0;

    for (int t = 0; t < BOX_TILES_COUNT; t++)
    {
        if (curr_tile == tiles[t]) continue;
        if (tiles[t]->number == solution) return 0;
        if (tiles[t]->positions[solution - 1]) return 0;
    }

    return 1;
}

int set_solution(Tile * tile, int solution)
{
    if (solution <= 0 || solution > DISTINCT_NUMBERS) return 0;

    tile->number = solution;

    for (int d = 0; d < DISTINCT_NUMBERS; d++)
    {
        tile->positions[d] = 0;
    }

    tile->positions[solution - 1] = 1;

    return 1;
}