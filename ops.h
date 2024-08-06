#ifndef OPS_H
#define OPS_H

typedef struct chip_8 chip_8;

#define INSTRUCTION(instr) void instr(chip_8* ch)

INSTRUCTION(chop_00e0); //cls
INSTRUCTION(chop_00ee); //ret
INSTRUCTION(chop_1nnn); //jp ADDR (no stack interaction)
INSTRUCTION(chop_2nnn); //call ADDR
INSTRUCTION(chop_3xkk); //SE Vx, byte (skip next instruction if Vx = kk)
INSTRUCTION(chop_4xkk); //SNE Vx, byte (skip next instruction if Vx != kk)
INSTRUCTION(chop_5xy0); //SE Vx, Vy (skip next instruction if Vx = Vy)
INSTRUCTION(chop_6xkk); //LD Vx, byte (set Vx = kk)
INSTRUCTION(chop_7xkk); //set Vx = Vx + kk
INSTRUCTION(chop_8xy0); //LD Vx, Vy (set Vx = Vy)
INSTRUCTION(chop_8xy1); //OR Vx, Vy (Vx = Vx or Vy)
INSTRUCTION(chop_8xy2); //AND Vx, Vy (Vx = Vx and Vy)
INSTRUCTION(chop_8xy3); //XOR Vx, Vy
INSTRUCTION(chop_8xy4); //ADD Vx, Vy (Vx = Vx + Vy, VF set to carry)
INSTRUCTION(chop_8xy5); //SUB Vx, Vy (Vx = Vx - Vy, VF = NOT borrow. Vx > Vy => Vf = 1
INSTRUCTION(chop_8xy6); //VX = VX SHR 1 (right shift by 1) - least-significant bit of Vx = 1, then Vf = 1. Otherwise 0. Vx then divide by 2 (right shift unsigned)
INSTRUCTION(chop_8xy7); //Vx = Vy - Vx (Vf = NOT borrow) - Y > X: Vf = 1, otherwise 0
INSTRUCTION(chop_8xye); //SHL Vx, Vy (Vx = Vx SHL 1) - if Vx's most significant bit is 1, Vf is 1, otherwise 0.
INSTRUCTION(chop_9xy0); //SNE Vx, Vy (skip if Vx != Vy)
INSTRUCTION(chop_Annn); //LD I, addr - set index to address
INSTRUCTION(chop_Bnnn); //JP V0, addr - jump to location addr + V0
INSTRUCTION(chop_Cxkk); //RND Vx, byte - set VX = random byte AND kk
INSTRUCTION(chop_Dxyn); //DRW Vx, Vy, nibble. COME BACK TO THIS ONE
INSTRUCTION(chop_Ex9E); //SKP Vx (skip next instruction if key with value Vx is pressed) increment PC by 2 to skip next instruction
INSTRUCTION(chop_ExA1); //SKNP Vx (skip next instr if key w/ value Vx is NOT pressed)
INSTRUCTION(chop_Fx07); //LD Vx, DT (Vx = delay timer value)
INSTRUCTION(chop_Fx0A); //LD Vx, K (wait for key press, store in Vx)
INSTRUCTION(chop_Fx15); //LD DT, Vx (set delay timer to Vx)
INSTRUCTION(chop_Fx18); //LD ST, Vx (set sound timer to Vx)
INSTRUCTION(chop_Fx1E); //set index to index plus Vx
INSTRUCTION(chop_Fx29); //set index to location of sprite for digit Vx
INSTRUCTION(chop_Fx33); //store BCD representation of Vx in I, I+1, I+2 (take decimal value of Vx, store hundreds digit in I, tens digit in I+1, ones digit in I+2)
INSTRUCTION(chop_Fx55); //store registers V0 through Vx in memory starting at location I
INSTRUCTION(chop_Fx65); //read memory starting at location I into registers 0 through Vx

INSTRUCTION(chop_NOP);

void table0f(chip_8*);
void table8f(chip_8*);
void tableef(chip_8*);
void tableff(chip_8*);

void (*table[16])(chip_8*);
void (*table0[16])(chip_8*);
void (*table8[16])(chip_8*);
void (*tablee[16])(chip_8*);
void (*tablef[102])(chip_8*);

void initialize_optable();

void decxec(chip_8*);

#endif
