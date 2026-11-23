#include <efi.h>
#include <efilib.h>
#include "2048.h"
#include "rng.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH  40
#define TILE_HEIGHT  5
#define TILE_WIDTH   10
#define BOARD_X      20
#define BOARD_Y      20
#define HORIZ        0x2500
#define VERTI        0x2502
#define TL_CO        0x250C
#define TR_CO        0x2510
#define BL_CO        0x2514
#define BR_CO        0x2518
#define ROW_COUNT    4
#define COL_COUNT    4

int debug_curr_line = 0;
int score = 0;

int game[ROW_COUNT][COL_COUNT] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

CHAR16 tile[TILE_HEIGHT][TILE_WIDTH + 1] = {
    {TL_CO, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, TR_CO, 0},
    {VERTI, ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   VERTI, 0},
    {VERTI, ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   VERTI, 0},
    {VERTI, ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   VERTI, 0},
    {BL_CO, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, HORIZ, BR_CO, 0}
};

BOOLEAN move_right()
{
    int base_index = 0;
    int action_index = 0;
    int iterator;
    BOOLEAN moved_tile = FALSE;
    
    for (iterator = 0; iterator < ROW_COUNT; iterator++)
    {
        base_index = 0;
        action_index = 1;
        while (action_index < COL_COUNT)
        {
            if (game[iterator][(ROW_COUNT - 1) - action_index])
            {
                if (game[iterator][(ROW_COUNT - 1) - action_index] == game[iterator][(ROW_COUNT - 1) - base_index])
                {
                    game[iterator][(ROW_COUNT - 1) - action_index] = 0;
                    game[iterator][(ROW_COUNT - 1) - base_index]++;
                    score += 1 << game[iterator][(ROW_COUNT - 1) - base_index];
                    base_index++;
                    moved_tile = TRUE;
                }
                else
                {
                    if (game[iterator][(ROW_COUNT - 1) - base_index] != 0)
                    {
                        base_index++;
                    }
                    if (base_index != action_index)
                    {
                        game[iterator][(ROW_COUNT - 1) - base_index] = game[iterator][(ROW_COUNT - 1) - action_index];
                        game[iterator][(ROW_COUNT - 1) - action_index] = 0;
                        moved_tile = TRUE;
                    }
                }
            }
            action_index++;
        }
    }

    return moved_tile;
}

BOOLEAN move_left()
{
    int base_index = 0;
    int action_index = 0;
    int iterator;
    BOOLEAN moved_tile = FALSE;
    
    for (iterator = 0; iterator < ROW_COUNT; iterator++)
    {
        base_index = 0;
        action_index = 1;
        while (action_index < COL_COUNT)
        {
            if (game[iterator][action_index])
            {
                if (game[iterator][action_index] == game[iterator][base_index])
                {
                    game[iterator][action_index] = 0;
                    game[iterator][base_index]++;
                    score += 1 << game[iterator][base_index];
                    base_index++;
                    moved_tile = TRUE;
                }
                else
                {
                    if (game[iterator][base_index] != 0)
                    {
                        base_index++;
                    }
                    if (base_index != action_index)
                    {
                        game[iterator][base_index] = game[iterator][action_index];
                        game[iterator][action_index] = 0;
                        moved_tile = TRUE;
                    }
                }
            }
            action_index++;
        }
    }

    return moved_tile;
}

BOOLEAN move_up()
{
    int base_index = 0;
    int action_index = 0;
    int iterator;
    BOOLEAN moved_tile = FALSE;
    
    for (iterator = 0; iterator < ROW_COUNT; iterator++)
    {
        base_index = 0;
        action_index = 1;
        while (action_index < COL_COUNT)
        {
            if (game[action_index][iterator])
            {
                if (game[action_index][iterator] == game[base_index][iterator])
                {
                    game[action_index][iterator] = 0;
                    game[base_index][iterator]++;
                    score += 1 << game[base_index][iterator];
                    base_index++;
                    moved_tile = TRUE;
                }
                else
                {
                    if (game[base_index][iterator] != 0)
                    {
                        base_index++;
                    }
                    if (base_index != action_index)
                    {
                        game[base_index][iterator] = game[action_index][iterator];
                        game[action_index][iterator] = 0;
                        moved_tile = TRUE;
                    }
                }
            }
            action_index++;
        }
    }

    return moved_tile;
}

