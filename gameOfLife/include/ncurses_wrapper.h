#if __has_include(<ncurses/ncurses.h>)
#include <ncurses/ncurses.h>
#elif __has_include(<ncurses.h>)
#include <ncurses.h>
#else
#error "ncurses not found"
#endif