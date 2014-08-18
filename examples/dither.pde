void setup() {
}

void draw() {
    background(0);

    for (int x = 0; x < width; x++) {
        stroke(map(x,0,width,0,255));
        line(x,0,x,height);
    }

    delay(40);
}