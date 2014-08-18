#include "processing-terminal.h"
int i = 0;
void setup() {
    printf("setup\n");
}

void draw() {
	background(0);
    text(width,30,0);
    text(height,35,0);
    text(i,40,20);
    point(width/2,height/2);
    //delay(10);
    line(i,0,i,height);
    i++;
    i = i% width;
}int main(int argc, char **argv) {
    running = 1;
    cv = caca_create_canvas(0, 0);
    if (!cv) {
        fprintf(stderr, "%s: unable to initialise libcaca\n", argv[0]);
        return 1;
    }

    dp = caca_create_display_with_driver(cv, "ncurses"); // x11, raw..

    if (dp == NULL ) {
        printf("Failed to create display\n");
        return 1;
    }
    width = caca_get_canvas_width(cv);
    height = caca_get_canvas_height(cv);

    setup();

    while (running) {
        width = caca_get_canvas_width(cv);
        height = caca_get_canvas_height(cv);
        draw();
        caca_refresh_display(dp);
        while (caca_get_event(dp, CACA_EVENT_ANY, &ev, 0)) {
            if ((caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS)) {
                running = 0;
            }
        }
    }
    caca_free_display(dp);
    cucul_free_canvas(cv);
    return 1;
}




void background(int color) {
    caca_clear_canvas(cv);
}

void text(int a, int x, int y) {
    caca_printf(cv, x, y, "%d", a);
}

void point(int x, int y) {
    //  mvprintw(y, x,  "*");
    //        caca_putchar(cv,x,y,'.'):
    caca_printf(cv, x, y, ".");

}
void line(int x0, int y0, int x1, int y1) {

    //println("line $x0 $y0 $x1 $y1");
    int dx = abs( x1 - x0 );
    int dy = abs( y1 - y0 );
    int sx = 0;
    int sy = 0;
    if ( x0 < x1 ) {
        sx = 1;
    } else {
        sx = -1;
    }
    if ( y0 < y1 ) {
        sy = 1;
    } else {
        sy = -1;
    }
    int err = ( dx - dy );

    while (1) {
        point( x0, y0);
        if ( x0 == x1 && y0 == y1 ) {
            break;
        }
        int e2 = 2 * err;
        if ( e2 > dy * -1 ) {
            err = err - dy;
            x0  = x0 + sx;
        }
        if ( e2 < dx ) {
            err = err + dx;
            y0  = y0 + sy;
        }

    }



}



void delay (unsigned int howLong) {
    struct timespec sleeper, dummy ;
    sleeper.tv_sec  = (time_t)(howLong / 1000) ;
    sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;
    nanosleep (&sleeper, &dummy) ;
}


void list_driver() {
    list = caca_get_display_driver_list();

    int i, cur = 0;
    caca_printf(cv, 2, 1, "Available drivers:");
    char const *driver;
    driver = caca_get_display_driver(dp);
    caca_set_color_ansi(cv, CACA_WHITE, CACA_RED);
    for (i = 0; list[i]; i += 2) {
        int match = !strcmp(list[i], driver);

        if (match) {
            cur = i;
        }
        caca_printf(cv, 2, i + 2, "%c %s (%s)",  match ? '*' : ' ', list[i], list[i + 1]);
    }
}


