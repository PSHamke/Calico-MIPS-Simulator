#pragma once
#include "MipsLayer.h"
#include "regex"
#include <stdexcept>
#include "HelperString.h"
#include "MipsInfo.h"
#include "Log.h"
#include "Callbacks.h"


namespace MIPSLayer {

	int PC = 0; // programming counter 
	char* M; // data memory 
	int RA = 0; // return m_Address
	
	void MIPS::InitInstructionMap() {

		m_InstructionUMap["add"]  = new InstructionFormatR("add", 0x0, 0x20, addCallback);
		m_InstructionUMap["sub"]  = new InstructionFormatR("sub", 0x0, 0x22, subCallback);
		m_InstructionUMap["and"]  = new InstructionFormatR("and", 0x0, 0x24, andCallback);
		m_InstructionUMap["or"]   = new InstructionFormatR("or", 0x0, 0x25, orCallback);
		m_InstructionUMap["slt"]  = new InstructionFormatR("slt", 0x0, 0x2A, sltCallback);
		m_InstructionUMap["jr"]   = new InstructionFormatR("jr", 0x0, 0x08, jrCallback);
		m_InstructionUMap["sll"]  = new InstructionFormatR("sll", 0x0, 0x00, sllCallback);
		m_InstructionUMap["srl"]  = new InstructionFormatR("srl", 0x0, 0x02, srlCallback);
		m_InstructionUMap["mult"] = new  InstructionFormatR("mul", 0x0, 0x00, mulCallback);
		m_InstructionUMap["addi"] = new InstructionFormatI("addi", 0x8, addiCallback);
		m_InstructionUMap["andi"] = new InstructionFormatI("andi", 0xC, andiCallback);
		m_InstructionUMap["ori"]  = new InstructionFormatI("ori", 0xD, oriCallback);
		m_InstructionUMap["slti"] = new InstructionFormatI("slti", 0xA, sltiCallback);
		m_InstructionUMap["beq"]  = new InstructionFormatI("beq", 0x4, beqCallback);
		m_InstructionUMap["bne"]  = new InstructionFormatI("bne", 0x5, bneCallback);
		m_InstructionUMap["lui"]  = new InstructionFormatI("lui", 0xF, luiCallback);
		m_InstructionUMap["lw"]   = new InstructionFormatI("lw", 0x23, lwCallback);
		m_InstructionUMap["sw"]   = new InstructionFormatI("sw", 0x2B, swCallback);
		m_InstructionUMap["lb"]   = new InstructionFormatI("lb", 0x20, lbCallback);
		m_InstructionUMap["sb"]   = new InstructionFormatI("sb", 0x28, sbCallback);
		m_InstructionUMap["jal"]  = new InstructionFormatJ("jal", 0x3, jalCallback);
		m_InstructionUMap["j"]    = new InstructionFormatJ("j", 0x2, jCallback);
	}


