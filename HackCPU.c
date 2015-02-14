#include HackCPU.h

#define MASK_INSTRUCTION 0x8000 // instruction code: 1 for C, 0 for A

#define MASK_A 0x1000 // y input code: 1 for M, 0 for A

// Computation specification bit masks
#define MASK_C1 0x800 // zx - zero the x input
#define MASK_C2 0x400 // nx - negate the x input
#define MASK_C3 0x200 // zy - zero the y input
#define MASK_C4 0x100 // ny - negate the y input
#define MASK_C5 0x080 // f  - function code: 1 for Add, 0 for And
#define MASK_C6 0x040 // no - negate the out output

// Destination specification bit masks
#define MASK_D1 0x20 // A register
#define MASK_D2 0x10 // D register
#define MASK_D3 0x08 // M register

// Jump specification bit masks
#define MASK_J1 0x4 // out < 0
#define MASK_J2 0x2 // out = 0
#define MASK_J3 0x1 // out > 0


int16_t a, d, pc;

extern Outputs emulate(int16_t inM, int16_t instruction, bool reset)
{
  pc++; // increment program counter

  Outputs ret = {};
  ret->addressM = a; // set the addressM output to the value stored in the A register

  // if A instruction (if the first bit is clear)
  if (!(instruction & MASK_INSTRUCTION))
  {
    emu->a = instruction // write instruction to A register
  }
  // if C instruction (if the first bit is set)
  else
  {
    int16_t x = d;                                // the x input is always the D register
    int16_t y = (instruction & MASK_A) ? inM : a; // the y input is either the M register (a-bit=set) or the A register (a-bit=clear)
    if (instruction & MASK_C1) x = 0;  // zero   the x input if the c1-bit is set
    if (instruction & MASK_C2) x = ~x; // negate the x input if the c2-bit is set
    if (instruction & MASK_C3) y = 0;  // zero   the y input if the c3-bit is set
    if (instruction & MASK_C4) y = ~y; // negate the y input if the c4-bit is set

    int16_t out = (instruction & MASK_C5) ? x + y : x & y; // the output is either x + y (c5-bit=set) or x & y (c5-bit=clear)
    if (instruction & MASK_C6) out = ~out; // negate the out output if the c6-bit is set
    ret->outM = out;

    if (instruction & MASK_D1) a = out;            // store the computed value in the A register if the d1-bit is set
    if (instruction & MASK_D2) d = out;            // store the computed value in the D register if the d2-bit is set
    if (instruction & MASK_D3) ret->writeM = true; // store the computed value in the M register if the d3-bit is set

    bool jump;
    jump = jump || ((instruction & MASK_J1) && (out < 0)); // If out > 0 set jump to true if the j1-bit is set
    jump = jump || ((instruction & MASK_J2) && (out = 0)); // If out = 0 set jump to true if the j2-bit is set
    jump = jump || ((instruction & MASK_J3) && (out > 0)); // If out < 0 set jump to true if the j3-bit is set
    if (jump) pc = a; // set the program counter to the value stored in the A register if jump is true
  }

  if (reset) pc = 0; // set the program counter to 0 if reset is true
  ret->pc = pc; // set the pc output to the value stored in the program counter

  return ret;
}
