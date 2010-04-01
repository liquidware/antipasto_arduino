/**
 *
 * ShapesRandom
 * Example by Chris Ladden
 *
 * This example demonstrates how to draw primitive
 * shapes on the TouchShield using the Processing language
 */

void setup() {
    fill(0,0,255); //fill the rectangle with a blue color
    stroke(255, 255, 255); //make the outline a white color
}

void loop() {
    rect(random(128), random(128), 40, 40); //draw a rectangle at a random location

    ellipse(random(128), random(128), 40, 40); //draw an ellipse at a random location

    line(0, 0, random(128), random(128));  //draw a line to a random location, ancored a zero

    delay(500);
}
