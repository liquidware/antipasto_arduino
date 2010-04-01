/**
 * This example displays a three bar graphs on the
 * screen of a Liquidware TouchShield Slide
 *  http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
 *
 * By: Chris Ladden, avrman@gmail.com
 */

#include <BarGraph.h>


BarGraph barA = BarGraph("BarA", 60, 20);
BarGraph barB = BarGraph("BarB", 60, 90);
BarGraph barC = BarGraph("BarC",60, 170);

void setup() {
}

void loop(){

    barA.setValue(random(-5,60));
    delay(random(0,30));

    barB.setValue(random(-5,60));
    delay(random(0,30));

    barC.setValue(random(-5,60));
    delay(random(0,30));

}

