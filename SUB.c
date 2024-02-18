#include "Instruction.h"

// LEVI STRIETER

void sub_immd_assm(void) {
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "SUB") != 0) {
        // If the op code doesnt match, this isnt the correct command
        state = WRONG_COMMAND;
        return;
    }
    
    /*
     Checking the type of parameters
     */
    
    // Generally the first parameter should always be a register
    if (PARAM1.type != REGISTER) {
        state = MISSING_REG;
        return;
    }
    
    // AND immediate can either have 1 or 2 registers total
    
    // If param2 is a register, we also need an immediate
    if ((PARAM2.type == REGISTER) && (PARAM3.type != IMMEDIATE)) {
        // This could be an AND register command
        state = WRONG_COMMAND;
        return;
    }
    
    // If param2 is an immediate, param3 should be empty
    if ((PARAM2.type == IMMEDIATE) && (PARAM3.type != EMPTY)) {
        state = INVALID_PARAM;
        return;
    }
    
    // This parameter should be empty
    if (PARAM4.type != EMPTY) {
        state = UNEXPECTED_PARAM;
        return;
    }
    
    /*
     Checking the value of parameters
     */
    
    // Rd should be 15 or less
    if (PARAM1.value > 15) {
        state = INVALID_REG;
        return;
    }
    
    // 2 registers
    if ((PARAM2.type == REGISTER)) {
        // Rn should be 15 or less
        if (PARAM2.value > 15) {
            state = INVALID_REG;
            return;
        }
        // The immediate value is limited to 12 bits, which is 0xFFF in hex
        if (PARAM3.value > 0xFFF) {
            state = INVALID_IMMED;
            return;
        }
    }
    // 1 register
    else {
        
        // The immediate value is limited to 12 bits, which is 0xFFF in hex
        if (PARAM2.value > 0xFFF) {
            state = INVALID_IMMED;
            return;
        }
    }
    
    /*
     Putting the binary together
     */
    
    // Set the conditional code
    // setBits_num(start_bit, number, number_width)
    setBits_num(31, cond2num(COND), 4);
    
    // Set the op code
    // setBits_str(start_bit, string)
    //  any x in string will be skipped
    setBits_str(27, "0010010");
    
    // Set S bit
    setBits_num(20, S_FLAG, 1);
    
    // set Rd
    setBits_num(15, PARAM1.value, 4);
    
    // 2 registers
    if ((PARAM2.type == REGISTER)) {
        // set Rn
        setBits_num(19, PARAM2.value, 4);
        
        // set immediate
        setBits_num(11, PARAM3.value, 12);
    }
    // 1 register
    else {
        // if we only have 1 register, Rd == Rn
        
        // set Rn
        setBits_num(19, PARAM1.value, 4);
        
        // set immediate
        setBits_num(11, PARAM2.value, 12);
    }
    
    // tell the system the encoding is done
    state = COMPLETE_ENCODE;
}


void sub_immd_bin(void) {
    // Check if the op code bits match
    // check_bits(start_bit, bit_string) returns 0 if the bit_string matches
    //  any x will be skipped
    // If the manual shows (0), then the value of that bit doesnt matter
    if (checkBits(27, "0010010") != 0) {
        state = WRONG_COMMAND;
        return;
    }

    // If the op code bits match, then the rest can be read as correctly

    /*
        Finding values in the binary
    */
    // getBits(start_bit, width)
    uint32_t cond = getBits(31, 4);
    S_FLAG = getBits(20, 1); // Directly setting the S flag
    uint32_t Rd = getBits(15, 4);
    uint32_t Rn = getBits(19, 4);
    uint32_t imm12 = getBits(11, 12);

    /*
        Setting Instuciton values
    */

    setOp("SUB");
    setCond_num(cond);
    // setParam(param_num, param_type, param_value)
    setParam(1, REGISTER, Rd);
    setParam(2, REGISTER, Rn);
    setParam(3, IMMEDIATE, imm12);

    // if you want, you could detect whether Rd == Rn
    //   and print the shortened version,
    //   ex. AND R1, #0x3 vs AND R1, R1, #0x3

    // tell the system the decoding is done
    state = COMPLETE_DECODE;

}

