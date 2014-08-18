int r_dist = 10;
int r_size = 8;
int offset = 2;
int x_size = 10;
int y_size = 4;
int i = 0;
void setup() {
    printf("setup\n");
    useDithering();
}

void draw() {
    background(255,0,0);
    int j = 0;
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
        	stroke(map(j,0,(x_size*y_size),0,255));
            //stroke(0,255,0);
		    rect(offset + r_dist*x, offset + r_dist*y, r_size, r_size);
		    j++;
        }

    }
    i++;
    i = i%height;
    delay(40);
}