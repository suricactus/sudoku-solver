#include "solver.h"
#include "node.c"

Node * read_sudoku_file(char * fname)
{
    FILE * fp = fopen(fname, "r");
    Node * head = NULL;
    Node * last = NULL;

    if (fp == NULL)
    {    
        perror("Error opening file!");
        fclose(fp);
    }

    while (1) {
        int x;
        int count = fscanf(fp, "%d", &x);

        if (count == EOF) break;

        if (count == 0) 
        {
            // Quietly consume 1 non-numeric character
            fgetc(fp);
            continue;
        }

        if (head == NULL)
        {
            head = malloc(sizeof(struct Node));
            last = head;

            last->value = x;
        }
        else
        {
            last = push_right(last, x);
        }
    }

    fclose(fp);

    return head;
}
