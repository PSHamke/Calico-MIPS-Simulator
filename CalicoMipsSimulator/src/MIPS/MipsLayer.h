#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include "Instruction.h"
#include "Registers.h"
namespace MIPSLayer {
	
	
	class MIPS { // Singleton
	public: 
		struct ExecutionTable {
			int address;
			std::string instruction;
			std::vector<std::reference_wrapper<int>> datas;
			std::vector<int> registerNames;
		};
		MIPS(const MIPS&) = delete;
		
		static MIPS& Get() {
			static MIPS instance;
			return instance;
		}
		
		static std::string TranslateToC(const std::string& data, const std::string& dataMem,int callReason) {
			return Get().ITranslateToC(data, dataMem, callReason);
		}
		static void Execute(const std::string& textMem, const std::string& dataMem) {
			return Get().IExecute(textMem, dataMem);
		}
		
		static std::unordered_map<std::string, Instruction*>& GetInstructionUMap() {
			return Get().IGet_InstructionUMap();
		}
		static std::unordered_map<std::string, Register*>& GetRegisterUMap() {
			return Get().IGet_RegisterUMap();
		}
		static void ResetRegisterUMap() {
			return Get().IResetRegisterUMap();
		}
		
		static std::vector<ExecutionTable>& GetExecutionTable() {
			return Get().IGet_ExecutionTable();
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
		void IResetRegisterUMap() {
			for (auto i = m_RegisterUMap.begin(); i != m_RegisterUMap.end(); i++) {
				i->second->SetValue(0);
			}
		}
		void IResetExecutionTable() {
			m_ExecutionTable.clear();
		}
		std::vector <ExecutionTable>& IGet_ExecutionTable() {
			return m_ExecutionTable;
		}
		std::string ITranslateToC(const std::string&  data , const std::string& dataMem, int callReason);
		void IExecute(const std::string& textMem, const std::string& dataMem);
	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
		std::unordered_map<int, std::string> m_Labels;
		std::vector<ExecutionTable> m_ExecutionTable;
	};
	
	std::map <int,std::string> DataMemoryHandler(const std::string & text);
}