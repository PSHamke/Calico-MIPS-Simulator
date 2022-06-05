#pragma once
#include <unordered_map>
#include "AssemblyCore/Registers.h"
#include "AssemblyCore/Instruction.h"

namespace NobleLayer {
	class Noble {
	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
	};
}