int line_pos = 0;
void setup() {

}

void draw() {
    text(100, 0, 0);
    background(255);
    stroke(255, 0, 0);
    line(2, 2, 12, 12);

    stroke(0, 255, 0);
    rect(16, 2, 10, 10);

    stroke(0, 0, 255);
    ellipseMode(CORNER);
    ellipse(30, 2, 10, 10);

    drawGradient(0, 16, width, 1);

    stroke(0);
    line(line_pos, 18, line_pos, 22);

    line_pos++;
    line_pos = line_pos % width;

    //delay(40);
}



void drawGradient(int x, int y, int width, int height) {
    color c1 = color(255, 0, 0);
    color c2 = color(0, 255, 255);
    for (int i = 0; i <= width; i++) {
        float inter = map(i, 0, width , 0, 1);
        color c = lerpColor(c1, c2, inter);
        stroke(c);
        line(x + i, y, x + i, y + height);
    }

}

