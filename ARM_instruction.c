#include "ARM_instruction.h"

Assm_Instruct assm_instruct;
uint32_t instruct;
uint16_t state;

/*----------------------------\
	   assembly_instructs
\----------------------------*/
// array containing all of the _assm functions
void (*assembly_instructs[])(void) = {
	// function names are placed below in a comma seperated list

	// immediate instructions
	add_immd_assm,
	and_immd_assm,
	cmp_immd_assm,
	eor_immd_assm,
	lsl_immd_assm,
	lsr_immd_assm,
	mov_immd_assm,
	mvn_immd_assm,
	orn_immd_assm,
	orr_immd_assm,
	ror_immd_assm,
	rsb_immd_assm,
	sub_immd_assm,

	// register functions
	add_reg_assm,
	and_reg_assm,
	cmp_reg_assm,
	eor_reg_assm,
	lsl_reg_assm,
	lsr_reg_assm,
	mla_reg_assm,
	mls_reg_assm,
	mov_reg_assm,
	mul_reg_assm,
	mvn_reg_assm,
	orn_reg_assm,
	orr_reg_assm,
	ror_reg_assm,
	rsb_reg_assm,
	sub_reg_assm,

	end_list
};


// array containing all of the _bin functions for 32 bit instructions
void (*binary_instructs[])(void) = {
	// function names are placed below in a comma seperated list
	
	// immediate instructions
	add_immd_bin,
	and_immd_bin,
	cmp_immd_bin,
	eor_immd_bin,
	lsl_immd_bin,
	lsr_immd_bin,
	mov_immd_bin,
	mvn_immd_bin,
	orn_immd_bin,
	orr_immd_bin,
	ror_immd_bin,
	rsb_immd_bin,
	sub_immd_bin,

	// register functions
	add_reg_bin,
	and_reg_bin,
	cmp_reg_bin,
	eor_reg_bin,
	lsl_reg_bin,
	lsr_reg_bin,
	mla_reg_bin,
	mls_reg_bin,
	mov_reg_bin,
	mul_reg_bin,
	mvn_reg_bin,
	orn_reg_bin,
	orr_reg_bin,
	ror_reg_bin,
	rsb_reg_bin,
	sub_reg_bin,
	end_list
};


/*
	Purpose: sets the global instrucion variables to the defualt values
	Params: none
	Return: none
*/
void initInstructs(void) {
	// clears any errors
	state = NO_ERROR;

	// sets the binary instruction to 0
	BIN32 = 0x00;

	// clears the op code
	memset(OP_CODE, '\0', OP_SIZE + 1);

	memset(COND, '\0', COND_SIZE + 1);

	S_FLAG = 0;

	// clears all parameters
	PARAM1.type = EMPTY;
	PARAM2.type = EMPTY;
	PARAM3.type = EMPTY;
	PARAM4.type = EMPTY;

	SHIFT = NONE;
}


/*
	Purpose: runs through each _assm function and tries to encode the parsed instruction
	Params: none
	Return: none
*/
void encode(void) {
	// clears any errors
	state = NO_ERROR;

	// loops through all of the instruction functions
	for (int i = 0; ((state == NO_ERROR) || (state == WRONG_COMMAND)); i++) {
		(*assembly_instructs[i])();
	}
}

/*
	Purpose: runs through each _bin function and tries to decode the parsed bits
	Params: none
	Return: none
*/
void decode(void) {
	// clears any errors
	state = NO_ERROR;

	// loops through each instruction function
	for (int i = 0; ((state == NO_ERROR) || (state == WRONG_COMMAND)); i++) {
		(*binary_instructs[i])();
	}
}

/*
	Purpose: serves as an indicator for the end of the instruction list
	Params: none
	Return: none
*/
void end_list(void) {
	state = UNRECOGNIZED_COMMAND;
}


/*----------------------------\
			Errors
\----------------------------*/
/*
	Purpose: prints formated error messaged
	Params: char* msg -- message to print
	Return: none
*/
void error(char* msg) {
	printf("ERROR: %s\n", msg);
}


