#define N 16

typedef struct _Maze {
    char arr[N][N];
    int pos; // first 4 bits row, 2nd 4 bits col
} Maze;

void generate(Maze *m);

int *solve(Maze *m, int *size); // return array of positions