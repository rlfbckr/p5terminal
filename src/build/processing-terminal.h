#ifndef CURSESSING_H
#define CURSESSING_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <caca.h>
#include <pthread.h>
bool EXIT;


char const *const *list;
caca_canvas_t *cv;
caca_display_t *dp;
caca_event_t ev;
int running;
void list_driver();
void *keyboard_thread();

// processing stuff
int width;
int height;

void setup(void);
void loop(void);

void delay (unsigned int howLong);



void text(int a, int x, int y);
void point(int x, int y);
void line(int x0, int y0, int x1, int y1);
void background(int color);
#endif


