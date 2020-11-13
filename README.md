# ASCII .obj file viewer
This is a simple .obj file viewer that render to your terminal with ascii. It allows you to view arbitrary 3D models using a FPS camera. When running, use the WASD keys to move forward, left, backward, and right, and the arrows keys to rotate your view.
It can handle up to ~100,000 polygons on a 2018 i7 (there is no GPU acceleration).

## Example
![Image of Violin Case](https://raw.githubusercontent.com/fingoldin/3dascii/master/examples/violin.png)

## Dependencies
The curses library

## Building
Run `make` from this directory.

## Running
Run `./main [file.obj]` from this directory.
