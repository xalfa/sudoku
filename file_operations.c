#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_operations.h"

// From https://stackoverflow.com/a/3463793
int read_file_and_fill_array(int arrayToFill[], char path[])
{
    FILE *file;
    int c;
    int i = 0;

    file = fopen(path, "r");
    if (file) {
        while ((c = getc(file)) != EOF){
            if (c != '\n')
            {
                arrayToFill[i] = c - 48;
                i++;
            }
        }
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}
