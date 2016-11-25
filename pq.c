#include "maze.h"

void siftdown(int **, int, int);
void siftup(int **, int, int);

void push(int **openset, int n, int x, int y, int dist, SearchState *st)
{
    // if DFS, openset is a stack, so push and pop from end
    // if BFS, openset is a queue, so push at end and pop from start
    // else, openset is a priority queue with priority dist + heur(st, x, y)
    openset[n][0] = x;
    openset[n][1] = y;
    openset[n][2] = dist + heur(st, x, y);
    
    if (st->type > BFS)
        siftup(openset, 0, n);
}

void pop(int **openset, int n, int *x, int *y, SearchState *st)
{
    if (n <= 0) return;
    
    if (st->type == DFS) {
        // pop from end
        *x = openset[n-1][0];
        *y = openset[n-1][1];
        return;
    }
    
    *x = openset[0][0];
    *y = openset[0][1];
    
    if (st->type == BFS) {
        for (int i = 0; i < n - 1; ++i) {
            openset[i][0] = openset[i+1][0];
            openset[i][1] = openset[i+1][1];
            openset[i][2] = openset[i+1][2];
        }
    } else if (n > 1) {
        openset[0][0] = openset[n-1][0];
        openset[0][1] = openset[n-1][1];
        openset[0][2] = openset[n-1][2];
        siftdown(openset, 0, n - 1);
    }
}

// consecutive swaps could be optimized
void siftdown(int **openset, int start, int end)
{
    int child = 2 * start + 1, temp, i;
    while (child < end) {
        // set child to index of smaller child
        if (child + 1 < end && openset[child + 1][2] < openset[child][2])
            ++child;
        
        // if child smaller than start, swap start with child
        if (openset[start][2] <= openset[child][2])
            break;
        for (i = 0; i < 3; ++i) {
            temp = openset[start][i];
            openset[start][i] = openset[child][i];
            openset[child][i] = temp;
        }
        
        start = child;
        child = 2 * child + 1;
    }
}

void siftup(int **openset, int start, int end)
{
    int parent, i, temp;
    while (end > start) {
        parent = (end - 1) >> 1;
        if (openset[parent][2] <= openset[end][2])
            break;
        for (i = 0; i < 3; ++i) {
            temp = openset[parent][i];
            openset[parent][i] = openset[end][i];
            openset[end][i] = temp;
        }
        end = parent;
    }
}
