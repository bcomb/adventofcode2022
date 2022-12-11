#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../day1_input.h"

int main(int argc, char* argv[])
{
    uint32_t elfid[3];
    uint32_t max_calories_per_elf[3] = {};   // keep it sorted in descendent order


    uint32_t current_elfid = 0;
    uint32_t current_calories = 0;

    // If current calories > max_calories
    auto checkCalories = [&]()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (current_calories > max_calories_per_elf[i])
            {
                // shift and take the place
                memmove(max_calories_per_elf + i + 1, max_calories_per_elf + i, sizeof(uint32_t) * (2 - i));
                memmove(elfid + i + 1, elfid + i, sizeof(uint32_t) * (2 - i));
                elfid[i] = current_elfid;
                max_calories_per_elf[i] = current_calories;
                break;
            }               
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

    printf("Result : %d\n", max_calories_per_elf[0] + max_calories_per_elf[1] + max_calories_per_elf[2]);
    
    return 0;
}