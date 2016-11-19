#define SIZE 16
#define SRCNUM 7
#define NONE -1

/* Depth-first, breadth-first, uniform-cost and A* search, the latter using four different heuristics:
   - Euclidean distance
   - Manhattan distance
   - Landmark heuristic, measured once in the beginning
   - Landmark heuristic, measured iteratively */
enum {DFS, BFS, UCS, ASE, ASM, ASL, ASI};

typedef char Maze[SIZE][SIZE];

// conduct search with given number of generations and tests for each generation
int *testsearch(int gennum, int testnum);
// initialize random maze
void init_maze(Maze m);
// returns size of queue
int search(Maze m, SearchState *st, int x, int y);
// push also counts in heuristic for A*
void push(int openset[][3], int n, int x, int y, int dist, SearchState *st);
void pop(int openset[][3], int n, int *x, int *y, int dist, SearchState *st);
// return array of neighbors
void get_neighbors(int ns[4][2], int *nc, int x, int y, Maze m);