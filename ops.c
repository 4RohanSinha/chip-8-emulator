#include "ops.h"
#include "chip8.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

static bool disassemble_mode_exec = false;
static bool disassemble_raw = false;
void set_disassemble_exec() { disassemble_mode_exec = true; }
void set_disassemble_raw() { disassemble_raw = true; }
#define PRINTI(format, ...) print_instruction(ch, format, ##__VA_ARGS__)

void print_instruction(chip_8* ch, char* format, ...) {
	va_list args;
	va_start(args, format);

	printf("0x%x:\t\t%x %x\t\t\t", (ch->pc)-2, (ch->opcode & 0xFF00u) >> 8, (ch->opcode & 0x00FFu));
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

INSTRUCTION(chop_00e0) {
	if(disassemble_mode_exec)
		PRINTI("cls");
	if (disassemble_raw) return;
	memset(ch->video, 0, sizeof(ch->video));
}

INSTRUCTION(chop_00ee) {
	if(disassemble_mode_exec)
		PRINTI("ret");
	if (disassemble_raw) return;
	ch->pc = ch->stack[--ch->sp];
}

INSTRUCTION(chop_1nnn) {
	unsigned short address = ch->opcode & 0x0FFFu;
	if(disassemble_mode_exec)
		PRINTI("jp 0x%x", address);
	if (disassemble_raw) return;
	ch->pc = address;
}

INSTRUCTION(chop_2nnn) {
	unsigned short address = ch->opcode & 0x0FFFu;
	if(disassemble_mode_exec)
		PRINTI("call 0x%x", address);
	if (disassemble_raw) return;
	ch->stack[ch->sp++] = ch->pc;
	ch->pc = address;
}

INSTRUCTION(chop_3xkk) {
	unsigned char r_id = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = (ch->opcode & 0x00FFu);
	if(disassemble_mode_exec)
		PRINTI("se V%d, %d", r_id, kk);
	if (disassemble_raw) return;
	if (ch->registers[r_id] == kk) ch->pc += 2;
}

INSTRUCTION(chop_4xkk) {
	unsigned char r_id = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = (ch->opcode & 0x00FFu);
	if(disassemble_mode_exec)
		PRINTI("sne V%d, %d", r_id, kk);
	if (disassemble_raw) return;
	if (ch->registers[r_id] != kk) ch->pc += 2;
}

INSTRUCTION(chop_5xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("se V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	if (ch->registers[r_x] == ch->registers[r_y]) ch->pc += 2;
}

INSTRUCTION(chop_6xkk) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = ch->opcode & 0x00FFu;
	if(disassemble_mode_exec)
		PRINTI("ld V%d, %d", r_x, kk);
	if (disassemble_raw) return;
	ch->registers[r_x] = kk;
}

INSTRUCTION(chop_7xkk) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char kk = ch->opcode & 0x00FFu;
	if(disassemble_mode_exec)
		PRINTI("add V%d, %d", r_x, kk);
	if (disassemble_raw) return;
	ch->registers[r_x] += kk;
}

INSTRUCTION(chop_8xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("ld V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	ch->registers[r_x] = ch->registers[r_y];
}

INSTRUCTION(chop_8xy1) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("or V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	ch->registers[r_x] |= ch->registers[r_y];
}

INSTRUCTION(chop_8xy2) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("and V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	ch->registers[r_x] &= ch->registers[r_y];
}

INSTRUCTION(chop_8xy3) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("xor V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	ch->registers[r_x] ^= ch->registers[r_y];
}

INSTRUCTION(chop_8xy4) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	unsigned short sum = ch->registers[r_x] + ch->registers[r_y];
	if(disassemble_mode_exec)
		PRINTI("add V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	if (sum > 255U) ch->registers[0xF] = 1;
	else ch->registers[0xF] = 0;

	ch->registers[r_x] = sum & 0xffu;
}

INSTRUCTION(chop_8xy5) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	if(disassemble_mode_exec)
		PRINTI("sub V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	if (ch->registers[r_x] > ch->registers[r_y]) ch->registers[0xf] = 1;
	else ch->registers[0xf] = 0;

	ch->registers[r_x] -= ch->registers[r_y];
}

INSTRUCTION(chop_8xy6) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	if(disassemble_mode_exec)
		PRINTI("shr V%d", r_x);
	if (disassemble_raw) return;
	ch->registers[0xf] = ch->registers[r_x] & 0x1u;
	ch->registers[r_x] >>= 1;
}