/*----------------------------\
		   Printing
\----------------------------*/
/*
	Purpose: prints a message based on the system status
	Params: none
	Return: none
*/
void printResult(void) {
	// checks the current state and prints a corresponding message
	switch (state) {
	case NO_ERROR: {
		puts("System is Error Free");
		break;
	}
	case COMPLETE_ENCODE: {
		printMachine();
		break;
	}
	case COMPLETE_DECODE: {
		printAssm();
		break;
	}
	case UNRECOGNIZED_COMMAND: {
		error("The given instruction was not recognized");
		break;
	}
	case UNRECOGNIZED_COND: {
		error("The given conditional is not recognized");
		break;
	}
	case MISSING_REG: {
		error("Missing register parameter");
		break;
	}
	case INVALID_REG: {
		error("The given register is invalid for the specified command");
		break;
	}
	case MISSING_PARAM: {
		error("Expected a param, none was found");
		break;
	}
	case INVALID_PARAM: {
		error("The given parameter is invalid for the specified command");
		break;
	}
	case UNEXPECTED_PARAM: {
		error("Found a parameter when none was expected");
		break;
	}
	case INVALID_IMMED: {
		error("The given immediate value is invalid for the specified command");
		break;
	}
	case MISSING_SPACE: {
		error("Expected a space, none was found");
		break;
	}
	case MISSING_COMMA: {
		error("Expected a comma, none was found");
		break;
	}
	case INVALID_SHIFT: {
		error("The given shift is invalid");
		break;
	}
	case MISSING_SHIFT: {
		error("Expected a shift value but none was found");
		break;
	}
	case UNDEF_ERROR:
	default: {
		error("An unknown error code has occured");
		break;
	}
	}
}

/*
	Purpose: prints the text instruction
	Params: none
	Return: none
*/
void printAssm(void) {
	// prints the op code
	printf("%s", assm_instruct.op);

	if (startswith(COND, "AL") != 1) {
		printf("%s", COND);
	}

	if (S_FLAG == 1) {
		printf("S");
	}

	printf(" ");

	// checks param 1 and prints if it isn't empty
	if (PARAM1.type != EMPTY) {
		printParam(&PARAM1);
	}

	// checks param 2 and prints if it isn't empty
	if (PARAM2.type != EMPTY) {
		printf(", ");
		printParam(&PARAM2);
	}


	if ((SHIFT != NONE) && (PARAM3.type == EMPTY)) {
		printf(", ");
		printShift();
	}

	// checks param 3 and prints if it isn't empty
	if (PARAM3.type != EMPTY) {
		printf(", ");
		if ((SHIFT != NONE) && (PARAM4.type == EMPTY)) {
			printShift();
		}
		printParam(&PARAM3);
	}

	// checks param 4 and prints if it isn't empty
	if (PARAM4.type != EMPTY) {
		printf(", ");
		if (SHIFT != NONE) {
			printShift();
		}
		printParam(&PARAM4);
	}

	// prints the new line
	printf("\n");
}

/*
	Purpose: prints a parameter
	Params: Param* param - the parameter to print
	Return: none
*/
void printParam(struct Param* param) {
	// checks the type of parameter and prints accordingly
	switch (param->type) {
	case EMPTY: {
		printf("<>");
	}
	case REGISTER: {
		printf("R%d", param->value);
		break;
	}
	case IMMEDIATE: {
		printf("#0x%X", param->value);
		break;
	}
	default: {
		printf("<unknown: %d, %d>", param->type, param->value);
		break;
	}
	}
}

void printShift() {
	switch (SHIFT) {
	case LSL: {
		printf("LSL ");
		break;
	}
	case LSR: {
		printf("LSR ");
		break;
	}
	case ASR: {
		printf("ASR ");
		break;
	}
	case ROR: {
		printf("ROR ");
		break;
	}
	case RRX: {
		printf("RRX");
		break;
	}
	}
}

/*
	Purpose: prints thebinary instruction
	Params: none
	Return: none
*/
void printMachine(void) {

	printf("Hex: 0x%08X\tBinary:", instruct);

	// prints the binary instrution in binary form
	for (int i = 31; i > -1; i--) {
		printf("%d", (BIN32 >> i) & 1);

		if (i % 4 == 0) {
			printf(" ");
		}
	}

	printf("\n");
}


