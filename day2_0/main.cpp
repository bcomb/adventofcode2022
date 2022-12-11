#include <stdio.h>
#include "../day2_input.h"

const int D = 3; // draw
const int W = 6; // win
const int L = 0; // lose
const int scoreTab[3][3] = // scoreTab[Opponent][Player]
{//   R   P   S
    {D+1,W+2,L+3},  // R (opponent play)
    {L+1,D+2,W+3},  // P
    {W+1,L+2,D+3}   // S
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