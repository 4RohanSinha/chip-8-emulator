#ifndef CHIP_8
#define CHIP_8

#define NUM_REGISTERS 16
#define MEM_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define VIDEO_SIZE 64 * 32
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

extern const unsigned int FONTSET_START_ADDRESS;

typedef struct chip_8 {
	unsigned char registers[NUM_REGISTERS];
	unsigned char memory[MEM_SIZE];
	unsigned short index;
	unsigned short pc;
	unsigned short stack[STACK_SIZE];
	unsigned char sp;
	unsigned char delayTimer;
	unsigned char soundTimer;
	unsigned char keypad[KEYPAD_SIZE];
	unsigned int video[VIDEO_SIZE];
	unsigned short opcode;
} chip_8;

void ch_initialize(chip_8* proc);
void ch_loadRom(chip_8* proc, char const* filename);
void ch_cycle(chip_8* proc);
unsigned char getRandomByte();

#endif