/*----------------------------\
		   Parsing
\----------------------------*/
/*
	reads the assembly instruction into the instruction struct
*/
void parseAssem(char* line) {
	// checks that parameters are valid
	if (line == NULL || strlen(line) == 0) {
		state = UNDEF_ERROR;
		return;
	}

	// clears instruction values
	initInstructs();

	// reads op code into the instruction op code
	if (startswith(line, "ADD") == 1) { setOp("ADD"); }
	else if (startswith(line, "AND") == 1) { setOp("AND"); }
	else if (startswith(line, "CMP") == 1) { setOp("CMP"); }
	else if (startswith(line, "EOR") == 1) { setOp("EOR"); }
	else if (startswith(line, "LSL") == 1) { setOp("LSL"); }
	else if (startswith(line, "LSR") == 1) { setOp("LSR"); }
	else if (startswith(line, "MLA") == 1) { setOp("MLA"); }
	else if (startswith(line, "MLS") == 1) { setOp("MLS"); }
	else if (startswith(line, "MOV") == 1) { setOp("MOV"); }
	else if (startswith(line, "MUL") == 1) { setOp("MUL"); }
	else if (startswith(line, "MVN") == 1) { setOp("MVN"); }
	else if (startswith(line, "ORN") == 1) { setOp("ORN"); }
	else if (startswith(line, "ORR") == 1) { setOp("ORR"); }
	else if (startswith(line, "ROR") == 1) { setOp("ROR"); }
	else if (startswith(line, "RSB") == 1) { setOp("RSB"); }
	else if (startswith(line, "SUB") == 1) { setOp("SUB"); }
	else { state = UNRECOGNIZED_COMMAND; return; }

	line += 3;

	// checks for sflag
	if ((*line == 's') || (*line == 'S')) {
		S_FLAG = 1;
		line++;
	}

	// checks for condition
	if (startswith(line, "EQ") == 1) { setCond_str("EQ"); }
	else if (startswith(line, "NE") == 1) { setCond_str("NE"); }
	else if (startswith(line, "CS") == 1) { setCond_str("CS"); }
	else if (startswith(line, "CC") == 1) { setCond_str("CC"); }
	else if (startswith(line, "MI") == 1) { setCond_str("MI"); }
	else if (startswith(line, "PL") == 1) { setCond_str("PL"); }
	else if (startswith(line, "VS") == 1) { setCond_str("VS"); }
	else if (startswith(line, "VC") == 1) { setCond_str("VC"); }
	else if (startswith(line, "HI") == 1) { setCond_str("HI"); }
	else if (startswith(line, "LS") == 1) { setCond_str("LS"); }
	else if (startswith(line, "GE") == 1) { setCond_str("GE"); }
	else if (startswith(line, "LT") == 1) { setCond_str("LT"); }
	else if (startswith(line, "GT") == 1) { setCond_str("GT"); }
	else if (startswith(line, "LE") == 1) { setCond_str("LE"); }
	else if (startswith(line, "AL") == 1) { setCond_str("AL"); }
	else if (startswith(line, " ") == 1) { setCond_str("AL"); }
	else { state = UNRECOGNIZED_COND; }

	if (*line != ' ') {
		line += 2;
	}

	// checks for sflag, incase its at the end
	if ((S_FLAG != 1) && ((*line == 's') || (*line == 'S'))) {
		S_FLAG = 1;
		line++;
	}

	if (*line != ' ') {
		state = MISSING_SPACE;
		return;
	}


	// eat any whitespace
	while (*line == ' ') { line++; }

	/*
		Param 1
	*/

	// tries to read a parameter
	line = readParam(line, &PARAM1);

	// checks if there was an error or if the line is empty
	if ((state != NO_ERROR) || (*line == '\0')) {
		return;
	}

	/*
		Param 2
	*/

	// tries to read a parameter
	line = readParam(line, &PARAM2);

	// checks if there was an error or if the line is empty
	if ((state != NO_ERROR) || (*line == '\0')) {
		return;
	}

	/*
		Param 3, might have shift before
	*/

	// tries to read a shift
	line = readShift(line);

	// checks if there was an error
	if (state != NO_ERROR) {
		return;
	}

	// if there is anything after a RRX shift, the assembly is invalid
	if ((SHIFT == RRX) && (*line != '\0')) {
		state = INVALID_SHIFT;
		return;
	}
	else if (SHIFT == RRX) {
		setParam(3, IMMEDIATE, 0);
	}

	// if there is nothing after a shift specifier, then the value is missing
	if ((SHIFT != NONE) && (*line == '\0')) {
		state = MISSING_SHIFT;
		return;
	}

	// tries to read a parameter
	line = readParam(line, &PARAM3);

	// checks if there was an error or if the line is empty
	if ((state != NO_ERROR) || (*line == '\0')) {
		return;
	}

	// if there was a shift, there can't be anything after
	if ((SHIFT != NONE) && (*line != '\0')) {
		state = UNEXPECTED_PARAM;
		return;
	}

	/*
		Param 4, might have shift before
	*/

	// tries to read a shift
	line = readShift(line);

	// checks if there was an error
	if (state != NO_ERROR) {
		return;
	}

	// if there is anything after a RRX shift, the assembly is invalid
	if ((SHIFT == RRX) && (*line != '\0')) {
		state = INVALID_SHIFT;
		return;
	}
	else if (SHIFT == RRX) {
		setParam(3, IMMEDIATE, 0);
	}

	// if there is nothing after a shift specifier, then the value is missing
	if ((SHIFT != NONE) && (*line == '\0')) {
		state = MISSING_SHIFT;
		return;
	}

	// tries to read a parameter
	line = readParam(line, &PARAM4);

}


