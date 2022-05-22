#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include "Instruction.h"
#include "Registers.h"
#include "Core.h"
namespace MIPSLayer {
	typedef int ErrorFlag;

	enum ErrorFlag_ {
		Error_None = 0,
		Segment_Start = BIT(1),
		Segment_Duplicate = BIT(2),
		Instruction_Start = BIT(3),
		Incompleted_Label = BIT(4),
		Label_Duplicate= BIT(5),
		Numeric_Value = BIT(6),
		Register_Value = BIT(7),
		Insufficient_Instruction = BIT(8),
		DataMemory_Too_Many_Args = BIT(9),
		DataMemory_Too_Few_Args = BIT(10),
		DataMemory_Invalid_Arg = BIT(11),

	};
	
	class MIPS { // Singleton
	public: 
		struct ExecutionTable { // Focus on that
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
		
		static std::string ValidateInput(const std::string& data, const std::string& dataMem,int callReason, bool& labelCheck) {
			return Get().IValidateInput(data, dataMem, callReason, labelCheck);
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
		static void ResetCResultMap() {
			return Get().IResetCResultMap();
		}

		static void ResetLabelUMap() {
			return Get().IResetLabelUmap();
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
			InitInstructionMap();
			InitRegistersMap();
			m_SegmentStart = false;
		}
		
		void InitRegistersMap();
		void InitInstructionMap();
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
		void IResetCResultMap() {
			m_CResultMap.clear();
		}
		void IResetLabelUmap() {
			m_LabelUMap.clear();
		}
		std::vector <ExecutionTable>& IGet_ExecutionTable() {
			return m_ExecutionTable;
		}
		std::string IValidateInput(const std::string& data , const std::string& dataMem, int callReason, bool &labelCheck);
		void IValidateLabel(const std::string& data,int p_CurrentLine, ErrorFlag& errorFlag);
		void IValidateInstructions(std::vector <std::string>& data,int p_CurrentLine, ErrorFlag& errorFlag);
		void IExecute(const std::string& textMem, const std::string& dataMem);
		bool ILabelInsert(const std::string& label, int address);
		bool ILabelCheck(const std::string& label);
		std::string IConstructCResult();
	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
		std::unordered_map<std::string, int> m_LabelUMap; // Label Name -> Relevant Address 
		std::map<int, std::string> m_CResultMap;
		std::vector<std::string> m_UnregisteredLabels;
		std::vector<ExecutionTable> m_ExecutionTable;
		bool m_SegmentStart;
	};
	
	std::map <int,std::string> DataMemoryHandler(const std::string & text);
}