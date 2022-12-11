#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../day2_input.h"


const int R = 1; // Rock
const int P = 2; // Paper
const int S = 3; // Scissor

const int D = 3; // draw
const int W = 6; // win
const int L = 0; // lose
const int scoreTab[3][3] = // scoreTab[Opponent][Player]
{//   L   D   W
    {0+S,3+R,6+P},    // R
    {0+R,3+P,6+S},    // P
    {0+P,3+S,6+R}     // S
};

int main(int argc, char* argv[])
{
    unsigned score = 0;
    const char* input = day2_input - 4;
    do
    {
        input += 4;
        int opponent = input[0] - 'A';
        int player = input[2] - 'X';
        score += scoreTab[opponent][player];
    } while (*(input + 3) != '\0');
    printf("Score : %d", score);
    return 0;
}