int i = 0;
float dither_scale = 1;
int ditherw = 12;
int ditherh = 8;
color col;
void setup() {
    size(200, 300);
    printf("setup\n");
    useDithering();
    // setDitherResolution(ditherw, ditherh);
    col = color(255, 0, 0);

}

void draw() {
    background(0);
    //stroke(255, 255, 255);
    //setDitherResolution(dither_scale * ditherw, dither_scale * ditherh);
    bla(i);

    i++;
    dither_scale = dither_scale + 0.1;
    //delay(80);
    i = i % width;
}


void  bla(int i) {
    for (int j = -10; j < 40; j++) {
        stroke(20, 255, 10);
        //stroke(col);
        //line(10,10,30,30);
        rect(i + (j * random(0, 2)), i - j, 20, 20);
    }

}