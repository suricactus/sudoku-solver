#include "solver.h"


Row ** create_rows()
{
    Row ** rows = malloc(sizeof(Row) * ROWS_COUNT);

    for (int x = 0; x < ROWS_COUNT; x++)
    {
        rows[x] = malloc(sizeof(Row));
        rows[x]->tiles = malloc(sizeof(Tile *) * COLS_COUNT);
    }
    
    return rows;
}
