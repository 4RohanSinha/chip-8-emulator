#include "chip8.h"
#include "ops.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define ZERO_INITIALIZE_ARR(arr, len) for (int i = 0; i < len; i++) arr[i] = 0;

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void ch_initialize() {
	ZERO_INITIALIZE_ARR(registers, NUM_REGISTERS);
	ZERO_INITIALIZE_ARR(memory, MEM_SIZE);
	ZERO_INITIALIZE_ARR(stack, STACK_SIZE);
	ZERO_INITIALIZE_ARR(keypad, KEYPAD_SIZE);
	ZERO_INITIALIZE_ARR(video, VIDEO_SIZE);

	index = 0;
	sp = 0;
	delayTimer = 0;
	soundTimer = 0;
	pc = START_ADDRESS;
	memcpy(memory + FONTSET_START_ADDRESS, fontset, FONTSET_SIZE);	

	srand(time(NULL));
}

static unsigned char getRandomByte() {
	return (unsigned char)(rand() & 0xFF);
}

void ch_loadRom(chip_8 proc, char const* filename) {
	FILE* rom = fopen(filename, "rb");

	if (!feof(rom)) {
		fread(memory + START_ADDRESS, MEM_SIZE - START_ADDRESS, 1, rom);
	}

	fclose(rom);

}