/*
	Purpose: reads a parameter from a given line
	Params: char* line - the line to read
			Param* param - the parameter to fill
	Return: char* - the ptr to after the param
*/
char* readParam(char* line, struct Param* param) {
	// eat any whitespace
	while (*line == ' ') { line++; }

	uint8_t comma_flag;

	// checks for a comma at the front, eats it if present
	if (*line != ',') {
		comma_flag = 0;
	}
	else {
		line++;
		comma_flag = 1;
	}

	// eat any whitespace
	while (*line == ' ') { line++; }

	if (*line == '\0') {
		state = MISSING_PARAM;
		return NULL;
	}

	// check parameter type and save value
	if (toupper(*line) == 'R') {
		line++;

		param->type = REGISTER;
		line = reg2num(line, &param->value);
	}
	else {
		if (*line == '#' || *line == '$') {
			line++;
		}

		param->type = IMMEDIATE;
		line = immd2num(line, &param->value);
	}

	// eat any whitespace
	while (*line == ' ') { line++; }

	// check for comma if there wasn't one at the beginning
	if ((comma_flag == 0) && (SHIFT == NONE)) {
		if (*line != ',') {
			state = MISSING_COMMA;
		}
	}


	// updated line ptr
	return line;
}

/*
	Purpose: reads a parameter from a given line
	Params: char* line - the line to read
			Param* param - the parameter to fill
	Return: char* - the ptr to after the param
*/
char* readShift(char* line) {
	// saves line, incase there is no shift
	char* temp = line;

	// eat any whitespace
	while (*line == ' ') { line++; }

	uint8_t comma_flag;

	// checks for a comma at the front, eats it if present
	if (*line != ',') {
		comma_flag = 0;
	}
	else {
		line++;
		comma_flag = 1;
	}

	// eat any whitespace
	while (*line == ' ') { line++; }

	// checks if its a register or imediate
	if (toupper(*line) == 'R') {
		// if there is a digit, it can't be a shift
		if (isdigit(*(line + 1)) != 0) {
			return temp;
		}
	}
	else {
		if (*line == '#' || *line == '$') {
			return temp;
		}
	}

	// checks for a valid shift
	if (startswith(line, "LSL") == 1) { SHIFT = LSL; }
	else if (startswith(line, "LSR") == 1) { SHIFT = LSR; }
	else if (startswith(line, "ASR") == 1) { SHIFT = ASR; }
	else if (startswith(line, "ROR") == 1) { SHIFT = ROR; }
	else if (startswith(line, "RRX") == 1) { SHIFT = RRX; }
	else { state = INVALID_SHIFT; return NULL; }

	line += 3;

	// eat any whitespace
	while (*line == ' ') { line++; }

	// check for comma if there wasn't one at the beginning
	if (comma_flag == 0) {
		if (*line != ',') {
			state = MISSING_COMMA;
		}
	}

	// updated line ptr
	return line;
}

