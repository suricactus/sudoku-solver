#include "solver.h"
#include "tile.c"
#include "box.c"
#include "row.c"
#include "col.c"
#include "node.c"

Sudoku * parse_puzzle(int ** puzzle)
{
    Sudoku * sudoku = malloc(sizeof(Sudoku));
    sudoku->tiles = malloc(sizeof(Tile **) * ROWS_COUNT);
    sudoku->boxes = create_boxes();
    sudoku->rows = create_rows();
    sudoku->cols = create_cols();

    int within_box_idxs[9] = { 0 };

    for (int y = 0; y < ROWS_COUNT; y++)
    {
        sudoku->tiles[y] = malloc(sizeof(Tile *) * ROWS_COUNT);

        for (int x = 0; x < COLS_COUNT; x++)
        {
            sudoku->tiles[y][x] = (Tile *) malloc(sizeof(Tile) * COLS_COUNT);
            sudoku->tiles[y][x]->number = puzzle[y][x];
            sudoku->tiles[y][x]->is_initial = puzzle[y][x] > 0 ? 1 : 0;
            sudoku->tiles[y][x]->y = y;
            sudoku->tiles[y][x]->x = x;
            sudoku->tiles[y][x]->box_idx = (int) y / 3 * 3 + ((int) x / 3);
            sudoku->tiles[y][x]->within_box_idx = within_box_idxs[sudoku->tiles[y][x]->box_idx]++;
            sudoku->tiles[y][x]->box = sudoku->boxes[sudoku->tiles[y][x]->box_idx];
            sudoku->tiles[y][x]->box->tiles[sudoku->tiles[y][x]->within_box_idx] = sudoku->tiles[y][x];
            sudoku->tiles[y][x]->col = sudoku->cols[x];
            sudoku->tiles[y][x]->col->tiles[y] = sudoku->tiles[y][x];
            sudoku->tiles[y][x]->row = sudoku->rows[y];
            sudoku->tiles[y][x]->row->tiles[x] = sudoku->tiles[y][x];

            for (int z = 0; z < COLS_COUNT; z++)
            {
                if (sudoku->tiles[y][x]->number == 0)
                {
                    sudoku->tiles[y][x]->positions[z] = 1;
                }
                else
                {
                    if (sudoku->tiles[y][x]->number == z + 1)
                    {
                        sudoku->tiles[y][x]->positions[z] = 1;
                    }
                    else
                    {
                        sudoku->tiles[y][x]->positions[z] = 0;
                    }
                }
            }
        }
    }

    for (int y = 0; y < ROWS_COUNT; y++)
    {
        for (int x = 0; x < COLS_COUNT; x++) 
        {
            if (sudoku->tiles[y][x]->number != 0)
            {
                update_sudoku(sudoku, sudoku->tiles[y][x]);
            }
        }
    }
    
    return sudoku;
}

void update_sudoku(Sudoku * sudoku, Tile * tile)
{
    int pos_idx = tile->number - 1;

    if (tile->number == 0) return;

    for (int y = 0; y < ROWS_COUNT; y++)
    {
        if (y == tile->y) continue;

        tile->row->tiles[y]->positions[pos_idx] = 0;
    }

    for (int x = 0; x < COLS_COUNT; x++)
    {
        if (x == tile->x) continue;

        tile->col->tiles[x]->positions[pos_idx] = 0;
    }

    for (int b = 0; b < BOXES_COUNT; b++)
    {
        if (b == tile->within_box_idx) continue;

        tile->box->tiles[b]->positions[pos_idx] = 0;
    }
    
}

int solve_sudoku(Sudoku * sudoku)
{
    return solve_sudoku_simple(sudoku)
        || solve_sudoku_box(sudoku)
        || solve_sudoku_rowcol(sudoku);
}

int solve_sudoku_simple(Sudoku * sudoku)
{
    int is_solved = 0;

    for (int y = 0; y < ROWS_COUNT; y++)
    {
        for (int x = 0; x < COLS_COUNT; x++)
        {
            if (is_tile_solvable(sudoku->tiles[y][x]))
            {
                solve_tile(sudoku->tiles[y][x]);
                update_sudoku(sudoku, sudoku->tiles[y][x]);

                is_solved = 1;
                return is_solved;
            }
        }
    }

    return is_solved;
}

