extern void __libc_csu_init (int argc, char **argv, char **envp);

extern void sketchEarlyInits();

extern void sketchLateInits();

extern void runSketch();

extern void init();

int main() {

  /* __libc_csu_init (0, 0, 0); Is this already handled for us?  */  /* run C++ static initializers */
  init();

  sketchEarlyInits();

  sketchLateInits();

  runSketch();

  return 0; /* NOT REACHED */
}
