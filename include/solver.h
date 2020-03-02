#pragma once
#include <stdio.h>
#include <stdlib.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

extern int ROWS_COUNT;
extern int COLS_COUNT;
extern int DISTINCT_NUMBERS;
extern int BOX_TILES_COUNT;
extern int BOXES_COUNT;


typedef struct Box
{
	struct Tile ** tiles;
} Box;

typedef struct Row
{
	struct Tile ** tiles;
} Row;

typedef struct Col
{
	struct Tile ** tiles;
} Col;


typedef struct Tile
{
	int number;
	/*
		[1] [1] [1] [1] [1] [1] [1] [1] [1]
		[1] [2] [3] [4] [5] [6] [7] [8] [9]
	 */
	int positions[9];
	int within_box_idx;
	int box_idx;
	Box * box;
	Row * row;
	Col * col;
	int x;
	int y;
	int is_initial;
} Tile;


typedef struct Sudoku
{
	struct Tile *** tiles;
	struct Box ** boxes;
	struct Row ** rows;
	struct Col ** cols;
} Sudoku;

int ** create_puzzle();

Box ** create_boxes();
Col ** create_cols();
Row ** create_rows();

Sudoku * parse_puzzle(int ** puzzle);

void update_sudoku(Sudoku * sudoku, Tile * tile);
void print_sudoku(Sudoku * sudoku);
int solve_sudoku(Sudoku * sudoku);
int solve_sudoku_simple(Sudoku * sudoku);
int solve_sudoku_rowcol(Sudoku * sudoku);
int solve_sudoku_box(Sudoku * sudoku);
int is_sudoku_solved(Sudoku * sudoku);
int solve_tile(Tile * tile);
int check_solvable(Tile * tile);
int is_tile_solvable(Tile * tile);