int solve_sudoku_box(Sudoku * sudoku)
{
    int is_solved = 0;

    for (int b = 0; b < BOXES_COUNT; b++) 
    {
        Box * box = sudoku->boxes[b];

        for (int t = 0; t < BOX_TILES_COUNT; t++)
        {
            Tile * tile = box->tiles[t];

            // skip already assigned tiles
            if (tile->number != 0) continue;

            for (int d = 0; d < DISTINCT_NUMBERS; d++)
            {
                // if there is no chance for given number, skip
                if (tile->positions[d] == 0) continue;
                if (check_if_resolves_to(box->tiles, tile, d + 1)) {
                    set_solution(tile, d + 1);
                    update_sudoku(sudoku, tile);

                    is_solved = 1;
                    return is_solved;
                }
            }
            
            box->tiles[t]->positions;
        }
    }

    return is_solved;
}


int solve_sudoku_rowcol(Sudoku * sudoku)
{
    int is_solved = 0;

    for (int x = 0; x < ROWS_COUNT; x++) 
    {
        for (int y = 0; y < COLS_COUNT; y++)
        {
            Tile * tile = sudoku->tiles[x][y];

            // skip already assigned tiles
            if (tile->number != 0) continue;

            for (int d = 0; d < DISTINCT_NUMBERS; d++)
            {
                // if there is no chance for given number, skip
                if (tile->positions[d] == 0) continue;

                if (check_if_resolves_to(tile->row->tiles, tile, d + 1)) {
                    set_solution(tile, d + 1);
                    update_sudoku(sudoku, tile);

                    is_solved = 1;
                    return is_solved;
                }
                else if (check_if_resolves_to(tile->col->tiles, tile, d + 1))
                {
                    set_solution(tile, d + 1);

                    is_solved = 1;
                    return is_solved;
                }
            }
        }
    }

    return is_solved;
}



int is_sudoku_solved(Sudoku * sudoku)
{
    for (int i = 0; i < ROWS_COUNT; i++)
    {
        for (int j = 0; j < COLS_COUNT; j++)
        {
            if (sudoku->tiles[i][j]->number == 0) return 0;
        }
    }

    return 1;    
}


int ** create_puzzle_from_list(Node * head)
{
    int ** puzzle = malloc(sizeof(int *) * ROWS_COUNT);
    
    for(int i = 0; i < ROWS_COUNT; i++) puzzle[i] = malloc(COLS_COUNT * sizeof(int));

    int idx = 0;

    Node * node = head;

    do
    {
        int row = idx / COLS_COUNT;
        int col = idx % COLS_COUNT;

        idx++;
        
        puzzle[row][col] = node->value;
    }
    while (node = node->next);

    return puzzle;
}

int ** create_puzzle()
{
    int ** puzzle;

    int array[9][9] = {
        0, 1, 9,        0, 0, 2,        0, 0, 0,
        4, 7, 0,        6, 9, 0,        0, 0, 1,
        0, 0, 0,        4, 0, 0,        0, 9, 0,

        8, 9, 4,        5, 0, 7,        0, 0, 0,
        0, 0, 0,        0, 0, 0,        0, 0, 0,
        0, 0, 0,        2, 0, 1,        9, 5, 8,

        0, 5, 0,        0, 0, 6,        0, 0, 0,
        6, 0, 0,        0, 2, 8,        0, 7, 9,
        0, 0, 0,        1, 0, 0,        8, 6, 0,
    };

    puzzle = malloc(sizeof(int *) * ROWS_COUNT);

    for (int i = 0; i < ROWS_COUNT; i++)
    {
        puzzle[i] = malloc(sizeof(int) * COLS_COUNT);

        for (int j = 0; j < COLS_COUNT; j++) 
        {
            puzzle[i][j] = array[i][j];
        }
    }

    return puzzle;
}

void print_sudoku(Sudoku * sudoku)
{
    printf(RED " --------------------------------\n" RESET);

    for (int i = 0; i < ROWS_COUNT; i++)
    {
        printf(RED " | " RESET);

        for (int j = 0; j < COLS_COUNT; j++)
        {
            if (sudoku->tiles[i][j]->is_initial)
            {
                printf(BLU " %d " RESET, sudoku->tiles[i][j]->number);
            }
            else
            {
                if (sudoku->tiles[i][j]->number == 0)
                {
                    printf(RESET "   " RESET);
                }
                else
                {
                    printf(RESET " %d " RESET, sudoku->tiles[i][j]->number);
                }
                

            }

            if ((j + 1) % 3 == 0)
            {
                printf(RED "|" RESET);
            }
            
        }
        
        printf("\n");

        if ((i + 1) % 3 == 0)
        {
            printf(RED " --------------------------------\n" RESET);
        }
    }
}
