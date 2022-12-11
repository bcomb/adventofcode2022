#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../day4_input.h"


struct Range
{
    uint8_t min;
    uint8_t max;

    // Check if r is inside the Range
    bool inside(const Range& r) const
    {
        return r.min >= min && r.max <= max;
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
    if(end == nullptr)
        end = strchr(input, '\0');
    return end;
}

int main(int argc, char* argv[])
{
    const char* input = day4_input;
    int fully_contain_count = 0;
    do
    {
        Range r0, r1;
        input = getRanges(input, r0, r1);
        fully_contain_count += r0.inside(r1) || r1.inside(r0);
        ++input; // skipt last char
    } while (*(input - 1) != '\0');

    printf("result= %d", fully_contain_count);

    return 0;
}