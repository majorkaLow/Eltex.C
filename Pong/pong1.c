#include <ncurses.h>
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RACKET_WIDTH 1

#define VERTICAL_LINE ACS_VLINE
#define HORIZONTAL_LINE ACS_HLINE
#define BALL ACS_BULLET
#define CORNER_UL ACS_ULCORNER
#define CORNER_UR ACS_URCORNER
#define CORNER_LR ACS_LRCORNER
#define CORNER_LL ACS_LLCORNER

#define SKIP_KEY 32

#define RACKET_1_KEY_UP 97
#define RACKET_1_KEY_DOWN 122

#define RACKET_2_KEY_UP 107
#define RACKET_2_KEY_DOWN 109

#define QUIT_KEY 113

#define TARGET_FRAME_TIME 16
#define BALL_PHYSICS_UPDATE_FRAMES_COUNT 4

#define GOALS_TO_WIN 1

#define EXIT_SCENE 0
#define GAME_SCENE 1
#define GAME_OVER_SCENE 2

void process_ball_collision_and_scoring(int next_ball_x, int width, int half_width, int racket1_score,
                                        int racket2_score, int current_scene, int next_ball_y,
                                        int half_height, int ball_y, int ball_x, int ball_velocity_x);

int init_ncurses(int width, int height);
int check_terminal_size(int width, int height);
void dispose_ncurses();
void render(int width, int height, int scene, int racket1_y, int racket1_x, int racket2_y, int racket2_x,
            int ball_y, int ball_x, int racket1_score, int racket2_score, int racket_height);

void draw_game_scene(int width, int height, int racket1_y, int racket1_x, int racket2_y, int racket2_x,
                     int ball_y, int ball_x, int racket1_score, int racket2_score, int racket_height);
void draw_game_over_scene(int width, int height, int racket1_score, int racket2_score);
void draw_rectangle(int y, int x, int height, int width);
void draw_horizontal_line(int y, int x, int width);
void draw_vertical_line(int y, int x, int height);
void draw_racket(int y, int x, int height);
void draw_ball(int y, int x);
void draw_score(int y, int x, int racket1_score, int racket2_score);
void draw_info(int y, int x, int racket1_score, int racket2_score);

int move_racket(int y, int racket_height, int zone_height);
int check_box_collision(int rect1_x, int rect1_y, int rect1_width, int rect1_height, int rect2_x, int rect2_y,
                        int rect2_width, int rect2_height);

int check_game_over(int racket1_score, int racket2_score);
int check_goal_scored(int next_ball_x, int width);
int update_ball_velocity_x(int ball_x, int ball_velocity_x, int width, int collision_with_racket1,
                           int collision_with_racket2);
int update_ball_velocity_y(int ball_y, int ball_velocity_y, int height);
int check_collision_with_racket2(int next_ball_x, int next_ball_y, int ball_size, int racket2_x,
                                 int racket2_y, int racket_width, int racket_height);
int check_collision_with_racket1(int next_ball_x, int next_ball_y, int ball_size, int racket1_x,
                                 int racket1_y, int racket_width, int racket_height);
int calculate_next_ball_x(int ball_x, int ball_velocity_x, int width);
int calculate_next_ball_y(int ball_y, int ball_velocity_y, int height);
int update_racket2_position(int ch, int current_racket2_y, int racket_height, int height);
int update_racket1_position(int ch, int current_racket2_y, int racket_height, int height);
void run_game_cycle(int width, int height, int racket_height, int ball_size);
int should_exit_game(int ch, int current_scene);

int main() {
    int width = 80;
    int height = 25;
    int racket_height = 3;
    int ball_size = 1;

    if (init_ncurses(width, height) < 0) {
        return EXIT_FAILURE;
    };

    run_game_cycle(width, height, racket_height, ball_size);

    dispose_ncurses();

    return EXIT_SUCCESS;
}

