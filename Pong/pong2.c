#include <stdio.h>

#define WEIGHT 80
#define HEIGHT 25
#define ROCKET_SIZE 3
#define GOALS_TO_WIN 1

void draw_score(int check_player_1, int check_player_2);
void draw_horizontal_borders(int width);
void draw_bottom_border(int width);
void draw_vertical_borders_and_content(int width, int height, int player_height_left_1,
                                       int player_height_right_2, int x_ball, int y_ball);
void draw_field_content(int width, int current_line, int player_height_left_1, int player_height_right_2,
                        int x_ball, int y_ball);
char get_cell_character(int x, int y, int player_height_left_1, int player_height_right_2, int x_ball,
                        int y_ball);
int is_left_racket(int x, int y, int player_height_left_1);
int is_right_racket(int x, int y, int player_height_right_2);
int is_ball(int x, int y, int ball_x, int ball_y);
void field_grafics(int width, int height, int player_height_left_1, int player_height_right_2, int x_ball,
                   int y_ball, int check_player_1, int check_player_2);
void draw_winner(int check_player_1, int check_player_2);

int can_move_down(int rocket);
int move_players(int rocket, char direction);
int can_move_up(int rocket);
int is_down_move(char direction);
int is_up_move(char direction);

int update_ball_x(int x_ball, int moving_x_ball);
int update_ball_y(int y_ball, int moving_y_ball);
int check_top_collision(int y_ball, int moving_y_ball);
int check_below_collision(int y_ball, int moving_y_ball);
int check_left_racket_collision(int x_ball, int y_ball, int player_height_left_1, int moving_x_ball);
int check_right_racket_collision(int x_ball, int y_ball, int player_height_right_2, int moving_x_ball);
int check_left_goal(int x_ball, int check_player_2);
int check_right_goal(int x_ball, int check_player_1);
#define RESET_BALL(x, y, mx, my, dir_x, dir_y) char get_player_input();
void invalid_input();
int player_2_input(char input, int player_height_2);
int player_1_input(char input, int player_height_1);

int handle_top_collision(int y_ball, int moving_y_ball);
int handle_bottom_collision(int y_ball, int moving_y_ball);
int handle_left_racket_collision(int x_ball, int y_ball, int player_height_1, int moving_x_ball);
int handle_right_racket_collision(int x_ball, int y_ball, int player_height_2, int moving_x_ball);

int main() {
    int x_ball = WEIGHT / 2;
    int y_ball = HEIGHT / 2;
    int moving_x_ball = 1;
    int moving_y_ball = 1;
    int check_player_1 = 0;
    int check_player_2 = 0;

    int player_height_1 = HEIGHT / 2 - ROCKET_SIZE / 2;
    int player_height_2 = HEIGHT / 2 - ROCKET_SIZE / 2;

    while (check_player_1 < GOALS_TO_WIN && check_player_2 < GOALS_TO_WIN) {
        field_grafics(WEIGHT, HEIGHT, player_height_1, player_height_2, x_ball, y_ball, check_player_1,
                      check_player_2);

        char input = get_player_input();
        player_height_1 = player_1_input(input, player_height_1);
        player_height_2 = player_2_input(input, player_height_2);

        x_ball = update_ball_x(x_ball, moving_x_ball);
        y_ball = update_ball_y(y_ball, moving_y_ball);

        moving_y_ball = handle_top_collision(y_ball, moving_y_ball);
        moving_y_ball = handle_bottom_collision(y_ball, moving_y_ball);

        moving_x_ball = handle_left_racket_collision(x_ball, y_ball, player_height_1, moving_x_ball);
        moving_x_ball = handle_right_racket_collision(x_ball, y_ball, player_height_2, moving_x_ball);

        if (x_ball >= WEIGHT) {
            check_player_1++;
            RESET_BALL(x_ball, y_ball, moving_x_ball, moving_y_ball, -1, -1);
        }
        if (x_ball < 0) {
            check_player_2++;
            RESET_BALL(x_ball, y_ball, moving_x_ball, moving_y_ball, 1, 1);
        }
    }
    draw_winner(check_player_1, check_player_2);
    return 0;
}

int handle_top_collision(int y_ball, int moving_y_ball) {
    if (y_ball <= 0) {
        return 1;
    }
    return moving_y_ball;
}

int handle_bottom_collision(int y_ball, int moving_y_ball) {
    if (y_ball >= HEIGHT - 1) {
        return -1;
    }
    return moving_y_ball;
}

int handle_left_racket_collision(int x_ball, int y_ball, int player_height_1, int moving_x_ball) {
    if (x_ball == 1 && y_ball >= player_height_1 && y_ball < player_height_1 + ROCKET_SIZE) {
        return 1;
    }
    return moving_x_ball;
}

int handle_right_racket_collision(int x_ball, int y_ball, int player_height_2, int moving_x_ball) {
    if (x_ball == WEIGHT - 2 && y_ball >= player_height_2 && y_ball < player_height_2 + ROCKET_SIZE) {
        return -1;
    }
    return moving_x_ball;
}

void draw_score(int check_player_1, int check_player_2) {
    printf("Player 1: %02d", check_player_1);
    int padding = WEIGHT - 20;
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("Player 2: %02d\n", check_player_2);
}

void field_grafics(int width, int height, int player_height_left_1, int player_height_right_2, int x_ball,
                   int y_ball, int check_player_1, int check_player_2) {
    draw_score(check_player_1, check_player_2);
    draw_horizontal_borders(width);
    draw_vertical_borders_and_content(width, height, player_height_left_1, player_height_right_2, x_ball,
                                      y_ball);
    draw_bottom_border(width);
}

