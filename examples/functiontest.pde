int d = 10;
int p1 = d;
int p2 = p1+d;
int p3 = p2+d;
int p4 = p3+d;

size(640, 360);
noSmooth();
background(0);
translate(140, 0);

text(p1, 0, 0);
text(p2, 0, 1);
text(p3, 0, 2);
text(p4, 0, 3);

stroke(153);
line(p3, p3, p2, p3);
line(p2, p3, p2, p2);
line(p2, p2, p3, p2);
line(p3, p2, p3, p3);

stroke(255);

point(p1, p1);
point(p1, p3); 
point(p2, p4);
point(p3, p1); 
point(p4, p2);
point(p4, p4);
