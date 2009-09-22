extern void sketchEarlyInits();

extern void sketchLateInits();

extern void runSketch();

extern void init();

int main() {

  init();

  sketchEarlyInits();

  sketchLateInits();

  runSketch();

  return 0; /* NOT REACHED */
}