/*
	Purpose: converts a string into a decimal number
	Params: char* line - the string to convert
			uint32_t* value - the number to fill
	Return: char* - the ptr to after the number
*/
char* reg2num(char* line, uint32_t* value) {
	uint32_t num = 0;

	// as long as there are valid digits, they are added to the running number
	while (isdigit(*line)) {
		num = (num * 10) + (*line - 48);
		line++;
	}

	// sets the gven number ptr
	*value = num;

	// updates line ptr
	return line;
}


/*
	Purpose: converts a string into either a hex or decimal number
	Params: char* line - the string to convert
			uint32_t* value - the number to fill
	Return: char* - the ptr to after the number
*/
char* immd2num(char* line, uint32_t* value) {
	uint32_t num = 0;

	// checks if the number is prefixed for hex
	if ((*line == '0') && (toupper(*(line + 1)) == 'X')) {
		line += 2;

		// as long as there are valid digits, they are added to the running number
		while (isxdigit(*line)) {
			if (isdigit(*line)) {
				num = (num * 16) + (*line - 48);
			}
			else {
				num = (num * 16) + (toupper(*line) - 55);
			}

			line++;
		}
	}
	else {
		// as long as there are valid digits, they are added to the running number
		while (isdigit(*line)) {
			num = (num * 10) + (*line - 48);
			line++;
		}
	}

	// sets the gven number ptr
	*value = num;

	// updates line ptr
	return line;
}


/*
	Purpose: parses the given hex line into into the binary instruction
	Params: char* line - the line to convert
	Return: none
*/
void parseHex(char* line) {
	// checks that parameters are valid
	if (line == NULL || strlen(line) == 0) {
		state = UNDEF_ERROR;
		return;
	}

	// clears instruction values
	initInstructs();

	uint32_t num = 0;

	// checks if there is a hex prefix
	if ((*line == '0') && (toupper(*(line + 1)) == 'X')) {
		line += 2;
	}

	// as long as there are valid digits, they are added to the running number
	while (isxdigit(*line)) {
		if (isdigit(*line)) {
			num = (num * 16) + (*line - 48);
		}
		else {
			num = (num * 16) + (toupper(*line) - 55);
		}

		line++;
	}

	// sets the binary instruction to the converted number
	BIN32 = num;
}


/*
	Purpose: parses the given binary line into into the binary instruction
	Params: char* line - the line to convert
	Return: none
*/
void parseBin(char* line) {
	// checks that parameters are valid
	if (line == NULL || strlen(line) == 0) {
		state = UNDEF_ERROR;
		return;
	}

	// clears instruction values
	initInstructs();

	uint32_t num = 0;

	// checks if there is a binary prefix
	if ((*line == '0') && (toupper(*(line + 1)) == 'b')) {
		line += 2;
	}

	// as long as there are valid digits, they are added to the running number
	while (*line != '\0') {
		if ((*line == '0') || (*line == '1')) {
			num = (num * 2) + (*line - 48);
		}
		line++;
	}

	// sets the binary instruction to the converted number
	BIN32 = num;
}


/*----------------------------\
		   Set Bits
\----------------------------*/
/*
	Purpose: sets bits in the binary instruction given a number and a number of bits
	Params: uint32_t start - the bit to start at
			uint32_t num - the number to use as a source of bits
			uint32_t size - the number of bits to set
	Return: none
*/
void setBits_num(uint32_t start, uint32_t num, uint32_t size) {
	// temp string for conversion purposes
	char str[40] = { '\0' };

	// converts the number to a string of the needed size
	num2bin(str, num, size);

	// sets the bits in the binary instruction
	setBits_str(start, str);
}