	void MIPS::InitRegistersMap()
	{
		m_RegisterUMap["$zero"] = new Register("zero", 0, NULL, registerStatus::free);
		m_RegisterUMap["$at"]   = new Register("at", 1, NULL, registerStatus::free);
		m_RegisterUMap["$v0"]   = new Register("v0", 2, NULL, registerStatus::free);
		m_RegisterUMap["$v1"]   = new Register("v1", 3, NULL, registerStatus::free);
		m_RegisterUMap["$a0"]   = new Register("a0", 4, NULL, registerStatus::free);
		m_RegisterUMap["$a1"]   = new Register("a1", 5, NULL, registerStatus::free);
		m_RegisterUMap["$a2"]   = new Register("a2", 6, NULL, registerStatus::free);
		m_RegisterUMap["$a3"]   = new Register("a3", 7, NULL, registerStatus::free);
		m_RegisterUMap["$t0"]   = new Register("t0", 8, NULL, registerStatus::free);
		m_RegisterUMap["$t1"]   = new Register("t1", 9, NULL, registerStatus::free);
		m_RegisterUMap["$t2"]   = new Register("t2", 10, NULL, registerStatus::free);
		m_RegisterUMap["$t3"]   = new Register("t3", 11, NULL, registerStatus::free);
		m_RegisterUMap["$t4"]   = new Register("t4", 12, NULL, registerStatus::free);
		m_RegisterUMap["$t5"]   = new Register("t5", 13, NULL, registerStatus::free);
		m_RegisterUMap["$t6"]   = new Register("t6", 14, NULL, registerStatus::free);
		m_RegisterUMap["$t7"]   = new Register("t7", 15, NULL, registerStatus::free);
		m_RegisterUMap["$s0"]   = new Register("s0", 16, NULL, registerStatus::free);
		m_RegisterUMap["$s1"]   = new Register("s1", 17, NULL, registerStatus::free);
		m_RegisterUMap["$s2"]   = new Register("s2", 18, NULL, registerStatus::free);
		m_RegisterUMap["$s3"]   = new Register("s3", 19, NULL, registerStatus::free);
		m_RegisterUMap["$s4"]   = new Register("s4", 20, NULL, registerStatus::free);
		m_RegisterUMap["$s5"]   = new Register("s5", 21, NULL, registerStatus::free);
		m_RegisterUMap["$s6"]   = new Register("s6", 22, NULL, registerStatus::free);
		m_RegisterUMap["$s7"]   = new Register("s7", 23, NULL, registerStatus::free);
		m_RegisterUMap["$t8"]   = new Register("t8", 24, NULL, registerStatus::free);
		m_RegisterUMap["$t9"]   = new Register("t9", 25, NULL, registerStatus::free);
		m_RegisterUMap["$k0"]   = new Register("k0", 26, NULL, registerStatus::free);
		m_RegisterUMap["$k1"]   = new Register("k1", 27, NULL, registerStatus::free);
		m_RegisterUMap["$gp"]   = new Register("gp", 28, NULL, registerStatus::free);
		m_RegisterUMap["$sp"]   = new Register("sp", 29, NULL, registerStatus::free);
		m_RegisterUMap["$fp"]   = new Register("fp", 30, NULL, registerStatus::free);
		m_RegisterUMap["$ra"]   = new Register("ra", 31, NULL, registerStatus::free);
	}


	bool isSegmentInstruction(const std::string& token) {

		static const char* const segmentInstructions[] = {
			"main:","data:"
		};

		for (auto& it : segmentInstructions) {
			if (token == it)
				return true;
		}
		return false;
	}