INSTRUCTION(chop_8xy7) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;

	if(disassemble_mode_exec)
		PRINTI("subn V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	if (ch->registers[r_x] < ch->registers[r_y]) ch->registers[0xf] = 1;
	else ch->registers[0xf] = 0;

	ch->registers[r_x] = ch->registers[r_y] - ch->registers[r_x];
}

INSTRUCTION(chop_8xye) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	if(disassemble_mode_exec)
		PRINTI("shl V%d", r_x);
	if (disassemble_raw) return;
	ch->registers[0xf] = (ch->registers[r_x] & 0x80u) >> 7;
	ch->registers[r_x] <<= 1;
}

INSTRUCTION(chop_9xy0) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8u;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4u;
	if(disassemble_mode_exec)
		PRINTI("sne V%d, V%d", r_x, r_y);
	if (disassemble_raw) return;
	if (ch->registers[r_x] != ch->registers[r_y]) ch->pc += 2;
}

INSTRUCTION(chop_Annn) {
	unsigned short address = (ch->opcode & 0x0FFFu);
	if(disassemble_mode_exec)
		PRINTI("ld I, 0x%x", address);
	if (disassemble_raw) return;
	ch->index = address;
}

INSTRUCTION(chop_Bnnn) {
	unsigned short address = (ch->opcode & 0x0FFFu);
	if(disassemble_mode_exec)
		PRINTI("jp V0, 0x%x", address);
	if (disassemble_raw) return;
	ch->pc = ch->registers[0] + address;
}

INSTRUCTION(chop_Cxkk) {
	unsigned char kk = (ch->opcode & 0x00FFu);
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("rnd V%d, %d", r_x, kk);
	if (disassemble_raw) return;
	ch->registers[r_x] = getRandomByte() && kk;
}

INSTRUCTION(chop_Dxyn) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char r_y = (ch->opcode & 0x00F0u) >> 4;
	unsigned char height = (ch->opcode & 0x000Fu);
	if(disassemble_mode_exec)
		PRINTI("drw V%d, V%d, %d", r_x, r_y, height);
	if (disassemble_raw) return;
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
	if(disassemble_mode_exec)
		PRINTI("skp V%d", r_x);
	if (disassemble_raw) return;
	if (ch->keypad[key]) ch->pc += 2;
}

INSTRUCTION(chop_ExA1) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char key = ch->registers[r_x];
	if(disassemble_mode_exec)
		PRINTI("sknp V%d", r_x);
	if (disassemble_raw) return;
	if (!ch->keypad[key]) ch->pc += 2;
}

INSTRUCTION(chop_Fx07) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("ld V%d, dt", r_x);
	if (disassemble_raw) return;
	ch->registers[r_x] = ch->delayTimer;
}

INSTRUCTION(chop_Fx0A) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("ld V%d, K", r_x);
	if (disassemble_raw) return;
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
	if(disassemble_mode_exec)
		PRINTI("ld DT, V%d", r_x);
	if (disassemble_raw) return;
	ch->delayTimer = ch->registers[r_x];
}

INSTRUCTION(chop_Fx18) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("ld ST, V%d", r_x);
	if (disassemble_raw) return;
	ch->soundTimer = ch->registers[r_x];
}

INSTRUCTION(chop_Fx1E) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("add I, V%d", r_x);
	if (disassemble_raw) return;
	ch->index += ch->registers[r_x];
}

INSTRUCTION(chop_Fx29) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char digit = ch->registers[r_x];
	if(disassemble_mode_exec)
		PRINTI("ld F, V%d", r_x);
	if (disassemble_raw) return;
	ch->index = FONTSET_START_ADDRESS + (5*digit);
}

INSTRUCTION(chop_Fx33) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	unsigned char val = ch->registers[r_x];
	if(disassemble_mode_exec)
		PRINTI("ld B, V%d", r_x);
	if (disassemble_raw) return;
	ch->memory[ch->index + 2] = val % 10;
	val /= 10;

	ch->memory[ch->index + 1] = val % 10;
	val /= 10;

	ch->memory[ch->index] = val % 10;
}

