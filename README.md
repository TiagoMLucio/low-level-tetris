# Low Level Tetris
A Tetris game coded in C, running directly on bare metal Raspberry Pi 2. No OS, direct hardware control for graphics and input.

## Compile Tests

```bash
g++ -o tests/tetris tests/tetris.c -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2
```