	std::string CreateCOutput(std::vector<std::string> data, int argCount, int instructionCode, int functCode) {
		std::string result = "";
		if (functCode == -1) {
			switch (instructionCode) {
			case 0x8:
				result = string_format("%s = %s + %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xC:
				result = string_format("%s = %s & %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xD:
				result = string_format("%s = %s | %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xA:
				result = string_format("%s = ( %s < %s ) ? 1 : 0;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x4:
				result = string_format("if (%s == %s) goto %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x5:
				result = string_format("if (%s != %s) goto %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xF:
				result = string_format("%s = ( %s << 16);", data[1].c_str(), data[2].c_str());
				break;
			case 0x23:
				result = string_format("%s = MEM[%s + %s];", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x2B:
				result = string_format("MEM[%s + %s] = %s;", data[3].c_str(), data[2].c_str(), data[1].c_str());
				break;
			case 0x20:
				result = string_format("%s = MEM[%s + %s];", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x28:
				result = string_format("MEM[%s + %s] = (0xFF & %s);", data[3].c_str(), data[2].c_str(), data[1].c_str());
				break;
			case 0x3:
				result = string_format("$ra = PC; goto %s;", data[1].c_str());
				break;
			case 0x2:
				result = string_format("goto %s;", data[1].c_str());
			default:
				break;
			}
		}
		else {
			switch (functCode)
			{
			case 0x20:
				result = string_format("%s = %s + %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x22:
				result = string_format("%s = %s - %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x24:
				result = string_format("%s = %s & %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x25:
				result = string_format("%s = %s | %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x2A:
				result = string_format("%s = %s < %s ? 1: 0;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x8:
				result = string_format("goto %s", data[2].c_str());
				break;
			case 0x0:
				result = string_format("%s = %s << %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x2:
				result = string_format("%s = %s >> %s;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			default:
				break;
			}
		}
		return result;
	}
	
	std::string MIPS::IValidateInput(const std::string& aData, const std::string& memData)
	{
		// Be sure that only 1 line of code passed here at a time 
		std::string c_OutputResult = "";
		std::string delimiter = "\n";
		std::string data = aData;
		size_t pos = 0;
		std::vector<std::pair<std::string, int>> Lines;
		std::vector<std::vector<std::string>> Tokens;
		int lineCounter = 0;
		std::string temp;
		
		Memory::FreeTextMemory();
		IResetRegisterUMap();
		Memory::FreeDataMemory();
		DataMemoryHandler(memData);
		Memory::SetPC(0);
		Memory::SetVirtualPC(0);
		Memory::SetCallingReason(0);
		ResetCResultMap();
		ResetLabelUMap();
		IResetExecutionTable();
		

		while ((pos = data.find(delimiter)) != std::string::npos) {
			temp = trim(data.substr(0, pos));
			if ((temp)!="")
				Lines.push_back(std::make_pair(temp.substr(0, pos), lineCounter++));
			data.erase(0, pos + delimiter.length());

		}
		if (trim(data) != "") {
			Lines.push_back(std::make_pair(data.substr(0, pos), lineCounter++));
		}

		for (auto& it : Lines) {

			std::vector<std::string> TokenizedLine;
			trim(it.first); // Trim line to prepare evaluate 
			bool check = true;
			charCheck(it.first, check); // Correct comma error

			if (!check) {
				//std::cout << "Comma Error at line = " << it.second << "\n";
			}

			Tokens.push_back(TokenizeMyString(it.first, " ,"));
		}

		
		unsigned int PC = 0;
		int shamt = 0;
		bool checkShamt = false;
		int special_case = 0;
		ErrorFlag errorflag = Error_None;
		// First find all labels construct label map, then investigate instructions 
		int TokenizedLineCount = 0;
		
		for (auto& token : Tokens) 
			IValidateLabel(token.at(0), TokenizedLineCount++, errorflag);
		
		// Should check all labels first 
		ExecutionTable temp1;
		TokenizedLineCount = 0;
		for (auto& token : Tokens) { // Loops through each line of tokens
			errorflag = Error_None;
			switch (token.size()) {
			case 1:
				temp1.m_Address = TokenizedLineCount;
				temp1.m_Instruction = "label";
				m_ExecutionTable.push_back(temp1);		
				CL_CORE_ERROR("Label : {}", token.at(0));
				break;
			case 2: case 3: case 4:
				IValidateInstructions(token,TokenizedLineCount, errorflag);
				break;
			default:
				m_CResultMap[TokenizedLineCount] = "Unexpected Number of parameters";
				break;
			}
			
			
			
			if (errorflag & Segment_Start) {
				c_OutputResult = string_format("%s[Error] Expected segment start.\n", c_OutputResult.c_str());
			}
			if (errorflag & Segment_Duplicate) {
					c_OutputResult = string_format("%s[Error] Duplicate segment start.\n", c_OutputResult.c_str());					
			}

			TokenizedLineCount++;
		}

		if (~(errorflag & Segment_Start))
			m_CResultMap[TokenizedLineCount] = string_format("\t%s\n}", "return 0;");
		return IConstructCResult();
	}
	std::map<int, std::string> DataMemoryHandler(const std::string& aText) {

		std::map<int, std::string> resultMap;

		std::string delimiter = "\n";
		std::string data = aText;
		std::cout << "Initial Data: " << data << " END\n";
		size_t pos = 0;
		std::vector<std::pair<std::string, int>> Lines;
		std::vector< std::vector <std::pair<std::string, int>>> Tokens;
		Memory::FreeDataMemory(); // Free memory
		int lineCounter = 0;

		while ((pos = data.find(delimiter)) != std::string::npos) {
			Lines.push_back(std::make_pair(data.substr(0, pos), lineCounter++));
			data.erase(0, pos + delimiter.length());

		}
		if (data != "") {
			Lines.push_back(std::make_pair(data.substr(0, pos), lineCounter++));
		}

		for (auto& it : Lines) {

			std::vector<std::pair<std::string, int>> TokenizedLine;
			trim(it.first);

			while ((pos = it.first.find(":")) != std::string::npos) {
				TokenizedLine.push_back(std::make_pair(it.first.substr(0, pos), it.second));
				it.first.erase(0, pos + delimiter.length());

			}
			if (it.first != "") {
				TokenizedLine.push_back(std::make_pair(it.first.substr(0, pos), it.second));
			}

			if (!TokenizedLine.empty()) {
				Tokens.push_back(TokenizedLine);
			}

		}

		int errorflag = Error_None;
		int tokenSize = 0;
		for (auto& tokens : Tokens) {
			std::string resultStr = "";
			errorflag = Error_None;
			bool check = false;
			int result = 0;
			tokenSize = tokens.size();
			CL_CORE_INFO("Token Size = {}", tokenSize);
			if (tokenSize > 2) {
				errorflag |= DataMemory_Too_Many_Args;

			}
			else if (tokenSize == 1) {
				result = AnalyzeString(tokens.at(0).first, check);
				if (check) {
					Memory::DataMemoryInsert(string_format("%8X", tokens.at(0).second), result, tokens.at(0).second);
				}
				else {
					errorflag |= DataMemory_Invalid_Arg;
				}
				CL_CORE_INFO("Res {0} at line {1}", result, tokens.at(0).second);
			}
			else if (tokenSize == 2) {
				result = AnalyzeString(tokens.at(1).first, check);
				if (check) {
					Memory::DataMemoryInsert(string_format("%s", tokens.at(0).first.c_str()), result, tokens.at(1).second);
				}
				else {
					errorflag |= DataMemory_Invalid_Arg;
				}


				CL_CORE_INFO("Res {0} at line {1}", result, tokens.at(1).second);
			}

			if (errorflag & DataMemory_Too_Many_Args) {
				resultStr = "Too many arguments";
				resultMap[tokens.at(0).second + 1] = resultStr;
			}
			if (errorflag & DataMemory_Invalid_Arg) {
				resultStr = "Invalid Entry";
				resultMap[tokens.at(0).second + 1] = resultStr;
			}

		}

		return resultMap;
	}

	void MIPS::IValidateLabel(const std::string& pInput, int pCurrentLine, ErrorFlag& pErrorFlag) { // Result should be map of line number and string. Lastly can be ordered
		bool check = false;
		if (isSegmentInstruction(pInput)) { // Check for segment main only to wrap with main function ** Better visualize not necessary **
			check = ILabelInsert(pInput, pCurrentLine); // Add to label map 
			if (!check) {
				pErrorFlag |= Segment_Duplicate;
				m_CResultMap[pCurrentLine] = string_format("[Error] Duplicate segment start.");
			}
			else {
				m_CResultMap[pCurrentLine] = string_format("int main(void) { ");
				m_SegmentStart = true;
				CL_CORE_INFO("Main label detected and inserted!");
			}
		}
		else if (pInput.back() == ':') { // Make sure its a label and ended with ':' 
			check = ILabelInsert(pInput.substr(0,pInput.size()-1), pCurrentLine); // insert label without ':'
			if (!check) {
				pErrorFlag |= Label_Duplicate;
				m_CResultMap[pCurrentLine] = string_format("[Error] Duplicate label.");
			}
			else {
				m_CResultMap[pCurrentLine] = pInput; // have a look at  // seems nice 
			}
			CL_CORE_INFO("Main again label detected and inserted!");
		}
		else {
			pErrorFlag |= Incompleted_Label;
			m_CResultMap[pCurrentLine] = string_format("[Error] incomplete label [%s] at line %d.", pInput.c_str(), pCurrentLine);
		}
	}

	void MIPS::IValidateInstructions(std::vector <std::string>& pInput, int pCurrentLine, ErrorFlag& pErrorFlag) {
		std::vector<int> expectedArguments = InstructionInfo(pInput.at(0));
		int expectedArgumentsSize = expectedArguments.size();
		int givenArgumentsSize = pInput.size();
		int specialCases = 0;
		int tempShamt = 0;
		bool checkShamt = false;
		std::string temp = ""; // Will used to swap the register in lw sw lb sb 
		std::vector<std::reference_wrapper<int>> datas;
		std::vector<int> vecRegisterNames;
		unsigned int PC = 0;
		if (expectedArgumentsSize == 4) { // Possibly R type need to validate Shift amount as well.
			checkShamt = (expectedArguments.at(3) & Special_Shamt);
			expectedArgumentsSize -= 1; // Shamt is not a part of the input 
		}
		if (expectedArgumentsSize == 3) { // Lui validation  Need to neglect 2nd register here 
			if (expectedArguments.at(1) & Special_Negligible) {
				expectedArgumentsSize -= 1;
				specialCases = 1;
			}
			if (expectedArguments.at(1) & Special_Paranthesis) { // lw sw lb sb validation // order of the arguments are differ // ie. lw $t1,offset($t2)

				if (givenArgumentsSize == 3) { //properly formatted by user 
					temp = pInput.at(2);
				}
				else if (givenArgumentsSize == 4) {
					temp = pInput.at(3);
				}
				// Rearrange token vector. 
				size_t open = temp.find("(");
				size_t close = temp.find(")");
				if (open == std::string::npos) {
					CL_CORE_INFO("Left brackets missing!");
				}
				if (close == std::string::npos) {
					CL_CORE_INFO("Right brackets missing!");
				}
				else {
					if (pInput.size() == 3) {
						pInput.at(2) = temp.substr(0, open);
						pInput.push_back(temp.substr(open + 1, close - open - 1));
					}
					else {
						pInput.at(3) = temp.substr(open + 1, close - open - 1);
					}

				}

			}
		}
		if (expectedArgumentsSize != givenArgumentsSize - 1) { // ??
			pErrorFlag |= Insufficient_Instruction;
			m_CResultMap[pCurrentLine] = string_format("[Error] Insufficient Instruction at line %d.\n", pCurrentLine);
			CL_CORE_INFO("Expected {0} Given {1}", expectedArgumentsSize, givenArgumentsSize);
		}
		else {// count m_Instruction name  +1  
			int i;
			int expectedArgument = 0;
			int  immediate = 0;
			for (i = 1; i <= expectedArgumentsSize; i++) {
				std::string& token = pInput.at(i);
				expectedArgument = expectedArguments.at(i - 1);
				if (expectedArgument & Special_Constant || expectedArgument & Special_Negligible) { // Check for constant and negligible
						
					if (is_number(token)) {
						immediate = (atoi(token.c_str()));

							if (expectedArguments.at(i - 1) & Special_Paranthesis)
								immediate = immediate / 4;
							//m_Immediates.push_back(m_Immediate);
							//m_Datas.push_back((m_Immediates.back()));
					}
					else if (expectedArgument & Special_Label) { // it might be label // label might not be registered to map yet.. 
						CL_CORE_ERROR("Label Expected!");
						if (ILabelCheck(token)) {
							immediate = m_LabelUMap[token];
							CL_CORE_ERROR("Label Expected! {0}, address {1}", token, immediate);
							//m_Datas.push_back((std::ref(m_Immediate)));
						}
						else {
							pErrorFlag |= Non_Exist_Label;
								
							break;
						}
							
					}
					else {
						pErrorFlag |= Numeric_Value;
						break;
					}

				}
				else if (expectedArgument & RegisterInfo(token)) {

					datas.push_back(m_RegisterUMap[token]->getRef());
					//std::cout << "Value of reg = " << m_RegisterUMap[token]->getRef() << "\n";
					//std::cout << "Value of reg = " << m_RegisterUMap[token]->getValue() << "\n";
					vecRegisterNames.push_back(m_RegisterUMap[token]->getNumber());
					if (specialCases== 1) {
						datas.push_back(specialCases);
					}
				}
				else if (RegisterInfo(token) & Reg_Error) {
					pErrorFlag |= Register_Value;
					break;
					
				}
			}

			if (pErrorFlag == 0) {
				if (checkShamt)
					datas.push_back(tempShamt);

				if(specialCases!=2)
					m_InstructionUMap[pInput.at(0)]->Execute(datas,immediate, vecRegisterNames, PC);
				ExecutionTable temp;
				temp.m_Address = pCurrentLine;
				temp.m_Datas = datas;
				temp.m_Immediate = immediate;
				temp.m_Instruction = pInput.at(0);
				temp.m_RegisterNames = vecRegisterNames;
				m_ExecutionTable.push_back(temp);
				m_CResultMap[pCurrentLine] = string_format("\t%s", CreateCOutput(pInput, expectedArguments.size(), m_InstructionUMap[pInput.at(0)]->getOpcode(), m_InstructionUMap[pInput.at(0)]->getFunct()).c_str());
			}
			else {
				if (pErrorFlag & Instruction_Start) {
					m_CResultMap[pCurrentLine] = string_format("[Error] Expected instruction, got[ %s ] at Line %d.", pInput.at(i).c_str(), pCurrentLine);
				}
				if (pErrorFlag & Numeric_Value) {
					m_CResultMap[pCurrentLine] = string_format("[Error] Expected numeric, got[ %s ] at Line %d.", pInput.at(i).c_str(), pCurrentLine);
						
				}
				if (pErrorFlag & Register_Value) {
					m_CResultMap[pCurrentLine] = string_format("[Error] Expected register type, got [ %s ] at Line %d.",pInput.at(i).c_str(), pCurrentLine);
						
				}
				if (pErrorFlag & Non_Exist_Label) {
					m_CResultMap[pCurrentLine] = string_format("[Error] Expected label, got [ %s ] at Line %d.", pInput.at(i).c_str(), pCurrentLine);
				}
			}

		}
			


}

	void MIPS::IExecute(int step)
	{
		unsigned int i = 0;
		int counter = 0;
		CL_CORE_ERROR("Virtual PC = {0} Step = {1}",Memory::GetVirtualPC(), step);
		CL_CORE_INFO("Execution Before TableSize {0} Current PC = {1}", m_ExecutionTable.size(), Memory::GetPC());
		//IResetRegisterUMap();
		//Memory::SetPC(0);
		Memory::SetCallingReason(1); // Set .text memory read only 
		while (Memory::GetPC() < m_ExecutionTable.size() && Memory::GetVirtualPC() < step) {
			CL_CORE_INFO("Execution done for address {0} Current PC = {1}", m_ExecutionTable[Memory::GetPC()].m_Address, Memory::GetPC());
			if (m_ExecutionTable[Memory::GetPC()].m_Instruction == "label") {
				Memory::SetPC(Memory::GetPC() + 1);
				CL_CORE_INFO("Label here {0}",Memory::GetPC());
				Memory::SetVirtualPC(Memory::GetVirtualPC() + 1);
				continue;
			}
			m_InstructionUMap[m_ExecutionTable[Memory::GetPC()].m_Instruction]->Execute(m_ExecutionTable[Memory::GetPC()].m_Datas,
				m_ExecutionTable[Memory::GetPC()].m_Immediate, m_ExecutionTable[Memory::GetPC()].m_RegisterNames,i);
			Memory::SetVirtualPC(Memory::GetVirtualPC()+1);
			counter++;
			if (counter > 10000) {
				CL_CORE_INFO("Infinite Loop aborted!");
				break;
			}
			
		}
		
	}

	bool MIPS::ILabelInsert(const std::string label, int address)
	{
		if (!ILabelCheck(label)) {
			m_LabelUMap[label] = address;
			return true;
		}
		else {
			return false;
		}
	}

	bool MIPS::ILabelCheck(const std::string label) // Return false if already exist key 
	{
		if (m_LabelUMap.find(label) == m_LabelUMap.end())
			return false;
		
		return true;

	}

	std::string MIPS::IConstructCResult()
	{
		std::string result = "";
		int previous = 0;
		for (const auto& [key, value] : m_CResultMap) {
			for (; previous < key; ++previous) {
				result = string_format("%s%s", result.c_str(), "\n");
			}
			result = string_format("%s%s", result.c_str(), value.c_str());
		}
		CL_CORE_TRACE(result);
		return result;
	}

}