INSTRUCTION(chop_Fx55) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("ld [I], V%d", r_x);
	if (disassemble_raw) return;
	memcpy(ch->memory + ch->index, ch->registers, r_x+1);
}

INSTRUCTION(chop_Fx65) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;
	if(disassemble_mode_exec)
		PRINTI("ld V%d, [I]", r_x);
	if (disassemble_raw) return;
	memcpy(ch->registers, ch->memory + ch->index, r_x+1);
}

//DEBUGGING INSTRUCTIONS - not in the standard:
// Fx95 - printdb <reg - Vx>
	//prints value of register
// 00EA - prints index address - prints I
// 00EB - prints value at index - prints [I]
// 00EC - prints string at index
	//follows addr and treats it as THE START OF A STRING - will keep printing each byte as a character until zero byte, as usual
// Fx97 - prints N rows - display of 1/0 bits starting at I
// 8kka - printb <byte - kk>
	//prints byte kk

INSTRUCTION(chdb_00ea) {
	unsigned short index = ch->index;
	if (disassemble_mode_exec)
		PRINTI("printdb I");
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console: %x\n", index);
}

INSTRUCTION(chdb_00eb) {
	unsigned char ind_val = ch->memory[ch->index];
	if (disassemble_mode_exec)
		PRINTI("printdb [I]");
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console: %c\n", ind_val);

}

INSTRUCTION(chdb_00ec) {
	unsigned short addr = ch->index;

	if (disassemble_mode_exec)
		PRINTI("prints 0x%x", addr);
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console: %s\n", ch->memory+addr);

}

INSTRUCTION(chdb_8kka) {
	unsigned char byte = (ch->opcode & 0x0FF0) >> 4;
	if (disassemble_mode_exec)
		PRINTI("printdb 0x%x", byte);
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console: %c\n", byte);
}

INSTRUCTION(chdb_Fx95) {
	unsigned char r_x = (ch->opcode & 0x0F00u) >> 8;

	if (disassemble_mode_exec)
		PRINTI("printdb V%d", r_x);
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console: %d\n",ch->registers[r_x]);
}

static void print_bin(unsigned char byte)
{
    int i = 8;
    while (i--) {
	printf("%c", '0'+((byte >> i) & 1));
    }
}

INSTRUCTION(chdb_Fx97) {
	unsigned char num_rows = (ch->opcode & 0x0F00u) >> 8;
	int i = 0;

	if (disassemble_mode_exec)
		PRINTI("prdrw I, %d", num_rows);
	if (disassemble_raw) return;
	printf("Chip 8 Debugging Console:\n");
	while (i < num_rows*2) {
		print_bin(ch->memory[ch->index+i]);
		i++;
		if (i % 2 == 0) printf("\n");
	}

}


INSTRUCTION(chop_NOP) {
	if (disassemble_mode_exec)
		PRINTI("?? ?? ??");
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

	for (int i = 0; i < 200; i++) tablef[i] = chop_NOP;

	table0[0] = chop_00e0;
	table0[0xe] = chop_00ee;

	table0[0xa] = chdb_00ea;
	table0[0xb] = chdb_00eb;
	table0[0xc] = chdb_00ec;
//	table0[0xd] = chdb_00ed;

	table8[0] = chop_8xy0;
	table8[1] = chop_8xy1;
	table8[2] = chop_8xy2;
	table8[3] = chop_8xy3;
	table8[4] = chop_8xy4;
	table8[5] = chop_8xy5;
	table8[6] = chop_8xy6;
	table8[7] = chop_8xy7;
	table8[0xe] = chop_8xye;

	table8[0xa] = chdb_8kka;

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

	tablef[0x95] = chdb_Fx95;
	tablef[0x97] = chdb_Fx97;
}

void table0f(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	if (index < 16) (table0[index])(ch);
	else chop_NOP(ch);
}

void table8f(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	if (index < 16) (table8[index])(ch);
	else chop_NOP(ch);
}

void tableef(chip_8* ch) {
	int index = ch->opcode & 0x000Fu;
	if (index < 16) (tablee[index])(ch);
	else chop_NOP(ch);
}

void tableff(chip_8* ch) {
	int index = ch->opcode & 0x00FFu;
	if (index < 200) (tablef[index])(ch);
	else chop_NOP(ch);
}

void decxec(chip_8* ch) {
	(table[((ch->opcode & 0xF000u) >> 12u)])(ch);
}
