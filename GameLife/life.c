#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 25
#define INIT_DELAY 100000
#define MIN_DELAY 10000
#define MAX_DELAY 500000
#define DELAY_STEP 25000

typedef struct {
    int grid[HEIGHT][WIDTH];
    int start_x;
    int start_y;
    int width;
    int height;
} Game;

typedef struct {
    int delay;
    int running;
    int paused;
    int use_file;
    char filename[50];
} GameState;

void draw_border(int start_x, int start_y, int width, int height);          // отрисовка рамки
void draw_vertical_lines(int start_x, int start_y, int width, int height);  // верхушку низ рисуем значит
void draw_horizontal_lines(int start_x, int start_y, int width,
                           int height);                              // ну тут соответственно боковушки
void draw_corners(int start_x, int start_y, int width, int height);  // а тут вот уголки красивенькие

void init_game(Game* game, int start_x, int start_y);
void load_random(Game* game);  // тут просто заполняю рандомом чтобы было проще для меня
int load_from_file(Game* game, const char* filename);  // тут подгрузка файликов...
void draw_grid(Game* game);                     // Подсчет количества живых соседей для клетки аля логика игры
int count_neighbors(Game* game, int x, int y);  // обновляем поле за каждый шаг типо того
void update_grid(Game* game);                   // инициализируем состояние игры

void init_game_state(GameState* state);
void increase_speed(GameState* state);  // тут короче уменьшаем задержку значит чтобы увеличить скорость
void decrease_speed(GameState* state);  // тут наоборот

void center_position(int* x, int* y, int width, int height);  // централизация игрульки хз надо не надо сделал
int show_menu(void);                                          // тут типо выбор

void run_game(void);                           // основная функция для запуска игры
void game_loop(Game* game, GameState* state);  // отрисовываем тут значит UI
void draw_ui(Game* game, GameState* state);    // обрабатываем тут значит всякое
void handle_input(GameState* state);           // тут кейс с кнопочками для увеличения уменьшения скорости
void handle_menu_choice(int choice, Game* game, GameState* state);  // тут файлики вставить надо... я не делал
int initialize_colors(void);    // инициализируем значит цвета для красоты ВАААУ
void initialize_ncurses(void);  // инициализируем библиотеку вообще в идеале проверять подкл она или нет

int is_live_cell(char c);
int is_dead_cell(char c);
int parse_configuration_line(char* buffer, int temp_grid[HEIGHT][WIDTH], int row);
void clear_game_grid(Game* game);
void copy_centered_grid(Game* game, int temp_grid[HEIGHT][WIDTH], int row_count, int max_cols);
void read_configuration_from_stdin(int temp_grid[HEIGHT][WIDTH], int* row_count, int* max_cols);
int has_pipe_data(void);
int load_from_pipe(Game* game);

void init_game(Game* game, int start_x, int start_y) {
    game->start_x = start_x;
    game->start_y = start_y;
    game->width = WIDTH;
    game->height = HEIGHT;
    memset(game->grid, 0, sizeof(game->grid));
}

void load_random(Game* game) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            game->grid[y][x] = arc4random_uniform(2);
        }
    }
}

int load_from_file(Game* game, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[WIDTH + 2];
    int y = 0;

    while (fgets(line, sizeof(line), file) && y < HEIGHT) {
        for (int x = 0; x < WIDTH && line[x] && line[x] != '\n'; x++) {
            game->grid[y][x] = (line[x] == '1') ? 1 : 0;
        }
        y++;
    }

    fclose(file);
    return 1;
}

void draw_corners(int start_x, int start_y, int width, int height) {
    mvaddch(start_y - 1, start_x - 1, ACS_ULCORNER);
    mvaddch(start_y - 1, start_x + width, ACS_URCORNER);
    mvaddch(start_y + height, start_x - 1, ACS_LLCORNER);
    mvaddch(start_y + height, start_x + width, ACS_LRCORNER);
}

void draw_horizontal_lines(int start_x, int start_y, int width, int height) {
    for (int x = 0; x < width; x++) {
        mvaddch(start_y - 1, start_x + x, ACS_HLINE);
        mvaddch(start_y + height, start_x + x, ACS_HLINE);
    }
}

