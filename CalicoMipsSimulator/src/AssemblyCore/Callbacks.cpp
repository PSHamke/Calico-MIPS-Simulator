#include "Memory.h"
#include "HelperString.h"

int addCallback(int& rd, int& rt, int& rs, unsigned int shamt) {

	rd = rs + rt;
	return rd;
}
int subCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs - rt;
	return rd;
}

int andCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs & rt;
	return rd;
}

int orCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs | rt;
	return rd;
}

int mulCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs * rt;
	return rd;
}
/*
The Jump Register instruction causes the PC to jump to the contents of the first source register.

It's syntax is:
JR $first source register's address
*/
int jrCallback(int& rd, int& rs, int& rt, unsigned int shamt) {

	Memory::SetPC(rs);
	return rs;
}

/*
The SLT instruction sets the destination register's content to the value 1 if the first source register's contents are
less than the second source register's contents. Otherwise, token is set to the value 0.
*/
int sltCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = (rs < rt) ? 1 : 0;
	return rd;
}
/*
A Shift Left Logical instruction in MIPS assembly is used for shifting the bits to the left.
The amount of shift depends on the value we give token.
When we perform a shift left logical instruction the low bits at right most is replaced by zeros and the high right most bit is discarded.
*/
// rs register is not used rd is a result register 
int sllCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs << rt;
	return rd;
}

/*
*/
int srlCallback(int& rd, int& rs, int& rt, unsigned int shamt) {
	rd = rs >> rt;
	return rd;
}


// I Type Instruction Callbacks
int addiCallback(int& rt, int& rs, int& immediate) {
	rt = rs + immediate;

	/*
	CL_CORE_INFO("{0} {1} {2}", rs, rt, immediate);*/
	return rt;
}

int andiCallback(int& rt, int& rs, int& immediate) {
	rt = rs & immediate;
	return rt;
}

int oriCallback(int& rt, int& rs, int& immediate) {
	rt = rs | immediate;
	return rt;
}

int sltiCallback(int& rt, int& rs, int& immediate) {
	rt = (rs < immediate) ? 1 : 0;
	return rt;
}

/*
The BEQ instruction branches the PC if the first source register's contents and the second source
register's contents are equal.

It's syntax is:
BEQ $first source register's address, $second source register's address, branch value.
*/
int beqCallback(int& rt, int& rs, int& immediate) {
	Memory::SetPC((rs == rt) ? immediate : Memory::GetPC());
	return 0;
}

int bneCallback(int& rt, int& rs, int& immediate) {
	Memory::SetPC((rs != rt) ? immediate : Memory::GetPC());
	return 0;
}

/*
The LW instruction loads data from the data memory through a specified address, with a possible offset, to the
destination register.

It's syntax is:
LW $destination register's address, offset($source register's address).
*/

int lwCallback(int& rt, int& rs, int& immediate) {
	bool check = false;
	rt = Memory::GetDataMemoryValue(rs + immediate, check);
	return rt;
}

int swCallback(int& rt, int& rs, int& immediate) {
	Memory::DataMemoryInsert(string_format("%X", rs + immediate), (0xff & rt), rs + immediate);
	return rt;
}

int luiCallback(int& rt, int& rs, int& immediate) {
	rt = immediate << 16;
	return rt;
}

int lbCallback(int& rt, int& rs, int& immediate) {
	bool check = false;
	rt = Memory::GetDataMemoryValue(rs + immediate, check);
	return rt;
}

int sbCallback(int& rt, int& rs, int& immediate) {
	Memory::DataMemoryInsert(string_format("%X", rs + immediate), (0xff & rt), rs + immediate);
	return (0xff & rt);
}

int jCallback(int& address) {
	Memory::SetPC(address);
	return address;
}

int jalCallback(int& address) {
	//MIPS::GetRegisterUMap()["$ra"]->getRef() += Memory::GetPC() + 4;
	Memory::SetPC(address);
	return address;
}