void sub_reg_assm(void) { 
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "SUB") != 0) {
        // If the op code doesnt match, this isnt the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Checking the type of parameters
    */

    // Generally the first parameter should always be a register
    if (PARAM1.type != REGISTER) {
        state = MISSING_REG;
        return;
    }

    // This is AND register, so param 2 needs to be a register
    if (PARAM2.type != REGISTER) {
        state = MISSING_REG;
        return;
    }

    // AND register can either have 2 or 3 registers total, with an optional shift

    // If param3 is an immediate, param4 should be empty, and there should be a shift
    if ((PARAM3.type == IMMEDIATE) && (PARAM4.type != EMPTY) && (SHIFT != NONE)) {
        state = INVALID_PARAM;
        return;
    }
    // param3 can also be empty or a register

    // If param4 is an immediate, there should be a shift
    if ((PARAM4.type == IMMEDIATE) && (SHIFT == NONE)) {
        state = INVALID_PARAM;
        return;
    }

    // If there is no shift, param4 shouldbe empty
    if ((SHIFT == NONE) && (PARAM4.type != EMPTY)) {
        state = UNEXPECTED_PARAM;
        return;
    }


    /*
        Checking the value of parameters
    */

    // Rd should be 15 or less
    if (PARAM1.value > 15) {
        state = INVALID_REG;
        return;
    }

    // param2 could be Rn or Rm, either should be 15 or less
    if (PARAM2.value > 15) {
        state = INVALID_REG;
        return;
    }

    // 3 registers
    if (PARAM3.type == REGISTER) {
        // registers should be 15 or less
        if (PARAM3.value > 15) {
            state = INVALID_REG;
            return;
        }
    }
    // 2 registers and shift
    if (PARAM3.type == IMMEDIATE) {
        // The immediate value is limited to 5 bits, which is 0x1F in hex
        if (PARAM3.value > 0x1F) {
            state = INVALID_REG;
            return;
        }
    }
    // 3 registers and shift
    else if (PARAM4.type == IMMEDIATE) {
        // The immediate value is limited to 5 bits, which is 0x1F in hex
        if (PARAM4.value > 0x1F) {
            state = INVALID_REG;
            return;
        }
    }


    /*
        Putting the binary together
    */

    // Set the conditional code
    // setBits_num(start_bit, number, number_width)
    setBits_num(31, cond2num(COND), 4);

    // Set the op code
    // setBits_str(start_bit, string)
    //  any x in string will be skipped
    setBits_str(27, "0000010");
    setBits_str(4, "0");

    // Set S bit
    setBits_num(20, S_FLAG, 1);

    // set Rd
    setBits_num(15, PARAM1.value, 4);

    // Only 2 registers
    if (PARAM3.type == EMPTY) {
        // set Rn
        setBits_num(19, PARAM1.value, 4);

        // set Rm
        setBits_num(3, PARAM2.value, 4);
    }
    // 3 registers
    else if (PARAM3.type == REGISTER) {
        // set Rn
        setBits_num(19, PARAM2.value, 4);

        // set Rm
        setBits_num(3, PARAM3.value, 4);
    }


    // 2 registers and shift
    if (PARAM3.type == IMMEDIATE) {
        // set Rn, Rn == Rd
        setBits_num(19, PARAM1.value, 4);

        // set Rm
        setBits_num(3, PARAM2.value, 4);

        // set shift type and value
        setBits_num(6, shift2num(SHIFT), 2);
        setBits_num(11, PARAM3.value, 5);
    }
    // 3 registers and shift
    else if (PARAM4.type == IMMEDIATE) {
        // set shift type and value
        setBits_num(6, shift2num(SHIFT), 2);
        setBits_num(11, PARAM4.value, 5);
    }
    // No shift given
    else {
        // set shift type and value
        setBits_num(6, shift2num(SHIFT), 2);
        setBits_num(11, 0, 5);
    }

    // tell the system the encoding is done
    state = COMPLETE_ENCODE;
}

