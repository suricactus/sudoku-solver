#include "solver.h"


Box ** create_boxes()
{
    Box ** boxes = malloc(sizeof(Box) * BOXES_COUNT);

    for (int x = 0; x < 9; x++)
    {
        boxes[x] = malloc(sizeof(Box));
        boxes[x]->tiles = malloc(sizeof(Tile *) * BOX_TILES_COUNT);
    }
    
    return boxes;
}