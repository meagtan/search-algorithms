#include "maze.h"
#include <math.h> // sqrt
#include <string.h> // memset
#include <stdlib.h> // malloc, free, rand, srand
#include <time.h> // time, for srand
#include <stdio.h>

#define alloc_arr(arr, typ, row, col, val) do { \
arr = malloc((row) * sizeof(typ *)); \
    for (int i = 0; i < (row); ++i) { \
        (arr)[i] = malloc((col) * sizeof(typ)); \
        for (int j = 0; j < (col); ++j) \
            (arr)[i][j] = (val); \
    } \
} while(0)

static const int dirs[4][2] = {0, 1, 1, 0, 0, -1, -1, 0};

// TODO problem: visits every point
void init_maze(Maze *m)
{
    // generate random maze on m, modification of depth-first search
    int **stack, n = 0,
        **visited,
        x, y, x1, y1,
        ns[4][2], nc, 
        c = LOOP_OFFSET + rand_num(LOOP_RANGE), i, k;
    SearchState st;
    
    alloc_arr(*m, char, SIZE, SIZE, 1); // every cell initially obstacle
    alloc_arr(stack, int, SIZE * SIZE, 3, 0);
    alloc_arr(visited, int, SIZE, SIZE, 0);
    
    init_state(&st, DFS, *m);
    push(stack, n++, 0, 0, 0, &st);
    
    x = y = 0;
    visited[x][y] = 1;
    (*m)[x][y] = 0;
    while (n) {
        // x = stack[n-1][0];
        // y = stack[n-1][1];
        
        // look for unvisited neighbors
        // TODO perhaps check for 2-neighbors, then clear 1-neighbors
        for (nc = 0, i = 0; i < 4; ++i) {
            x1 = x + dirs[i][0];
            y1 = y + dirs[i][1];
            if (x1 >= 0 && x1 < SIZE &&
                y1 >= 0 && y1 < SIZE &&
                (!visited[x1][y1] || 
                (!(--c) && (c = LOOP_OFFSET + rand_num(LOOP_RANGE))))) { // create loops every c checks
                ns[nc][0] = x1;
                ns[nc][1] = y1;
                ++nc;
            }
        }
        
        if (nc) {
            // choose neighbor at random
            k = rand_num(nc);
            push(stack, n++, x = ns[k][0], y = ns[k][1], 0, &st);
            // mark it empty and visited
            (*m)[x][y] = 0;
            visited[x][y] = 1;
        } else {
            pop(stack, n--, &x, &y, &st);
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            free(stack[i * SIZE + j]);
        }
        free(visited[i]);
    }
    free(stack);
    free(visited);
}

void free_maze(Maze m)
{
    for (int i = 0; i < SIZE; ++i)
        free(m[i]);
    free(m);
}

void init_state(SearchState *st, int k, Maze m)
{
    st->type = k >= 6 ? k - 4 : k;
    st->iter = k >= 6;
    // for other types the heuristic is either unused or calculated directly
    alloc_arr(st->heur, int, SIZE, SIZE, 0);
    if (st->type == ASL) {
        // calculate heuristic for each point using Dijkstra's algorithm
        SearchState s1;
        int x, y, i, j, **dists, heur;
        
        alloc_arr(dists, int, SIZE, SIZE, -1);
        init_state(&s1, UCS, m);
        
        rand_point(m, &x, &y);
        search(m, &s1, x, y, dists); // needn't check whether result is negative, maze contains spanning tree
        
        for (i = 0; i < SIZE; ++i) {
            for (j = 0; j < SIZE; ++j) {
                if (dists[i][j] != NONE) {
                    heur = dists[0][0] - dists[i][j];
                    st->heur[i][j] = heur < 0 ? -heur : heur;
                }
            }
        }
        
        for (i = 0; i < SIZE; ++i)
            free(dists[i]);
        free(dists);
        free_state(&s1);
    }
}

void free_state(SearchState *st)
{
    for (int i = 0; i < SIZE; ++i)
        free(st->heur[i]);
    free(st->heur);
}

void update_state(SearchState *st, int **dists)
{
    int i, j, a;

    if (st->iter) {
        for (i = 0; i < SIZE; ++i) {
            for (j = 0; j < SIZE; ++j) {
                if (dists[i][j] != NONE) {
                    a = dists[0][0] - dists[i][j];
                    if (a > st->heur[i][j])
                        st->heur[i][j] = a;
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

void rand_point(Maze m, int *x, int *y)
{
    do {
        *x = rand_num(SIZE);
        *y = rand_num(SIZE);
    } while (m[*x][*y] == NONE);
}

void get_neighbors(int ns[4][2], int *nc, int x, int y, Maze m)
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

int rand_num(int max)
{
    // seed rand once
    static int _a = 1;
    if (_a) { 
        srand(time(NULL)); 
        _a = 0;
    }
    return rand() % max; // may not be uniform
}
