#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include "AssemblyCore/Instruction.h"
#include "AssemblyCore/Registers.h"
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
		Non_Exist_Label= BIT(6),
		Numeric_Value = BIT(7),
		Register_Value = BIT(8),
		Insufficient_Instruction = BIT(9),
		DataMemory_Too_Many_Args = BIT(10),
		DataMemory_Too_Few_Args = BIT(11),
		DataMemory_Invalid_Arg = BIT(12),

	};
	
	class MIPS { // Singleton
	public: 
		struct ExecutionTable { // Focus on that
			int address;
			std::string instruction;
			std::vector<std::reference_wrapper<int>> datas;
			int immediate;
			std::vector<int> registerNames;
		};
		MIPS(const MIPS&) = delete;
		
		static MIPS& Get() {
			static MIPS instance;
			return instance;
		}
		
		static std::string ValidateInput(const std::string& data, const std::string& dataMem) {
			return Get().IValidateInput(data, dataMem);
		}
		static void Execute(int pStep) {
			return Get().IExecute(pStep);
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
		static std::unordered_map<std::string, int>& GetLabelUMap() {
			return Get().IGetLabelUMap();
		}
		
		static void printExecutionTable() {
			return Get().IPrintExecutionTable();
		}
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
		std::unordered_map<std::string, int> IGetLabelUMap() {
			return m_LabelUMap;
		}
		std::vector <ExecutionTable>& IGet_ExecutionTable() {
			return m_ExecutionTable;
		}
		std::string IValidateInput(const std::string& data , const std::string& dataMem);
		void IValidateLabel(const std::string& data,int p_CurrentLine, ErrorFlag& errorFlag);
		void IValidateInstructions(std::vector <std::string>& data,int p_CurrentLine, ErrorFlag& errorFlag);
		void IExecute(int pStep);
		bool ILabelInsert(const std::string label, int address);
		bool ILabelCheck(const std::string label);
		std::string IConstructCResult();
		void IPrintExecutionTable() {
			for (const auto& it : m_ExecutionTable) {
				CL_CORE_INFO("Address at {0}", it.address);
				CL_CORE_INFO("Instruction {}", it.instruction);
				for (const auto& it2 : it.datas) {
					CL_CORE_INFO("Datas : {0}", it2);
				}
				CL_CORE_INFO("Immediate {0}", it.immediate);
				for (const auto& it3 : it.registerNames) {
					CL_CORE_INFO("Register Name : {}", it3);
				}

			}
		}
	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
		std::unordered_map<std::string, int> m_LabelUMap; // Label Name -> Relevant Address 
		std::map<int, std::string> m_CResultMap;
		std::vector<std::string> m_UnregisteredLabels;
		std::vector<ExecutionTable> m_ExecutionTable;
		std::vector<int> m_Immediates;
		bool m_SegmentStart;
	};
	
	std::map <int,std::string> DataMemoryHandler(const std::string & text);
}