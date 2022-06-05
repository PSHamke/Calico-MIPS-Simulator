#pragma once
#include <string.h>
#include <iostream>
#include <functional>
#include <vector>
#include <Memory.h>
#include "Core.h"
typedef int ErrorFlag;

enum ErrorFlag_ {
	Error_None = 0,
	Segment_Start = BIT(1),
	Segment_Duplicate = BIT(2),
	Instruction_Start = BIT(3),
	Incompleted_Label = BIT(4),
	Label_Duplicate = BIT(5),
	Non_Exist_Label = BIT(6),
	Numeric_Value = BIT(7),
	Register_Value = BIT(8),
	Insufficient_Instruction = BIT(9),
	DataMemory_Too_Many_Args = BIT(10),
	DataMemory_Too_Few_Args = BIT(11),
	DataMemory_Invalid_Arg = BIT(12),

};

enum class InstructionType {
	R, I, J
};

class Instruction {
public:
	Instruction(const std::string name, const InstructionType type, const unsigned int opcode, const int funct,
		const unsigned int regCount, const unsigned int argCount)
		: m_Name(name), m_Type(type), m_Opcode(opcode), m_Funct(funct), m_RegisterCount(regCount), m_ArgumentCount(argCount) {}
	Instruction() {};
private:
	std::string m_Name;
	InstructionType m_Type;
	unsigned int m_Opcode;
	unsigned int m_RegisterCount;
	unsigned int m_ArgumentCount;
	unsigned int m_Funct;
public:
	std::string getName() {
		return m_Name;
	}
	InstructionType getType() {
		return m_Type;
	}
	unsigned int getOpcode() {
		return m_Opcode;
	}
	unsigned int getRegisterCount() {
		return m_RegisterCount;
	}
	unsigned int getArgumentCount() {
		return m_ArgumentCount;
	}
	unsigned int getFunct() {
		return m_Funct;
	}
	virtual int Execute(std::vector<std::reference_wrapper<int>>& Registers,int immediate,std::vector<int>& registerNumbers, unsigned int& PC) = 0;
};



class InstructionFormatR : public Instruction {
public:
	InstructionFormatR(const std::string name, const unsigned int opcode, int funct, std::function<int(int&, int&, int&, int&)> f)
		: Instruction(name, InstructionType::R, opcode, funct, 3, 4), m_Functionality(std::move(f)) {}

private:

	std::function<int(int&, int&, int&, int&)> m_Functionality;

public:

	// rs rt rd shamt 
	int Execute(std::vector<std::reference_wrapper<int>>& Registers, int immediate, std::vector<int>& registerNumbers, unsigned int& PC) override {
		int textMem = this->getOpcode() << 26 | registerNumbers[0] << 21 | registerNumbers[1] << 16 | registerNumbers[2] << 11 | immediate << 6 | this->getFunct(); // how memory handled
		if (Memory::GetCallingReason() == 0) {
			Memory::GetTextMemory().push_back(textMem);
		}
		Memory::SetPC(Memory::GetPC() + 1);
		return m_Functionality(Registers[0], Registers[1], Registers[2], Registers[3]);
	}
};

class InstructionFormatI : public Instruction {
public:
	InstructionFormatI(const std::string name, const unsigned int opcode, std::function<int(int&, int&, int&)> f)
		: Instruction(name, InstructionType::R, opcode, -1, 2, 3), m_Functionality(std::move(f)) {}

private:
	std::function<int(int&, int&, int&)> m_Functionality;
public:
	int Execute(std::vector<std::reference_wrapper<int>>& Registers, int immediate, std::vector<int>& registerNumbers, unsigned int& PC) override {
		unsigned int textMem;
		if (registerNumbers.size() == 2) {
			textMem= this->getOpcode() << 26 | registerNumbers[0] << 21 | registerNumbers[1] << 16 | immediate; // how memory handled
		}
		else {
			textMem = this->getOpcode() << 26 | registerNumbers[0] << 21  | immediate;
		}
		

		if (Memory::GetCallingReason() == 0) {
			Memory::GetTextMemory().push_back(textMem);
		} 
		Memory::SetPC(Memory::GetPC() + 1);
		return m_Functionality(Registers[0], Registers[1], immediate);
	}
};

class InstructionFormatJ : public Instruction {
public:
	InstructionFormatJ(const std::string name, const unsigned int opcode, std::function<int(int&)> f)
		: Instruction(name, InstructionType::R, opcode, -1, 0, 1), m_Functionality(std::move(f)) {}

private:
	std::function<int(int&)> m_Functionality;
public:
	int Execute(std::vector<std::reference_wrapper<int>>& Registers, int immediate, std::vector<int>& registerNumbers, unsigned int& PC) override {
		// process for PC
		int textMem = this->getOpcode() << 26 | immediate; //how memory handled
		if (Memory::GetCallingReason() == 0) {
			Memory::GetTextMemory().push_back(textMem);
		}
		return m_Functionality(immediate);
	}
};
