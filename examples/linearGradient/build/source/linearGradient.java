import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class linearGradient extends PApplet {

/**
 * Simple Linear Gradient
 *
 * The lerpColor() function is useful for interpolating
 * between two colors.
 */

// Constants
int Y_AXIS = 1;
int X_AXIS = 2;
int b1, b2, c1, c2;

public void setup() {
    size(640, 360);
    // Define colors
    b1 = color(255);
    b2 = color(0);
    c1 = color(204, 102, 0);
    c2 = color(0, 102, 153);
    // setDitherResolution(640, 360);
    noLoop();
}

public void draw() {
    // Background
    dsetGradient(0, 0, width / 2, height, b1, b2, X_AXIS);
    dsetGradient(width / 2, 0, width / 2, height, b2, b1, X_AXIS);
    // Foreground
    dsetGradient(50, 90, 540, 80, c1, c2, Y_AXIS);
    dsetGradient(50, 190, 540, 80, c2, c1, X_AXIS);
}

public void dsetGradient(int x, int y, float w, float h, int c1, int c2, int axis ) {

    noFill();

    if (axis == Y_AXIS) {  // Top to bottom gradient
        for (int i = y; i <= y + h; i++) {
            float inter = map(i, y, y + h, 0, 1);
            int c = lerpColor(c1, c2, inter);
            stroke(c);
            line(x, i, x + w, i);
        }
    } else if (axis == X_AXIS) { // Left to right gradient
        for (int i = x; i <= x + w; i++) {
            float inter = map(i, x, x + w, 0, 1);
            int c = lerpColor(c1, c2, inter);
            stroke(c);
            line(i, y, i, y + h);
        }
    }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "linearGradient" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
