#include "ops.h"
#include "chip8.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

INSTRUCTION(chop_00e0) {
	memset(ch->video, 0, sizeof(ch->video));
}

INSTRUCTION(chop_00ee) {
	ch->pc = ch->stack[--ch->sp];
}

INSTRUCTION(chop_1nnn) {
	unsigned short address = ch->opcode & 0x0FFFu;
	ch->pc = address;
}

INSTRUCTION(chop_2nnn) {
	unsigned short address = ch->opcode & 0x0FFFu;
	ch->stack[ch->sp++] = ch->pc;
	ch->pc = address;
}

INSTRUCTION(chop_3xkk) {
	unsigned char r_id = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = (ch->opcode & 0x00FFu);

	if (ch->registers[r_id] == kk) ch->pc += 2;
}

INSTRUCTION(chop_4xkk) {
	unsigned char r_id = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = (ch->opcode & 0x00FFu);

	if (ch->registers[r_id] != kk) ch->pc += 2;
}

INSTRUCTION(chop_5xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	if (ch->registers[r_x] == ch->registers[r_y]) ch->pc += 2;
}

INSTRUCTION(chop_6xkk) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = ch->opcode & 0x00FFu;

	ch->registers[r_x] = kk;
}

INSTRUCTION(chop_7xkk) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = ch->opcode & 0x00FFu;

	ch->registers[r_x] += kk;
}

INSTRUCTION(chop_8xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	ch->registers[r_x] = ch->registers[r_y];
}

INSTRUCTION(chop_8xy1) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	ch->registers[r_x] |= ch->registers[r_y];
}

INSTRUCTION(chop_8xy2) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	ch->registers[r_x] &= ch->registers[r_y];
}

INSTRUCTION(chop_8xy3) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	ch->registers[r_x] ^= ch->registers[r_y];
}

INSTRUCTION(chop_8xy4) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	unsigned short sum = ch->registers[r_x] + ch->registers[r_y];
	if (sum > 255U) ch->registers[0xF] = 1;
	else ch->registers[0xF] = 0;

	ch->registers[r_x] = sum & 0xffu;
}

INSTRUCTION(chop_8xy5) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	if (ch->registers[r_x] > ch->registers[r_y]) ch->registers[0xf] = 1;
	else ch->registers[0xf] = 0;

	ch->registers[r_x] -= ch->registers[r_y];
}

INSTRUCTION(chop_8xy6) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;

	ch->registers[0xf] = ch->registers[r_x] & 0x1u;
	ch->registers[r_x] >>= 1;
}

INSTRUCTION(chop_8xy7) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	if (ch->registers[r_x] < ch->registers[r_y]) ch->registers[0xf] = 1;
	else ch->registers[0xf] = 0;

	ch->registers[r_x] = ch->registers[r_y] - ch->registers[r_x];
}

INSTRUCTION(chop_8xye) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;

	ch->registers[0xf] = (ch->registers[r_x] & 0x80u) >> 7;
	ch->registers[r_x] <<= 1;
}

INSTRUCTION(chop_9xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if (ch->registers[r_x] != ch->registers[r_y]) ch->pc += 2;
}

INSTRUCTION(chop_Annn) {
	unsigned short address = (ch->opcode & 0x0FFFu);
	ch->index = address;
}

INSTRUCTION(chop_Bnnn) {
	unsigned short address = (ch->opcode & 0x0FFFu);
	ch->pc = ch->registers[0] + address;
}

INSTRUCTION(chop_Cxkk) {
	unsigned char kk = (ch->opcode & 0x00FFu);
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;

	ch->registers[r_x] = getRandomByte() && kk;
}

INSTRUCTION(chop_Dxyn) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4;
	unsigned char height = (ch->opcode & 0x000Fu);

	unsigned char xPos = ch->registers[r_x] % VIDEO_WIDTH;
	unsigned char yPos = ch->registers[r_y] % VIDEO_HEIGHT;

	ch->registers[0xf] = 0;

	for (int row = 0; row < height; row++) {
		unsigned char sprite = ch->memory[ch->index+row];
		for (int col = 0; col < 8; col++) {
			unsigned char spritePixel = sprite & (0x80u >> col);
			unsigned int* screenPixel = &ch->video[(yPos+row)*VIDEO_WIDTH + (xPos+col)];

			if (spritePixel) {
				if (*screenPixel == 0xFFFFFFFF)
					ch->registers[0xf] = 1;
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

INSTRUCTION(chop_Ex9E) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char key = ch->registers[r_x];

	if (ch->keypad[key]) ch->pc += 2;
}

INSTRUCTION(chop_ExA1) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char key = ch->registers[r_x];

	if (!ch->keypad[key]) ch->pc += 2;
}

INSTRUCTION(chop_Fx07) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	ch->registers[r_x] = ch->delayTimer;
}

