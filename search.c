#include "maze.h"

#define ELEM(a,b,c) tests[(a) * testnum * SRCNUM + (b) * SRCNUM + (c)]

int *testsearch(int gennum, int testnum)
{
    if (gennum <= 0 || testnum <= 0) return NULL;
    
    int *tests = malloc(gennum * testnum * SRCNUM * sizeof(int));
    if (tests == NULL) return NULL;
    
    Maze m;
    SearchState st;
    int i, j, k, x0, y0, dists[SIZE][SIZE];
    
    for (i = 0; i < gennum; ++i) {
        init_maze(m);
        
        for (k = 0; k < SRCNUM; ++k) {
            init_state(&st, k, m);
        
            for (j = 0; j < testnum; ++j) {
                rand_point(m, &x0, &y0);
                ELEM(i, j, k) = search(m, &st, x0, y0, dists);
            }
        }
    }
    
    return tests;
}

// returns size of queue
int search(Maze m, SearchState *st, int x, int y, int dists[SIZE][SIZE])
{
    // points represented as arrays of size 2, the 3rd element in openset is priority
    int openset[SIZE * SIZE][3] = {0}, n = 0, // openset and index
        preds[SIZE][SIZE][2] = {0}, // needn't assign to NONE as (0,0) will never be a predecessor to some other point
        ns[4][2], nc, // neighbors and count
        res = 0, i, newdist;
    
    for (i = 0; i < SIZE * SIZE; ++i)
        dists[i / SIZE][i % SIZE] = NONE;
    
    push(openset, n++, x, y, 0, st);
    dists[x][y] = 0;
    
    while (n) {
        ++res;
        pop(openset, n--, &x, &y, st);
        
        // already visited
        if (preds[x][y][0] || preds[x][y][1])
            continue;
        
        // found solution
        if (!x && !y) {
            update_state(st, dists);
            return res;
        }
        
        get_neighbors(ns, &nc, x, y, m);
        newdist = dists[x][y] + 1; // all edges are of length 1
        for (i = 0; i < nc; ++i) {
            if (dists[ns[i][0]][ns[i][1]] == NONE || newdist < dists[ns[i][0]][ns[i][1]]) {
                preds[ns[i][0]][ns[i][1]][0] = x;
                preds[ns[i][0]][ns[i][1]][1] = y;
                push(openset, n++, ns[i][0], ns[i][1], dists[ns[i][0]][ns[i][1]] = newdist, st);
            }
        }
    }
    
    return -res; // negative means no solution found
}