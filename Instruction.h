#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <string.h>
#include "ARM_instruction.h"
#include "global_data.h"

// ADD.c
void add_immd_assm(void);
void add_immd_bin(void);
void add_reg_assm(void);
void add_reg_bin(void);

// AND.c
void and_immd_assm(void);
void and_immd_bin(void);
void and_reg_assm(void);
void and_reg_bin(void);

// AND.c
void and_immd_assm(void);
void and_immd_bin(void);
void and_reg_assm(void);
void and_reg_bin(void);

// CMP.c
void cmp_immd_assm(void);
void cmp_immd_bin(void);
void cmp_reg_assm(void);
void cmp_reg_bin(void);

// EOR.c
void eor_immd_assm(void);
void eor_immd_bin(void);
void eor_reg_assm(void);
void eor_reg_bin(void);

// LSL.c
void lsl_immd_assm(void);
void lsl_immd_bin(void);
void lsl_reg_assm(void);
void lsl_reg_bin(void);

// LSR.c
void lsr_immd_assm(void);
void lsr_immd_bin(void);
void lsr_reg_assm(void);
void lsr_reg_bin(void);

// MLA.c
void mla_reg_assm(void);
void mla_reg_bin(void);

// MLS.c
void mls_reg_assm(void);
void mls_reg_bin(void);

// MOV.c
void mov_immd_assm(void);
void mov_immd_bin(void);
void mov_reg_assm(void);
void mov_reg_bin(void);

// MUL.c
void mul_reg_assm(void);
void mul_reg_bin(void);

// MVN.c
void mvn_immd_assm(void);
void mvn_immd_bin(void);
void mvn_reg_assm(void);
void mvn_reg_bin(void);

// ORN.c
void orn_immd_assm(void);
void orn_immd_bin(void);
void orn_reg_assm(void);
void orn_reg_bin(void);

// ORR.c
void orr_immd_assm(void);
void orr_immd_bin(void);
void orr_reg_assm(void);
void orr_reg_bin(void);

// ROR.c
void ror_immd_assm(void);
void ror_immd_bin(void);
void ror_reg_assm(void);
void ror_reg_bin(void);

// RSB.c
void rsb_immd_assm(void);
void rsb_immd_bin(void);
void rsb_reg_assm(void);
void rsb_reg_bin(void);

// SUB.c
void sub_immd_assm(void);
void sub_immd_bin(void);
void sub_reg_assm(void);
void sub_reg_bin(void);

#endif