INSTRUCTION(chop_Fx0A) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	bool keyPressed = false;
	for (int i = 0; i < 16; i++) {
		if (ch->keypad[i]) {
			keyPressed = true;
			ch->registers[r_x] = i;
		}
	}

	if (!keyPressed) ch->pc -= 2;
}

INSTRUCTION(chop_Fx15) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	ch->delayTimer = ch->registers[r_x];
}

INSTRUCTION(chop_Fx18) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	ch->soundTimer = ch->registers[r_x];
}

INSTRUCTION(chop_Fx1E) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	ch->index += ch->registers[r_x];
}

INSTRUCTION(chop_Fx29) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char digit = ch->registers[r_x];
	ch->index = FONTSET_START_ADDRESS + (5*digit);
}

INSTRUCTION(chop_Fx33) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char val = ch->registers[r_x];

	ch->memory[ch->index + 2] = val % 10;
	val /= 10;
	
	ch->memory[ch->index + 1] = val % 10;
	val /= 10;

	ch->memory[ch->index] = val % 10;
}

INSTRUCTION(chop_Fx55) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	memcpy(ch->memory + ch->index, ch->registers, r_x+1);
}

INSTRUCTION(chop_Fx65) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	memcpy(ch->registers, ch->memory + ch->index, r_x+1);
}

#include <stdio.h>
INSTRUCTION(chop_NOP) {
	printf("There has been an error.\n");
}

void initialize_optable() {
	table[0] = table0f;
	table[1] = chop_1nnn;
	table[2] = chop_2nnn;
	table[3] = chop_3xkk;
	table[4] = chop_4xkk;
	table[5] = chop_5xy0;
	table[6] = chop_6xkk;
	table[7] = chop_7xkk;
	table[8] = table8f;
	table[9] = chop_9xy0;
	table[0xA] = chop_Annn;
	table[0xb] = chop_Bnnn;
	table[0xc] = chop_Cxkk;
	table[0xd] = chop_Dxyn;
	table[0xe] = tableef;
	table[0xf] = tableff;

	for (int i = 0; i < 16; i++) {
		table0[i] = chop_NOP;
		table8[i] = chop_NOP;
		tablee[i] = chop_NOP;
	}

	for (int i = 0; i < 102; i++) tablef[i] = chop_NOP;

	table0[0] = chop_00e0;
	table0[0xe] = chop_00ee;
	
	table8[0] = chop_8xy0;
	table8[1] = chop_8xy1;
	table8[2] = chop_8xy2;
	table8[3] = chop_8xy3;
	table8[4] = chop_8xy4;
	table8[5] = chop_8xy5;
	table8[6] = chop_8xy6;
	table8[7] = chop_8xy7;
	table8[0xe] = chop_8xye;

	tablee[0x1] = chop_ExA1;
	tablee[0xe] = chop_Ex9E;

	tablef[0x07] = chop_Fx07;
	tablef[0x0A] = chop_Fx0A;
	tablef[0x15] = chop_Fx15;
	tablef[0x18] = chop_Fx18;
	tablef[0x1E] = chop_Fx1E;
	tablef[0x29] = chop_Fx29;
	tablef[0x33] = chop_Fx33;
	tablef[0x55] = chop_Fx55;
	tablef[0x65] = chop_Fx65;
}

void table0f(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	(table0[index])(ch);
}

void table8f(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	(table8[index])(ch);
}

void tableef(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	(tablee[index])(ch);
}

void tableff(chip_8* ch) {
	int index = ch->opcode & 0x00FFu;
	(tablef[index])(ch);
}

void decxec(chip_8* ch) {
	(table[((ch->opcode & 0xF000u) >> 12u)])(ch);
}
