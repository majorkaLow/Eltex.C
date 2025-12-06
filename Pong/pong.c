
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RACKET_WIDTH 1

#define VERTICAL_LINE ACS_VLINE
#define HORIZONTAL_LINE ACS_HLINE
#define EMPTY_SPACE ' '
#define BALL ACS_BULLET
#define CORNER_UL ACS_ULCORNER
#define CORNER_UR ACS_URCORNER
#define CORNER_LR ACS_LRCORNER
#define CORNER_LL ACS_LLCORNER

#define SKIP_KEY ' '

#define RACKET_1_KEY_UP 97
#define RACKET_1_KEY_DOWN 122

#define RACKET_2_KEY_UP 107
#define RACKET_2_KEY_DOWN 109

#define QUIT_KEY 113

#define TARGET_FRAME_TIME 16
#define BALL_PHYSICS_UPDATE_FRAMES_COUNT 4

#define GOALS_TO_WIN 21

typedef struct {
    int x;
    int y;
} Vector2;

typedef enum {
    EXIT,
    GAME,
    GAME_OVER,
} Scene;

typedef struct {
    int width;
    int height;
    int racket_height;
    int ball_size;
} GameConfig;

typedef struct {
    int racket_1_score;
    int racket_2_score;
    Vector2 racket_1_position;
    Vector2 racket_2_position;
    Vector2 ball_position;
    Vector2 ball_velocity;
    Scene current_scene;
} GameState;

GameState get_initial_state(GameConfig config);

GameState update_state(int key, GameConfig config, GameState state, int frame_count);

void draw_scene(GameConfig config, GameState state);

bool is_box_collision(int rect1_x, int rect1_y, int rect1_width, int rect1_height, int rect2_x, int rect2_y,
                      int rect2_width, int rect2_height);

void draw_score(int y, int x, int racket_1_score, int racket_2_score);

void draw_racket(int y, int x, int height);
void draw_ball(int y, int x);

void draw_rectangle(int y, int x, int width, int height);

void draw_horizontal_line(int y, int x, int width);
void draw_vertical_line(int y, int x, int height);

void draw_game_scene(GameConfig config, GameState state);

GameState process_game_scene(int key, GameConfig config, GameState state, int frame_count);

void draw_game_over_scene(GameConfig config, GameState state);

GameState process_game_over_scene(int key, GameConfig config, GameState state);

int run(GameConfig config);

int init(GameConfig config);
int check_terminal_size(GameConfig config);
void dispose();

void run_game_cycle(GameConfig config);

int main() {
    GameConfig config = {.width = 80, .height = 25, .racket_height = 3, .ball_size = 1};

    if (run(config) < 0) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
}

int run(GameConfig config) {
    int result = init(config);
    if (result < 0) {
        return result;
    };

    run_game_cycle(config);

    dispose();

    return EXIT_SUCCESS;
}

