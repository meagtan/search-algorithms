#define SIZE 16
#define SRCNUM 7
#define NONE -1
#define ELEM(a,b,c) tests[(a) * testnum * SRCNUM + (b) * SRCNUM + (c)]

/* Depth-first, breadth-first, uniform-cost and A* search, the latter using four different heuristics:
   - Euclidean distance
   - Manhattan distance
   - Landmark heuristic, measured once in the beginning
   - Landmark heuristic, measured iteratively */
enum {DFS, BFS, UCS, ASE, ASM, ASL, ASI} Search;

typedef char Maze[SIZE][SIZE];

int *testsearch(int gennum, int testnum)
{
    if (gennum <= 0 || testnum <= 0) return NULL;
    
    int *tests = malloc(gennum * testnum * SRCNUM * sizeof(int));
    if (tests == NULL) return NULL;
    
    Maze m;
    SearchState st[SRCNUM];
    int i, j, k, x0, y0;
    
    for (i = 0; i < gennum; ++i) {
        init_maze(m);
        
        for (k = 0; k < SRCNUM; ++k)
            init_state(st + k, k);
        
        for (j = 0; j < testnum; ++j) {
            // define the RNG later
            // verify that x0 and y0 are not obstacles in m
            x0 = random(SIZE);
            y0 = random(SIZE);
            
            for (k = 0; k < SRCNUM; ++k)
                ELEM(i, j, k) = search(m, st + k, x0, y0);
        }
    }
    
    return tests;
}

void init_maze(Maze m)
{
    memset(m, 0, sizeof(m)); // clear m
    
    // TODO generate random maze on m, representing obstacles by 1
}

// returns size of queue
int search(Maze m, SearchState *st, int x, int y)
{
    // points represented as arrays of size 2, the 3rd element in openset is priority
    int openset[SIZE * SIZE][3] = {0}, n = 0, // openset and index
        preds[SIZE][SIZE][2] = {0}, // needn't assign to NONE as (0,0) will never be a predecessor to some other point
        dists[SIZE][SIZE] = {NONE}, 
        res = 0, i, newdist,
        ns[4][2], nc; // neighbors and count
    
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
    
    return NONE;
}

// push also counts in heuristic for A*
void push(int openset[][3], int n, int x, int y, int dist, SearchState *st);
void pop(int openset[][3], int n, int *x, int *y, int dist, SearchState *st);

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