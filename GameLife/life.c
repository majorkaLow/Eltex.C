#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
int show_menu();                                              // тут типо выбор

void run_game();                               // основная функция для запуска игры
void game_loop(Game* game, GameState* state);  // отрисовываем тут значит UI
void draw_ui(Game* game, GameState* state);    // обрабатываем тут значит всякое
void handle_input(GameState* state);           // тут кейс с кнопочками для увеличения уменьшения скорости
void handle_menu_choice(int choice, Game* game, GameState* state);  // тут файлики вставить надо... я не делал
int initialize_colors();    // инициализируем значит цвета для красоты ВАААУ
void initialize_ncurses();  // инициализируем библиотеку вообще в идеале проверять подкл она или нет

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
            game->grid[y][x] = rand() % 2;
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

int show_menu() {
    clear();

    printw("=== GAME OF LIFE ===\n\n");
    printw("Выберите пожалуйста:\n\n");
    printw("1 - тут короче надо файлики вставить\n");
    printw("2 - тут короче надо файлики вставить\n");
    printw("3 - тут короче надо файлики вставить\n");
    printw("4 - тут короче надо файлики вставить\n");
    printw("5 - тут короче надо файлики вставить\n");
    printw("6 - тут короче надо файлики вставить\n\n");
    printw("Ждемс кнопочку...\n");

    refresh();

    timeout(-1);
    int choice = getch();
    nodelay(stdscr, TRUE);

    return choice;
}

void initialize_ncurses() {
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
}

int initialize_colors() {
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
            if (!load_from_file(game, "file1.txt")) {
                load_random(game);
            }
            break;
        case '3':
            state->use_file = 1;
            if (!load_from_file(game, "file2.txt")) {
                load_random(game);
            }
            break;
        case '4':
            state->use_file = 1;
            if (!load_from_file(game, "file3.txt")) {
                load_random(game);
            }
            break;
        case '5':
            state->use_file = 1;
            if (!load_from_file(game, "file4.txt")) {
                load_random(game);
            }
            break;
        case '6':
            state->use_file = 1;
            if (!load_from_file(game, "file5.txt")) {
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
        usleep(state->delay);
    }
}

void run_game() {
    initialize_ncurses();

    if (!initialize_colors()) {
        endwin();
        return;
    }

    srand(time(NULL));

    int choice = show_menu();

    Game game;
    GameState state;
    init_game_state(&state);

    int start_x, start_y;
    center_position(&start_x, &start_y, WIDTH + 2, HEIGHT + 2);
    init_game(&game, start_x, start_y);

    handle_menu_choice(choice, &game, &state);

    game_loop(&game, &state);

    endwin();
}

int main() {
    run_game();
    return 0;
}