#pragma once
#include <iostream>
#include <unordered_map>
#include "Instruction.h"
#include "Registers.h"
namespace MIPSLayer {
	

	class MIPS { // Singleton
	public: 
		
		MIPS(const MIPS&) = delete;
		
		static MIPS& Get() {
			static MIPS instance;
			return instance;
		}
		
		static std::string TranslateToC(const std::string& data) {
			return Get().ITranslateToC(data);
		}
		std::string Execute(std::string& const data);
		
		static std::unordered_map<std::string, Instruction*>& GetInstructionUMap() {
			return Get().IGet_InstructionUMap();
		}
		static std::unordered_map<std::string, Register*>& GetRegisterUMap() {
			return Get().IGet_RegisterUMap();
		}
		/*static std::vector<unsigned int>& GetTextMemory() {
			return Get().IGet_TextMemory();
		}
		static void IExecuteWrapper(std::string& instruction, std::vector<std::reference_wrapper<int>>& memory, unsigned int& PC) {
			Get().IGet_InstructionUMap()[instruction]->Execute(memory, PC);
		}*/
	private:
		
		MIPS() {
			initInstructionMap();
			initRegistersMap();
		}
		
		void initRegistersMap();
		void initInstructionMap();
		std::unordered_map<std::string, Instruction*>& IGet_InstructionUMap() {
			return m_InstructionUMap;
		}
		std::unordered_map<std::string, Register*>& IGet_RegisterUMap() {
			return m_RegisterUMap;
		}
		
		std::string ITranslateToC(const std::string&  data);
		
	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
		
	};
	
}