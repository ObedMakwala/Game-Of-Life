/****************************************************/
// LIBRARIES

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/****************************************************/
// GLOBAL VARIABLES

// X = width // Y = height
#define BOUNDARY_X 40
#define BOUNDARY_Y 20

#define SECONDS 1

struct frame
{
    int current;
    int future;
};

/****************************************************/
// FUNCTIONS PROTOTYPES

void ncurses_start();
void ncurses_end();

void show_current_generation(struct frame grid[BOUNDARY_X][BOUNDARY_Y]);

void run_game(int generations, struct frame grid[BOUNDARY_X][BOUNDARY_Y]);

void load_game_from_file(char file_name[50], struct frame grid[BOUNDARY_X][BOUNDARY_Y]);

int neighbors_count(struct frame grid[BOUNDARY_X][BOUNDARY_Y], int x, int y);

/****************************************************/
// MAIN CODE

int main(void)
{

    // variable decleration

    struct frame grid[BOUNDARY_X][BOUNDARY_Y] = {0, 0};

    //

    load_game_from_file("game.txt", grid);

    printf("How many Generations: ");

    int generations = 0;
    scanf("%d", &generations);

    // ncurses code

    ncurses_start();

    run_game(generations, grid);

    ncurses_end();

    return 0;
}

/****************************************************/
// FUNCTIONS

/*
NAME: show_current_generation()
INPUT:
RETURNS:
FUNCTION: shows the current state of the game from grid[][].current
*/
void show_current_generation(struct frame grid[BOUNDARY_X][BOUNDARY_Y])
{

    for (int i = 0; i < BOUNDARY_Y; i++)
    {

        for (int j = 0; j < BOUNDARY_X; j++)
        {

            int bit = grid[j][i].current;

            move(i, j);

            if (bit)
            {

                attron(COLOR_PAIR(2));
                printw("X");
            }
            else
            {

                printw(" ");
            }

            // printw("%d",bit);
            printw(" ");
            attroff(COLOR_PAIR(2));
        }
    }

    refresh();
}

/*
NAME: run_game()
INPUT: int generations
RETURNS:
FUNCTION: runs game of life cell logic
*/
void run_game(int generations, struct frame grid[BOUNDARY_X][BOUNDARY_Y])
{

    for (int i = 0; i < generations; i++)
    {

        clear();
        show_current_generation(grid);

        for (int j = 0; j < BOUNDARY_Y; j++)
        {
            for (int k = 0; k < BOUNDARY_X; k++)
            {

                int neighbors = neighbors_count(grid, k, j);

                // die birth logic
                if (neighbors <= 1)
                {

                    // die
                    grid[k][j].future = 0;
                }
                else if (neighbors >= 4)
                {

                    // die
                    grid[k][j].future = 0;
                }
                else if (neighbors == 3)
                {

                    // birth
                    grid[k][j].future = 1;
                }
                else if (neighbors == 2 && grid[k][j].current != 0)
                {

                    // survive
                    grid[k][j].future = 1;
                }
            }
        }

        // future to current

        for (int i = 0; i < BOUNDARY_Y; i++)
        {
            for (int j = 0; j < BOUNDARY_X; j++)
            {
                grid[j][i].current = grid[j][i].future;
            }
        }

        sleep(SECONDS);
    }
}

/*
NAME: load_game_from_file()
INPUT: char file_name[50]
RETURNS:
FUNCTION: reads file and ads it to struct frame grid[X][Y] = {0,0};
*/
void load_game_from_file(char file_name[50], struct frame grid[BOUNDARY_X][BOUNDARY_Y])
{

    FILE *file;
    file = fopen(file_name, "r");

    if (file != NULL)
    {

        int x = 0;
        int y = 0;

        char c;

        for (int i = 0; !feof(file); i++)
        {

            fscanf(file, "%c", &c);

            if (c == '0')
            {
                x++;
            }

            if (c == '1')
            {
                grid[x][y].current = 1;
                x++;
            }

            if (c == '\n')
            {
                // printf("NEWLINE");
                x = 0;
                y++;
            }
        }
    }
    else
    {

        printf("FAILED TO READ FILE\n");
    }

    fclose(file);
}

/*
NAME: ncuses_start()
INPUT:
RETURNS:
FUNCTION: sets up ncurses mode and colors
*/
void ncurses_start()
{

    initscr();     // enables ncurses window state
    start_color(); // enables color

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    bkgd(COLOR_PAIR(1)); // sets background and foreground color
    refresh();
}

/*
NAME: neighbors_count()
INPUT:
RETURNS:
FUNCTION: counts the number of neighbors
*/
int neighbors_count(struct frame grid[BOUNDARY_X][BOUNDARY_Y], int x, int y)
{

    int my_neighbors = 0;

    // top
    if (x - 1 > -1 && x - 1 < BOUNDARY_X && y - 1 > -1 && y - 1 < BOUNDARY_Y)
    {
        if (grid[x - 1][y - 1].current == 1)
        {
            my_neighbors++;
        }
    }

    if (x > -1 && x < BOUNDARY_X && y - 1 > -1 && y - 1 < BOUNDARY_Y)
    {
        if (grid[x][y - 1].current == 1)
        {
            my_neighbors++;
        }
    }

    if (x + 1 > -1 && x + 1 < BOUNDARY_X && y - 1 > -1 && y - 1 < BOUNDARY_Y)
    {
        if (grid[x + 1][y - 1].current == 1)
        {
            my_neighbors++;
        }
    }

    // middle
    if (x - 1 > -1 && x - 1 < BOUNDARY_X && y > -1 && y < BOUNDARY_Y)
    {
        if (grid[x - 1][y].current == 1)
        {
            my_neighbors++;
        }
    }

    if (x + 1 > -1 && x + 1 < BOUNDARY_X && y > -1 && y < BOUNDARY_Y)
    {
        if (grid[x + 1][y].current == 1)
        {
            my_neighbors++;
        }
    }

    // down
    if (x - 1 > -1 && x - 1 < BOUNDARY_X && y + 1 > -1 && y + 1 < BOUNDARY_Y)
    {
        if (grid[x - 1][y + 1].current == 1)
        {
            my_neighbors++;
        }
    }

    if (x > -1 && x < BOUNDARY_X && y + 1 > -1 && y + 1 < BOUNDARY_Y)
    {
        if (grid[x][y + 1].current == 1)
        {
            my_neighbors++;
        }
    }

    if (x + 1 > -1 && x + 1 < BOUNDARY_X && y + 1 > -1 && y + 1 < BOUNDARY_Y)
    {
        if (grid[x + 1][y + 1].current == 1)
        {
            my_neighbors++;
        }
    }

    return my_neighbors;
}

/*
NAME: ncurses_end()
INPUT:
RETURNS:
FUNCTION: ends ncurses mode
*/
void ncurses_end()
{

    getch();
    endwin();
}
