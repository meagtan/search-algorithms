#include <string.h> // memset
#include <stdlib.h> // malloc, free
#include <math.h> // sqrt

#define SIZE 16
#define SRCNUM 10 // 10 search strategies, the last 4 employing iterative landmark optimizations
#define NONE -1

// TODO perhaps make the iterative landmark heuristic a separate boolean value

/* Depth-first, breadth-first, uniform-cost and A* search, the latter using four different heuristics:
 * - Euclidean distance
 * - Manhattan distance
 * - Landmark heuristic, measured once in the beginning 
 */
enum {DFS, BFS, UCS, ASE, ASM, ASL};

typedef char Maze[SIZE][SIZE];

typedef struct {
    int type;
    int iter; // wheter to use iterative landmark optimization
    int heur[SIZE][SIZE][2];
} SearchState;

// conduct search with given number of generations and tests for each generation
int *testsearch(int gennum, int testnum);
// returns size of queue
int search(Maze m, SearchState *st, int x, int y);

void init_maze(Maze m);
void init_state(SearchState *st, int k);
void update_state(SearchState *st, int dists[SIZE][SIZE]);
int heur(SearchState *st, int x, int y);

// push also counts in heuristic for A*
// n is size, > 0 for pop
void push(int openset[][], int n, int x, int y, int dist, SearchState *st);
void pop(int openset[][], int n, int *x, int *y, SearchState *st);

// return array of neighbors
void get_neighbors(int ns[][], int *nc, int x, int y, Maze m);

int random(int max);