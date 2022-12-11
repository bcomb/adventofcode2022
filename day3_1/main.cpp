#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../day3_input.h"

unsigned itemCount(const char* input)
{
    unsigned count = 0;
    while (!(*input == '\n' || *input == '\0'))
    {
        ++count;
        ++input;
    }
    return count;
}

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

char findBadge(const char* input, int count0, int count1, int count2)
{
    const char* bag0 = input;
    const char* bag1 = input + count0 + 1; // skip '\n'
    const char* bag2 = input + count0 + count1 + 2;
    for (int i = 0; i < count0; ++i)
    {
        for (int j = 0; j < count1; ++j)
        {
            if (bag0[i] == bag1[j])
            {
                for (int k = 0; k < count2; ++k)
                {
                    if (bag1[j] == bag2[k])
                        return bag2[k];
                }
            }
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
    do
    {
        int item_count0 = itemCount(input);
        int item_count1 = itemCount(input + item_count0 + 1);
        int item_count2 = itemCount(input + item_count0 + item_count1 + 2);

        char c = findBadge(input, item_count0, item_count1, item_count2);
        sum += c < 'a' ? c - 'A' + 27 : c - 'a' + 1;
        input += item_count0 + item_count1 + item_count2 + 3;        
    } while (*(input - 1) != '\0');

    printf("result= %d", sum);

    return 0;
}