void and_reg_bin(void) {
    // Check if the op code bits match
    // check_bits(start_bit, bit_string) returns 0 if the bit_string matches
    //  any x will be skipped
    // If the manual shows (0), then the value of that bit doesnt matter
    if (checkBits(27, "0000000") != 0) {
        state = WRONG_COMMAND;
        return;
    }
    
    // If the op code bits match, then the rest can be read as correctly
    
    /*
     Finding values in the binary
     */
    // getBits(start_bit, width)
    uint32_t cond = getBits(31, 4);
    S_FLAG = getBits(20, 1); // Directly setting the S flag
    uint32_t Rd = getBits(15, 4);
    uint32_t Rn = getBits(19, 4);
    uint32_t Rm = getBits(3, 4);
    
    Shift_Type shift_type = num2shift(getBits(6, 2));
    uint32_t shift_value = getBits(11, 5);
    
    /*
     Setting Instuciton values
     */
    
    setOp("AND");
    setCond_num(cond);
    // setParam(param_num, param_type, param_value)
    setParam(1, REGISTER, Rd);
    setParam(2, REGISTER, Rn);
    setParam(3, REGISTER, Rm);
    
    if (shift_value == 0) {
        // RRX case
        if (shift_type == ROR) {
            SHIFT = RRX;
            setParam(4, IMMEDIATE, 0);
        }
        // no shift case
        else {
            SHIFT = NONE;
        }
    }
    // shift present
    else {
        SHIFT = shift_type;
        setParam(4, IMMEDIATE, shift_value);
    }
    
    
    // if you want, you could detect whether Rd == Rn
    //   and print the shortened version,
    //   ex. AND R1, R2 vs AND R1, R1, R2
    
    // tell the system the decoding is done
    state = COMPLETE_DECODE;
}


void sub_reg_bin(void) {
    // Check if the op code bits match
    // check_bits(start_bit, bit_string) returns 0 if the bit_string matches
    //  any x will be skipped
    // If the manual shows (0), then the value of that bit doesnt matter
    if (checkBits(27, "000010") != 0) {
        state = WRONG_COMMAND;
        return;
    }

    // If the op code bits match, then the rest can be read as correctly

    /*
        Finding values in the binary
    */
    // getBits(start_bit, width)
    uint32_t cond = getBits(31, 4);
    S_FLAG = getBits(20, 1); // Directly setting the S flag
    uint32_t Rd = getBits(15, 4);
    uint32_t Rn = getBits(19, 4);
    uint32_t Rm = getBits(3, 4);

    Shift_Type shift_type = num2shift(getBits(6, 2));
    uint32_t shift_value = getBits(11, 5);

    /*
        Setting Instuciton values
    */

    setOp("SUB");
    setCond_num(cond);
    // setParam(param_num, param_type, param_value)
    setParam(1, REGISTER, Rd);
    setParam(2, REGISTER, Rn);
    setParam(3, REGISTER, Rm);

    if (shift_value == 0) {
        // RRX case
        if (shift_type == ROR) {
            SHIFT = RRX;
            setParam(4, IMMEDIATE, 0);
        }
        // no shift case
        else {
            SHIFT = NONE;
        }
    }
    // shift present
    else {
        SHIFT = shift_type;
        setParam(4, IMMEDIATE, shift_value);
    }


    // if you want, you could detect whether Rd == Rn
    //   and print the shortened version,
    //   ex. AND R1, R2 vs AND R1, R1, R2

    // tell the system the decoding is done
    state = COMPLETE_DECODE;
}
