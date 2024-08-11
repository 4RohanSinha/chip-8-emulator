# Chip 8 Virtual Machine

A C-based emulator of Chip 8 and its instruction set. Graphics rendered by SDL2.

# Usage

First compile, as usual:
```
make
```
To run the emulator:
```
./chip8 <video-scale> <cycle-delay> <rom-file> <OPTIONAL: disassemble-options>
```
* video-scale: an integer to scale the display up or down  
* cycle-delay: controls speed of the game  
* rom-file: pass in any ROM file valid for Chip-8  
* OPTIONAL: disassemble-options - either of the following:  
	* `-d`: program will dump each CHIP 8 instruction it runs to stdout  
	* `-raw`: program will dump each instruction, line by line, from the ROM file, translating opcodes into human-readable instruction format
