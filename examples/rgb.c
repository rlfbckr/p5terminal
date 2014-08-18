#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

void print_color(long color);
long build_int_color(int r, int g, int b, int a);
long lerpColor(long c1, long c2, float amt);

int main(int argc, char **argv) {
    int c1 = build_int_color(255,0,0,255);
    int c2 = build_int_color(0,0,255,0);
    printf("c1 = \t");
    print_color(c1);
    printf("c2 = \t");
    print_color(c2);
    int mix = lerpColor(c1,c2,0.5);
    printf("mix = \t");
    print_color(mix);

}

long build_int_color(int r, int g, int b, int a) {
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

void print_color(long color) {
    int a = ((color >> 24) & 0xff);
    int r = (color >> 16) & 0xff;
    int g = (color >> 8) & 0xff;
    int b = color & 0xff;

    printf(" %d\t\t\t-->\t %d\t%d\t%d\t(%d)\n", color,r,g,b,a);
}



long lerpColor(long c1, long c2, float amt) {
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

 

    return  ret;
}

