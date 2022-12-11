#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../day3_input.h"

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

// Find the item present in the first and second slice
// Slice1 [input, count/2], Slice2 [input+count/2, count/2]
char wrongItem(const char* input, int count)
{
    int half_size = count >> 1;
    for (int i = 0; i < half_size; ++i)
    {        
        for (int j = 0; j < half_size; ++j)
        {
            if (input[i] == input[half_size + j])
                return input[i];
        }
    }

    assert(false); // should not happen with this data
    return '\0';
}

int main(int argc, char* argv[])
{
    unsigned score = 0;
    const char* input = day3_input;
    unsigned sum = 0;
    unsigned item_count;
    do
    {      
        item_count = lineLength(input);
        char c = wrongItem(input, item_count);
        sum += c < 'a' ? c - 'A' + 27: c - 'a' + 1;
        input += item_count + 1;
    } while (*(input - 1) != '\0');

    printf("result= %d", sum);
    
    return 0;
}