#define RESET_BALL(x, y, mx, my, dir_x, dir_y) \
    do {                                       \
        (x) = WEIGHT / 2;                      \
        (y) = HEIGHT / 2;                      \
        (mx) = (dir_x);                        \
        (my) = (dir_y);                        \
    } while (0)

void draw_vertical_borders_and_content(int width, int height, int player_height_left_1,
                                       int player_height_right_2, int x_ball, int y_ball) {
    for (int i = 0; i <= height; i++) {
        printf("┃");
        draw_field_content(width, i, player_height_left_1, player_height_right_2, x_ball, y_ball);
        printf("┃\n");
    }
}

void draw_field_content(int width, int current_line, int player_height_left_1, int player_height_right_2,
                        int x_ball, int y_ball) {
    for (int j = 0; j <= width; j++) {
        char cell_char =
            get_cell_character(j, current_line, player_height_left_1, player_height_right_2, x_ball, y_ball);
        printf("%c", cell_char);
    }
}

char get_cell_character(int x, int y, int player_height_left_1, int player_height_right_2, int x_ball,
                        int y_ball) {
    if (is_left_racket(x, y, player_height_left_1)) {
        return '|';
    }
    if (is_right_racket(x, y, player_height_right_2)) {
        return '|';
    }
    if (is_ball(x, y, x_ball, y_ball)) {
        return 'o';
    }
    return ' ';
}

int is_left_racket(int x, int y, int player_height_left_1) {
    return (x == 0 && y >= player_height_left_1 && y < player_height_left_1 + ROCKET_SIZE);
}

int is_right_racket(int x, int y, int player_height_right_2) {
    return (x == WEIGHT - 1 && y >= player_height_right_2 && y < player_height_right_2 + ROCKET_SIZE);
}

int is_ball(int x, int y, int ball_x, int ball_y) { return (x == ball_x && y == ball_y); }

void draw_horizontal_borders(int width) {
    printf("┏");
    for (int i = 0; i <= width; i++) {
        printf("━");
    }
    printf("┓\n");
}

void draw_bottom_border(int width) {
    printf("┗");
    for (int i = 0; i <= width; i++) {
        printf("━");
    }
    printf("┛\n");
}

int is_up_move(char direction) {
    return direction == 'A' || direction == 'a' || direction == 'K' || direction == 'k';
}

int is_down_move(char direction) {
    return direction == 'Z' || direction == 'z' || direction == 'M' || direction == 'm';
}

int can_move_up(int rocket) { return rocket > 0; }

int can_move_down(int rocket) { return rocket < HEIGHT - ROCKET_SIZE; }

int move_players(int rocket, char direction) {
    if (is_up_move(direction) && can_move_up(rocket)) return rocket - 1;
    if (is_down_move(direction) && can_move_down(rocket)) return rocket + 1;
    return rocket;
}

int update_ball_x(int x_ball, int moving_x_ball) { return x_ball + moving_x_ball; }

int update_ball_y(int y_ball, int moving_y_ball) { return y_ball + moving_y_ball; }

char get_player_input() {
    char input;
    while (1) {
        input = getchar();
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        if (input == 'A' || input == 'a' || input == 'Z' || input == 'z' || input == 'K' || input == 'k' ||
            input == 'M' || input == 'm' || input == ' ') {
            return input;
        } else {
            printf("Ошибка Ввода! Используйте A/Z (Player 1), K/M (Player 2) или SPACE\n");
            printf("Команда: ");
        }
    }
}

int player_1_input(char input, int player_height_1) {
    switch (input) {
        case 'A':
        case 'a':
        case 'Z':
        case 'z':
            return move_players(player_height_1, input);
        default:
            return player_height_1;
    }
}

int player_2_input(char input, int player_height_2) {
    switch (input) {
        case 'K':
        case 'k':
        case 'M':
        case 'm':
            return move_players(player_height_2, input);
        default:
            return player_height_2;
    }
}

void draw_winner(int check_player_1, int check_player_2) {
    printf("\033[2J\033[H");

    if (check_player_1 >= 21) {
        printf("\n\n");
        printf("          ╔══════════════════════════════════╗\n");
        printf("          ║                                  ║\n");
        printf("          ║        🏆 PLAYER 1 WINS! 🏆      ║\n");
        printf("          ║                                  ║\n");
        printf("          ║        °º¤ø,¸¸,ø¤º°`°º¤ø,¸       ║\n");
        printf("          ║                                  ║\n");
        printf("          ║         Final: %02d - %02d           ║\n", check_player_1, check_player_2);
        printf("          ║                                  ║\n");
        printf("          ║       Press any key to exit      ║\n");
        printf("          ║                                  ║\n");
        printf("          ╚══════════════════════════════════╝\n");
    } else {
        printf("\n\n");
        printf("          ╔══════════════════════════════════╗\n");
        printf("          ║                                  ║\n");
        printf("          ║        🏆 PLAYER 2 WINS! 🏆      ║\n");
        printf("          ║                                  ║\n");
        printf("          ║        °º¤ø,¸¸,ø¤º°`°º¤ø,¸       ║\n");
        printf("          ║                                  ║\n");
        printf("          ║         Final: %02d - %02d           ║\n", check_player_1, check_player_2);
        printf("          ║                                  ║\n");
        printf("          ║       Press any key to exit      ║\n");
        printf("          ║                                  ║\n");
        printf("          ╚══════════════════════════════════╝\n");
    }
    getchar();
}