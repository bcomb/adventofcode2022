#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../day6_input.h"
#include <algorithm>

int main(int argc, char* argv[])
{
    const int marker_size = 14; // 4 for part 1 & 14 for part 2

    const char* input = day6_input - 1;
    int char_read = marker_size - 1;
    do
    {
        ++char_read;
        ++input;

        char seq[marker_size];
        memcpy(seq, input, marker_size);
        std::sort(seq, seq + marker_size);
        char* it = std::unique(seq, seq + marker_size);
        bool all_unique = (it - seq) == marker_size;
        if (all_unique)
            break;
    } while (*(input + marker_size) != '\0');

    printf("Read char before code : %d\n", char_read);
    return 0;
}