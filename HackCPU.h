#ifndef HackCPU_h
#define HackCPU_h

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int16_t a, d; // CPU-resident registers
extern int16_t pc; // program counter

struct Outputs
{
  int16_t outM;
  bool writeM;
  uint16_t addressM;
  uint16_t pc;
}

extern Outputs emulate(int16_t inM, int16_t instruction, bool reset); // emulate 1 clock cycle

#ifdef __cplusplus
}
#endif

#endif
