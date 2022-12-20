/*
*
* @file        riscv_isa.cpp
* @version     1.0
* @author      Pavani Tripathi
*
* Thanks to Dario Soares for contributing his model at
* https://github.com/DarioSoares/riscv-archc
*
* @date        Aug 2016
* @brief       The ArchC RISC-V functional model
*
* @Source      http://people.eecs.berkeley.edu/~krste/papers/riscv-spec-2.0.pdf
*
*/

#include "riscv_isa.H"
#include "riscv_isa_init.cpp"
#include "riscv_bhv_macros.H"
#include <fenv.h>

// Uncomment for debug Information
#define DEBUG_MODEL
#include "ac_debug_model.H"

#define Ra 1
#define Sp 14

// For using all the RISC-V parameters
using namespace riscv_parms;

static int processors_started = 0;
#define DEFAULT_STACK_SIZE (512 * 1024);


// Generic instruction behavior method
void ac_behavior(instruction) {
  dbg_printf("---PC=%#x---%lld\n", (int)ac_pc, ac_instr_counter);
  ac_pc = ac_pc + 4;
  RB[0] = 0x0;
}

// Instruction Format behavior methods
void ac_behavior(Type_R) {}
void ac_behavior(Type_R4) {}
void ac_behavior(Type_I) {}
void ac_behavior(Type_S) {}
void ac_behavior(Type_SB) {}
void ac_behavior(Type_U) {}
void ac_behavior(Type_UJ) {}
void ac_behavior(Type_V) {}
void ac_behavior(Type_VM) {}


// Behavior called before starting simulation
void ac_behavior(begin) {
  dbg_printf("@@@ begin behavior @@@\n");

  for (int regNum = 0; regNum < 32; regNum++) {
    RB[regNum] = 0;
    RBF[regNum] = 0;
    RBF[regNum+32] = 0;
    RBV[regNum] = 0;
    RBV[regNum+32] = 0;
    RBV[regNum+64] = 0;
    RBV[regNum+96] = 0;
  }
  fcsr = 0;
  frm = 0;
  fflags = 0;
  vstart = 0;
  vl = 0;
  vtype = 0;
  vlenb = 16; // VLEN / 8 ( 128 / 8 )
}


// Behavior called after finishing simulation
void ac_behavior(end) {
  dbg_printf("@@@ end behavior @@@\n");
}

