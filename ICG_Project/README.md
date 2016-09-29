## What is SoftwareRenderer?
It is our final project for ICG (interactive computer graphics) 2016 course. A simple 3-D rendering engine which uses CPU only. <br />

## Dependencies
This project uses SDL (Simple DirectMedia Layer) version 2.0.4 for the windowing routines, and the binaries provided are for Windows only.

## How to Compile
#### Windows
- set "./Source" as an additional include directory
- set "./Library/SDL2-2.0.4/include" as an additional include directory
- "./Library/SDL2-2.0.4/lib/x64/SDL2.dll" is needed in order to run the program

#### Ubuntu
- set "./Source" as an additional include directory
- set "./Library/SDL2-2.0.4/include" as an additional include directory
- to get the binaries for SDL, please visit https://www.libsdl.org/download-2.0.php, download the appropriate source and compile
- after doing the steps above, compile (e.g., type (g++ -std=c++11 Main.cpp Window.cpp LdrFrame.cpp Frame.cpp `pkg-config --cflags --libs sdl2` -o main) at the terminal)
- run: ./main

## Appendix
- a tutorial for SDL installation (https://www.youtube.com/watch?v=iggmjJ_C_C4) which might be useful for Linux users

## About the Authors
to be edited... <br />
