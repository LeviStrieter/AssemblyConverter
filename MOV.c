#include "Instruction.h"

void mov_immd_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "MOV") != 0) {
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
	if (PARAM2.type != IMMEDIATE) {
		state = WRONG_COMMAND;
		return;
	}

	// This parameter should be empty
	if ((PARAM3.type != EMPTY) || (PARAM4.type != EMPTY)) {
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
	if (PARAM2.value > 0xFFF) {
		state = INVALID_IMMED;
		return;
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
	setBits_str(27, "0011101x0000");

	// Set S bit
	setBits_num(20, S_FLAG, 1);

	// set Rd
	setBits_num(15, PARAM1.value, 4);

	// set immediate
	setBits_num(11, PARAM2.value, 12);

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void mov_immd_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	if (checkBits(27, "0011101xxxxxxxxx") != 0) {
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
	uint32_t imm12 = getBits(11, 12);

	/*
		Setting Instuciton values
	*/

	setOp("MOV");
	setCond_num(cond);
	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd);
	setParam(2, IMMEDIATE, imm12);

	// tell the system the decoding is done
	state = COMPLETE_DECODE;
}

void mov_reg_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "MOV") != 0) {
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
		state = WRONG_COMMAND;
		return;
	}


	// This parameter should be empty
	if ((PARAM3.type != EMPTY) || (PARAM4.type != EMPTY)) {
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

	// Rm should be 15 or less
	if (PARAM2.value > 15) {
		state = INVALID_REG;
		return;
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
	// This can also be slpit into multiple function calls
	setBits_str(27, "0001101x0000xxxx00000000");

	// Set S bit
	setBits_num(20, S_FLAG, 1);

	// set Rd
	setBits_num(15, PARAM1.value, 4);

	// set immediate
	setBits_num(3, PARAM2.value, 4);

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void mov_reg_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	if (checkBits(27, "0001101xxxxxxxxx00000000") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the op code bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/
	// getBits(start_bit, width)
	uint32_t cond = getBits(31, 4);
	S_FLAG = getBits(20, 1); // Directly settign the S flag
	uint32_t Rd = getBits(15, 4);
	uint32_t Rm = getBits(3, 4);

	/*
		Setting Instuciton values
	*/

	setOp("MOV");
	setCond_num(cond);
	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd);
	setParam(2, REGISTER, Rm);

	// tell the system the decoding is done
	state = COMPLETE_DECODE;
}
