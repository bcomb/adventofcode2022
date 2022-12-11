#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../day1_input.h"

unsigned lineLength(const char* input)
{
    unsigned count = 0;
    while (!(*input == '\n' || *input == '\0'))
    {
        ++count;
        ++input;
    }
    return count;
}

int main(int argc, char* argv[])
{
    unsigned max_calories = 0;
    unsigned current_elfid = 0;
    unsigned current_calories = 0;

    const char* input = day1_input;
    do
    {
        int length = lineLength(input);        
        if (length == 0) // blank, empty line
        {
            if (current_calories > max_calories)
                max_calories = current_calories;

            current_calories = 0;
        }
        else
        {            
            int value = atoi(input);
            current_calories += value;
        }

        input += length + 1;

    } while(*(input-1) != '\0');
    
    printf("Result (%d)\n", max_calories);    
    return 0;
}