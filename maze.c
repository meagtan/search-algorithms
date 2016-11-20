#include "maze.h"

void init_maze(Maze m)
{
    memset(m, 1, sizeof(m)); // clear m and set each cell to be an obstacle
    
    // generate random maze on m, modification of depth-first search
    int stack[SIZE*SIZE][3], visited[SIZE][SIZE] = {0}, n = 0, x, y, ns[4][2], nc, i, k;
    SearchState st;
    
    init_state(&st, DFS, NULL); // do not need to pass m
    push(stack, n++, 0, 0, 0, &st);
    
    x = y = 0;
    visited[x][y] = 1;
    while (n) {
        // x = stack[n-1][0];
        // y = stack[n-1][1];
        
        // look for unvisited neighbors
        for (nc = 0, i = 0; i < 4; ++i) {
            // TODO also add a disjunct that is true with frequency p, in order to create loops sporadically
            if (!visited[x + dir[i][0]][y + dir[i][1]]) {
                ns[nc][0] = x + dir[i][0];
                ns[nc][1] = y + dir[i][1];
                ++nc;
            }
        }
        
        if (nc) {
            // choose neighbor at random
            k = random(nc);
            push(stack, n++, ns[k][0], ns[k][1], 0, &st);
            // mark it empty and visited
            m[ns[k][0]][ns[k][1]] = 0;
            visited[ns[k][0]][ns[k][1]] = 1;
        } else {
            pop(stack, n--, &x, &y, &st);
        }
    }
}

void init_state(SearchState *st, int k, Maze m)
{
    st->type = k >= 6 ? k - 4 : k;
    st->iter = k >= 6;
    // for other types the heuristic is either unused or calculated directly
    if (type == ASL) {
        // TODO calculate heuristic for each point using Dijkstra's algorithm
        SearchState s1;
        int x, y, i, j, dists[SIZE][SIZE];
        
        init_state(&s1, UCS, NULL); // needn't pass m for UCS
        // TODO choose x and y
        search(m, &s1, x, y, dists); // TODO check whether result is negative
        
        for (i = 0; i < SIZE; ++i) {
            for (j = 0; j < SIZE; ++j) {
                if (dists[i][j] != NONE && dists[x][y] > dists[i][j]) {
                    st->heur[i][j] = dists[x][y] - dists[i][j];
                }
            }
        }
    }
}

void update_state(SearchState *st, int dists[][])
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

void get_neighbors(int ns[][], int *nc, int x, int y, Maze m)
{
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