int init(GameConfig config) {
    initscr();

    int result = check_terminal_size(config);
    if (result < 0) {
        dispose();
        return result;
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    return EXIT_SUCCESS;
}

int check_terminal_size(GameConfig config) {
    int mrow;
    int mcol;

    getmaxyx(stdscr, mrow, mcol);

    if (mrow < config.height || mcol < config.width) {
        fprintf(stderr, "Terminal too small\nCurrent size: %dx%d\nMinimum size: %dx%d\n", mcol, mrow,
                config.width, config.height);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void dispose() {
    keypad(stdscr, FALSE);
    nodelay(stdscr, FALSE);
    endwin();
}

void render(GameConfig config, GameState state) {
    clear();
    draw_scene(config, state);
    refresh();
}

void run_game_cycle(GameConfig config) {
    GameState state = get_initial_state(config);

    int frame_count = 0;
    int ch = 0;
    while (true) {
        ch = getch();

        state = update_state(ch, config, state, frame_count);

        if (state.current_scene == EXIT) {
            break;
        }

        render(config, state);

        frame_count++;

        napms(TARGET_FRAME_TIME);
    }
}

void draw_game_over_scene(GameConfig config, GameState state) {
    int half_height = config.height / 2;
    int half_width = config.width / 2;

    draw_rectangle(0, 0, config.height, config.width);
    mvprintw(half_height - 2, half_width - 3, "Game Over");
    draw_score(half_height, half_width - 5, state.racket_1_score, state.racket_2_score);
    mvprintw(half_height + 2, half_width - 16, "Press Space to restart or Q to quit");
}

GameState process_game_over_scene(int key, GameConfig config, GameState state) {
    if (key == QUIT_KEY) {
        state.current_scene = EXIT;
    }

    if (key == SKIP_KEY) {
        state = get_initial_state(config);
    }

    return state;
}

void draw_info(int y, int x, GameState state) {
    draw_score(y, x, state.racket_1_score, state.racket_2_score);
    mvprintw(y, x + 13, "Racket 1: A/Z, Racket 2: K/M, Quit: Q, Skip: Space");
}

void draw_game_scene(GameConfig config, GameState state) {
    draw_rectangle(0, 0, config.height, config.width);
    draw_racket(state.racket_1_position.y, state.racket_1_position.x, config.racket_height);
    draw_racket(state.racket_2_position.y, state.racket_2_position.x, config.racket_height);
    draw_ball(state.ball_position.y, state.ball_position.x);
    draw_info(config.height + 1, 1, state);
}

int move_racket(int y, int racket_height, int zone_height) {
    if (y - 1 < 0) {
        return 1;
    } else if (y + racket_height > zone_height) {
        return zone_height - racket_height;
    } else {
        return y;
    }
}

GameState process_key(int key, GameConfig config, GameState state) {
    if (key == QUIT_KEY) {
        state.current_scene = EXIT;
    }

    if (key == RACKET_1_KEY_UP) {
        state.racket_1_position.y =
            move_racket(state.racket_1_position.y - 1, config.racket_height, config.height);
    }

    if (key == RACKET_1_KEY_DOWN) {
        state.racket_1_position.y =
            move_racket(state.racket_1_position.y + 1, config.racket_height, config.height);
    }

    if (key == RACKET_2_KEY_UP) {
        state.racket_2_position.y =
            move_racket(state.racket_2_position.y - 1, config.racket_height, config.height);
    }

    if (key == RACKET_2_KEY_DOWN) {
        state.racket_2_position.y =
            move_racket(state.racket_2_position.y + 1, config.racket_height, config.height);
    }

    return state;
}

GameState move_ball(GameConfig config, GameState state) {
    int next_ball_y = state.ball_position.y + state.ball_velocity.y;
    int next_ball_x = state.ball_position.x + state.ball_velocity.x;

    if (next_ball_y <= 1 || next_ball_y >= config.height - 1) {
        state.ball_velocity.y = -state.ball_velocity.y;
        next_ball_y = state.ball_position.y;  // Оставляем на текущей позиции
    }

    bool collision_with_racket_1 = is_box_collision(
        next_ball_x, next_ball_y, config.ball_size, config.ball_size, state.racket_1_position.x,
        state.racket_1_position.y, RACKET_WIDTH, config.racket_height);

    bool collision_with_racket_2 = is_box_collision(
        next_ball_x, next_ball_y, config.ball_size, config.ball_size, state.racket_2_position.x,
        state.racket_2_position.y, RACKET_WIDTH, config.racket_height);

    if (collision_with_racket_1 || collision_with_racket_2) {
        state.ball_velocity.x = -state.ball_velocity.x;
        next_ball_x = state.ball_position.x;
    }

    bool intersects_with_left_wall = next_ball_x <= 1;
    bool intersects_with_right_wall = next_ball_x >= config.width - 1;

    // Goal
    if (intersects_with_left_wall || intersects_with_right_wall) {
        if (intersects_with_left_wall) {
            state.racket_2_score += 1;
        } else {
            state.racket_1_score += 1;
        }

        if (state.racket_1_score == GOALS_TO_WIN || state.racket_2_score == GOALS_TO_WIN) {
            state.current_scene = GAME_OVER;
        }

        state.ball_position.y = config.height / 2;
        state.ball_position.x = config.width / 2;
        state.ball_velocity.x = -state.ball_velocity.x;
        return state;
    }

    state.ball_position.y = next_ball_y;
    state.ball_position.x = next_ball_x;

    return state;
}

GameState process_updates(GameConfig config, GameState state, int frame_count) {
    if (frame_count % BALL_PHYSICS_UPDATE_FRAMES_COUNT == 0) {
        state = move_ball(config, state);
    }

    return state;
}

GameState process_game_scene(int key, GameConfig config, GameState state, int frame_count) {
    state = process_key(key, config, state);
    state = process_updates(config, state, frame_count);
    return state;
}

void draw_score(int y, int x, int racket_1_score, int racket_2_score) {
    mvprintw(y, x, "Score: %02d/%02d", racket_1_score, racket_2_score);
}

void draw_racket(int y, int x, int height) { draw_vertical_line(y, x, height); }

void draw_ball(int y, int x) { mvaddch(y, x, BALL); }

void draw_rectangle(int y, int x, int height, int width) {
    int ul_corner_y = y;
    int ul_corner_x = x;

    int ur_corner_y = y;
    int ur_corner_x = x + width;

    int lr_corner_y = y + height;
    int lr_corner_x = x + width;

    int ll_corner_y = y + height;
    int ll_corner_x = x;

    mvaddch(ul_corner_y, ul_corner_x, CORNER_UL);

    draw_horizontal_line(ul_corner_y, ul_corner_x + 1, width - 1);

    mvaddch(ur_corner_y, ur_corner_x, CORNER_UR);

    draw_vertical_line(ur_corner_y + 1, ur_corner_x, height - 1);

    mvaddch(lr_corner_y, lr_corner_x, CORNER_LR);

    draw_horizontal_line(ll_corner_y, ll_corner_x + 1, width - 1);

    mvaddch(ll_corner_y, ll_corner_x, CORNER_LL);

    draw_vertical_line(ul_corner_y + 1, ll_corner_x, height - 1);
}

void draw_horizontal_line(int y, int x, int width) {
    for (int i = x; i < x + width; i++) {
        mvaddch(y, i, HORIZONTAL_LINE);
    }
}

void draw_vertical_line(int y, int x, int height) {
    for (int i = y; i < y + height; i++) {
        mvaddch(i, x, VERTICAL_LINE);
    }
}

bool is_box_collision(int rect1_x, int rect1_y, int rect1_width, int rect1_height, int rect2_x, int rect2_y,
                      int rect2_width, int rect2_height) {
    return (rect1_x < rect2_x + rect2_width && rect1_x + rect1_width > rect2_x &&
            rect1_y < rect2_y + rect2_height && rect1_y + rect1_height > rect2_y);
}

void draw_scene(GameConfig config, GameState state) {
    switch (state.current_scene) {
        case GAME:
            draw_game_scene(config, state);
            break;
        case GAME_OVER:
            draw_game_over_scene(config, state);
            break;
        default:
            break;
    }
}

GameState get_initial_state(GameConfig config) {
    int half_height = config.height / 2;
    int half_width = config.width / 2;

    GameState state = {
        .racket_1_score = 0,
        .racket_2_score = 0,
        .racket_1_position = {.y = half_height / 2, .x = 1},
        .racket_2_position = {.y = half_height / 2, .x = config.width - 1},
        .ball_position = {.y = half_height, .x = half_width},
        .ball_velocity = {.y = 1, .x = 1},
        .current_scene = GAME,
    };

    return state;
}

GameState update_state(int key, GameConfig config, GameState state, int frame_count) {
    switch (state.current_scene) {
        case GAME:
            return process_game_scene(key, config, state, frame_count);
        case GAME_OVER:
            return process_game_over_scene(key, config, state);
        default:
            return state;
    }
}