/*
	Purpose: sets bits in the binary instruction given a string
	Params: uint32_t start - the bit to start at
			const char* str - the string to use as a source of bits, skips any x or X characters
	Return: none
*/
void setBits_str(uint32_t start, const char* str) {
	// loops through the sting ad sets the bits in the binary instruction
	for (int i = 0; i < strlen(str); i++) {
		if ((str[i] == '0') || (str[i] == '1')) {
			BIN32 |= (str[i] - 48) << (start - i);
		}
	}
}


/*----------------------------\
		  Check Bits
\----------------------------*/
/*
	Purpose: checks if the given series of bits is in the binary instruction
	Params: uint32_t start - the bit to start at
			const char* str - the series of bits to look for, skips any x or X characters
	Return: int - 0 for present, 1 for not present
*/
int checkBits(uint32_t start, const char* str) {
	// loops thorugh and checks each bit in the bianry instruction
	for (int i = 0; i < strlen(str); i++) {
		// skips anything that isn't a 1 or 0
		if ((str[i] != '0') && (str[i] != '1')) {
			continue;
		}

		if ((str[i] - 48) != ((BIN32 >> (start - i)) & 0x1)) {
			return 1;
		}
	}

	// no error
	return 0;
}


/*----------------------------\
		   Get Bits
\----------------------------*/
/*
	Purpose: gets a group of bits from the binary instruction
	Params: uint32_t start - the bit to strt grabbing from
			uint32_t the number of bits to grab
	Return: int - the number represented from the bits
*/
uint32_t getBits(uint32_t start, uint32_t size) {
	uint32_t num = 0;

	// finds the last bit to grab, non-inclusive
	int end = start - size;

	// loops through each bit and adds it to the running num
	for (int i = start; i > end; i--) {
		num = (num * 2) + ((BIN32 >> i) & 0x1);
	}

	// returns the extracted number
	return num;
}


/*----------------------------\
	 Set Instruction Parts
\----------------------------*/
/*
	Purpose: sets the specified parameter to the specified type and value
	Params: uint32_t param_num - which parameter to change
			Param_Type type - what type to set the parameter to
			uint32_t value - what value to set the parameter to
	Return: 0 for no error
*/
int setParam(uint32_t param_num, Param_Type type, uint32_t value) {
	// finds which parameter is being set and set the type and value
	switch (param_num) {
	case 1: {
		PARAM1.type = type;
		PARAM1.value = value;
		break;
	}
	case 2: {
		PARAM2.type = type;
		PARAM2.value = value;
		break;
	}
	case 3: {
		PARAM3.type = type;
		PARAM3.value = value;
		break;
	}
	case 4: {
		PARAM4.type = type;
		PARAM4.value = value;
		break;
	}
	default: {
		return 1;
		break;
	}
	}

	// no error
	return 0;
}

/*
	Purpose: sets the opcode field in the instruction
	Params: char* opCode - string to set the op code to
	Return: none
*/
void setOp(char* opCode) {
	for (int i = 0; i < OP_SIZE; i++) {
		OP_CODE[i] = *opCode++;

		if (*opCode == '\0') {
			OP_CODE[i + 1] = '\0';
			break;
		}
	}
}

/*
	Purpose: sets the condition
	Params: char* cond - string to set the condition to
	Return: none
*/
void setCond_str(char* cond) {
	for (int i = 0; i < COND_SIZE; i++) {
		COND[i] = *cond++;

		if (*cond == '\0') {
			COND[i + 1] = '\0';
			break;
		}
	}
}

/*
	Purpose: sets the condition
	Params: uint32_t num - bit code to set the condition to
	Return: none
*/
void setCond_num(uint32_t num) {
	if (num == 0000) { setCond_str("EQ"); }
	else if (num == 0001) { setCond_str("NE"); }
	else if (num == 0010) { setCond_str("CS"); }
	else if (num == 0011) { setCond_str("CC"); }
	else if (num == 0100) { setCond_str("MI"); }
	else if (num == 0101) { setCond_str("PL"); }
	else if (num == 0110) { setCond_str("VS"); }
	else if (num == 0111) { setCond_str("VC"); }
	else if (num == 1000) { setCond_str("HI"); }
	else if (num == 1001) { setCond_str("LS"); }
	else if (num == 1010) { setCond_str("GE"); }
	else if (num == 1011) { setCond_str("LT"); }
	else if (num == 1100) { setCond_str("GT"); }
	else if (num == 1101) { setCond_str("LE"); }
	else if (num == 1110) { setCond_str("AL"); }
}


