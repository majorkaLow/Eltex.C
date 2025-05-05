#include <fileManager.h>

void display_panel(WINDOW *win, Panel *panel) {
    int width, height;
    getmaxyx(win, height, width);
    werase(win);

    for (int i = 0; i < panel->file_count; i++) {
        if (i == panel->highlight) {
            wattron(win, A_REVERSE);
        }
        mvwprintw(win, i + 1, 1, "%s", panel->files[i]);
        wattroff(win, A_REVERSE);
    }

    box(win, 0, 0);
    mvwprintw(win, 0, 0, "%s", panel->path);
    wrefresh(win);
}