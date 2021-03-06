#define SIZE 32
#define SRCNUM 10 // 10 search strategies, the last 4 employing iterative landmark optimizations
#define NONE -1
#define LOOP_OFFSET 100 // add loop to maze with frequency between 1/LOOP_OFFSET and 1/(LOOP_OFFSET + LOOP_RANGE)
#define LOOP_RANGE  25

/* Depth-first, breadth-first, uniform-cost and A* search, the latter using four different heuristics:
 * - Euclidean distance
 * - Manhattan distance
 * - Landmark heuristic, measured once in the beginning 
 */
enum {DFS, BFS, UCS, ASE, ASM, ASL};
typedef char **Maze;
typedef struct {
    int type;
    int iter; // whether to use iterative landmark optimization
    int **heur;
} SearchState;

// conduct search with given number of generations and tests for each generation
int *testsearch(int gennum, int testnum);
// returns size of queue, modifies dists
int search(Maze m, SearchState *st, int x, int y, int **dists);

void init_maze(Maze *m);
void init_state(SearchState *st, int k, Maze m);
void update_state(SearchState *st, int **dists);
int heur(SearchState *st, int x, int y);

// choose random point on maze
void rand_point(Maze m, int *x, int *y);

// push also counts in heuristic for A*
// n is size, > 0 for pop
void push(int **openset, int n, int x, int y, int dist, SearchState *st);
void pop(int **openset, int n, int *x, int *y, SearchState *st);

// return array of neighbors
void get_neighbors(int ns[4][2], int *nc, int x, int y, Maze m); // x and y assumed to be available, as with search

int rand_num(int max);
