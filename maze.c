#include "maze.h"

void init_maze(Maze m)
{
    memset(m, 0, sizeof(m)); // clear m
    
    // TODO generate random maze on m, representing obstacles by 1
}

void get_neighbors(int ns[4][2], int *nc, int x, int y, Maze m)
{
    static const int dirs[4][2] = {0, 1, 1, 0, 0, -1, -1, 0};
    int i;
    
    *nc = 0;
    for (i = 0; i < 4; ++i) {
        ns[*nc][0] = x + dirs[i][0];
        ns[*nc][1] = y + dirs[i][1];
        
        // boundary and availability check
        if (ns[*nc][0] >= 0 && ns[*nc][1] >= 0 &&
            ns[*nc][0] < SIZE && ns[*nc][1] < SIZE &&
            !m[ns[*nc][0]][ns[*nc][1]])
            ++*nc;
    }
}