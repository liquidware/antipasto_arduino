/**
 *
 * LiquidPic
 * Example by Chris Ladden
 *
 * This example demonstrates how to store 24-bit color
 * bitmaps onto the TouchShield's flash.
 *
 *  1.) Upload this sketch to the TouchShield
 *  2.) Open the Tools->Image Uploader window
 *  3.) Click Transfer in the Image Uploader window
 */

PImage liquidPic;                  //An image

void setup() {
  background(0);
  open(FlashTransfer);             //Make the TouchShield listen for images

  background(0);
  liquidPic = loadImage("liquid.bmp");  //read a bitmap from onboard flash
  image(liquidPic, 0, 0);               //draw bitmap

  image(liquidPic, 60, 60);            // draw bitmap
}

void loop() {
  ;
}

