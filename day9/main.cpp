#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <conio.h>
#include "../day9_input.h"

#define DRAW 0              // draw during computation or not
#define DRAW_PLAYER 0       // interactive player at the end (+/- to seek, 0 to begin, 1 to end)

struct Vec2
{
    int x, y;
    inline Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    inline Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2 operator-(const Vec2& o) const { return Vec2{ x - o.x, y - o.y }; }
    Vec2 operator+(const Vec2& o) const { return Vec2{ x + o.x, y + o.y }; }
};

int sqr_dist(const Vec2& a, const Vec2& b)
{
    int x = (a.x - b.x);
    int y = (a.y - b.y);
    return x * x + y * y;
}

// Tail to far from Head when euclidian dist >= 2
bool tooFar(const Vec2& a, const Vec2& b)
{
    return sqr_dist(a,b) >= 4;
}

int manhattan(const Vec2& a, const Vec2& b)
{
    int x = (a.x - b.x);
    int y = (a.y - b.y);
    return abs(x) + abs(y);
}

struct Rope
{
    Rope()
    {
        head = tail = {0, 0};
        head_history.push_back(head);
        tail_history.push_back(tail);
    }

    Vec2 head;
    Vec2 tail;

    void move(char c, int num)
    {
        Vec2 dir;
        switch (c)
        {
        case 'D': dir = { 0,-1}; break;
        case 'U': dir = { 0, 1}; break;
        case 'R': dir = { 1, 0 }; break;
        case 'L': dir = {-1, 0 }; break;
        default: assert(false && "unknown");
        }

        for (int i = 0; i < num; ++i)
        {
            Vec2 previous_head = head;
            head += dir;
            if (tooFar(head, tail))
            {
                tail = previous_head;
            }

            head_history.push_back(head);
            tail_history.push_back(tail);
        }
    }

    std::vector<Vec2> head_history;
    std::vector<Vec2> tail_history;
};


const int kKnotCount = 10;
struct ChainRope
{
    ChainRope()
    {
        auto a = sizeof(knot);
        auto b = sizeof(knot[0]);
        memset(knot, 0, sizeof(knot));

        for(int i=0; i < kKnotCount; ++i)
            history[i].push_back(Vec2{0,0});        
    }

    Vec2 knot[kKnotCount];

    void move(char c, int num)
    {
        Vec2 dir;
        switch (c)
        {
        case 'D': dir = { 0,-1 }; break;
        case 'U': dir = { 0, 1 }; break;
        case 'R': dir = { 1, 0 }; break;
        case 'L': dir = { -1, 0 }; break;
        default: assert(false && "unknown");
        }

        auto follow = [](Vec2& parent, Vec2& child)
        {
            if (abs(parent.y - child.y) >= 2) {
                child.y += (parent.y - child.y) / 2;

                if (abs(parent.x - child.x) >= 2)
                    child.x += (parent.x - child.x) / 2;
                else
                    child.x += parent.x - child.x;
            }
            else if (abs(parent.x - child.x) >= 2) {
                child.x += (parent.x - child.x) / 2;
                child.y += parent.y - child.y;
            }
        };


        for (int i = 0; i < num; ++i)
        {
            knot[0] += dir;
            for (int k = 1; k < kKnotCount; ++k)
                follow(knot[k - 1], knot[k]);

            for (int k = 0; k < kKnotCount; ++k)
                history[k].push_back(knot[k]);

#if DRAW
            draw(history[0].size() - 1);

#endif
        }
    }

