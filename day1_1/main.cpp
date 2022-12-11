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
    unsigned current_elfid = 0;
    unsigned current_calories = 0;
    unsigned max_calories_per_elf[3] = {};   // keep it sorted in descendent order

    const char* input = day1_input;
    do
    {
        int length = lineLength(input);
        if (length == 0) // blank, empty line
        {
            for (int i = 0; i < 3; ++i)
            {
                if (current_calories > max_calories_per_elf[i])
                {
                    // shift and take the place
                    memmove(max_calories_per_elf + i + 1, max_calories_per_elf + i, sizeof(unsigned) * (2 - i));
                    max_calories_per_elf[i] = current_calories;
                    break;
                }
            }

            current_calories = 0;
        }
        else
        {
            int value = atoi(input);
            current_calories += value;
        }

        input += length + 1;

    } while (*(input - 1) != '\0');

    unsigned max_calories = max_calories_per_elf[0] + max_calories_per_elf[1] + max_calories_per_elf[2];
    printf("Result (%d)\n", max_calories);
    return 0;
}