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

#define GOALS_TO_WIN 21

#define EXIT_SCENE 0
#define GAME_SCENE 1
#define GAME_OVER_SCENE 2

int init_ncurses(int width, int height);
int check_terminal_size(int width, int height);
void dispose_ncurses();
void render(int width, int height, int scene, int racket1_y, int racket1_x, int racket2_y, int racket2_x,
            int ball_y, int ball_x, int racket1_score, int racket2_score, int racket_height);
void run_game_cycle(int width, int height, int racket_height, int ball_size);

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

void run_game_cycle(int width, int height, int racket_height, int ball_size) {
    int half_height = height / 2;
    int half_width = width / 2;

    int racket1_score = 0;
    int racket2_score = 0;
    int racket1_y = half_height / 2;
    int racket1_x = 1;
    int racket2_y = half_height / 2;
    int racket2_x = width - 1;
    int ball_y = half_height;
    int ball_x = half_width;
    int ball_velocity_y = 1;
    int ball_velocity_x = 1;
    int current_scene = GAME_SCENE;

    int frame_count = 0;
    int ch = 0;

    while (1) {
        ch = getch();

        if (current_scene == GAME_SCENE) {
            if (ch == QUIT_KEY) {
                current_scene = EXIT_SCENE;
            }

            if (ch == RACKET_1_KEY_UP) {
                racket1_y = move_racket(racket1_y - 1, racket_height, height);
            }

            if (ch == RACKET_1_KEY_DOWN) {
                racket1_y = move_racket(racket1_y + 1, racket_height, height);
            }

            if (ch == RACKET_2_KEY_UP) {
                racket2_y = move_racket(racket2_y - 1, racket_height, height);
            }

            if (ch == RACKET_2_KEY_DOWN) {
                racket2_y = move_racket(racket2_y + 1, racket_height, height);
            }

            if (frame_count % BALL_PHYSICS_UPDATE_FRAMES_COUNT == 0) {
                int next_ball_y = ball_y + ball_velocity_y;
                int next_ball_x = ball_x + ball_velocity_x;

                if (next_ball_y <= 0 || next_ball_y >= height) {
                    ball_velocity_y = -ball_velocity_y;
                    next_ball_y = ball_y;
                }

                int collision_with_racket_1 =
                    check_box_collision(next_ball_x, next_ball_y, ball_size, ball_size, racket1_x, racket1_y,
                                        RACKET_WIDTH, racket_height);

                int collision_with_racket_2 =
                    check_box_collision(next_ball_x, next_ball_y, ball_size, ball_size, racket2_x, racket2_y,
                                        RACKET_WIDTH, racket_height);

                if (collision_with_racket_1 || collision_with_racket_2) {
                    ball_velocity_x = -ball_velocity_x;
                    next_ball_x = ball_x;
                }

                int intersects_with_left_wall = next_ball_x <= 0;
                int intersects_with_right_wall = next_ball_x >= width;

                if (intersects_with_left_wall || intersects_with_right_wall) {
                    if (intersects_with_left_wall) {
                        racket2_score += 1;
                    } else {
                        racket1_score += 1;
                    }

                    if (racket1_score == GOALS_TO_WIN || racket2_score == GOALS_TO_WIN) {
                        current_scene = GAME_OVER_SCENE;
                    }

                    ball_y = height / 2;
                    ball_x = width / 2;
                    ball_velocity_x = -ball_velocity_x;
                } else {
                    ball_y = next_ball_y;
                    ball_x = next_ball_x;
                }
            }
        }

        else if (current_scene == GAME_OVER_SCENE) {
            if (ch == QUIT_KEY) {
                current_scene = EXIT_SCENE;
            }

            if (ch == SKIP_KEY) {
                racket1_score = 0;
                racket2_score = 0;
                racket1_y = half_height / 2;
                racket1_x = 2;
                racket2_y = half_height / 2;
                racket2_x = width - 2;
                ball_y = half_height;
                ball_x = half_width;
                ball_velocity_y = 1;
                ball_velocity_x = 1;
                current_scene = GAME_SCENE;
            }
        }

        if (current_scene == EXIT_SCENE) {
            break;
        }

        clear();
        if (current_scene == GAME_SCENE) {
            draw_game_scene(width, height, racket1_y, racket1_x, racket2_y, racket2_x, ball_y, ball_x,
                            racket1_score, racket2_score, racket_height);
        } else if (current_scene == GAME_OVER_SCENE) {
            draw_game_over_scene(width, height, racket1_score, racket2_score);
        }
        refresh();

        frame_count++;
        napms(TARGET_FRAME_TIME);
    }
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

/* Логика игры */

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
    return (rect1_x < rect2_x + rect2_width && rect1_x + rect1_width > rect2_x &&
            rect1_y < rect2_y + rect2_height && rect1_y + rect1_height > rect2_y);
}