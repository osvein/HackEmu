#include HackCPU.h

#define MASK_INSTRUCTION 0x8000 // instruction code: 1 for C, 0 for A

#define MASK_A 0x1000 // y input code: 1 for M, 0 for A

// computation specification bits
#define MASK_C1 0x800 // zero the x input
#define MASK_C2 0x400 // negate the x input
#define MASK_C3 0x200 // zero the y input
#define MASK_C4 0x100 // negate the y input
#define MASK_C5 0x080 // function code: 1 for Add, 0 for And
#define MASK_C6 0x040 // negate the out output

// destination specification bits
#define MASK_D1 0x20 // A register
#define MASK_D2 0x10 // D register
#define MASK_D3 0x08 // M register

// jump specification bits
#define MASK_J1 0x4 // out < 0
#define MASK_J2 0x2 // out = 0
#define MASK_J3 0x1 // out > 0

int16_t a, d, pc;

extern Outputs emulate(int16_t inM, int16_t instruction, bool reset)
{
  Outputs ret = {};
  ret->pc = ++pc;
  ret->addressM = a;

  // if A instruction (the first bit is clear)
  if (!(instruction & MASK_INSTRUCTION))
  {
    emu->a = instruction // write instruction to A register
  }
  // if C-instruction (the first bit is set)
  else
  {
    int16_t x = d;
    int16_t y = (instruction & MASK_A) ? inM : a;
    if (instruction & MASK_C1) x = 0;
    if (instruction & MASK_C2) x = ~x;
    if (instruction & MASK_C3) y = 0;
    if (instruction & MASK_C4) y = ~y;

    int16_t out = (instruction & MASK_C5) ? x + y : x & y;
    if (instruction & MASK_C6) out = ~out;
    ret->outM = out;

    if (instruction & MASK_D1) a = out;
    if (instruction & MASK_D2) d = out;
    if (instruction & MASK_D3) ret->writeM = true;

    bool jump;
    jump = jump || ((instruction & MASK_J1) && (out < 0));
    jump = jump || ((instruction & MASK_J2) && (out = 0));
    jump = jump || ((instruction & MASK_J3) && (out > 0));
    if (jump) pc = a;

    if (reset) pc = 0;
  }

  return ret;
}
