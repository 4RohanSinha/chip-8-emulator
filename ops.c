#include "ops.h"
#include "chip8.h"

#define TABLE8OP(i) chop_8xy##i

INSTRUCTION(chop_00e0) {
	memset(ch->video, 0, VIDEO_SIZE);
}

INSTRUCTION(chop_00ee) {
	ch->pc = ch->stack[--sp];
}

INSTRUCTION(chop_1nnn) {
	short address = ch->opcode & 0x0FFFu;
	ch->pc = address;
}

INSTRUCTION(chop_2nnn) {
	short address = ch->opcode & 0x0FFFu;
	stack[sp++] = ch->pc;
	ch->pc = address;
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
		table0[i] = chop_NULL;
		table8[i] = chop_NULL;
		tablee[i] = chop_NULL;
	}

	for (int i = 0; i < 102; i++) tablef[i] = chop_NULL;

	table0[0] = chop_00e0;
	table0[0xe] = chop_00ee;
	
	for (int i = 0; i < 8; i++) table8[i] = TABLE8OP(i);
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
	int index = ch->opcode & 0x000Fu;
	(tablef[index])(ch);
}
