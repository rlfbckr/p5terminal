/*
    Main stub
*/

int main(int argc, char **argv) {

    if (init()) {
        return 1;
    }
    setup();
    int PT_keyblocked_t  = 0;
    while (PT_running) {
        PT_keyblocked_t--;
        if (PT_keyblocked_t <= 0) {
            PT_keyblocked = 0;
        }
        caca_event_t ev;
        //        width = caca_get_canvas_width(cv) / X_SCALE;
        width = caca_get_canvas_width(cv);

        height = caca_get_canvas_height(cv);
        if (!PT_USE_DITHERING) {
            width = caca_get_canvas_width(cv) / X_SCALE;
            height = caca_get_canvas_height(cv);
        } else {
            width = PT_bitmap_width;
            height = PT_bitmap_height;

        }
        if (!PT_paused) {
            draw();
        }
        if (PT_USE_DITHERING) {
            caca_dither_bitmap(caca_get_canvas(dp), 0, 0, caca_get_canvas_width(cv),
                               caca_get_canvas_height(cv), PT_dither, PT_buffer);
        }
        char evc = 'Z';
        while (caca_get_event(dp, CACA_EVENT_ANY, &ev, 0)) {
            evc = caca_get_event_key_ch(&ev);
            if (!PT_keyblocked) {
                //if ((caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS)) {
                //    PT_running = 0;
                //}

                if (evc == CACA_KEY_ESCAPE) {
                    PT_running = 0;
                } else if (evc == ' ') {
                    PT_paused = !PT_paused;
                    PT_keyblocked = 1;
                    PT_keyblocked_t = 100;
                }

            }
        }
        caca_printf(cv, 0, height - 1, "%d/%d blocked=%d t=%d key=%c", width, height, PT_keyblocked, PT_keyblocked_t, evc);
        caca_refresh_display(dp);
        //if (PT_USE_DITHERING)  caca_free_dither(PT_dither);

    }
    caca_free_display(dp);
    caca_free_canvas(cv);
    return 1;
}


int init() {
    cv = caca_create_canvas(0, 0);
    if (!cv) {
        fprintf(stderr, "unable to initialise libcaca\n");
        return 1;
    }

    dp = caca_create_display_with_driver(cv, "ncurses"); // x11, raw..
    if (dp == NULL ) {
        printf("Failed to create display\n");
        return 1;
    }
    if (!PT_USE_DITHERING) {
        width = caca_get_canvas_width(cv) / X_SCALE;
        height = caca_get_canvas_height(cv);
    } else {
        width = PT_bitmap_width;
        height = PT_bitmap_height;

    }
    setDitherResolution(caca_get_canvas_width(cv), caca_get_canvas_height(cv));
    background(0);
    return 0;
}


color_pt color(int r, int g, int b) {
    int ret = (r << 16) | (g << 8) | (b << 0);
    return (long long) ret;
}

color_pt color(int b) {

    return color(b, b, b);
}


void set_pixel_in_bitmap(int x, int y, int color) {
    if (x >= 0 && x < PT_bitmap_width && y >= 0 && y < PT_bitmap_height) {
        PT_buffer[y * PT_bitmap_width + x] = color;
    }
}

