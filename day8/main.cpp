#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../day8_input.h"


// Helper to use the input string directly as array
// use the input as array, remember to skip the '\n' char
struct Map
{
    Map(const char* input)
    {
        size_t len0 = strchr(input, '\n') - input;
        size_t len = strlen(input);
        width = len0;
        height = (len + 1) / (len0 + 1);
        data = input;
    }

    inline char get(int x, int y) const
    {
        assert(x < width && y < height);
        return data[y * (width + 1) + x] - '0'; // width+1 to skip the '\n' char
    }

    bool hori_visible(int x, int y) const
    {
        char c = get(x, y);
        bool visible = true;
        for (int xx = x + 1; xx < width && visible; ++xx)   // visible from right side?
            visible &= c > get(xx, y);

        if (!visible)
        {
            visible = true;
            for (int xx = x - 1; xx >= 0 && visible; --xx)  // visible from left side?
                visible &= c > get(xx, y);
        }

        return visible;
    }
    bool vert_visible(int x, int y) const
    {
        char c = get(x, y);
        bool visible = true;
        for (int yy = y + 1; yy < height && visible; ++yy) // visible from top side?
            visible &= c > get(x, yy);

        if (!visible)
        {
            visible = true;
            for (int yy = y - 1; yy >= 0 && visible; --yy) // visible from bottom side?
                visible &= c > get(x, yy);
        }

        return visible;
    }

    int scenic_score(int x, int y)
    {
        int c = get(x, y);

        int visible_right = 0;
        for (int i = x+1; i < width; ++i)
        {
            ++visible_right;
            if (get(i, y) >= c) break;
        }

        int visible_left = 0;
        for (int i = x - 1; i >= 0; --i)
        {
            ++visible_left;
            if (get(i, y) >= c) break;
        }

        int visible_top = 0;
        for (int i = y - 1; i >= 0; --i)
        {
            ++visible_top;
            if (get(x, i) >= c) break;
        }

        int visible_bottom = 0;
        for (int i = y + 1; i < height; ++i)
        {
            ++visible_bottom;
            if (get(x, i) >= c) break;
        }

        return visible_right * visible_left * visible_top * visible_bottom;
    }

    const char* data;
    size_t width;
    size_t height;
};


void printScenicScore(Map& map, int x, int y)
{
    int scenic_score = map.scenic_score(x, y);
    printf("(%d,%d) = %d\n", x, y, scenic_score);
}


int main()
{    
    // Init array
    Map map(day8_input);

    // Part1
    {
        size_t visible_tree = (map.width * map.height) - ((map.width - 2) * (map.height - 2));
        for (int y = 1; y < map.height - 1; ++y)
        {
            for (int x = 1; x < map.width - 1; ++x)
            {
                if (map.hori_visible(x, y) || map.vert_visible(x, y))
                {
                    ++visible_tree;
                }
            }
        }
        printf("Visible trees = %zd\n", visible_tree);
    }

    // Part2
    {            
        int best_scenic_score = 0;
        int best_x = 1, best_y = 1;
        for (int y = 1; y < map.height - 1; ++y)
        {
            for (int x = 1; x < map.width - 1; ++x)
            {
                int scenic_score = map.scenic_score(x, y);
                if (scenic_score > best_scenic_score)
                {
                    best_x = x;
                    best_y = y;
                    best_scenic_score = scenic_score;
                }
            }
        }

        printf("Best place (%d,%d) = %d\n", best_x, best_y, best_scenic_score);
    }

    return 0;
}