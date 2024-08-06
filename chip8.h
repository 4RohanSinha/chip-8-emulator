#ifndef CHIP_8
#define CHIP_8

#define NUM_REGISTERS 16
#define MEM_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define VIDEO_SIZE 64 * 32

typedef struct chip_8 {
	char registers[NUM_REGISTERS];
	char memory[MEM_SIZE];
	short index;
	short pc;
	short stack[STACK_SIZE];
	char sp;
	char delayTimer;
	char soundTimer;
	char keypad[KEYPAD_SIZE];
	int video[VIDEO_SIZE];
	short opcode;
} chip_8;

void ch_initialize(chip_8 proc);
void ch_loadRom(chip_8 proc, char const* filename);

#endif
