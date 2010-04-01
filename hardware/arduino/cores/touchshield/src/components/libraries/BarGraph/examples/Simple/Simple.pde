/**
 * This example displays a bar graph on the
 * screen of a Liquidware TouchShield Slide
 *  http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
 *
 * By: Chris Ladden, avrman@gmail.com
 */

#include <BarGraph.h>


BarGraph bar = BarGraph("Bar Graph", 60, 20);

void setup() {
}

void loop(){
    bar.setValue(random(-5,60));
    delay(random(0,30));
}

