#include "processing-terminal.h"

/*
    Copyright (c) 2014 by Ralf Baecker <rlfbckr ~AT~ gmail.com>

    This file is part of Processing-Terminal.

    A wrapper around libcaca and ncurses to offer an ascii-art version to
    the processing language

    Processing-Terminal is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Processing-Terminal.  If not, see
    <http://www.gnu.org/licenses/>.
*/

processingterminal pt;
int width;
int height;
double frameRate;
char key;

// for millis
static uint64_t pt_epochMilli, pt_epochMicro ;


// kbhit
static struct termios initial_settings, new_settings;
static int peek_character = -1;
void *keyboard_thread(void *) {
    while (1) {
        key = readch();
        if (key == 27) {
            pt.running = 0;
        }
        if (key == 32) {
            pt.PT_paused = !pt.PT_paused;
        }

    }
    return NULL;

}
void processing_terminal() {

    setup();
    while (pt.running) {

        //width = caca_get_canvas_width(pt.cv);
        // height = caca_get_canvas_height(pt.cv);

        if (!pt.PT_USE_DITHERING) {
            width = caca_get_canvas_width(pt.cv) / X_SCALE;
            height = caca_get_canvas_height(pt.cv);
        } else {
            width = pt.PT_bitmap_width;
            height = pt.PT_bitmap_height;

        }
        pt.DITHERED_DRAW = 0;
        pt.textrenderjoblist_items = 0;
        if (!pt.PT_paused) {
            draw();
        }

        if (pt.PT_USE_DITHERING &&  pt.DITHERED_DRAW) {
            caca_dither_bitmap(caca_get_canvas(pt.dp), 0, 0, caca_get_canvas_width(pt.cv),
                               caca_get_canvas_height(pt.cv), pt.PT_dither, pt.PT_buffer);
        }
        render_text_items();
        /*
        last_frame_millis = this_frame_millis;
        this_frame_millis = millis();
        long duration =  (this_frame_millis - last_frame_millis);
        */
        int duration = caca_get_display_time(pt.dp);
        if (duration > 0) {
            frameRate = (frameRate * 0.9) + ((1000000 / duration) * 0.1); //(1000 / duration);
        }
        caca_printf(pt.cv, 0, height - 1, "%d/%d  key=%c (%d) fps=%f (%d)", width, height, key, key, frameRate,duration);
        caca_refresh_display(pt.dp);

    }
    if (pt.PT_USE_DITHERING)  caca_free_dither(pt.PT_dither);
    caca_free_display(pt.dp);
    caca_free_canvas(pt.cv);
    closeKeyboard();
}


int init() {
    initMillis () ;
    initKeyboard();

    pthread_t k_t;
    pthread_create(&k_t, NULL, &keyboard_thread, NULL);

    printf("init ");
    sprintf(pt.density, " .',-+:;=o&%%/$*W@#");

    pt.C_stroke_r = 255;
    pt.C_stroke_g = 255;
    pt.C_stroke_b = 255;

    pt.C_color = 0;
    pt.C_pixel = '.';
    pt.running = 1;
    pt.PT_paused = 0;
    pt.PT_keyblocked = 0;

 

    pt.PT_bitmap_width  = 120;
    pt.PT_bitmap_height = 80;

    ellipseMode(CENTER);
    pt.cv = caca_create_canvas(0, 0);
    if (!pt.cv) {
        fprintf(stderr, "unable to initialise libcaca\n");
        return 1;
    }

    pt.dp = caca_create_display_with_driver(pt.cv, "ncurses"); // x11, raw..
    if (pt.dp == NULL ) {
        printf("Failed to create display\n");
        return 1;
    }
    if (!pt.PT_USE_DITHERING) {
        width = caca_get_canvas_width(pt.cv) / X_SCALE;
        height = caca_get_canvas_height(pt.cv);
    } else {
        width = pt.PT_bitmap_width;
        height = pt.PT_bitmap_height;

    }
   // caca_set_display_time(pt.dp,40000);
    useDithering();
    stroke(255);
    setDitherResolution(caca_get_canvas_width(pt.cv), caca_get_canvas_height(pt.cv));
    background(0);
    printf("done ");

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
    if (x >= 0 && x < pt.PT_bitmap_width && y >= 0 && y < pt.PT_bitmap_height) {
        pt.PT_buffer[y * pt.PT_bitmap_width + x] = color;
        pt.DITHERED_DRAW = 1;
    }
}

void set_pixel_in_bitmap(int x, int y, int r, int g, int b, int a) {
    if (x >= 0 && x < pt.PT_bitmap_width && y >= 0 && y < pt.PT_bitmap_height) {
        pt.PT_buffer[y * pt.PT_bitmap_width + x] = (r << 16) | (g << 8) | (b << 0);
        pt.DITHERED_DRAW = 1;
    }
}