// Instruction ADD behavior method. (no check for overflow)
void ac_behavior(ADD) {
  dbg_printf("ADD r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  RB[rd] = RB[rs1] + RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SUB behavior method. (no check for overflow)
void ac_behavior(SUB) {
  dbg_printf("SUB r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  RB[rd] = RB[rs1] - RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLL behavior method.
void ac_behavior(SLL) {
  dbg_printf("SLL r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] << RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLT behavior method.
void ac_behavior(SLT) {
  dbg_printf("SLT r%d, r%d, r%d\n", rd, rs1, rs2);
  if ((ac_Sword)RB[rs1] < (ac_Sword)RB[rs2])
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLTU behavior method.
void ac_behavior(SLTU) {
  dbg_printf("SLTU r%d, r%d, r%d\n", rd, rs1, rs2);
  if ((ac_Uword)RB[rs1] < (ac_Uword)RB[rs2])
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction XOR behavior method.
void ac_behavior(XOR) {
  dbg_printf("XOR r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] ^ RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRL behavior method.
void ac_behavior(SRL) {
  dbg_printf("SRL r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] >> RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SRA behavior method.
void ac_behavior(SRA) {
  dbg_printf("SRA r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = ((ac_Sword)RB[rs1]) >> RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction OR behavior method.
void ac_behavior(OR) {
  dbg_printf("OR r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] | RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction AND behavior method.
void ac_behavior(AND) {
  dbg_printf("AND r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] & RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LB behavior method.
void ac_behavior(LB) {
  char byte;
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LB r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  byte = DM.read_byte(RB[rs1] + sign_ext);
  RB[rd] = sign_extend(byte, 8);
  dbg_printf("RB[rs1] = %#x, byte = %#x\n", RB[rs1], byte);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LH behavior method.
void ac_behavior(LH) {
  short int half;
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LH r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  half = DM.read_half(RB[rs1] + sign_ext);
  RB[rd] = sign_extend(half, 16);
  dbg_printf("RB[rs1] = %#x, half = %#x\n", RB[rs1], half);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LW behavior method.
void ac_behavior(LW) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LW r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LBU behavior method.
void ac_behavior(LBU) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LBU r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read_byte(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LHU behavior method.
void ac_behavior(LHU) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LHU r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read_half(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ADDI behavior method.
void ac_behavior(ADDI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ADDI r%d, r%d, %d\n", rd, rs1, imm);
  if ((rd == 0) && (rs1 == 0) && (imm == 0)) {
    dbg_printf("NOP executed!");
  } else {
    int sign_ext;

    sign_ext = sign_extend(imm, 12);
    ac_Sword rs1_value = RB[rs1];
    RB[rd] = rs1_value + sign_ext;
    dbg_printf("RB[rs1] = %d\n", RB[rs1]);
    dbg_printf("imm = %d\n", sign_ext);
    dbg_printf("Result = %d\n\n", RB[rd]);
  }
}

// Instruction SLTI behavior method.
void ac_behavior(SLTI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SLTI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  if (RB[rs1] < sign_ext)
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("imm = %d\n", sign_ext);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLTIU behavior method.
void ac_behavior(SLTIU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SLTIU r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  if ((ac_Uword)RB[rs1] < (ac_Uword)sign_ext)
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction XORI behavior method.
void ac_behavior(XORI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("XORI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] ^ sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ORI behavior method.
void ac_behavior(ORI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ORI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] | sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ANDI behavior method.
void ac_behavior(ANDI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ANDI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] & sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction JALR behavior method.
void ac_behavior(JALR) {
  int target_addr;
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("JALR r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  target_addr = RB[rs1] + sign_ext;
  target_addr = target_addr >> 1;
  target_addr = target_addr << 1;
  if (rd != 0)
    RB[rd] = ac_pc;
  ac_pc = target_addr;
  dbg_printf("Target = %#x\n", (ac_pc & 0xF0000000) | target_addr);
  dbg_printf("Target = %#x\n", target_addr);
  dbg_printf("Return = %#x\n\n", RB[rd]);
}

// Instruction SLLI behavior method.
void ac_behavior(SLLI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SLLI r%d, r%d, %d\n", rd, rs1, shamt);
  RB[rd] = RB[rs1] << shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRLI behavior method.
void ac_behavior(SRLI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SRLI r%d, r%d, %d\n", rd, rs1, shamt);
  RB[rd] = RB[rs1] >> shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRAI behavior method.
void ac_behavior(SRAI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SRAI r%d, r%d, %d\n", rd, rs1, shamt);
  if ((RB[rs1] >> 31) == 1)
    RB[rd] = (RB[rs1] >> shamt) | (0xFFFFFFFF << (32 - shamt));
  else
    RB[rd] = RB[rs1] >> shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ECALL behavior method.
void ac_behavior(ECALL) {
  dbg_printf("ECALL\n");
  printf("System Call\n\n");
  stop();
}

// Instruction SBREAK behavior method.
void ac_behavior(SBREAK) {
  dbg_printf("SBREAK\n");
  printf("Breakpoint\n\n");
  stop();
}

// Instruction RDCYCLE behavior method.
void ac_behavior(RDCYCLE) {
  dbg_printf("RDCYCLE r%d\n", rd);
  RB[rd] = ac_pc;
  dbg_printf("Result = %#x\n", RB[rd]);
}

// Instruction RDCYCLEH behavior method.
void ac_behavior(RDCYCLEH) { dbg_printf("RDCYCLEH r%d\n", rd); }

// Instruction RDTIME behavior method.
void ac_behavior(RDTIME) { dbg_printf("RDTIME r%d\n", rd); }

// Instruction RDTIMEH behavior method.
void ac_behavior(RDTIMEH) { dbg_printf("RDTIMEH r%d\n", rd); }

// Instruction RDINSTRET behavior method.
void ac_behavior(RDINSTRET) { dbg_printf("RDINSTRET r%d\n", rd); }

// Instruction RDINSTRETH behavior method.
void ac_behavior(RDINSTRETH) { dbg_printf("RDINSTRETH r%d\n", rd); }

// Instruction FENCE behavior method.
void ac_behavior(FENCE) { dbg_printf("FENCE r%d\n", rd); }

// Instruction FENCE_I behavior method.
void ac_behavior(FENCE_I) { dbg_printf("FENCE_I r%d\n", rd); }

// Instruction CSRRW behavior method.
void ac_behavior(CSRRW) {
 dbg_printf("CSRRW csr:%d\n", csr);
 ac_word tmp = RB[rd];
 ac_reg<riscv_parms::ac_word> mapped = csr_map(csr);
 if(rd != 0x0){
  RB[rd] = mapped;
 }
 mapped = tmp;
}

// Instruction CSRRS behavior method.
void ac_behavior(CSRRS) {
 dbg_printf("CSRRS csr:%d\n", csr);
 ac_reg<riscv_parms::ac_word> mapped = csr_map(csr);
 ac_word tmp = RB[rd];
 RB[rd] = mapped;
 mapped |= tmp;
}

// Instruction CSRRC behavior method.
void ac_behavior(CSRRC) {
 dbg_printf("CSRRC csr:%d\n", csr);
 ac_reg<riscv_parms::ac_word> mapped = csr_map(csr);
 ac_word tmp = RB[rd];
 RB[rd] = mapped;
 mapped &= ~tmp;
}

// Instruction SB behavior method
void ac_behavior(SB) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SB r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned char byte = RB[rs2] & 0xFF;
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write_byte(RB[rs1] + sign_ext, byte);
  dbg_printf("addr: %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result: %#x\n\n\n", byte);
}

// Instruction SH behavior method
void ac_behavior(SH) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SH r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  unsigned short int half = RB[rs2] & 0xFFFF;
  DM.write_half(RB[rs1] + sign_ext, half);
  dbg_printf("addr: %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result: %#x\n\n\n", half);
}

// Instruction SW behavior method
void ac_behavior(SW) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SW r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RB[rs2]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction BEQ behavior method
void ac_behavior(BEQ) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BEQ r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (RB[rs1] == RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken--- to %#x\n\n", addr);
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BNE behavior method
void ac_behavior(BNE) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BNE r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (RB[rs1] != RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BLT behavior method
void ac_behavior(BLT) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BLT r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  dbg_printf("blt r%d, r%d, %#x\n", rs1, rs2, addr);
  dbg_printf("addr = %#x\n", addr);
  dbg_printf("rs1 = %#x\n", RB[rs1]);
  dbg_printf("rs2 = %#x\n", RB[rs2]);
  if ((ac_Sword)RB[rs1] < (ac_Sword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BGE behavior method
void ac_behavior(BGE) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BGE r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if ((ac_Sword)RB[rs1] >= (ac_Sword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BLTU behavior method
void ac_behavior(BLTU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BLTU r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if ((ac_Uword)RB[rs1] < (ac_Uword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BGEU behavior method
void ac_behavior(BGEU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BGEU r%d, r%d, %d\n", rs1, rs2, imm);
  int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (((ac_Uword)RB[rs1] > (ac_Uword)RB[rs2]) ||
      ((ac_Uword)RB[rs1] == (ac_Uword)RB[rs2])) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction LUI behavior method
void ac_behavior(LUI) {
  dbg_printf("LUI r%d, %d\n", rd, imm);
  RB[rd] = imm << 12;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction AUIPC behavior method
void ac_behavior(AUIPC) {
  dbg_printf("AUIPC r%d, %d\n", rd, imm);
  unsigned int offset;
  offset = imm << 12;
  RB[rd] = ac_pc + offset - 4;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction JAL behavior method
void ac_behavior(JAL) {
  int imm;
  imm = (imm4 << 19) | (imm3 << 11) | (imm2 << 10) | imm1;
  dbg_printf("JAL r%d, %d\n", rd, imm);
  int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFF00000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (rd != 0)
    RB[rd] = ac_pc;
  ac_pc = (ac_pc & 0xF0000000) | addr;
  dbg_printf("--- Jump taken ---\n\n");
}

// Instruction MUL behavior method
void ac_behavior(MUL) {
  dbg_printf("MUL r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  mult = (ac_Sword)RB[rs1];
  mult *= (ac_Sword)RB[rs2];
  int half;
  half = mult;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULH behavior method
void ac_behavior(MULH) {
  dbg_printf("MULH r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  int half;
  mult = (ac_Sword)RB[rs1];
  mult *= (ac_Sword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULHSU behavior method
void ac_behavior(MULHSU) {
  dbg_printf("MULHSU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  int half;
  mult = RB[rs1];
  mult *= (ac_Uword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULHU behavior method
void ac_behavior(MULHU) {
  dbg_printf("MULHU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  unsigned long long mult;
  unsigned int half;
  mult = (ac_Uword)RB[rs1];
  mult *= (ac_Uword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction DIV behavior method
void ac_behavior(DIV) {
  dbg_printf("DIV r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("rs1 = %d\n", RB[rs1]);
  dbg_printf("rs2 = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = -1;
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    RB[rd] = ((-2) ^ 31);
  else
    RB[rd] = (ac_Sword)RB[rs1] / (ac_Sword)RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction DIVU behavior method
void ac_behavior(DIVU) {
  dbg_printf("DIVU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = -1;
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    dbg_printf("Wrong division exception!");
  else
    RB[rd] = (ac_Uword)RB[rs1] / (ac_Uword)RB[rs2];
  dbg_printf("Result = %#x\n\n", (ac_Uword)RB[rd]);
}

// Instruction REM behavior method
void ac_behavior(REM) {
  dbg_printf("REM r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = RB[rs1];
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    RB[rd] = 0;
  else
    RB[rd] = RB[rs1] % RB[rs2];
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction REMU behavior method
void ac_behavior(REMU) {
  dbg_printf("REMU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = RB[rs1];
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    dbg_printf("Wrong division exception!");
  else
    RB[rd] = (ac_Uword)RB[rs1] % (ac_Uword)RB[rs2];
  dbg_printf("Result = %#x\n\n", (ac_Uword)RB[rd]);
}

// Instruction LR.W behavior method
void ac_behavior(LR_W) { RB[rd] = DM.read(RB[rs1]); }

// Instruction SC.w behavior method
void ac_behavior(SC_W) {
  DM.write(RB[rs1], RB[rs2]);
  RB[rd] = 0; // indicating success
}

// Instruction AMOSWAP.W behavior method
void ac_behavior(AMOSWAP_W) {
  dbg_printf("AMOSWAP.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  int temp;
  temp = RB[rs2];
  RB[rs2] = RB[rd];
  RB[rd] = temp;
  dbg_printf("After swapping RB[rd] = %d\n\n", RB[rd]);
  DM.write(RB[rs1], RB[rd]);
}

// Instruction AMOADD.W behavior method
void ac_behavior(AMOADD_W) {
  dbg_printf("AMOADD.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], ((ac_Sword)RB[rd] + (ac_Sword)RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] + RB[rs2]);
}

// Instruction AMOXOR.W behavior method
void ac_behavior(AMOXOR_W) {
  dbg_printf("AMOXOR.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] ^ RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] ^ RB[rs2]);
}

// Instruction AMOAND.W behavior method
void ac_behavior(AMOAND_W) {
  dbg_printf("AMOAND.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] & RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] & RB[rs2]);
}

// Instruction AMOOR.W behavior method
void ac_behavior(AMOOR_W) {
  dbg_printf("AMOOR.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] | RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] | RB[rs2]);
}

// Instruction AMOMIN.W behavior method
void ac_behavior(AMOMIN_W) {
  dbg_printf("AMOMIN.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rd] < RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMAX.W behavior method
void ac_behavior(AMOMAX_W) {
  dbg_printf("AMOMAX.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n\n", RB[rs2]);
  if (RB[rd] > RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMINU.W behavior method
void ac_behavior(AMOMINU_W) {
  dbg_printf("AMOMINU.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n\n", RB[rs2]);
  if ((ac_Uword)RB[rd] < (ac_Uword)RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMAXU.W behavior method
void ac_behavior(AMOMAXU_W) {
  dbg_printf("AMOMAXU.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if ((ac_Uword)RB[rd] > (ac_Uword)RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction FLW behavior method
void ac_behavior(FLW) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FLW r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RBF[rd] = DM.read(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %.3f\n\n", (float)RBF[rd]);
}

// Instruction FSW behavior method
void ac_behavior(FSW) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FSW r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RBF[rs2]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction FADD.S behavior method
void ac_behavior(FADD_S) {
  dbg_printf("FADD.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float sum;
  sum = load_float(rs1) + load_float(rs2);
  save_float(sum, rd);
  dbg_printf("Result = %.3f\n\n", sum);
}

// Instruction FSUB.S behavior method
void ac_behavior(FSUB_S) {
  dbg_printf("FSUB.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float diff;
  diff = load_float(rs1) - load_float(rs2);
  save_float(diff, rd);
  dbg_printf("Result = %.3f\n\n", diff);
}

// Instruction FMUL.S behavior method
void ac_behavior(FMUL_S) {
  dbg_printf("FMUL.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float product;
  product = load_float(rs1) * load_float(rs2);
  save_float(product, rd);
  dbg_printf("Result = %.3f\n\n", product);
}

// Instruction FDIV.S behavior method
void ac_behavior(FDIV_S) {
  dbg_printf("FDIV.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float quot;
  quot = load_float(rs1) / load_float(rs2);
  save_float(quot, rd);
  dbg_printf("Result = %.3f\n\n", quot);
}

// Instruction FMIN.S behavior method
void ac_behavior(FMIN_S) {
  dbg_printf("FMIN.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float temp;
  if (load_float(rs1) < load_float(rs2)) {
    temp = load_float(rs1);
    save_float(temp, rd);
  } else {
    temp = load_float(rs2);
    save_float(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMAX.S behavior method
void ac_behavior(FMAX_S) {
  dbg_printf("FMAX.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float temp;
  if (load_float(rs1) > load_float(rs2)) {
    temp = load_float(rs1);
    save_float(temp, rd);
  } else {
    temp = load_float(rs2);
    save_float(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FSQRT.S behavior method
void ac_behavior(FSQRT_S) {
  dbg_printf("FSQRT.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  float temp;
  if (load_float(rs1) < 0) {
    dbg_printf("Invalid!");
    stop();
  } else
    RBF[rd] = sqrt(RBF[rs1]);
  dbg_printf("Result = %.3f\n\n", (float)RBF[rd]);
}

// Instruction FMADD.S behavior method
void ac_behavior(FMADD_S) {
  dbg_printf("FMADD.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = load_float(rs1) * load_float(rs2) + load_float(rs3);
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FMSUB.S behavior method
void ac_behavior(FMSUB_S) {
  dbg_printf("FMSUB.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = load_float(rs1) * load_float(rs2) - load_float(rs3);
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMSUB.S behavior method
void ac_behavior(FNMSUB_S) {
  dbg_printf("FNMSUB.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = (-(load_float(rs1) * load_float(rs2) - load_float(rs3)));
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMADD.S behavior method
void ac_behavior(FNMADD_S) {
  dbg_printf("FNMADD.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = (-(load_float(rs1) * load_float(rs2) + load_float(rs3)));
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FCVT.W.S behavior method
void ac_behavior(FCVT_W_S) {
  dbg_printf("FCVT.W.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_float(rs1));
  RB[rd] = round(load_float(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.WU.S behavior method
void ac_behavior(FCVT_WU_S) {
  dbg_printf("FCVT.WU.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_float(rs1));
  RB[rd] = (unsigned int)(load_float(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.S.W behaior method
void ac_behavior(FCVT_S_W) {
  dbg_printf("FCVT.S.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float temp;
  ac_Sword b = RB[rs1];
  temp = (float)b;
  save_float(temp, rd);
}

// Instruction FCVT_S_WU behaior method
void ac_behavior(FCVT_S_WU) {
  dbg_printf("FCVT.S.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float temp;
  ac_Uword b = RB[rs1];
  temp = (float)b;
  save_float(temp, rd);
}

// Instruction FSGNJ_S behavior method
void ac_behavior(FSGNJ_S) {
  dbg_printf("FSGNJ.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float_cast f1 = { .f = load_float(rs1) };
  float_cast f2 = { .f = load_float(rs2) };
  f1.parts.sign = f2.parts.sign;
  save_float(f1.f, rd);
}
// Instruction FSGNJN_S behavior method
void ac_behavior(FSGNJN_S) {
  dbg_printf("FSGNJ.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float_cast f1 = { .f = load_float(rs1) };
  float_cast f2 = { .f = load_float(rs2) };
  f1.parts.sign = !f2.parts.sign;
  save_float(f1.f, rd);
}

// Instruction FSGNJX_S behavior method
void ac_behavior(FSGNJX_S) {
  dbg_printf("FSGNJX.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float_cast f1 = { .f = load_float(rs1) };
  float_cast f2 = { .f = load_float(rs2) };
  f1.parts.sign = f1.parts.sign ^ f2.parts.sign;
  save_float(f1.f, rd);
}

// Instruction FSGNJ_D behavior method
void ac_behavior(FSGNJ_D) {
  dbg_printf("FSGNJ.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double_cast d1 = { .d = load_double(rs1) };
  double_cast d2 = { .d = load_double(rs2) };
  d1.parts.sign = d2.parts.sign;
  save_double(d1.d, rd);
}
// Instruction FSGNJN_D behavior method
void ac_behavior(FSGNJN_D) {
  dbg_printf("FSGNJ.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double_cast d1 = { .d = load_double(rs1) };
  double_cast d2 = { .d = load_double(rs2) };
  d1.parts.sign = !d2.parts.sign;
  save_double(d1.d, rd);
}

// Instruction FSGNJX_D behavior method
void ac_behavior(FSGNJX_D) {
  dbg_printf("FSGNJX.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double_cast d1 = { .d = load_double(rs1) };
  double_cast d2 = { .d = load_double(rs2) };
  d1.parts.sign = d1.parts.sign ^ d2.parts.sign;
  save_double(d1.d, rd);
}


// Instruction FMV_X_S behavior method
void ac_behavior(FMV_X_S) {
  dbg_printf("FMV.X.S r%d, r%d \n", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  memcpy(&RB[rd], &RBF[rs1], sizeof(uint32_t));
  // RB[rd] = (int)load_float(rs1);
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FMV_S_X behavior method
void ac_behavior(FMV_S_X) {
  dbg_printf("FMV.S.X r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  memcpy(&RBF[rd], &RB[rs1], sizeof(uint32_t));
  // save_float(RB[rs1], rd);
  dbg_printf("RBF[rd] = %f \n \n", load_float(rd));
}

// Instruction FEQ_S behavior method
void ac_behavior(FEQ_S) {
  dbg_printf("FEQ.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((custom_isnan(load_float(rs1)) == 1) || (custom_isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) == load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLE_S behavior method
void ac_behavior(FLE_S) {
  dbg_printf("FLE.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((custom_isnan(load_float(rs1)) == 1) || (custom_isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) <= load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLT_S behavior method
void ac_behavior(FLT_S) {
  dbg_printf("FLT.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((custom_isnan(load_float(rs1)) == 1) || (custom_isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) < load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FMV.S behavior method
void ac_behavior(FMV_S) {
  dbg_printf("FMV.S r%d, r%d", rd, rs1);
  float temp;
  temp = load_float(rs1);
  save_float(temp, rd);
}

// Instruction FLD behavior method
void ac_behavior(FLD) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FLD r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RBF[rd * 2] = DM.read(RB[rs1] + sign_ext);
  RBF[rd * 2 + 1] = DM.read(RB[rs1] + sign_ext + 4);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  double temp = load_double(rd);
  dbg_printf("Double: %lf", temp);
}

// Instruction FSD behavior method
void ac_behavior(FSD) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FSD r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RBF[rs2 * 2]);
  DM.write(RB[rs1] + sign_ext + 4, RBF[rs2 * 2 + 1]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction FADD.D behavior method
void ac_behavior(FADD_D) {
  dbg_printf("FADD.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double sum;
  sum = load_double(rs1) + load_double(rs2);
  save_double(sum, rd);
  dbg_printf("Result = %.3f\n\n", sum);
}

// Instruction FSUB.D behavior method
void ac_behavior(FSUB_D) {
  dbg_printf("FSUB.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double diff;
  diff = load_double(rs1) - load_double(rs2);
  save_double(diff, rd);
  dbg_printf("Result = %.3f\n\n", diff);
}

// Instruction FMUL.D behavior method
void ac_behavior(FMUL_D) {
  dbg_printf("FMUL.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double product;
  product = load_double(rs1) * load_double(rs2);
  save_double(product, rd);
  dbg_printf("Result = %.3f\n\n", product);
}

// Instruction FDIV.D behavior method
void ac_behavior(FDIV_D) {
  dbg_printf("FDIV.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double quot;
  quot = load_double(rs1) / load_double(rs2);
  save_double(quot, rd);
  dbg_printf("Result = %.3f\n\n", quot);
}

// Instruction FMIN.D behavior method
void ac_behavior(FMIN_D) {
  dbg_printf("FMIN.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double temp;
  if (load_double(rs1) < load_double(rs2)) {
    temp = load_double(rs1);
    save_double(temp, rd);
  } else {
    temp = load_double(rs2);
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMAX.D behavior method
void ac_behavior(FMAX_D) {
  dbg_printf("FMAX.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double temp;
  if (load_double(rs1) > load_double(rs2)) {
    temp = load_double(rs1);
    save_double(temp, rd);
  } else {
    temp = load_double(rs2);
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FSQRT.D behavior method
void ac_behavior(FSQRT_D) {
  dbg_printf("FSQRT.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  double temp;
  if (load_double(rs1) < 0) {
    dbg_printf("Invalid!");
    stop();
  } else {
    temp = sqrt(load_double(rs1));
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMADD.D behavior method
void ac_behavior(FMADD_D) {
  dbg_printf("FMADD.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = load_double(rs1) * load_double(rs2) + load_double(rs3);
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FMSUB.D behavior method
void ac_behavior(FMSUB_D) {
  dbg_printf("FMSUB.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = load_double(rs1) * load_double(rs2) - load_double(rs3);
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMSUB.D behavior method
void ac_behavior(FNMSUB_D) {
  dbg_printf("FNMSUB.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = (-(load_double(rs1) * load_double(rs2) - load_double(rs3)));
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMADD.D behavior method
void ac_behavior(FNMADD_D) {
  dbg_printf("FNMADD.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = (-(load_double(rs1) * load_double(rs2) + load_double(rs3)));
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FCVT.W.D behavior method
void ac_behavior(FCVT_W_D) {
  dbg_printf("FCVT.W.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_double(rs1));
  RB[rd] = lround(load_double(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.WU.D behavior method
void ac_behavior(FCVT_WU_D) {
  dbg_printf("FCVT.WU.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_double(rs1));
  RB[rd] = (unsigned int)(load_double(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT_D_W behaior method
void ac_behavior(FCVT_D_W) {
  dbg_printf("FCVT.D.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double temp;
  ac_Sword b = RB[rs1];
  temp = (double)b;
  save_double(temp, rd);
}

// Instruction FCVT_D_WU behaior method
void ac_behavior(FCVT_D_WU) {
  dbg_printf("FCVT.D.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double temp;
  ac_Uword b = RB[rs1];
  temp = (double)b;
  save_double(temp, rd);
}

// Instruction FCVT_S_D behavior method
void ac_behavior(FCVT_S_D) {
  dbg_printf("FCVT.S.D r%d, r%d", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  float temp;
  temp = (float)(load_double(rs1));
  save_float(temp, rd);
}

// Instruction FCVT_D_S behavior method
void ac_behavior(FCVT_D_S) {
  dbg_printf("FCVT.D.S r%d, r%d", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  double temp;
  temp = (double)(load_float(rs1));
  save_double(temp, rd);
}

// Instruction FMV.D behavior method
void ac_behavior(FMV_D) {
  dbg_printf("FMV.D r%d, r%d", rd, rs1);
  double temp;
  temp = load_double(rs1);
  save_double(temp, rd);
}

// Instruction FEQ_D behavior method
void ac_behavior(FEQ_D) {
  dbg_printf("FEQ.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) == load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLE_D behavior method
void ac_behavior(FLE_D) {
  dbg_printf("FLE.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) <= load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLT_D behavior method
void ac_behavior(FLT_D) {
  dbg_printf("FLT.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) < load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}


// Instruction VSETVLI behavior method
void ac_behavior(VSETVLI) {

  vtype = (imm2 & 0x1f);

  int sewf8 = 1 << ( vtype >> 3 );
  int lmul = 1 << ( vtype & 0x7 );
  int vlmax = lmul * ( vlenb & 0xff ) / sewf8;
  int avl = RB[rs1];

  vl = ( avl < vlmax ) ? avl : vlmax;
  RB[rd] = vl;

  dbg_printf("VSETVLI r%d, r%d, e%d, m%d\n", rd, rs1, 8 * sewf8, lmul );
  dbg_printf("VL = %d\n\n", RB[rd] );

}


// Instruction VLE behavior method
void ac_behavior(VLE) {

  int eew;
  int addr = RB[rs1];
  v128 *v = ( v128 * ) &RBV[ 4 * vd ];

  if ( funct3 == 0 ) {
    eew = 1;
  } else if ( funct3 == 5 ) {
    eew = 2;
  } else if ( funct3 == 6 ) {
    eew = 4;
  }

  int vlmax = 128 / ( 8 * eew );
  
  dbg_printf("VLE%d.v v%d, (%d)\n", 8 * eew, vd, addr );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( eew == 1 ) {
      v->set8( idx, DM.read_byte( addr + el * eew ) );
    } else if ( eew == 2 ) {
      v->set16( idx, DM.read_half( addr + el * eew ) );
    } else if ( eew == 4 ) {
      v->set32( idx, DM.read( addr + el * eew ) );
    }

    if ( idx == vlmax-1 ) v++;

  }

  v = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * eew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v->w[ i ]);
    if ( i == 3 ) v++;
  }
  dbg_printf("bytes = %d\n\n", vl * eew);

}

// Instruction VLSE behavior method
void ac_behavior(VLSE) {

  int eew;
  int addr = RB[rs1];
  int stride = RB[rs2];
  v128 *v = ( v128 * ) &RBV[ 4 * vd ];

  if ( funct3 == 0 ) {
    eew = 1;
  } else if ( funct3 == 5 ) {
    eew = 2;
  } else if ( funct3 == 6 ) {
    eew = 4;
  }

  int vlmax = 128 / ( 8 * eew );
  
  dbg_printf("VLSE%d.v v%d, (%d), %d\n", 8 * eew, vd, addr, rs2 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( eew == 1 ) {
      v->set8( idx, DM.read_byte( addr + el * stride ) );
    } else if ( eew == 2 ) {
      v->set16( idx, DM.read_half( addr + el * stride ) );
    } else if ( eew == 4 ) {
      v->set32( idx, DM.read( addr + el * stride ) );
    }

    if ( idx == vlmax-1 ) v++;

  }

  v = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * eew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v->w[ i ]);
    if ( i == 3 ) v++;
  }
  dbg_printf("bytes = %d\n\n", vl * eew);

}

// Instruction VSE behavior method
void ac_behavior(VSE) {

  int eew;
  int addr = RB[rs1];
  v128 *v = ( v128 * ) &RBV[ 4 * vd ];

  if ( funct3 == 0 ) {
    eew = 1;
  } else if ( funct3 == 5 ) {
    eew = 2;
  } else if ( funct3 == 6 ) {
    eew = 4;
  }

  int vlmax = 128 / ( 8 * eew );
  
  dbg_printf("VSE%d.v v%d, (%d)\n", 8 * eew, vd, addr );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( eew == 1 ) {
      DM.write_byte( addr + el * eew, v->get8( idx ) );
    } else if ( eew == 2 ) {
      DM.write_half( addr + el * eew, v->get16( idx ) );
    } else if ( eew == 4 ) {
      DM.write( addr + el * eew, v->get32( idx ) );
    }

    if ( idx == vlmax-1 ) v++;

  }

  for( int c = 0; c < ( vl * eew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("M[%d] = %d\n", addr + c * 4, DM.read( addr + c * 4 ) );
    if ( i == 3 ) v++;
  }
  dbg_printf("bytes = %d\n\n", vl * eew);

}

// Instruction VSSE behavior method
void ac_behavior(VSSE) {

  int eew;
  int addr = RB[rs1];
  int stride = RB[rs2];
  v128 *v = ( v128 * ) &RBV[ 4 * vd ];

  if ( funct3 == 0 ) {
    eew = 1;
  } else if ( funct3 == 5 ) {
    eew = 2;
  } else if ( funct3 == 6 ) {
    eew = 4;
  }

  int vlmax = 128 / ( 8 * eew );
  
  dbg_printf("VSSE%d.v v%d, (%d) r%d\n", 8 * eew, vd, addr, rs2 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( eew == 1 ) {
      DM.write_byte( addr + el * stride, v->get8( idx ) );
    } else if ( eew == 2 ) {
      DM.write_half( addr + el * stride, v->get16( idx ) );
    } else if ( eew == 4 ) {
      DM.write( addr + el * stride, v->get32( idx ) );
    }

    if ( idx == vlmax-1 ) v++;

  }

  for( int c = 0; c < ( vl * eew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("M[%d] = %d\n", addr + c * 4, DM.read( addr + c * 4 ) );
    if ( i == 3 ) v++;
  }
  dbg_printf("bytes = %d\n\n", vl * eew);

}

// Instruction VADDVV behavior method
void ac_behavior(VADDVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VADD.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) + ( int8_t ) v2->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) + ( int16_t ) v2->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) + ( int32_t ) v2->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSUBVV behavior method
void ac_behavior(VSUBVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSUB.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v2->get8( idx ) - ( int8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v2->get16( idx ) - ( int16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v2->get32( idx ) - ( int32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMULVV behavior method
void ac_behavior(VMULVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 16 / ( sew );
  
  dbg_printf("VMUL.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v2->get8( idx ) * ( int8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v2->get16( idx ) * ( int16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v2->get32( idx ) * ( int32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VADDVX behavior method
void ac_behavior(VADDVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VADD.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) + ( int8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) + ( int16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) + ( int32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSUBVX behavior method
void ac_behavior(VSUBVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSUB.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) - ( int8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) - ( int16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) - ( int32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VADDVI behavior method
void ac_behavior(VADDVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VADD.VI v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) + imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) + imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) + imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VANDVV behavior method
void ac_behavior(VANDVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VAND.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v2->get8( idx ) & ( int8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v2->get16( idx ) & ( int16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v2->get32( idx ) & ( int32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VANDVX behavior method
void ac_behavior(VANDVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VAND.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) & ( int8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) & ( int16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) & ( int32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VANDVI behavior method
void ac_behavior(VANDVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VAND.VI v%d, v%d, %d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) & imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) & imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) & imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VORVV behavior method
void ac_behavior(VORVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VOR.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v2->get8( idx ) | ( int8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v2->get16( idx ) | ( int16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v2->get32( idx ) | ( int32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VORVX behavior method
void ac_behavior(VORVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VOR.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) | ( int8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) | ( int16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) | ( int32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VORVI behavior method
void ac_behavior(VORVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VOR.VI v%d, v%d, %d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) | imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) | imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) | imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VXORVV behavior method
void ac_behavior(VXORVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VXOR.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v2->get8( idx ) ^ ( int8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v2->get16( idx ) ^ ( int16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v2->get32( idx ) ^ ( int32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VXORVX behavior method
void ac_behavior(VXORVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VXOR.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) ^ ( int8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) ^ ( int16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) ^ ( int32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VXORVI behavior method
void ac_behavior(VXORVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VXOR.VI v%d, v%d, %d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( int8_t ) v1->get8( idx ) ^ imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( int16_t ) v1->get16( idx ) ^ imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( int32_t ) v1->get32( idx ) ^ imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMINUVV behavior method
void ac_behavior(VMINUVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMINU.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      if ( v2->get8( idx ) < v1->get8( idx ) )
        v0->set8( idx, v2->get8( idx ) );
      else
        v0->set8( idx, v1->get8( idx ) );
    } else if ( sew == 2 ) {
      if ( v2->get16( idx ) < v1->get16( idx ) )
        v0->set16( idx, v2->get16( idx ) );
      else
        v0->set16( idx, v1->get16( idx ) );
    } else if ( sew == 4 ) {
      if ( v2->get32( idx ) < v1->get32( idx ) )
        v0->set32( idx, v2->get32( idx ) );
      else
        v0->set32( idx, v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMINUVX behavior method
void ac_behavior(VMINUVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMINU.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      if ( ( uint8_t ) RB[rs1] < v1->get8( idx ) )
        v0->set8( idx, ( uint8_t ) RB[rs1] );
      else
        v0->set8( idx, v1->get8( idx ) );
    } else if ( sew == 2 ) {
      if ( ( uint16_t ) RB[rs1] < v1->get16( idx ) )
        v0->set16( idx, ( uint16_t ) RB[rs1] );
      else
        v0->set16( idx, v1->get16( idx ) );
    } else if ( sew == 4 ) {
      if ( ( uint32_t ) RB[rs1] < v1->get32( idx ) )
        v0->set32( idx, ( uint32_t ) RB[rs1] );
      else
        v0->set32( idx, v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMAXUVV behavior method
void ac_behavior(VMAXUVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMAXU.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      if ( v2->get8( idx ) > v1->get8( idx ) )
        v0->set8( idx, v2->get8( idx ) );
      else
        v0->set8( idx, v1->get8( idx ) );
    } else if ( sew == 2 ) {
      if ( v2->get16( idx ) > v1->get16( idx ) )
        v0->set16( idx, v2->get16( idx ) );
      else
        v0->set16( idx, v1->get16( idx ) );
    } else if ( sew == 4 ) {
      if ( v2->get32( idx ) > v1->get32( idx ) )
        v0->set32( idx, v2->get32( idx ) );
      else
        v0->set32( idx, v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMAXUVX behavior method
void ac_behavior(VMAXUVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMAXU.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      if ( ( uint8_t ) RB[rs1] > v1->get8( idx ) )
        v0->set8( idx, ( uint8_t ) RB[rs1] );
      else
        v0->set8( idx, v1->get8( idx ) );
    } else if ( sew == 2 ) {
      if ( ( uint16_t ) RB[rs1] > v1->get16( idx ) )
        v0->set16( idx, ( uint16_t ) RB[rs1] );
      else
        v0->set16( idx, v1->get16( idx ) );
    } else if ( sew == 4 ) {
      if ( ( uint32_t ) RB[rs1] > v1->get32( idx ) )
        v0->set32( idx, ( uint32_t ) RB[rs1] );
      else
        v0->set32( idx, v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMVXS behavior method
void ac_behavior(VMVXS) {

  int copy;
  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * rs2 ];

  dbg_printf("VMV.X.S r%d, v%d\n", vd, rs2 );
  dbg_printf("RB[%d] = v%d[0]\n", vd, rs2 );

  if ( sew == 1 ) {
    copy = sign_extend( v0->get8( 0 ), 8 );
  } else if ( sew == 2 ) {
    copy = sign_extend( v0->get16( 0 ), 16 );
  } else if ( sew == 4 ) {
    copy = ( int32_t ) v0->get32( 0 );
  }

  RB[vd] = copy;

  dbg_printf("RB[%d] = %d\n\n", vd, RB[vd]);

}

// Instruction VSLLVV behavior method
void ac_behavior(VSLLVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSLL.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v2->get8( idx ) << ( uint8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, v2->get16( idx ) << ( uint16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, v2->get32( idx ) << ( uint32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSLLVX behavior method
void ac_behavior(VSLLVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSLL.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v1->get8( idx ) << ( uint8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, v1->get16( idx ) << ( uint16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, v1->get32( idx ) << ( uint32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSLLVI behavior method
void ac_behavior(VSLLVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = rs1;
  
  dbg_printf("VSLL.VI v%d, v%d, %d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v1->get8( idx ) << imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, v1->get16( idx ) << imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, v1->get32( idx ) << imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSRLVV behavior method
void ac_behavior(VSRLVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSRL.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v2->get8( idx ) >> ( uint8_t ) v1->get8( idx ) );
    } else if ( sew == 2 ) {
      v0->set16( idx, v2->get16( idx ) >> ( uint16_t ) v1->get16( idx ) );
    } else if ( sew == 4 ) {
      v0->set32( idx, v2->get32( idx ) >> ( uint32_t ) v1->get32( idx ) );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSRLVX behavior method
void ac_behavior(VSRLVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VSRL.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v1->get8( idx ) >> ( uint8_t ) RB[rs1] );
    } else if ( sew == 2 ) {
      v0->set16( idx, v1->get16( idx ) >> ( uint16_t ) RB[rs1] );
    } else if ( sew == 4 ) {
      v0->set32( idx, v1->get32( idx ) >> ( uint32_t ) RB[rs1] );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VSRLVI behavior method
void ac_behavior(VSRLVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = rs1;
  
  dbg_printf("VSRL.VI v%d, v%d, %d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, v1->get8( idx ) >> imm );
    } else if ( sew == 2 ) {
      v0->set16( idx, v1->get16( idx ) >> imm );
    } else if ( sew == 4 ) {
      v0->set32( idx, v1->get32( idx ) >> imm );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMVSX behavior method
void ac_behavior(VMVSX) {

  int copy = RB[rs1];
  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];

  dbg_printf("VMV.S.X v%d, r%d\n", vd, rs1 );
  dbg_printf("v%d[0] = RB[%d]\n", vd, rs1 );

  if ( sew == 1 ) {
    v0->set8( 0, copy );
    dbg_printf("v%d[0] = %d\n\n", vd, v0->get8( 0 ) );
  } else if ( sew == 2 ) {
    v0->set16( 0, copy );
    dbg_printf("v%d[0] = %d\n\n", vd, v0->get16( 0 ) );
  } else if ( sew == 4 ) {
    v0->set32( 0, copy );
    dbg_printf("v%d[0] = %d\n\n", vd, v0->get32( 0 ) );
  }

}

// Instruction VMSEQVV behavior method
void ac_behavior(VMSEQVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSEQ.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v2->get8( idx ) == ( uint8_t ) v1->get8( idx ) ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v2->get16( idx ) == ( uint16_t ) v1->get16( idx ) ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v2->get32( idx ) == ( uint32_t ) v1->get32( idx ) ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSEQVX behavior method
void ac_behavior(VMSEQVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSEQ.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) == ( uint8_t ) RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) == ( uint16_t ) RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) == ( uint32_t ) RB[rs1] ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSEQVI behavior method
void ac_behavior(VMSEQVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VMSEQ.VI v%d, v%d, r%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) == imm ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) == imm ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) == imm ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSNEVV behavior method
void ac_behavior(VMSNEVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSNE.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v2->get8( idx ) != ( uint8_t ) v1->get8( idx ) ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v2->get16( idx ) != ( uint16_t ) v1->get16( idx ) ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v2->get32( idx ) != ( uint32_t ) v1->get32( idx ) ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSNEVX behavior method
void ac_behavior(VMSNEVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSNE.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) != ( uint8_t ) RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) != ( uint16_t ) RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) != ( uint32_t ) RB[rs1] ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSNEVI behavior method
void ac_behavior(VMSNEVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VMSNE.VI v%d, v%d, r%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) != imm ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) != imm ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) != imm ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSLTVV behavior method
void ac_behavior(VMSLTVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSLT.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v2->get8( idx ) < v1->get8( idx ) ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v2->get16( idx ) < v1->get16( idx ) ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v2->get32( idx ) < v1->get32( idx ) ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSLTVX behavior method
void ac_behavior(VMSLTVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSLT.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) < RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) < RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) < RB[rs1] ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSLEVV behavior method
void ac_behavior(VMSLEVV) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSLE.VV v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v2->get8( idx ) <= v1->get8( idx ) ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v2->get16( idx ) <= v1->get16( idx ) ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v2->get32( idx ) <= v1->get32( idx ) ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++; v2++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSLEVX behavior method
void ac_behavior(VMSLEVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSLE.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) <= RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) <= RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) <= RB[rs1] ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSLEVI behavior method
void ac_behavior(VMSLEVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VMSLE.VI v%d, v%d, r%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) <= imm ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) <= imm ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) <= imm ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSGTVX behavior method
void ac_behavior(VMSGTVX) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 128 / ( 8 * sew );
  
  dbg_printf("VMSGT.VX v%d, v%d, r%d\n", vd, rs2, rs1 );
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) > RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) > RB[rs1] ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) > RB[rs1] ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VMSGTVI behavior method
void ac_behavior(VMSGTVI) {

  int sew = 1 << ( vtype >> 3 );
  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];

  int vlmax = 128 / ( 8 * sew );
  int imm = sign_extend( rs1, 5 );
  
  dbg_printf("VMSGT.VI v%d, v%d, r%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      v0->set8( idx, ( v1->get8( idx ) > imm ) ? 1 : 0 );
    } else if ( sew == 2 ) {
      v0->set16( idx, ( v1->get16( idx ) > imm ) ? 1 : 0 );
    } else if ( sew == 4 ) {
      v0->set32( idx, ( v1->get32( idx ) > imm ) ? 1 : 0 );
    }

    if ( idx == vlmax-1 ) {
      v0++; v1++;
    }

  }

  v0 = ( v128 * ) &RBV[ 4 * vd ];
  for( int c = 0; c < ( vl * sew ) / 4; c++ ) {
    int i = c % 4;
    dbg_printf("v%d[%d] = %d\n", vd + c/4, i, v0->w[ i ]);
    if ( i == 3 ) v0++;
  }
  dbg_printf("bytes = %d\n\n", vl * sew);

}

// Instruction VREDSUMVS behavior method
void ac_behavior(VREDSUMVS) {

  int sum;
  int sew = 1 << ( vtype >> 3 );
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 16 / sew;
 
  // current sum vs1[0]
  if ( sew == 1 ) {
    sum = sign_extend( v1->get8( 0 ), 8 );
  } else if ( sew == 2 ) {
    sum = sign_extend( v1->get16( 0 ), 16 );
  } else if ( sew == 4 ) {
    sum = v1->get32( 0 );
  }
  
  dbg_printf("VREDSUM.VS v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      sum += sign_extend( v2->get8( idx ), 8 );
    } else if ( sew == 2 ) {
      sum += sign_extend( v2->get16( idx ), 16 );
    } else if ( sew == 4 ) {
      sum += ( int8_t ) v2->get32( idx );
    }

    if ( idx == vlmax-1 ) {
      v2++;
    }

  }

  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];

  if ( sew == 1 ) {
    v0->set8( 0, sum );
    dbg_printf("v%d[0] = %d\n\n", vd, ( int8_t ) v0->get8( 0 ) );
  } else if ( sew == 2 ) {
    v0->set16( 0, sum );
    dbg_printf("v%d[0] = %d\n\n", vd, ( int16_t ) v0->get16( 0 ) );
  } else if ( sew == 4 ) {
    v0->set32( 0, sum );
    dbg_printf("v%d[0] = %d\n\n", vd, ( int32_t )  v0->get32( 0 ) );
  }

}

// Instruction VWREDSUMUVS behavior method
void ac_behavior(VWREDSUMUVS) {

  int sum;
  int sew = 1 << ( vtype >> 3 );
  v128 *v1 = ( v128 * ) &RBV[ 4 * rs1 ];
  v128 *v2 = ( v128 * ) &RBV[ 4 * rs2 ];

  int vlmax = 16 / sew;
 
  // 2 * SEW - current sum vs1[0]
  if ( sew == 1 ) {
    sum = v1->get16( 0 );
  } else if ( sew == 2 ) {
    sum = v1->get32( 0 );
  }
  
  dbg_printf("VWREDSUMU.VS v%d, v%d, v%d\n", vd, rs2, rs1 );

  for (int el = 0; el < vl; el++) {

    int idx = el % vlmax;

    if ( sew == 1 ) {
      sum += v2->get8( idx );
    } else if ( sew == 2 ) {
      sum += v2->get16( idx );
    }

    if ( idx == vlmax-1 ) {
      v2++;
    }

  }

  v128 *v0 = ( v128 * ) &RBV[ 4 * vd ];

  if ( sew == 1 ) {
    v0->set16( 0, sum );
    dbg_printf("v%d[0] = %d\n\n", vd, v0->get16( 0 ) );
  } else if ( sew == 2 ) {
    v0->set32( 0, sum );
    dbg_printf("v%d[0] = %d\n\n", vd, v0->get32( 0 ) );
  }

}
