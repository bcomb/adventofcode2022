#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../day1_input.h"

int main(int argc, char* argv[])
{
    uint32_t elfid = 0;
    uint32_t max_calories = 0;

    uint32_t current_elfid = 0;
    uint32_t current_calories = 0;

    // If current calories > max_calories
    auto checkCalories = [&]()
    {
        if (current_calories > max_calories)
        {
            max_calories = current_calories;
            elfid = current_elfid;
        }
    };

    const char* r = day1_input;
    do
    {
        const char* next = strchr(r, '\n');
        if (next == nullptr)    // Not found (read the last number)
            next = strchr(r, '\0');
        if (r == next) // blank, empty line
        {
            printf("Elf %d : %d\n", current_elfid, current_calories);
            checkCalories();

            ++current_elfid;
            current_calories = 0;
            ++r;
        }
        else
        {            
            int value = atoi(r);
            current_calories += value;
            r = next + 1;
        }

    } while(*r != '\0');
    
    // Don't forget the last elf count
    checkCalories();

    printf("The Elf (%d) have max calories (%d)\n", elfid, max_calories);
    
    return 0;
}