void list_driver() {
    pt.list = caca_get_display_driver_list();

    int i, cur = 0;
    caca_printf(pt.cv, 2, 1, "Available drivers:");
    char const *driver;
    driver = caca_get_display_driver(pt.dp);
    caca_set_color_ansi(pt.cv, CACA_WHITE, CACA_RED);
    for (i = 0; pt.list[i]; i += 2) {
        int match = !strcmp(pt.list[i], driver);

        if (match) {
            cur = i;
        }
        caca_printf(pt.cv, 2, i + 2, "%c %s (%s)",  match ? '*' : ' ', pt.list[i], pt.list[i + 1]);
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


void fill(color_pt color) {
    pt.C_color =  (int) color;
}
void fill(float b) {
    fill((int) b);
}

void fill(int b) {
    if (pt.PT_USE_DITHERING) {
        fill(b, b, b);
    } else {
        pt.C_Fpixel = pt.density[(int)map(b, 0, 255, 0, sizeof(pt.density) - 1)];

    }
}

void fill(int r, int g, int b) {
    pt.C_Fcolor = (r << 16) | (g << 8) | (b << 0);
    pt.C_Fstroke_r = r;
    pt.C_Fstroke_g = g;
    pt.C_Fstroke_b = b;
}
void noStroke() {

}
void frameRate_pt(int rate) {

}
void stroke(color_pt color) {
    pt.C_color =  (int) color;
}
void stroke(float b) {
    stroke((int) b);
}

void stroke(int b) {
    if (pt.PT_USE_DITHERING) {
        stroke(b, b, b);
    } else {
        pt.C_pixel = pt.density[(int)map(b, 0, 255, 0, sizeof(pt.density) - 1)];

    }
}

void stroke(int r, int g, int b) {
    pt.C_color = (r << 16) | (g << 8) | (b << 0);
    pt.C_stroke_r = r;
    pt.C_stroke_g = g;
    pt.C_stroke_b = b;
}

void size(int w, int h) {
    // dummy
    setDitherResolution(w, h);
}

void translate(int x, int y) {
    // dummy
}

void background(color_pt color) {
    for (int y = 0; y < pt.PT_bitmap_height; y++) {
        for (int x = 0; x < pt.PT_bitmap_width; x++) {
            set_pixel_in_bitmap(x, y, color);
        }
    }
}

void background(int color) {

    background(color, color, color);
}


void background(int r, int g, int b) {
    if (pt.PT_USE_DITHERING) {
        for (int y = 0; y < pt.PT_bitmap_height; y++) {
            for (int x = 0; x < pt.PT_bitmap_width; x++) {
                set_pixel_in_bitmap(x, y, r, g, b, 255);
            }
        }
    } else {
        caca_clear_canvas(pt.cv);
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
void text(unsigned int a, int x, int y) {
    textrenderjob_pt newtext;
    newtext.value_u_int = a;
    newtext.x = x;
    newtext.y = y;
    newtext.type = TEXT_U_INT;

    pt.textrenderjoblist[pt.textrenderjoblist_items] = newtext;
    pt.textrenderjoblist_items++;
}
void text(int a, int x, int y) {
    textrenderjob_pt newtext;
    newtext.value_int = a;
    newtext.x = x;
    newtext.y = y;
    newtext.type = TEXT_INT;

    pt.textrenderjoblist[pt.textrenderjoblist_items] = newtext;
    pt.textrenderjoblist_items++;
    //    caca_printf(pt.cv, x * X_SCALE, y, "%d", a);
}

void text(double a, int x, int y) {
    textrenderjob_pt newtext;
    newtext.value_float = a;
    newtext.x = x;
    newtext.y = y;
    newtext.type = TEXT_FLOAT;

    pt.textrenderjoblist[pt.textrenderjoblist_items] = newtext;
    pt.textrenderjoblist_items++;
    //    caca_printf(pt.cv, x * X_SCALE, y, "%d", a);
}

void render_text_items() {
    for (int i = 0; i < pt.textrenderjoblist_items; i++) {
        if (pt.textrenderjoblist[i].type == TEXT_INT) {
            caca_printf(pt.cv, pt.textrenderjoblist[i].x * X_SCALE, pt.textrenderjoblist[i].y, "%d", pt.textrenderjoblist[i].value_int);
        }
        if (pt.textrenderjoblist[i].type == TEXT_FLOAT) {
            caca_printf(pt.cv, pt.textrenderjoblist[i].x * X_SCALE, pt.textrenderjoblist[i].y, "%f", pt.textrenderjoblist[i].value_float);
        }
        if (pt.textrenderjoblist[i].type == TEXT_U_INT) {
            caca_printf(pt.cv, pt.textrenderjoblist[i].x * X_SCALE, pt.textrenderjoblist[i].y, "%d", pt.textrenderjoblist[i].value_u_int);
        }
    }
}

void rect(int x, int y, int w, int h) {
    if (!pt.PT_USE_DITHERING) {
        x = x * X_SCALE;
        w = w * X_SCALE;
        caca_draw_box(pt.cv, x, y, w, h, pt.C_pixel);
    } else {
        line(x, y, x + w, y);
        line(x + w, y, x + w, y + h);
        line(x + w, y + h, x, y + h);
        line(x, y + h, x, y);
    }
}


void point(int x, int y) {
    if (!pt.PT_USE_DITHERING) {
        caca_printf(pt.cv, x * X_SCALE, y, "%c", pt.C_pixel);
    } else {

        set_pixel_in_bitmap(x, y, pt.C_color);
    }
}


void ellipseMode(int mode) {
    pt.ellipsemode = mode;
}

void ellipse (int x0, int y0, int width, int height) {
    if (pt.ellipsemode == CENTER) {
        width = width / 2;
        height = height / 2;
    } else if (pt.ellipsemode == CORNER) {
        width = width / 2;
        height = height / 2;
        x0 = x0 + (width);
        y0 = y0 + (height);
    }

    int a2 = width * width;
    int b2 = height * height;
    int fa2 = 4 * a2, fb2 = 4 * b2;

    int  x, y, sigma;

    /* first half */
    for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2 * x <= a2 * y; x++) {
        set_pixel_in_bitmap (x0 + x, y0 + y, pt.C_color);
        set_pixel_in_bitmap (x0 - x, y0 + y, pt.C_color);
        set_pixel_in_bitmap (x0 + x, y0 - y, pt.C_color);
        set_pixel_in_bitmap (x0 - x, y0 - y, pt.C_color);
        if (sigma >= 0) {
            sigma += fa2 * (1 - y);
            y--;
        }
        sigma += b2 * ((4 * x) + 6);
    }

    /* second half */
    for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2 * y <= b2 * x; y++) {
        set_pixel_in_bitmap (x0 + x, y0 + y, pt.C_color);
        set_pixel_in_bitmap (x0 - x, y0 + y, pt.C_color);
        set_pixel_in_bitmap (x0 + x, y0 - y, pt.C_color);
        set_pixel_in_bitmap (x0 - x, y0 - y, pt.C_color);
        if (sigma >= 0) {
            sigma += fb2 * (1 - x);
            x--;
        }
        sigma += a2 * ((4 * y) + 6);
    }
}

void line(int x0, int y0, int x1, int y1) {
    if (!pt.PT_USE_DITHERING) {
        caca_draw_line(pt.cv, x0 * X_SCALE, y0, x1 * X_SCALE, y1, pt.C_pixel);
    } else {

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
            set_pixel_in_bitmap(x0, y0, pt.C_color);
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
void  colorMode(int mode, int a, int b, int c) {

}
void setDitherResolution(int width, int height) {
    pt.PT_bitmap_width = width / X_SCALE;
    pt.PT_bitmap_height = height;
    pt.PT_dither = caca_create_dither(32, pt.PT_bitmap_width, pt.PT_bitmap_height, 4 *  pt.PT_bitmap_width,
                                      0x00ff0000, 0x0000ff00, 0x000000ff, 0x0);

}

void useDithering() {
    pt.PT_USE_DITHERING = 1;
    setDitherResolution(caca_get_canvas_width(pt.cv), caca_get_canvas_height(pt.cv));

}
void noDithering() {
    pt.PT_USE_DITHERING = 0;
}

void noFill() {

}

void noLoop() {

}
color_pt lerpColor(color_pt c1, color_pt c2, float amt) {
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
    int ret = (((int) (a1 + (a2 - a1) * amt) << 24) |
               ((int) (r1 + (r2 - r1) * amt) << 16) |
               ((int) (g1 + (g2 - g1) * amt) << 8) |
               ((int) (b1 + (b2 - b1) * amt)));
    return (long long) ret;
}

color_pt lerpColor(color_pt c1, color_pt c2, float amt, int mode) {
    return lerpColor( c1,  c2,  amt);
}


unsigned int millis (void) {
    struct timeval tv ;
    uint64_t now ;
    gettimeofday (&tv, NULL) ;
    now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;
    return (uint32_t)(now - pt_epochMilli) ;
}

static void initMillis(void) {
    struct timeval tv ;
    gettimeofday (&tv, NULL) ;
    pt_epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000) ;
    pt_epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec) ;
}

void initKeyboard() {
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    key = ' ';
}

void closeKeyboard() {
    tcsetattr(0, TCSANOW, &initial_settings);
}


int readch() {
    char ch;

    if (peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0, &ch, 1);
    return ch;
}

