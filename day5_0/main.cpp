#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../day5_input.h"
#include <vector>

typedef std::vector<std::vector<char>> Supplies;

// Parse supply and return a pointer on the first instruction
const char* parseSupplies(const char* input, Supplies& s)
{
    const char* end_first_line = strchr(input, '\n');
    size_t line_length = end_first_line - input + 1;
    int num_stack = (int)(line_length / 4);

    s.resize(num_stack);

    input += 1; // Point on the crate char

    while (*input != '1')
    {
        // read line    
        for (int i = 0; i < num_stack; ++i, input += 4)
        {
            char crate = *input;
            if (crate != ' ')
                s[i].push_back(crate);
        }
    }

    input = strchr(input, '\n');
    input += 2; // skip '\n\n'

    return input;
}

// Execute the instruction and return a pointer on the next instruction
const char* execute(const char* input, Supplies& s)
{
    // Parsing
    int move = atoi(input + 4);
    input = strchr(input + 4, 'm') + 1; // point after the 'm' of 'from'
    int from = atoi(input) - 1;
    input = strchr(input, 'o') + 1; // point after the 'o' of 'to'
    int to = atoi(input) - 1;
    const char* end_line = strchr(input, '\n');
    if(end_line == nullptr)
        end_line = strchr(input, '\0');

    // Execute
    s[to].insert(s[to].begin(), move, ' ');
    for (int i = 0; i < move; ++i)
    {        
        s[to][i] = s[from][move-i-1];
    }
    s[from].erase(s[from].begin(), s[from].begin() + move);

    return end_line + 1;
}

int main(int argc, char* argv[])
{
    const char* input = day5_input_sample;

    Supplies s;
    input = parseSupplies(input, s);
    do
    {
        input = execute(input, s);
    } while (*(input - 1) != '\0');
    
    char str[16] = {};
    for (size_t i = 0; i < s.size(); ++i)
    {
        str[i] = s[i][0];
    }

    printf("Top crate : %s\n", str);
    return 0;
}