BOOLEAN move_down()
{
    int base_index = 0;
    int action_index = 0;
    int iterator;
    BOOLEAN moved_tile = FALSE;
    
    for (iterator = 0; iterator < ROW_COUNT; iterator++)
    {
        base_index = 0;
        action_index = 1;
        while (action_index < COL_COUNT)
        {
            if (game[(ROW_COUNT - 1) - action_index][iterator])
            {
                if (game[(ROW_COUNT - 1) - action_index][iterator] == game[(ROW_COUNT - 1) - base_index][iterator])
                {
                    game[(ROW_COUNT - 1) - action_index][iterator] = 0;
                    game[(ROW_COUNT - 1) - base_index][iterator]++;
                    score += 1 << game[(ROW_COUNT - 1) - base_index][iterator];
                    base_index++;
                    moved_tile = TRUE;
                }
                else
                {
                    if (game[(ROW_COUNT - 1) - base_index][iterator] != 0)
                    {
                        base_index++;
                    }
                    if (base_index != action_index)
                    {
                        game[(ROW_COUNT - 1) - base_index][iterator] = game[(ROW_COUNT - 1) - action_index][iterator];
                        game[(ROW_COUNT - 1) - action_index][iterator] = 0;
                        moved_tile = TRUE;
                    }
                }
            }
            action_index++;
        }
    }

    return moved_tile;
}

BOOLEAN add_number()
{
    int counter = 0;
    int filled_tile = 0;
    int x_index;
    int y_index;

    for (x_index = 0; x_index < ROW_COUNT; x_index++)
    {
        for (y_index = 0; y_index < COL_COUNT; y_index++)
        {
            if (!game[x_index][y_index])
            {
                counter++;
            }
        }
    }

    if (counter == 0)
    {
        return FALSE;
    }

    filled_tile = fetch_rng() % counter;
    counter = 0;

    for (x_index = 0; counter <= filled_tile; x_index++)
    {
        for (y_index = 0; counter <= filled_tile; y_index++)
        {
            if (!game[x_index][y_index])
            {
                if (counter == filled_tile)
                {
                    if (fetch_rng() % 10)
                    {
                        game[x_index][y_index] = 1;
                    }
                    else
                    {
                        game[x_index][y_index] = 2;
                    }
                }
                counter++;
            }
        }
    }

    return TRUE;
}

BOOLEAN is_lost()
{
    int x_index;
    int y_index;

    /* Check for empty spaces */
    for (x_index = 0; x_index < ROW_COUNT; x_index++)
    {
        for (y_index = 0; y_index < COL_COUNT; y_index++)
        {
            if (game[x_index][y_index] == 0)
            {
                return FALSE;
            }
        }
    }

    /* Check for possible moves */
    for (x_index = 0; x_index < ROW_COUNT; x_index++)
    {
        for (y_index = 0; y_index < COL_COUNT; y_index++)
        {
            if (x_index > 0 && game[x_index][y_index] == game[x_index - 1][y_index])
            {
                return FALSE;
            }
            if (x_index < COL_COUNT && game[x_index][y_index] == game[x_index + 1][y_index])
            {
                return FALSE;
            }
            if (y_index > 0 && game[x_index][y_index] == game[x_index][y_index - 1])
            {
                return FALSE;
            }
            if (y_index < ROW_COUNT && game[x_index][y_index] == game[x_index][y_index + 1])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void print_game()
{
    int rows;
    int columns;
    int iterator;
    CHAR16 number[10];

    for (rows = 0; rows < ROW_COUNT; rows++)
    {
        for (columns = 0; columns < COL_COUNT; columns++)
        {
            for (iterator = 0; iterator < TILE_HEIGHT; iterator++)
            {
                PrintAt(columns * TILE_WIDTH, rows * TILE_HEIGHT + iterator, tile[iterator]);
            }
            if (game[rows][columns])
            {
                ValueToString(number, 0, 1 << (game[rows][columns]));
                for (iterator = 0; number[iterator] != 0; iterator++)
                {
                    number[iterator] += 0xFF10 - '0';
                }
                PrintAt(columns * TILE_WIDTH + 5 - strlen(number), rows * TILE_HEIGHT + 2, number);
            }
        }
    }

    PrintAt(0, ROW_COUNT * TILE_HEIGHT + 1, u"Score: %d", score);

    return;
}

int strlen(CHAR16 *initial_char)
{
    CHAR16 *curr_char;

    for (curr_char = initial_char; *curr_char != 0; curr_char++);

    return curr_char - initial_char;
}

void print_debug(short unsigned int *string)
{
    PrintAt(100, debug_curr_line % 63, string);
    debug_curr_line++;
    return;
}

void clear_game()
{
    int x_index = 0;
    int y_index = 0;

    for (y_index = 0; y_index < ROW_COUNT; y_index++)
    {
        for (x_index = 0; x_index < COL_COUNT; x_index++)
        {
            game[y_index][x_index] = 0;
        }
    }
    score = 0;

    return;
}
