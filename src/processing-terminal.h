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

typedef long color_pt; // fake a color type


typedef struct {
    char density[19];
    int C_stroke_r;
    int C_stroke_g;
    int C_stroke_b;

    int C_color;
    char C_pixel;
    int PT_running;
    int PT_paused;
    int PT_keyblocked;

    int PT_USE_DITHERING;

    int PT_bitmap_width;
    int PT_bitmap_height;
    int width;
    int height;

    char const *const *list;
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_dither_t *PT_dither;
    uint32_t PT_buffer[640 * 480];

} processingterminal;

extern int width;
extern int height;
extern processingterminal pt;

void processing_terminal();
int init();

void setup(void);
void draw(void);

void noLoop();
void noFill();
void useDithering();
void noDithering();

void rect(int x, int y, int w, int h);
void size(int w, int h);
void delay (unsigned int howLong);
color_pt lerpColor(color_pt cp1, color_pt cp2, float amt);
//color_pt lerpColor(int c1, int c2, float amt, int mode);
void stroke(color_pt color);
void stroke(int b);
void stroke(float b);
void stroke(int r, int g, int b);

void noSmooth(void);
void translate(int x, int y);
void text(int a, int x, int y);
void point(int x, int y);
void line(int x0, int y0, int x1, int y1);
void rect(int x, int y, int w, int h);
void background(int color);
void background(int r, int g, int b);
void background(color_pt b);

//int map(int x, int in_min, int in_max, int out_min, int out_max);
float map(float x, float in_min, float in_max, float out_min, float out_max);
int random(int min, int max);
int random(int max);
color_pt color(int r, int g, int b);
color_pt color(int b);
void setDitherResolution(int width, int height);

// libcaca stuff
#define X_SCALE 1.9 // because terminal chars are not rectengular


void list_driver();




void set_pixel_in_bitmap(int x, int y, int r, int g, int b, int a);
#endif