void draw_vertical_lines(int start_x, int start_y, int width, int height) {
    for (int y = 0; y < height; y++) {
        mvaddch(start_y + y, start_x - 1, ACS_VLINE);
        mvaddch(start_y + y, start_x + width, ACS_VLINE);
    }
}

void draw_border(int start_x, int start_y, int width, int height) {
    draw_corners(start_x, start_y, width, height);
    draw_horizontal_lines(start_x, start_y, width, height);
    draw_vertical_lines(start_x, start_y, width, height);
}

void draw_grid(Game* game) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            if (game->grid[y][x]) {
                attron(COLOR_PAIR(1));
                mvaddch(game->start_y + y, game->start_x + x, ACS_CKBOARD);
                attroff(COLOR_PAIR(1));
            } else {
                mvaddch(game->start_y + y, game->start_x + x, ' ');
            }
        }
    }
}

int count_neighbors(Game* game, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            int nx = (x + dx + game->width) % game->width;
            int ny = (y + dy + game->height) % game->height;

            if (game->grid[ny][nx]) count++;
        }
    }
    return count;
}

void update_grid(Game* game) {
    int new_grid[HEIGHT][WIDTH];

    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            int neighbors = count_neighbors(game, x, y);

            if (game->grid[y][x]) {
                new_grid[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                new_grid[y][x] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    memcpy(game->grid, new_grid, sizeof(new_grid));
}

void init_game_state(GameState* state) {
    state->delay = INIT_DELAY;
    state->running = 1;
    state->paused = 0;
    state->use_file = 0;
    strcpy(state->filename, "");
}

void increase_speed(GameState* state) {
    state->delay -= DELAY_STEP;
    if (state->delay < MIN_DELAY) {
        state->delay = MIN_DELAY;
    }
}

void decrease_speed(GameState* state) {
    state->delay += DELAY_STEP;
    if (state->delay > MAX_DELAY) {
        state->delay = MAX_DELAY;
    }
}

void center_position(int* x, int* y, int width, int height) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    *x = (max_x - width) / 2;
    *y = (max_y - height) / 2;
}

int show_menu(void) {
    clear();

    printw("=== GAME OF LIFE ===\n\n");
    printw("Выберите пожалуйста:\n\n");
    printw("1 - random<3\n");
    printw("2 - pattern1\n");
    printw("3 - pattern2\n");
    printw("4 - pattern3\n");
    printw("5 - pattern4\n");
    printw("6 - pattern5\n\n");
    printw("Waiting for the button...\n");

    refresh();

    timeout(-1);
    int choice = getch();
    nodelay(stdscr, TRUE);

    return choice;
}

void initialize_ncurses(void) {
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
}

int initialize_colors(void) {
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal doesn't support colors\n");
        return 0;
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    return 1;
}

void handle_menu_choice(int choice, Game* game, GameState* state) {
    switch (choice) {
        case '1':
            state->use_file = 0;
            load_random(game);
            break;
        case '2':
            state->use_file = 1;
            if (!load_from_file(game, "pattern1.txt")) {
                load_random(game);
            }
            break;
        case '3':
            state->use_file = 1;
            if (!load_from_file(game, "pattern2.txt")) {
                load_random(game);
            }
            break;
        case '4':
            state->use_file = 1;
            if (!load_from_file(game, "pattern3.txt")) {
                load_random(game);
            }
            break;
        case '5':
            state->use_file = 1;
            if (!load_from_file(game, "pattern4.txt")) {
                load_random(game);
            }
            break;
        case '6':
            state->use_file = 1;
            if (!load_from_file(game, "pattern5.txt")) {
                load_random(game);
            }
            break;
        default:
            state->use_file = 0;
            load_random(game);
            break;
    }
}

void handle_input(GameState* state) {
    int ch;
    while ((ch = getch()) != ERR) {
        switch (ch) {
            case 'a':
            case 'A':
                increase_speed(state);
                break;
            case 'z':
            case 'Z':
                decrease_speed(state);
                break;
            case ' ':
                state->running = 0;
                break;
            case 'p':
            case 'P':
                state->paused = !state->paused;
                break;
            case 'q':
            case 'Q':
                state->running = 0;
                break;
            default:
                break;
        }
    }
}

void draw_ui(Game* game, GameState* state) {
    draw_border(game->start_x, game->start_y, game->width, game->height);
    draw_grid(game);

    mvprintw(game->start_y + game->height + 2, game->start_x - 1,
             "Controls: A - Faster, Z - Slower, SPACE - Exit, P - Pause");

    int speed_level = (MAX_DELAY - state->delay) / DELAY_STEP + 1;
    int max_speed_level = (MAX_DELAY - MIN_DELAY) / DELAY_STEP + 1;
    mvprintw(game->start_y + game->height + 3, game->start_x - 1, "Speed: %d/%d (delay: %d ms)", speed_level,
             max_speed_level, state->delay / 1000);

    if (state->paused) {
        mvprintw(game->start_y + game->height + 4, game->start_x - 1, "PAUSED - Press P to resume");
    }
}

void game_loop(Game* game, GameState* state) {
    nodelay(stdscr, TRUE);

    while (state->running) {
        clear();

        draw_ui(game, state);
        refresh();

        handle_input(state);

        if (!state->paused) {
            update_grid(game);
        }
        napms(state->delay / 1000);
    }
}

void run_game(void) {
    initialize_ncurses();

    if (!initialize_colors()) {
        endwin();
        return;
    }

    Game game;
    GameState state;
    init_game_state(&state);

    int start_x, start_y;
    center_position(&start_x, &start_y, WIDTH + 2, HEIGHT + 2);
    init_game(&game, start_x, start_y);

    if (!load_from_pipe(&game)) {
        int choice = show_menu();
        handle_menu_choice(choice, &game, &state);
    } else {
        state.use_file = 1;
        mvprintw(0, 0, "Loaded from pipe | Press SPACE to exit, P to pause");
        refresh();
        getch();
    }
    game_loop(&game, &state);
    endwin();
}

int is_live_cell(char c) { return c == '1' || c == 'X' || c == '#'; }

int is_dead_cell(char c) { return c == '0' || c == '.' || c == ' '; }

int parse_configuration_line(char* buffer, int temp_grid[HEIGHT][WIDTH], int row) {
    int col = 0;
    for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n' && col < WIDTH; i++) {
        if (is_live_cell(buffer[i])) {
            temp_grid[row][col] = 1;
            col++;
        } else if (is_dead_cell(buffer[i])) {
            temp_grid[row][col] = 0;
            col++;
        }
    }
    return col;
}