void set_pixel_in_bitmap(int x, int y, int r, int g, int b, int a) {
    if (x >= 0 && x < PT_bitmap_width && y >= 0 && y < PT_bitmap_height) {
        PT_buffer[y * PT_bitmap_width + x] = (r << 16) | (g << 8) | (b << 0);
    }
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




/*

    Processing-Core Functions

*/




void delay (unsigned int howLong) {
    usleep(howLong * 1000);
}


void noSmooth(void) {
    // dummy
}

void stroke(color_pt color) {
    C_color =  (int) color;
}
void stroke(float b) {
    stroke((int) b);
}

void stroke(int b) {
    if (PT_USE_DITHERING) {
        stroke(b, b, b);
    } else {
        C_pixel = density[(int)map(b, 0, 255, 0, sizeof(density) - 1)];

    }
}


void stroke(int r, int g, int b) {
    C_color = (r << 16) | (g << 8) | (b << 0);
    C_stroke_r = r;
    C_stroke_g = g;
    C_stroke_b = b;
}

void size(int w, int h) {
    // dummy
    setDitherResolution(w, h);
}

void translate(int x, int y) {
    // dummy
}

void background(color_pt color) {
    for (int y = 0; y < PT_bitmap_height; y++) {
        for (int x = 0; x < PT_bitmap_width; x++) {
            set_pixel_in_bitmap(x, y, color);
        }
    }
}

void background(int color) {

    background(color, color, color);
}


void background(int r, int g, int b) {
    if (PT_USE_DITHERING) {
        for (int y = 0; y < PT_bitmap_height; y++) {
            for (int x = 0; x < PT_bitmap_width; x++) {
                set_pixel_in_bitmap(x, y, r, g, b, 255);
            }
        }
    } else {
        caca_clear_canvas(cv);
    }
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int random(int max) {
    return random(0, max);
}
int random(int min, int max) {
    return (rand() % (max - min)) + min;
}
void text(int a, int x, int y) {
    caca_printf(cv, x * X_SCALE, y, "%d", a);
}
void rect(int x, int y, int w, int h) {
    if (!PT_USE_DITHERING) {
        x = x * X_SCALE;
        w = w * X_SCALE;
        caca_draw_box(cv, x, y, w, h, C_pixel);
    } else {
        line(x, y, x + w, y);
        line(x + w, y, x + w, y + h);
        line(x + w, y + h, x, y + h);
        line(x, y + h, x, y);
    }
}


void point(int x, int y) {
    //  mvprintw(y, x,  "*");
    //        caca_putchar(cv,x,y,'.'):
    //    caca_putstr(cv, x, y*y_spead, ".");
    if (!PT_USE_DITHERING) {
        caca_printf(cv, x * X_SCALE, y, "%c", C_pixel);
    } else {
        //x =   map(x, 0, width, 0, PT_bitmap_width);
        // y =   /map(y, 0, height, 0, PT_bitmap_height);
        set_pixel_in_bitmap(x, y, C_color);
    }
}
void line(int x0, int y0, int x1, int y1) {
    if (!PT_USE_DITHERING) {
        caca_draw_line(cv, x0 * X_SCALE, y0, x1 * X_SCALE, y1, C_pixel);
    } else {
        /*
        x0 = map(x0, 0, width, 0, PT_bitmap_width);
        x1 = map(x1, 0, width, 0, PT_bitmap_width);
        y0 = map(y0, 0, width, 0, PT_bitmap_width);
        y1 = map(y1, 0, width, 0, PT_bitmap_width);
        */
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
            // point( x0, y0);
            set_pixel_in_bitmap(x0, y0, C_color);
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
}

void setDitherResolution(int width, int height) {
    PT_bitmap_width = width;
    PT_bitmap_height = height;
    PT_dither = caca_create_dither(32, PT_bitmap_width, PT_bitmap_height, 4 *  PT_bitmap_width,
                                   0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);

}

void useDithering() {
    PT_USE_DITHERING = 1;
    setDitherResolution(caca_get_canvas_width(cv), caca_get_canvas_height(cv));

}
void noDithering() {
    PT_USE_DITHERING = 0;
}

void noFill() {

}

void noLoop() {

}
color_pt lerpColor(color_pt c1, color_pt c2, float amt) {
    //    int c1 = (int) cp1;
    //    int c2 = (int) cp2;
    // int c1 = (int) color(0,255,0);
    //  int c2 = (int) color(0,0,255);
    if (amt < 0) amt = 0;
    if (amt > 1) amt = 1;

    float a1 = ((c1 >> 24) & 0xff);
    float r1 = (c1 >> 16) & 0xff;
    float g1 = (c1 >> 8) & 0xff;
    float b1 = c1 & 0xff;

    float a2 = (c2 >> 24) & 0xff;
    float r2 = (c2 >> 16) & 0xff;
    float g2 = (c2 >> 8) & 0xff;
    float b2 = c2 & 0xff;
    /*
        int ret = (((int) (a1 + (a2 - a1) * amt) << 24) |
                   ((int) (r1 + (r2 - r1) * amt) << 16) |
                   ((int) (g1 + (g2 - g1) * amt) << 8) |
                   ((int) (b1 + (b2 - b1) * amt)));

     */
    int ret = (
                  ((int) (r1 + (r2 - r1) * amt) << 16) |
                  ((int) (g1 + (g2 - g1) * amt) << 8) |
                  ((int) (b1 + (b2 - b1) * amt))
              );

    return (long long) ret;
}

