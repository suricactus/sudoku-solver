#include "solver.h"


Col ** create_cols()
{
    Col ** rows = malloc(sizeof(Col) * COLS_COUNT);

    for (int x = 0; x < COLS_COUNT; x++)
    {
        rows[x] = malloc(sizeof(Col));
        rows[x]->tiles = malloc(sizeof(Tile *) * ROWS_COUNT);
    }
    
    return rows;
}
