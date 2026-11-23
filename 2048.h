/* Movement */
BOOLEAN move_right();
BOOLEAN move_left();
BOOLEAN move_up();
BOOLEAN move_down();

/* Game mechanics */
BOOLEAN add_number();
BOOLEAN is_lost();

/* Display */
void print_debug(short unsigned int *string);
void clear_game();
void print_game();

/* Support */
int strlen(CHAR16 *initial_char);
