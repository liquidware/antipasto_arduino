/**
 * EllipseAtTouch
 * Example by Mike Gionfriddo
 *
 * This example draws an ellipe at
 * the point of touch on the TouchShield
 */
void setup() {
    background(0,0,0); //this paints the whole background black
    stroke(0,0,255);   //the outline of the next shape I draw will be blue
    fill(0,0,255);     //the inside fill of the next shape I draw will be blue
}

void loop() {
    gettouch(); //find out where a finger is touching
    ellipse(mouseX, mouseY, 3, 3); // draw an ellipse(x, y, width, height)
}
