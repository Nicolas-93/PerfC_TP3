#define _DEFAULT_SOURCE

#include "TimeUtils/TimeUtils.h"
#include <ncurses.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>

#define REFRESH 500

void print_time(FormattedTime d) {
    mvprintw(0, 0, "%d : %2d : %2d : %2d\n", d.hour, d.min, d.sec, d.cs);
}

void add_to_chrono(FormattedTime* chrono, time_t tps_ms) {
    chrono->cs += nb_ms_vers_centiemes(tps_ms);
    chrono->sec += nb_ms_vers_secondes(tps_ms);
    chrono->min += nb_ms_vers_minutes(tps_ms);
    chrono->hour += nb_ms_vers_heures(tps_ms);
}

int main(int argc, char* argv[]) {
    struct timeval debut = {0}, fin = {0};
    long int duree_totale = 0;
    bool pause = true;
    int touche;
    FormattedTime chrono = {0, 0, 0, 0};

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    while (1) {
        touche = getch();
        chrono = ms_to_FormattedTime(duree_totale);
        print_time(chrono);
        refresh();
        if (touche == ' ') {
            if (pause) {
                gettimeofday(&debut, NULL);
            }
            pause ^= 1;
        } else if (touche == 'r') {
            pause = true;
            duree_totale = 0;
        } else if (touche == 'q')
            break;

        usleep(REFRESH * 1000);

        if (!pause) {
            gettimeofday(&fin, NULL);
            duree_totale += intervalle_ms(debut, fin);
            debut = fin;
        }
    }
    getch();
    endwin();

    return 0;
}