/*----------------------------\
			 Misc
\----------------------------*/
/*
	Purpose: fills a string with a binary version of a number, and insures a minimum length
	Params: char* str - the string to fill, uint32_t num - the number to convert, uint32_t size - the minimum size
	Return: char* - the same as the str pointer
*/
char* num2bin(char* str, uint32_t num, uint32_t size) {
	int index = 0;

	// extracts the LSB until the numebr is zero
	while (num > 0) {
		str[index++] = (num % 2) + 48;
		num /= 2;
	}

	// makes sure that the string is terminated
	str[index] = '\0';

	// zeros to fill to the minimum size
	while (strlen(str) < size) {
		str[index++] = '0';
		str[index] = '\0';
	}

	// swaps the bits to reverse the string
	index--;
	for (int i = 0; i < index; i++, index--) {
		char c = str[index];
		str[index] = str[i];
		str[i] = c;
	}

	// returns the str ptr
	return str;
}

/*
	Purpose: converts a condition string to number
	Params: char* cond - the condition to convert
	Return: bit code for the condition
*/
int cond2num(char* cond) {
	if (strcmp(cond, "EQ") == 0) { return 0000; }
	else if (strcmp(cond, "NE") == 0) { return 0b0001; }
	else if (strcmp(cond, "CS") == 0) { return 0b0010; }
	else if (strcmp(cond, "CC") == 0) { return 0b0011; }
	else if (strcmp(cond, "MI") == 0) { return 0b0100; }
	else if (strcmp(cond, "PL") == 0) { return 0b0101; }
	else if (strcmp(cond, "VS") == 0) { return 0b0110; }
	else if (strcmp(cond, "VC") == 0) { return 0b0111; }
	else if (strcmp(cond, "HI") == 0) { return 0b1000; }
	else if (strcmp(cond, "LS") == 0) { return 0b1001; }
	else if (strcmp(cond, "GE") == 0) { return 0b1010; }
	else if (strcmp(cond, "LT") == 0) { return 0b1011; }
	else if (strcmp(cond, "GT") == 0) { return 0b1100; }
	else if (strcmp(cond, "LE") == 0) { return 0b1101; }
	else { return 0b1110; }
}

/*
	Purpose: converts a shift type to number
	Params: Shift_Type shift - the shift to convert
	Return: bit code for the shift
*/
int shift2num(Shift_Type shift) {
	switch(shift) {
		case LSL: {
			return 0b00;
		}
		case LSR: {
			return 0b01;
		}
		case ASR: {
			return 0b10;
		}
		case ROR: {
			return 0b11;
		}
		case RRX: {
			return 0b11;
		}
		default: {
			return 0b00;
		}
	}
}

/*
	Purpose: converts a number to a shift type
	Params: int shift_code - the number to convert
	Return: Shift_Type for the code
*/
Shift_Type num2shift(int shift_code) {
    switch (shift_code & 0b11) {
    case 0b00: {
        return LSL;
    }
    case 0b01: {
        return LSR;
    }
    case 0b10: {
        return ASR;
    }
    case 0b11: {
        return ROR;
    }
    }

    return NONE;
}
/*
	Purpose: checks if the given prefix is at the beginning of the given string
	Params: char* line - the string to search in
			char* prefix - the string to search for
	Return: 0 if they don't match, 1 if they do
*/
int startswith(char* line, char* prefix) {
	while (*prefix != '\0') {
		char c1 = *line++;
		char c2 = *prefix++;

		if ((c1 <= 'z') && (c1 >= 'a')) {
			c1 -= 32;
		}

		if ((c2 <= 'z') && (c2 >= 'a')) {
			c2 -= 32;
		}

		if (c2 != c1) {
			return 0;
		}
	}
	return 1;
}

