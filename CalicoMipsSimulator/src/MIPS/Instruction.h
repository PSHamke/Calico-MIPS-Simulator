#pragma once
#include <string.h>
#include <iostream>
#include <functional>
#include <vector>

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
	virtual int Execute(std::vector<std::reference_wrapper<int>>& memory, unsigned int& PC) = 0;
};



class InstructionFormatR : public Instruction {
public:
	InstructionFormatR(const std::string name, const unsigned int opcode, int funct, std::function<int(int&, int&, int&, int&)> f)
		: Instruction(name, InstructionType::R, opcode, funct, 3, 4), m_Functionality(std::move(f)) {}

private:

	std::function<int(int&, int&, int&, int&)> m_Functionality;

public:

	// rs rt rd shamt 
	int Execute(std::vector<std::reference_wrapper<int>>& memory, unsigned int& PC) override {
		return m_Functionality(memory[0], memory[1], memory[2], memory[3]);
	}
};

class InstructionFormatI : public Instruction {
public:
	InstructionFormatI(const std::string name, const unsigned int opcode, std::function<int(int&, int&, int&)> f)
		: Instruction(name, InstructionType::R, opcode, -1, 2, 3), m_Functionality(std::move(f)) {}

private:
	std::function<int(int&, int&, int&)> m_Functionality;
public:
	int Execute(std::vector<std::reference_wrapper<int>>& memory, unsigned int& PC) override {
		return m_Functionality(memory[0], memory[1], memory[2]);
	}
};

class InstructionFormatJ : public Instruction {
public:
	InstructionFormatJ(const std::string name, const unsigned int opcode, std::function<int(int&)> f)
		: Instruction(name, InstructionType::R, opcode, -1, 0, 1), m_Functionality(std::move(f)) {}

private:
	std::function<int(int&)> m_Functionality;
public:
	int Execute(std::vector<std::reference_wrapper<int>>& memory, unsigned int& PC) override {
		// process for PC
		return m_Functionality(memory[0]);
	}
};
