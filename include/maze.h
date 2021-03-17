#ifndef MAZE
#define MAZE

struct maze {
    int **grid;
    int rows;
    int columns;
};

struct maze* new_maze(int, int);

void delete_maze(struct maze*);

void MazeRead(struct maze*);

#endif
