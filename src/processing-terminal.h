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

#ifndef PROCESSING_TERMINAL_H
#define PROCESSING_TERMINAL_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <caca.h>
typedef long color_pt;
//using color_pt = int ;

int width;
int height;
int init();
void noLoop();
void noFill();
void useDithering();
void noDithering();
void setup(void);
void loop(void);
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
color_pt color(int r,int g, int b);
color_pt color(int b);


#define X_SCALE 1.9 // because terminal chars are not rectengular
char const *const *list;
caca_canvas_t *cv;
caca_display_t *dp;
caca_dither_t *PT_dither;

void list_driver();


char density[] = " .',-+:;=o&%/$*W@#";
int C_stroke_r = 255;
int C_stroke_g = 255;
int C_stroke_b = 255;
int C_color = 0;
char C_pixel = '.';
int PT_running = 1;
int PT_paused = 0;
int PT_keyblocked = 0;
void setDitherResolution(int width, int height);
int PT_USE_DITHERING = 1;

int PT_bitmap_width  = 120;
int  PT_bitmap_height = 80;

uint32_t PT_buffer[640*480];
void set_pixel_in_bitmap(int x, int y, int r, int g, int b, int a);
#endif


