#include "maze.h"

void init_maze(Maze m)
{
    memset(m, 0, sizeof(m)); // clear m
    
    // TODO generate random maze on m, representing obstacles by 1
}

void init_state(SearchState *st, int k)
{
    st->type = k >= 6 ? k - 4 : k;
    st->iter = k >= 6;
    // for other types the heuristic is either unused or calculated directly
    if (type == ASL) {
        // TODO calculate heuristic for each point using Dijkstra's algorithm
    }
}

void update_state(SearchState *st, int dists[SIZE][SIZE])
{
    int i, j, a;
    if (st->iter) {
        for (i = 0; i < SIZE; ++i) {
            for (j = 0; j < SIZE; ++j) {
                if (dists[i][j] != NONE) {
                    a = dists[0][0] - dists[i][j];
                    if (a > heur[i][j])
                        heur[i][j] = a;
                }
            }
        }
    }
}

int heur(SearchState *st, int x, int y)
{
    if (st->type < UCS) return 0;
    
    int res;
    switch (st->type) {
        case UCS: res = 0; break;
        case ASE: res = sqrt(x * x + y * y); break;
        case ASM: res = x + y; break;
        case ASL: res = st->heur[x][y]; break; // can just return it too
    }
    if (st->iter && res < st->heur[x][y])
        res = st->heur[x][y];
    return res;
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