
color c1;
color c2;

void setup() {
    size(640, 360);
    useDithering();
    // Define colors
    c1 = color(255, 0, 0);
    c2 = color(20, 0, 250);

}

void draw() {
    for (int i = 0; i < width; i++) {
        color c = lerpColor(c1, c2, map(i, 0, width, 0, 1));
        stroke(map(i, 0, width, 0, 255), 0, 0);
        stroke(c);
        line(i, 10, i, height);

    }
}