    void move2(char c, int num)
    {
        Vec2 dir;
        switch (c)
        {
        case 'D': dir = { 0,-1 }; break;
        case 'U': dir = { 0, 1 }; break;
        case 'R': dir = { 1, 0 }; break;
        case 'L': dir = { -1, 0 }; break;
        default: assert(false && "unknown");
        }

        for (int i = 0; i < num; ++i)
        {
            Vec2 previous_knot[kKnotCount];
            memcpy(previous_knot, knot, sizeof(knot));

            Vec2 previous_head = knot[0];
            knot[0] += dir;
            if (tooFar(knot[0], knot[1]))
            {
                knot[1] = previous_head;
            }
            
            
            for (int j = 2; j < kKnotCount; ++j)
            {
                Vec2 t = knot[j-1] - previous_knot[j-1];
                if (tooFar(knot[j], knot[j - 1]))
                {
                    if (manhattan(knot[j], knot[j - 1]) == 3)
                    {
                        knot[j] += t;
                    }
                    else
                    {
                        knot[j] = previous_knot[j - 1];
                    }
                    
                }
            }

            for (int i = 0; i < kKnotCount; ++i)
                history[i].push_back(knot[i]);
        }
    }

    void draw(size_t seek)
    {
        //system("cls");

        // Num of char in your console
        const int width = 120;
        const int height = 30;

        char map[width * height + 1];
        for (int i = 0; i < sizeof(map); ++i)
        {
            map[i] = ' ';
        }
        map[sizeof(map) - 1] = '\0';
        Vec2 middle = { width /2, height/2 };

        auto set_map = [&](Vec2 pos, char c)
        {
            pos += middle;                  // center at middle
            pos.y = height - pos.y;         // vertical flip
            int offset = pos.y * width + pos.x;
            //assert(offset >= 0 && offset < sizeof(map) - 1);
            if (offset >= 0 && offset < sizeof(map) - 1) // avoid oob on big sample
            {
                map[offset] = c;
            }
        };


        // draw Start point
        set_map(history[0][0], 's');

        // draw rope
        for (int i = kKnotCount - 1 ; i >= 0; --i)
        {
            set_map(history[i][seek], (i == 0) ? 'H' : '0' + i);
        }

        // draw tail history
        const auto& tail_history = history[kKnotCount - 1];
        for (int i = 0; i < std::min(tail_history.size(), seek); ++i)
        {
            set_map(tail_history[i], '#');
        }

        printf("%s", map);
    }

    std::vector<Vec2> history[kKnotCount];
};

const char* nextline(const char* input)
{
    while (!(*input == '\n' || *input == '\0'))
        ++input;
    return input + 1;
}

void part1()
{
    Rope rope;
    const char* input = day9_input;
    do
    {
        char order = input[0];
        int num = atoi(input + 2);
        rope.move(order, num);

        input = nextline(input);
    } while (*(input - 1) != '\0');

    std::sort(rope.tail_history.begin(), rope.tail_history.end(), [](const Vec2& a, const Vec2& b) { return std::tie(a.x, a.y) < std::tie(b.x, b.y); });
    auto lToRemove = std::unique(rope.tail_history.begin(), rope.tail_history.end(), [](const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; });
    rope.tail_history.erase(lToRemove, rope.tail_history.end());

    printf("Part 1 Unique tail position %zd\n", rope.tail_history.size());
}

void part2()
{
    ChainRope rope;
    const char* input = day9_input;
    do
    {
        char order = input[0];
        int num = atoi(input + 2);
        rope.move(order, num);

        input = nextline(input);
    } while (*(input - 1) != '\0');


    // Mini interactive player
#if DRAW_PLAYER
    int seek = 0;
    rope.draw(seek);
    int c;
    do
    {
        seek = std::min((int)rope.history[0].size()-1, std::max(seek, 0));
        rope.draw(seek);
        c = _getch();
        switch (c)
        {
        case '0': seek = 0; break;
        case '1': seek = (int)rope.history[0].size() - 1; break;
        case '+': ++seek; break;
        case '-': --seek; break;
        }        
        
    } while (c != 27); // escape
#endif

    auto& tail_history = rope.history[kKnotCount - 1];
    std::sort(tail_history.begin(), tail_history.end(), [](const Vec2& a, const Vec2& b) { return std::tie(a.x, a.y) < std::tie(b.x, b.y); });
    auto lToRemove = std::unique(tail_history.begin(), tail_history.end(), [](const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; });
    tail_history.erase(lToRemove, tail_history.end());

    printf("Part 2 : Unique tail position %zd\n", tail_history.size());
}



int main()
{
    part1();
    part2();
    return 0;
}