void clear_game_grid(Game* game) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            game->grid[i][j] = 0;
        }
    }
}

void copy_centered_grid(Game* game, int temp_grid[HEIGHT][WIDTH], int row_count, int max_cols) {
    int start_row = (HEIGHT - row_count) / 2;
    int start_col = (WIDTH - max_cols) / 2;

    for (int i = 0; i < row_count && (start_row + i) < HEIGHT; i++) {
        for (int j = 0; j < max_cols && (start_col + j) < WIDTH; j++) {
            game->grid[start_row + i][start_col + j] = temp_grid[i][j];
        }
    }
}

void read_configuration_from_stdin(int temp_grid[HEIGHT][WIDTH], int* row_count, int* max_cols) {
    char buffer[2048];
    *row_count = 0;
    *max_cols = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL && *row_count < HEIGHT) {
        int cols_in_row = parse_configuration_line(buffer, temp_grid, *row_count);
        if (cols_in_row > *max_cols) {
            *max_cols = cols_in_row;
        }
        (*row_count)++;
    }
}

int has_pipe_data(void) {
    nodelay(stdscr, TRUE);
    int ch = getch();
    nodelay(stdscr, FALSE);

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    clearerr(stdin);
    if (feof(stdin)) {
        return 1;
    }

    return 0;
}

int load_from_pipe(Game* game) {
    if (!has_pipe_data()) {
        return 0;
    }

    int temp_grid[HEIGHT][WIDTH] = {0};
    int row_count = 0;
    int max_cols = 0;
    read_configuration_from_stdin(temp_grid, &row_count, &max_cols);
    freopen("/dev/tty", "r", stdin);
    clear_game_grid(game);
    copy_centered_grid(game, temp_grid, row_count, max_cols);

    return 1;
}

int main(void) {
    run_game();
    return 0;
}