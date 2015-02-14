#ifndef HackCPU_h
#define HackCPU_h

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int16_t a, d; // CPU-resident registers
extern int16_t pc;   // Program counter

/*
 * CPU outputs.
 */
struct Outputs
{
  int16_t outM;      // M value output
  bool writeM;       // Write to M?
  uint16_t addressM; // Address of M in data memory
  uint16_t pc;       // Address of next instruction
}

/*
 * Emulates the next CPU clock cycle
 * inM         - M value input
 * instruction - Instruction for execution
 * reset       - Signals whether to restart the current program (reset=true) or continue executing the current program (reset=false)
 * returns CPU outputs
 */
extern Outputs emulate(int16_t inM, int16_t instruction, bool reset);

#ifdef __cplusplus
}
#endif

#endif
