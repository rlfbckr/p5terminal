int i = 0;
float dither_scale = 1;
int ditherw = 12;
int ditherh = 8;
void setup() {
    printf("setup\n");
    useDithering();
   // setDitherResolution(ditherw, ditherh);
}

void draw() {
    background(0);
    stroke(255, 255, 255);
    // text(i, 0, 0);
    //setDitherResolution(dither_scale * ditherw, dither_scale * ditherh);
    line(0, 0, width, height);
    stroke(0,255,0);
    point(width / 2, height / 2);

    /*
        for (int j = -10; j < 40; j++) {
            stroke(random(255));
            rect(i+(j*random(1,4)), i-j, 12, 12);
        }
        */

    i++;
    dither_scale = dither_scale + 0.1;
    delay(80);
    i = i % width;
}