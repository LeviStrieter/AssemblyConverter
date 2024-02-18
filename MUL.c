#include "Instruction.h"

// LEVI STRIETER
void mul_reg_assm(void) {
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "MUL") != 0) {
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

    // This is MOV immediate, so param 2 needs to be an immediate
    if (PARAM2.type != REGISTER) {
        state = MISSING_REG;
        return;
    }

    // This parameter should be empty
    if (PARAM3.type != REGISTER) {
        state = MISSING_REG;
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

    // The immediate value is limited to 12 bits, which is 0xFFF in hex
    if (PARAM2.value > 15) {
        state = INVALID_REG;
        return;
    }
    
    if (PARAM3.value > 15) {
        state = INVALID_REG;
        return;
    }
    
    // Set the conditional code
    // setBits_num(start_bit, number, number_width)
    setBits_num(31, cond2num(COND), 4);

    // Set the op code
    // setBits_str(start_bit, string)
    //  any x in string will be skipped
    setBits_str(27, "0000000xxxxx0000xxxx1001");

    // Set S bit
    setBits_num(20, S_FLAG, 1);

    // set Rd
    setBits_num(19, PARAM1.value, 4);

    // set Rm
    setBits_num(11, PARAM2.value, 4);

    setBits_num(3, PARAM3.value, 4);

    
    // tell the system the encoding is done
    state = COMPLETE_ENCODE;
}

void mul_reg_bin(void) { 
    // Check if the op code bits match
    // check_bits(start_bit, bit_string) returns 0 if the bit_string matches
    //  any x will be skipped
    // If the manual shows (0), then the value of that bit doesnt matter
    if (checkBits(27, "0000000xxxxx0000xxxx1001") != 0) {
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
    uint32_t Rd = getBits(19, 4);
    uint32_t Rm = getBits(11, 4);
    uint32_t Rn = getBits(3, 4);
    S_FLAG = getBits(20, 1); // Directly setting the S flag

    /*
        Setting Instuciton values
    */

    setOp("MUL");
    setCond_num(cond);
    // setParam(param_num, param_type, param_value)
    setParam(1, REGISTER, Rd);
    setParam(2, REGISTER, Rn);
    setParam(3, REGISTER, Rm);

    // tell the system the decoding is done
    state = COMPLETE_DECODE;
}



