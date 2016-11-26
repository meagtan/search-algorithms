#include "maze.h"
#include <stdlib.h>

#define ELEM(a,b,c) tests[(a) * testnum * SRCNUM + (b) * SRCNUM + (c)]
#define alloc_arr(arr, typ, row, col, val) do { \
    arr = malloc((row) * sizeof(typ *)); \
    for (int i = 0; i < (row); ++i) { \
        (arr)[i] = malloc((col) * sizeof(typ)); \
        for (int j = 0; j < (col); ++j) \
            (arr)[i][j] = (val); \
    } \
} while (0)

int *testsearch(int gennum, int testnum)
{
    if (gennum <= 0 || testnum <= 0) return NULL;
    
    int *tests = malloc(gennum * testnum * SRCNUM * sizeof(int));
    if (tests == NULL) return NULL;
    
    Maze m;
    SearchState st;
    int i, j, k, x0, y0, **dists;
    alloc_arr(dists, int, SIZE, SIZE, NONE);
    
    for (i = 0; i < gennum; ++i) {
        init_maze(&m);
        
        for (k = 0; k < SRCNUM; ++k) {
            init_state(&st, k, m);
        
            for (j = 0; j < testnum; ++j) {
                rand_point(m, &x0, &y0);
                ELEM(i, j, k) = search(m, &st, x0, y0, dists);
            }
            
            free_state(&st);
        }
        free_maze(m);
    }
    
    return tests;
}

// returns size of queue
int search(Maze m, SearchState *st, int x, int y, int **dists)
{
    // TODO allocate these arrays dynamically
    // points represented as arrays of size 2, the 3rd element in openset is priority
    int **openset, n = 0, // openset and index
        ***preds, // needn't assign to NONE as (0,0) will never be a predecessor to some other point
        **visited,
        ns[4][2], nc, // neighbors and count
        res = 0, i, newdist;
    alloc_arr(openset, int, SIZE * SIZE, 3, 0);
    alloc_arr(visited, int, SIZE, SIZE, 0);
    preds = malloc(SIZE * sizeof(int **));
    for (int i = 0; i < SIZE; ++i) {
        preds[i] = malloc(SIZE * sizeof(int *));
        for (int j = 0; j < SIZE; ++j) {
            preds[i][j] = malloc(3 * sizeof(int));
            for (int k = 0; k < 3; ++k)
                preds[i][j][k] = 0;
        }
    } \
    
    for (i = 0; i < SIZE * SIZE; ++i)
        dists[i / SIZE][i % SIZE] = NONE;
    
    push(openset, n++, x, y, 0, st);
    dists[x][y] = 0;
    
    while (n) {
        ++res;
        pop(openset, n--, &x, &y, st);
        
        // already visited
        if (visited[x][y])
            continue;
        visited[x][y] = 1;
        
        // found solution
        if (!x && !y) {
            update_state(st, dists);
            
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    free(preds[i][j]);
                    free(openset[i * SIZE + j]);
                }
                free(preds[i]);
                free(visited[i]);
            }
            free(openset);
            free(preds);
            free(visited);
                
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
    
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            free(preds[i][j]);
            free(openset[i * SIZE + j]);
        }
        free(preds[i]);
        free(visited[i]);
    }
    free(openset);
    free(preds);
    free(visited);
    
    return -res; // negative means no solution found
}
