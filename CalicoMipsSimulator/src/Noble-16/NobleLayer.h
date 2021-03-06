#pragma once
#include <unordered_map>
#include <map>
#include "AssemblyCore/Registers.h"
#include "AssemblyCore/Instruction.h"
#include "AssemblyCore/Callbacks.h"
#include "ExecutionTable.h"
#include "HelperString.h"
#include "NobleInfo.h"

namespace NobleLayer {
	class Noble {
	public:
		Noble(const Noble&) = delete;

		static Noble& Get() {
			static Noble instance;
			return instance;
		}

		static std::string ValidateInput(const std::string& data, const std::string& dataMem,int callingReason) {
			return Get().IValidateInput(data, dataMem,callingReason);
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
		static void ResetOutputBuffer(){
			return Get().IResetOutputBuffer();
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
		
		static std::string GetOutputString() {
			return Get().IGetOutputString();
		}
		
		static void CreateInstructionTable() {
			return Get().ICreateInstructionTable();
		}
		static std::vector<ExecutionTable>& GetInstructionTable() {
			return Get().IGetInstructionTable();
		}
		static bool GetStatus() {
			return Get().IGetStatus();
		}
		static void SetStatus(bool status) {
			return Get().ISetStatus(status);
		}

		static int GetEditorLine() {
			return Get().IGetEditorLine();
		}
		static void SetEditorLine(int line) {
			Get().ISetEditorLine(line);
		}
		static int GetActiveInsMem() {
			return Get().IGetActiveInsMem();
		}

		static void SetActiveInsMem(int activeMem) {
			Get().ISetActiveInsMem(activeMem);
		}

		static int GetTotalCycle() {
			return Get().IGetTotalCycle();
		}
	private:

		Noble() {
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
		void IResetOutputBuffer() {
			m_OutputBuffer.clear();
		}
		std::unordered_map<std::string, int> IGetLabelUMap() {
			return m_LabelUMap;
		}
		std::vector <ExecutionTable>& IGet_ExecutionTable() {
			return m_ExecutionTable;
		}
		std::vector <ExecutionTable>& IGetInstructionTable() {
			return m_InstructionTable;
		}
		std::string IGetOutputString();
		std::string IValidateInput(const std::string& data, const std::string& dataMem,int callingReason);
		void IValidateLabel(const std::string& data, int p_CurrentLine,int& instCount,ErrorFlag& errorFlag,int callingReason);
		void IValidateInstructions(std::vector <std::string>& data, int p_CurrentLine, ErrorFlag& errorFlag);
		void IExecute(int pStep);
		bool ILabelInsert(const std::string label, int address);
		bool ILabelCheck(const std::string label);
		std::string IConstructCResult();
		std::string Noble::CreateNobleOutput(int registerNum);
		std::string CreateCOutput(std::vector<std::string> data, int argCount, int instructionCode, int functCode);
		void ICreateInstructionTable();
		int IValidateRelativePC(int PC);
		bool IGetStatus() {
			return m_Status;
		}
		int IGetEditorLine() {
			return m_EditorLine;
		}
		void ISetEditorLine(int line) {
			m_EditorLine = line;
		}

		void ISetExecPC(int PC) {
			m_ExecPC = PC;
		}
		int IGetExecPC() {
			return m_ExecPC;
		}
		int IGetTotalCycle() {
			return m_TotalCycle;
		}
		void ISetStatus(bool status) {
			m_Status = status;
		}
		int IGetActiveInsMem() {
			return m_ActiveInsMem;
		}
		void ISetActiveInsMem(int activeMem) {
			m_ActiveInsMem = activeMem;
		}
		void IPrintExecutionTable() {
			for (const auto& it : m_ExecutionTable) {
				CL_CORE_INFO("Address at {0}", it.m_Address);
				CL_CORE_INFO("Instruction {}", it.m_Instruction);
				for (const auto& it2 : it.m_Datas) {
					CL_CORE_INFO("Datas : {0}", it2);
				}
				CL_CORE_INFO("Immediate {0}", it.m_Immediate);
				for (const auto& it3 : it.m_RegisterNames) {
					CL_CORE_INFO("Register Name : {}", it3);
				}

			}
		}
		std::string CreateReadableAsm(std::vector <std::string>& pInput);

	private:
		std::unordered_map<std::string, Register*> m_RegisterUMap;
		std::unordered_map<std::string, Instruction*> m_InstructionUMap;
		std::unordered_map<std::string, int> m_LabelUMap; // Label Name -> Relevant Address 
		std::map<int, std::string> m_CResultMap;
		std::vector<std::string> m_UnregisteredLabels;
		std::vector<ExecutionTable> m_ExecutionTable;
		std::vector<ExecutionTable> m_InstructionTable;
		std::vector<int> m_Immediates;
		std::vector<std::string> m_OutputBuffer;
		bool m_SegmentStart = false;
		bool m_Status = false;
		int m_EditorLine = 0;
		int m_ExecPC;
		int m_ActiveInsMem = -1;
		int m_TotalCycle = 0;
	};

	bool isSegmentInstruction(const std::string& token);
	std::map<int, std::string> DataMemoryHandler(const std::string& aText);
}