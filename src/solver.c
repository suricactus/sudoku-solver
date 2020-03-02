#include "sudoku.c"
#include "iofile.c"

int ROWS_COUNT = 9;
int COLS_COUNT = 9;
int DISTINCT_NUMBERS = 9;
int BOX_TILES_COUNT = 9;
int BOXES_COUNT = 9;

int main(int argc, char** argv) 
{
	if (argc > 2)
	{
		perror("Too many arguments, expected as single argument to sudoku definition");
		return 1;
	}

	int ** puzzle;

	if (argc == 2)
	{
		Node * puzzle_n = read_sudoku_file(argv[1]);

		if (puzzle_n == NULL)
		{
			perror("Unable to load sudoku!");

			return 2;
		}

		if (list_len(puzzle_n) != ROWS_COUNT * COLS_COUNT)
		{
			perror("Wrong number of values in the input file!");

			return 3;
		}

		puzzle = create_puzzle_from_list(puzzle_n);
	}
	else 
	{
		puzzle = create_puzzle();
	}

	Sudoku * sudoku = parse_puzzle(puzzle);

	print_sudoku(sudoku);

	int step = 0;

	do {
		printf("Step %d\n", ++step);
		print_sudoku(sudoku);
	} while (solve_sudoku(sudoku));

	if (is_sudoku_solved(sudoku))
	{
		printf("Yuppy, the sudoku is solved!\n");
	}
	else
	{
		printf("Jeez, this is hard!\n");
	}

	return 0;
}
