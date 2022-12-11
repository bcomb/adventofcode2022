#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../day4_input.h"


struct Range
{
    uint8_t min;
    uint8_t max;

    // Check if 'r' overlap this (care if this is fully inside 'r' return false)
    bool overlap(const Range& r) const
    {
        return (r.min >= min && r.min <= max) || (r.max >= min && r.max <= max);
    }
};

// Parse line and return pointer on the last char of the line
const char* getRanges(const char* input, Range& r0, Range& r1)
{
    r0.min = atoi(input);
    input = strchr(input, '-') + 1;
    r0.max = atoi(input);
    input = strchr(input, ',') + 1;
    r1.min = atoi(input);
    input = strchr(input, '-') + 1;
    r1.max = atoi(input);

    // Finished by '\n' or '\0'
    const char* end = strchr(input, '\n');
    if (end == nullptr)
        end = strchr(input, '\0');
    return end;
}

int main(int argc, char* argv[])
{
    const char* input = day4_input;
    int overlap_count = 0;
    do
    {
        Range r0, r1;
        input = getRanges(input, r0, r1);
        overlap_count += r0.overlap(r1) || r1.overlap(r0);
        ++input; // skipt last char
    } while (*(input - 1) != '\0');

    printf("result= %d", overlap_count);

    return 0;
}