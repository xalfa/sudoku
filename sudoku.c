#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "sudoku.h"
#include "generator.h"

// Static functions
// Fill the check_array with zeros, to reuse the array
static void fill_checkArray(int array[SUDOKU_SIZE])
{
    int i;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        array[i] = 0;
    }
}

// Print the horizontal seperators
static void print_horizontal_seperator()
{
    int i, j;
    for (i = 0; i < SUDOKU_SEPERATOR; i++)
    {
        printf("+");
        for (j = 0; j < SUDOKU_SEPERATOR * 3; j++)
        {
            printf("-");
        }
    }
    printf("+\n");
}

// Check if the rows are correct
static int check_sudoku_rows(SudokuField arraySudoku[][SUDOKU_SIZE])
{
    int i, j, x;
    int checkArray[SUDOKU_SIZE];

    for(i = 0; i < SUDOKU_SIZE; i++)
    {
        fill_checkArray(checkArray);
        for(j = 0; j < SUDOKU_SIZE; j++)
        {
            x = arraySudoku[i][j].value;
            if(x == 0)
            {
                return 1;
            }
            if( checkArray[x-1] == 0)
            {
                checkArray[x-1] = 1;
            }
            else
            {
                return 1;
            }

        }
    }
    return 0;
}

// Check if the columns are correct
static int check_sudoku_cols(SudokuField arraySudoku[][SUDOKU_SIZE])
{
    int i, j, x;
    int checkArray[SUDOKU_SIZE];

    for(i = 0; i < SUDOKU_SIZE; i++)
    {
        fill_checkArray(checkArray);
        for(j = 0; j < SUDOKU_SIZE; j++)
        {
            x = arraySudoku[j][i].value;
            if(x == 0)
            {
                return 1;
            }
            if( checkArray[x-1] == 0)
            {
                checkArray[x-1] = 1;
            }
            else
            {
                return 1;
            }

        }
    }
    return 0;
}

// Check if the boxes are correct
static int check_sudoku_boxes(SudokuField arraySudoku[][SUDOKU_SIZE])
{
    int i, j, k, l, x;
    int checkArray[SUDOKU_SIZE];

    //i and j point to the first field in every 3x3 box
    //k and l iterate through every field in the box
    for(i = 0; i < SUDOKU_SIZE; i = i + SUDOKU_SEPERATOR)
    {
        for(j = 0; j < SUDOKU_SIZE; j = j + SUDOKU_SEPERATOR)
        {
            fill_checkArray(checkArray);
            for(k = 0; k < SUDOKU_SEPERATOR; k++)
            {
                for(l = 0; l < SUDOKU_SEPERATOR; l++)
                {
                    x = arraySudoku[i+k][j+l].value;
                    if(x == 0)
                    {
                        return 1;
                    }
                    if( checkArray[x-1] == 0)
                    {
                        checkArray[x-1] = 1;
                    }
                    else
                    {
                        return 1;
                    }

                }
            }
        }
    }
    return 0;
}

// Functions

// Use the generate library to generate a sudoku
void generateSudoku(SudokuField arraySudoku[][SUDOKU_SIZE], int difficulty)
{
    int exit = 0;

    do
    {
        createRandomPuzzle(arraySudoku);

        if (solve(arraySudoku))
        {
            exit = 1;
        }
        else
        {
            fill_sudoku(arraySudoku);
        }
    }
    while (!exit);
    createEmptyFields(arraySudoku, difficulty);
}

// Fill a sudoku with zeros
void fill_sudoku(SudokuField arraySudoku[SUDOKU_SIZE][SUDOKU_SIZE])
{
    int i, j;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            arraySudoku[i][j].value = 0;
        }
    }
}

// Shows the user the sudoku, possibly error messages and the menu
void print_sudoku(SudokuField arraySudoku[SUDOKU_SIZE][SUDOKU_SIZE], Cursor cursor, char message[])
{
    int i, j;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        if(i%SUDOKU_SEPERATOR == 0)
        {
            print_horizontal_seperator();
        }
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if(j%SUDOKU_SEPERATOR == 0)
            {
                printf("|");
            }
            if (i == cursor.y && j == cursor.x)
            {
                SetConsoleTextAttribute(HCONSOLE, 42);
            }
            if(arraySudoku[i][j].value != 0)
            {
                if (arraySudoku[i][j].value > 9)
                {
                    printf("%d ", arraySudoku[i][j].value);
                }
                else
                {
                    printf(" %d ", arraySudoku[i][j].value);
                }
            }
            else
            {
                printf(" . ");
            }
            SetConsoleTextAttribute(HCONSOLE, 7);
        }

        printf("|\n");
    }
    print_horizontal_seperator();
    printf("\n%s\n", message);
    printf("\nInput:\n"
           "Use the Arrowkeys or WASD to move.\n"
           "[c] to check your solution\n"
           "[S] to save your game\n"
           "[x] to exit the game\n\n");
}

// Check if the sudoku is correct
int check_sudoku(SudokuField arraySudoku[SUDOKU_SIZE][SUDOKU_SIZE])
{
    int isMissingRow, isMissingCols, isMissingBoxes;

    isMissingRow = check_sudoku_rows(arraySudoku);
    isMissingCols = check_sudoku_cols(arraySudoku);
    isMissingBoxes = check_sudoku_boxes(arraySudoku);

    if (isMissingRow || isMissingCols || isMissingBoxes)
    {
        return 0;
    }

    return 1;
}

// Sets the editability of the fields if they are not specified
void set_editable(SudokuField arraySudoku[SUDOKU_SIZE][SUDOKU_SIZE])
{
    int i, j;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if(arraySudoku[i][j].value == 0)
            {
                arraySudoku[i][j].isEditable = 1;
            }
            else
            {
                arraySudoku[i][j].isEditable = 0;
            }

        }
    }
}

// Moves the cursor to its new location while checking if the cursor is out of bounds
void check_and_move_sudoku_cursor(Cursor *cursor)
{
    // Up
    if (cursor->y < 0)
    {
        cursor->y = SUDOKU_SIZE - 1;
    }

    // Down
    if(cursor->y >= SUDOKU_SIZE)
    {
        cursor->y = 0;
    }

    // Left
    if(cursor->x < 0)
    {
        cursor->x = SUDOKU_SIZE - 1;
    }

    // Right
    if(cursor->x >= SUDOKU_SIZE)
    {
        cursor->x = 0;
    }
}

/*
    Using the property of integers to only return integers when divided
    this loop copies a 1 dimensional array into a 2 dimensional array.
*/
void import_to_sudoku(SudokuField arraySudoku[SUDOKU_SIZE][SUDOKU_SIZE], int arrayImport[])
{
    int i;
    for (i = 0; i < SUDOKU_BOARD_SIZE + 1; i++)
    {
        arraySudoku[i/SUDOKU_SIZE][i%SUDOKU_SIZE].value = arrayImport[i];
    }
}
