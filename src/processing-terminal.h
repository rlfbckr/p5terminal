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

#ifndef _PROCESSING_TERMINAL_H_
#define _PROCESSING_TERMINAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <caca.h>
#include <sys/time.h>

typedef long color_pt; // fake a color type


typedef struct {
    int type;
    int value_int;
    float value_float;
    unsigned int value_u_int;
    int x;
    int y;
    int bg;
    int fg;
} textrenderjob_pt;


typedef struct {
    char density[19];
    int C_stroke_r;
    int C_stroke_g;
    int C_stroke_b;
    int C_color;

    int C_Fstroke_r;
    int C_Fstroke_g;
    int C_Fstroke_b;
    int C_Fcolor;
    char C_Fpixel;

    char C_pixel;
    int no_stroke;
    int PT_running;
    int PT_paused;
    int PT_keyblocked;

    int PT_USE_DITHERING;

    int PT_bitmap_width;
    int PT_bitmap_height;
    int width;
    int height;
    int ellipsemode;
    int DITHERED_DRAW;
    char const *const *list;
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_dither_t *PT_dither;
    uint32_t PT_buffer[640 * 480];
    textrenderjob_pt textrenderjoblist[2000];
    int textrenderjoblist_items;

} processingterminal;

extern int width;
extern int height;
extern double  frameRate;
extern processingterminal pt;

void processing_terminal();
int init();
void render_text_items();
void setup(void);
void draw(void);

void noStroke();
void noLoop();
void noFill();
void useDithering();
void noDithering();
void frameRate_pt(int rate);
void rect(int x, int y, int w, int h);
void size(int w, int h);
void delay (unsigned int howLong);
color_pt lerpColor(color_pt c1, color_pt c2, float amt);
color_pt lerpColor(color_pt c1, color_pt c2, float amt, int mode);
void stroke(color_pt color);
void stroke(int b);
void stroke(float b);
void stroke(int r, int g, int b);



void fill(color_pt color);
void fill(int b);
void fill(float b);
void fill(int r, int g, int b);

void noSmooth(void);
void translate(int x, int y);
void text(int a, int x, int y);
void text(double a, int x, int y);
void text(unsigned int a, int x, int y);
void point(int x, int y);
void line(int x0, int y0, int x1, int y1);
void rect(int x, int y, int w, int h);
void ellipse(int px, int py, int width, int height) ;
void background(int color);
void background(int r, int g, int b);
void background(color_pt b);
void  colorMode(int mode, int a, int b, int c);
//int map(int x, int in_min, int in_max, int out_min, int out_max);
float map(float x, float in_min, float in_max, float out_min, float out_max);
int random(int min, int max);
int random(int max);
color_pt color(int r, int g, int b);
color_pt color(int b);
void setDitherResolution(int width, int height);

// libcaca stuff
#define X_SCALE 1.9 // because terminal chars are not rectengular

#define RADIUS  0
#define CENTER  1
#define CORNER  2
#define CORNERS 3

#define TEXT_INT 0
#define TEXT_FLOAT 1
#define TEXT_STRING 2
#define TEXT_U_INT 3

#define HSB 0

void ellipseMode(int mode);

void list_driver();


unsigned int millis (void);
static void initialiseEpoch (void);

void set_pixel_in_bitmap(int x, int y, int r, int g, int b, int a);
#endif


