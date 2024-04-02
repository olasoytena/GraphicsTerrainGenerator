# Graphics Terrain Generator

## How to use
- OpenGL 
- Type "make" in terminal while in directory
- Run executable from terminal
- Type "make clean" to remove executable

## Key bindings
- WASD/wasd    Move in first person
- g/G     Generate new terrain and tree placement
- ESC     Exits program

## Details

The program generates a 100x100 grid representing terrain. The terrain will initialize flat when the program is opened. When the user presses 'G', the terrain is regenerated with random height values for each grid cell. The player can freely move around on the terrain using arrow keys or WASD keys. Trees are scattered randomly across the terrain, and collision detection ensures the player cannot walk through them. 

