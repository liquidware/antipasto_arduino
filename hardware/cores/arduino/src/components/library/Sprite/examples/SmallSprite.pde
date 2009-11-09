#include <SmallSprite.h>

Sprite smallSprite = Sprite(1,1);

void setup() {
   smallSprite.write(1,1,5);
   smallSprite.read(1,1);
}

void loop() {

}