int init_ncurses(int width, int height) {
    initscr();

    int result = check_terminal_size(width, height);
    if (result < 0) {
        endwin();
        return result;
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    return EXIT_SUCCESS;
}

int should_exit_game(int ch, int current_scene) {
    if (ch == QUIT_KEY) {
        return 1;
    }
    return 0;
}

int check_terminal_size(int width, int height) {
    int mrow;
    int mcol;

    getmaxyx(stdscr, mrow, mcol);

    if (mrow < height || mcol < width) {
        fprintf(stderr, "Terminal too small\nCurrent size: %dx%d\nMinimum size: %dx%d\n", mcol, mrow, width,
                height);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void dispose_ncurses() {
    keypad(stdscr, FALSE);
    nodelay(stdscr, FALSE);
    endwin();
}

#define reset_values()              \
    do {                            \
        racket1_score = 0;          \
        racket2_score = 0;          \
        racket1_y = height / 4;     \
        racket1_x = 2;              \
        racket2_y = height / 4;     \
        racket2_x = width - 2;      \
        ball_y = height / 2;        \
        ball_x = width / 2;         \
        ball_velocity_y = 1;        \
        ball_velocity_x = 1;        \
        current_scene = GAME_SCENE; \
    } while (0)

void run_game_cycle(int width, int height, int racket_height, int ball_size) {
    int half_height = height / 2, half_width = width / 2, racket1_score = 0, racket2_score = 0,
        racket1_y = half_height / 2, racket1_x = 1, racket2_y = half_height / 2, racket2_x = width - 1,
        ball_y = half_height, ball_x = half_width, ball_velocity_y = 1, ball_velocity_x = 1,
        current_scene = GAME_SCENE, frame_count = 0, ch = 0;

    while (current_scene != EXIT_SCENE) {
        ch = getch();
        switch (current_scene) {
            case GAME_SCENE: {
                if (should_exit_game(ch, current_scene)) {
                    current_scene = EXIT_SCENE;
                }
                racket1_y = update_racket1_position(ch, racket1_y, racket_height, height);
                racket2_y = update_racket2_position(ch, racket2_y, racket_height, height);
                if (frame_count % BALL_PHYSICS_UPDATE_FRAMES_COUNT == 0) {
                    int next_ball_y = calculate_next_ball_y(ball_y, ball_velocity_y, height);
                    int next_ball_x = calculate_next_ball_x(ball_x, ball_velocity_x, width);
                    int collision1 =
                        check_collision_with_racket1(next_ball_x, next_ball_y, ball_size, racket1_x,
                                                     racket1_y, RACKET_WIDTH, racket_height);
                    int collision2 =
                        check_collision_with_racket2(next_ball_x, next_ball_y, ball_size, racket2_x,
                                                     racket2_y, RACKET_WIDTH, racket_height);
                    ball_velocity_y = update_ball_velocity_y(ball_y, ball_velocity_y, height);
                    ball_velocity_x =
                        update_ball_velocity_x(ball_x, ball_velocity_x, width, collision1, collision2);
                    int goal_result = check_goal_scored(next_ball_x, width);

                    if (goal_result == 1) {
                        racket2_score += 1;
                    } else if (goal_result == 2) {
                        racket1_score += 1;
                    }

                    if (check_game_over(racket1_score, racket2_score)) {
                        current_scene = GAME_OVER_SCENE;
                    }

                    if (goal_result == 0) {
                        ball_y = next_ball_y;
                        ball_x = next_ball_x;
                    } else {
                        ball_y = half_height;
                        ball_x = half_width;
                        ball_velocity_x = -ball_velocity_x;
                    }
                }
            }
            case GAME_OVER_SCENE: {
                if (ch == QUIT_KEY) {
                    current_scene = EXIT_SCENE;
                }

                if (ch == SKIP_KEY) {
                    reset_values();
                    mvprintw(0, 0, "Game reset! Scene: %d", current_scene);
                    refresh();
                    napms(100);
                }
            }
            default:
                break;
        }

        clear();
        switch (current_scene) {
            case GAME_SCENE:
                draw_game_scene(width, height, racket1_y, racket1_x, racket2_y, racket2_x, ball_y, ball_x,
                                racket1_score, racket2_score, racket_height);
                break;
            case GAME_OVER_SCENE:
                draw_game_over_scene(width, height, racket1_score, racket2_score);
                break;
            default:
                break;
        }
        refresh();

        frame_count++;
        napms(TARGET_FRAME_TIME);
    }
}

int update_racket1_position(int ch, int current_racket1_y, int racket_height, int height) {
    if (ch == RACKET_1_KEY_UP) {
        return move_racket(current_racket1_y - 1, racket_height, height);
    }
    if (ch == RACKET_1_KEY_DOWN) {
        return move_racket(current_racket1_y + 1, racket_height, height);
    }
    return current_racket1_y;
}

int update_racket2_position(int ch, int current_racket2_y, int racket_height, int height) {
    if (ch == RACKET_2_KEY_UP) {
        return move_racket(current_racket2_y - 1, racket_height, height);
    }
    if (ch == RACKET_2_KEY_DOWN) {
        return move_racket(current_racket2_y + 1, racket_height, height);
    }
    return current_racket2_y;
}

int calculate_next_ball_y(int ball_y, int ball_velocity_y, int height) {
    int next_ball_y = ball_y + ball_velocity_y;

    if (next_ball_y <= 0 || next_ball_y >= height) {
        return ball_y;
    }

    return next_ball_y;
}

int calculate_next_ball_x(int ball_x, int ball_velocity_x, int width) {
    int next_ball_x = ball_x + ball_velocity_x;

    if (next_ball_x <= 0 || next_ball_x >= width) {
        return ball_x;
    }

    return next_ball_x;
}

int check_collision_with_racket1(int next_ball_x, int next_ball_y, int ball_size, int racket1_x,
                                 int racket1_y, int racket_width, int racket_height) {
    return check_box_collision(next_ball_x, next_ball_y, ball_size, ball_size, racket1_x, racket1_y,
                               racket_width, racket_height);
}

int check_collision_with_racket2(int next_ball_x, int next_ball_y, int ball_size, int racket2_x,
                                 int racket2_y, int racket_width, int racket_height) {
    return check_box_collision(next_ball_x, next_ball_y, ball_size, ball_size, racket2_x, racket2_y,
                               racket_width, racket_height);
}

int update_ball_velocity_y(int ball_y, int ball_velocity_y, int height) {
    int next_ball_y = ball_y + ball_velocity_y;

    if (next_ball_y <= 0 || next_ball_y >= height) {
        return -ball_velocity_y;
    }

    return ball_velocity_y;
}

int update_ball_velocity_x(int ball_x, int ball_velocity_x, int width, int collision_with_racket1,
                           int collision_with_racket2) {
    int next_ball_x = ball_x + ball_velocity_x;

    if (collision_with_racket1 || collision_with_racket2) {
        return -ball_velocity_x;
    }

    return ball_velocity_x;
}

int check_goal_scored(int next_ball_x, int width) {
    if (next_ball_x <= 1) {
        return 1;
    }
    if (next_ball_x >= width - 1) {
        return 2;
    }
    return 0;
}

int check_game_over(int racket1_score, int racket2_score) {
    if (racket1_score == GOALS_TO_WIN || racket2_score == GOALS_TO_WIN) {
        return 1;
    }
    return 0;
}

void draw_game_scene(int width, int height, int racket1_y, int racket1_x, int racket2_y, int racket2_x,
                     int ball_y, int ball_x, int racket1_score, int racket2_score, int racket_height) {
    draw_rectangle(0, 0, height, width);
    draw_racket(racket1_y, racket1_x, racket_height);
    draw_racket(racket2_y, racket2_x, racket_height);
    draw_ball(ball_y, ball_x);
    draw_info(height + 1, 1, racket1_score, racket2_score);
}

void draw_game_over_scene(int width, int height, int racket1_score, int racket2_score) {
    int half_height = height / 2;
    int half_width = width / 2;

    draw_rectangle(0, 0, height, width);
    mvprintw(half_height - 2, half_width - 3, "Game Over");
    draw_score(half_height, half_width - 5, racket1_score, racket2_score);
    mvprintw(half_height + 2, half_width - 16, "Press Space to restart or Q to quit");
}

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

void draw_racket(int y, int x, int height) { draw_vertical_line(y, x, height); }

void draw_ball(int y, int x) { mvaddch(y, x, BALL); }

void draw_score(int y, int x, int racket1_score, int racket2_score) {
    mvprintw(y, x, "Score: %02d/%02d", racket1_score, racket2_score);
}

void draw_info(int y, int x, int racket1_score, int racket2_score) {
    draw_score(y, x, racket1_score, racket2_score);
    mvprintw(y, x + 13, "Racket 1: A/Z, Racket 2: K/M, Quit: Q, Skip: Space");
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

int check_box_collision(int rect1_x, int rect1_y, int rect1_width, int rect1_height, int rect2_x, int rect2_y,
                        int rect2_width, int rect2_height) {
    return (rect1_x <= rect2_x + rect2_width && rect1_x + rect1_width >= rect2_x &&
            rect1_y <= rect2_y + rect2_height && rect1_y + rect1_height >= rect2_y);
}