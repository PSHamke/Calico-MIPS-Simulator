#pragma once
#include "MipsLayer.h"
#include "regex"
#include <stdexcept>
#include "HelperString.h"
#include "MipsInfo.h"
//-add*, sub*, and*, or* , addi**, andi**, ori**
//
//- slt*, slti**, j***, jr*
//
//- jal***, sll*, srl*, beq**, bne**
//
//- mul?, muli?, lui**
//
//- lw**, sw**, lb**, sb**

namespace MIPSLayer {

	int PC = 0; // programming counter 
	char* M; // data memory 
	int RA = 0; // return address
	int addCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {

		rd = rs + rt;
		std::cout << "Executed!\n";
		return rd;
	}

	int subCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rs - rt;
		return rd;
	}

	int andCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rs & rt;
		return rd;
	}

	int orCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rs | rt;
		return rd;
	}

	int mulCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rs * rt;
		return rd;
	}
	/*
	The Jump Register instruction causes the PC to jump to the contents of the first source register.

	It's syntax is:
	JR $first source register's address
	*/
	int jrCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {

		MIPSLayer::PC = rs;
		return rs;
	}

	/*
	The SLT instruction sets the destination register's content to the value 1 if the first source register's contents are
	less than the second source register's contents. Otherwise, it is set to the value 0.
	*/
	int sltCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = (rs < rt) ? 1 : 0;
		return rd;
	}
	/*
	A Shift Left Logical instruction in MIPS assembly is used for shifting the bits to the left.
	The amount of shift depends on the value we give it.
	When we perform a shift left logical instruction the low bits at right most is replaced by zeros and the high right most bit is discarded.
	*/
	// rs register is not used rd is a result register 
	int sllCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rt << shamt;
		return rd;
	}

	/*
	*/
	int srlCallback(const int& rs, const int& rt, int& rd, unsigned int shamt) {
		rd = rt >> shamt;
		return rd;
	}


	// I Type Instruction Callbacks
	int addiCallback(int& rs, int& rt, int& immediate) {
		rt = rs + immediate;
		return rt;
	}

	int andiCallback(int& rs, int& rt, int& immediate) {
		rt = rs & immediate;
		return rt;
	}

	int oriCallback(int& rs, int& rt, int& immediate) {
		rt = rs | immediate;
		return rt;
	}

	int sltiCallback(int& rs, int& rt, int& immediate) {
		rt = (rs < immediate) ? 1 : 0;
		return rt;
	}

	/*
	The BEQ instruction branches the PC if the first source register's contents and the second source
	register's contents are equal.

	It's syntax is:
	BEQ $first source register's address, $second source register's address, branch value.
	*/
	int beqCallback(int& rs, int& rt, int& immediate) {
		MIPSLayer::PC = (rs == rt) ? immediate : MIPSLayer::PC;
		return MIPSLayer::PC;
	}

	int bneCallback(int& rs, int& rt, int& immediate) {
		MIPSLayer::PC = (rs != rt) ? immediate : MIPSLayer::PC;
		return MIPSLayer::PC;
	}

	/*
	The LW instruction loads data from the data memory through a specified address, with a possible offset, to the
	destination register.

	It's syntax is:
	LW $destination register's address, offset($source register's address).
	*/

	int lwCallback(int& rs, int& rt, int& immediate) {
		//rt = MIPSLayer::M[rs + immediate];
		return rt;
	}

	int swCallback(int& rs, int& rt, int& immediate) {
		//MIPSLayer::M[rs + immediate] = rt;
		return rt;
	}

	int luiCallback(int& rs, int& rt, int& immediate) {
		rt = immediate << 16;
		return rt;
	}

	int lbCallback(int& rs, int& rt, int& immediate) {
		//rt = MIPSLayer::M[rs + immediate];
		return rt;
	}

	int sbCallback(int& rs, int& rt, int& immediate) {
		//MIPSLayer::M[rs + immediate] = (0xff & rt);
		return (0xff & rt);
	}

	int jCallback(int& address) {
		MIPSLayer::PC = address;
		return address;
	}

	int jalCallback(int& address) {
		MIPSLayer::RA = MIPSLayer::PC + 8;
		MIPSLayer::PC = address;
		return address;
	}
	

	void MIPS::initInstructionMap() {
		
		m_InstructionUMap["add"] = new InstructionFormatR("add", 0x0, 0x20, MIPSLayer::addCallback);
		m_InstructionUMap["sub"] = new InstructionFormatR("sub", 0x0, 0x22, MIPSLayer::subCallback);
		m_InstructionUMap["and"] = new InstructionFormatR("and", 0x0, 0x22, MIPSLayer::andCallback);
		m_InstructionUMap["or"] = new InstructionFormatR("or", 0x0, 0x25, MIPSLayer::orCallback);
		m_InstructionUMap["slt"] = new InstructionFormatR("slt", 0x0, 0x2A, MIPSLayer::sltCallback);
		m_InstructionUMap["jr"] = new InstructionFormatR("jr", 0x0, 0x08, MIPSLayer::jrCallback);
		m_InstructionUMap["sll"] = new InstructionFormatR("sll", 0x0, 0x00, MIPSLayer::sllCallback);
		m_InstructionUMap["srl"] = new InstructionFormatR("srl", 0x0, 0x02, MIPSLayer::srlCallback);
		//m_InstructionUMapap["mul"] = new  InstructionFormatR("mul", 0x0, 0x00, MIPSLayer::mulCallback);
		m_InstructionUMap["addi"] = new InstructionFormatI("addi", 0x8, MIPSLayer::addiCallback);
		m_InstructionUMap["andi"] = new InstructionFormatI("andi", 0xC, MIPSLayer::andiCallback);
		m_InstructionUMap["ori"] = new InstructionFormatI("ori", 0xD, MIPSLayer::oriCallback);
		m_InstructionUMap["slti"] = new InstructionFormatI("slti", 0xA, MIPSLayer::sltiCallback);
		m_InstructionUMap["beq"] = new InstructionFormatI("beq", 0x4, MIPSLayer::beqCallback);
		m_InstructionUMap["bne"] = new InstructionFormatI("bne", 0x5, MIPSLayer::bneCallback);
		m_InstructionUMap["lui"] = new InstructionFormatI("lui", 0xF, MIPSLayer::luiCallback);
		m_InstructionUMap["lw"] = new InstructionFormatI("lw", 0x23, MIPSLayer::lwCallback);
		m_InstructionUMap["sw"] = new InstructionFormatI("sw", 0x2B, MIPSLayer::swCallback);
		m_InstructionUMap["lb"] = new InstructionFormatI("lb", 0x20, MIPSLayer::lbCallback);
		m_InstructionUMap["sb"] = new InstructionFormatI("sb", 0x28, MIPSLayer::sbCallback);
		m_InstructionUMap["jal"] = new InstructionFormatJ("jal", 0x3, MIPSLayer::jalCallback);
		m_InstructionUMap["j"] = new InstructionFormatJ("j", 0x2, MIPSLayer::jCallback);
	}

	

	void MIPS::initRegistersMap()
	{
		m_RegisterUMap["$zero"] = new Register("zero", 0, NULL, registerStatus::free);
		m_RegisterUMap["$at"] = new Register("at", 1, NULL, registerStatus::free);
		m_RegisterUMap["&v0"] = new Register("v0", 2, NULL, registerStatus::free);
		m_RegisterUMap["$v1"] = new Register("v1", 3, NULL, registerStatus::free);
		m_RegisterUMap["$a0"] = new Register("a0", 4, NULL, registerStatus::free);
		m_RegisterUMap["$a1"] = new Register("a1", 5, NULL, registerStatus::free);
		m_RegisterUMap["$a2"] = new Register("a2", 6, NULL, registerStatus::free);
		m_RegisterUMap["$a3"] = new Register("a3", 7, NULL, registerStatus::free);
		m_RegisterUMap["$t0"] = new Register("t0", 8, NULL, registerStatus::free);
		m_RegisterUMap["$t1"] = new Register("t1", 9, NULL, registerStatus::free);
		m_RegisterUMap["$t2"] = new Register("t2", 10, NULL, registerStatus::free);
		m_RegisterUMap["$t3"] = new Register("t3", 11, NULL, registerStatus::free);
		m_RegisterUMap["$t4"] = new Register("t4", 12, NULL, registerStatus::free);
		m_RegisterUMap["$t5"] = new Register("t5", 13, NULL, registerStatus::free);
		m_RegisterUMap["$t6"] = new Register("t6", 14, NULL, registerStatus::free);
		m_RegisterUMap["$t7"] = new Register("t7", 15, NULL, registerStatus::free);
		m_RegisterUMap["$s0"] = new Register("s0", 16, NULL, registerStatus::free);
		m_RegisterUMap["$s1"] = new Register("s1", 17, NULL, registerStatus::free);
		m_RegisterUMap["$s2"] = new Register("s2", 18, NULL, registerStatus::free);
		m_RegisterUMap["$s3"] = new Register("s3", 19, NULL, registerStatus::free);
		m_RegisterUMap["$s4"] = new Register("s4", 20, NULL, registerStatus::free);
		m_RegisterUMap["$s5"] = new Register("s5", 21, NULL, registerStatus::free);
		m_RegisterUMap["$s6"] = new Register("s6", 22, NULL, registerStatus::free);
		m_RegisterUMap["$s7"] = new Register("s7", 23, NULL, registerStatus::free);
		m_RegisterUMap["$t8"] = new Register("t8", 24, NULL, registerStatus::free);
		m_RegisterUMap["$t9"] = new Register("t9", 25, NULL, registerStatus::free);
		m_RegisterUMap["$k0"] = new Register("k0", 26, NULL, registerStatus::free);
		m_RegisterUMap["$k1"] = new Register("k1", 27, NULL, registerStatus::free);
		m_RegisterUMap["$gp"] = new Register("gp", 28, NULL, registerStatus::free);
		m_RegisterUMap["$sp"] = new Register("sp", 29, NULL, registerStatus::free);
		m_RegisterUMap["$fp"] = new Register("fp", 30, NULL, registerStatus::free);
		m_RegisterUMap["$ra"] = new Register("ra", 31, NULL, registerStatus::free);
	
	}

	

	std::vector<std::string> tokenizeMyString(const std::string& s, const std::string& del)
	{
		const int dictSize = 256;
		static bool dict[dictSize] = { false };

		std::vector<std::string> res;
		for (int i = 0; i < del.size(); ++i) {
			dict[del[i]] = true;
		}

		std::string token("");
		for (auto& i : s) {
			if (dict[i]) {
				if (!token.empty()) {
					res.push_back(token);
					token.clear();
				}
			}
			else {
				token += i;
			}
		}
		if (!token.empty()) {
			res.push_back(token);
		}
		return res;
	}

	bool isSegmentInstruction(const std::string& token) {
		
		static const char* const segmentInstructions[] = {
			"main:","data:"
		};
		
		for (auto& it: segmentInstructions) {
			if (token == it)
				return true;
		}
		return false;
	}
	
	std::string createCOutput(std::vector<std::string> data, int argCount, int instructionCode, int functCode) {
		std::string result = "";
		if (functCode == -1) {
			switch (instructionCode) {
			case 0x8:
				result = string_format("%s = %s + %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xC:
				result = string_format("%s = %s & %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xD:
				result = string_format("%s = %s | %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xA:
				result = string_format("%s = ( %s < %s ) ? 1 : 0 ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x4:
				result = string_format("if (%s == %s) goto %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x5:
				result = string_format("if (%s != %s) goto %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0xF:
				result = string_format("%s = ( %s << 16) ;", data[1].c_str(), data[2].c_str());
				break;
			case 0x23:
				result = string_format("%s = MEM[%s + %s] ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x2B:
				result = string_format("MEM[%s + %s] = %s ;", data[3].c_str(), data[2].c_str(), data[1].c_str());
				break;
			case 0x20:
				result = string_format("%s = MEM[%s + %s] ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x28:
				result = string_format("MEM[%s + %s] = (0xFF & %s) ;", data[3].c_str(), data[2].c_str(), data[1].c_str());
				break;
			default:
				break;
			}
		}
		else {
			switch (functCode)
			{
			case 0x20:
				result = string_format("%s = %s + %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
				break;
			case 0x22:
				result = string_format("%s = %s - %s ;", data[1].c_str(), data[2].c_str(), data[3].c_str());
			default:
				break;
			}
		}
		return result;
	}
	enum  _ErrorFlag {
		Error_None = 0,
		Segment_Start = BIT(1),
		Segment_Duplicate = BIT(2),
		Instruction_Start= BIT(3),
		Numeric_Value = BIT(4),
		Register_Value = BIT(5)
	};
	std::string MIPS::ITranslateToC(const std::string& aData)
	{
		static const char* const registerNames[] = {
			"$t1","$t2"
		};
		
		//-add*, sub*, and*, or* , addi**, andi**, ori**
		//
		//- slt*, slti**, j***, jr*
		//
		//- jal***, sll*, srl*, beq**, bne**
		//
		//- mul?, muli?, lui**
		//
		//- lw**, sw**, lb**, sb**
		std::string result = "";
		std::string delimiter = "\n";
		std::string data = aData;
		std::cout <<"Initial Data: " << data << " END\n";
		size_t pos = 0;
		std::vector<std::pair<std::string,int>> Lines;
		std::vector<std::vector<std::string>> Tokens;
		int lineCounter = 0;
		while ((pos = data.find(delimiter)) != std::string::npos) {
			Lines.push_back(std::make_pair(data.substr(0, pos),lineCounter++));
			data.erase(0, pos + delimiter.length());

		}
		if (data != "") {
			Lines.push_back(std::make_pair(data.substr(0, pos), lineCounter++));
		}
		
		for (auto& it : Lines) {
			
			std::vector<std::string> TokenizedLine;
			trim(it.first); // Trim line to prepare evaluate 
			//tokenize ---- must be use , for now 
			//check misspell commas 
			bool check = true;
			charCheck(it.first,check); // Correct comma error
			
			if (!check) {
				std::cout << "Comma Error at line = " << it.second << "\n";
				// Do error in GUI 
			}
	
			/*while ((pos = it.first.find(",")) != std::string::npos) {
				TokenizedLine.push_back(it.first.substr(0, pos).erase(0,it.first.substr(0, pos).find_first_not_of(" \t\n\r\f\v")));
				it.first.erase(0, pos + delimiter.length());
			}
			if (it.first != "") {
				TokenizedLine.push_back(it.first.substr(0, pos).erase(0, it.first.substr(0, pos).find_first_not_of(" \t\n\r\f\v")));
			}*/

			Tokens.push_back(tokenizeMyString(it.first," ,"));
		
			//std::cout << it<<"\n";
		}
		std::cout << "Tokens\n";
		std::vector<std::reference_wrapper<int>> datas;
		int TokenizedLineCount = 0;
		unsigned int PC = 0;
		int shamt = 0;
		bool checkShamt = false;
		int special_case = 0;
		int errorflag = _ErrorFlag::Error_None;
		for (auto& it : Tokens) {
			std::vector<int> expectedArguments;
			int expectedArgumentsSize;
			if (it.empty()) {
				break;
			}
			if (TokenizedLineCount == 0) {
				if (isSegmentInstruction(it.at(0))) {
					result = string_format("%sint main (void) { \n", result.c_str());
				}
				else {
					errorflag |= _ErrorFlag::Segment_Start;
				}
			}
			else {
				switch (it.size()) {
				case 1:
					if (isSegmentInstruction(it.at(0))) {
						errorflag |= _ErrorFlag::Segment_Duplicate;
					}
					break;
				case 2: case 3: case 4:
					std::cout << ".Text instruction \n";
					expectedArguments = InstructionInfo(it.at(0));
					expectedArgumentsSize = expectedArguments.size();
					if (expectedArgumentsSize < 1) {
						errorflag |= _ErrorFlag::Instruction_Start;
						break;
					}
					if (expectedArgumentsSize == 4) {
						checkShamt = (expectedArguments.at(3) & Special_Shamt);
						expectedArgumentsSize -= 1; // subtract shamt
					}
					if (expectedArguments.at(1) & Special_Negligible) {
						expectedArgumentsSize -= 1;
						special_case = 1;
					}
					if (it.size() == expectedArgumentsSize + 1) { // count instruction +1 
						int i = 1;
						for (; i <= expectedArgumentsSize; i++) {
							if (expectedArguments.at(i - 1) & Special_Constant || expectedArguments.at(i - 1) & Special_Negligible) {
								if (is_number(it.at(i))) {
									int immediate = atoi(it.at(i).c_str());
									std::cout << "Approved ! Number \n";
									datas.push_back(immediate);
								}
								else {
									errorflag |= _ErrorFlag::Numeric_Value;
									break;
								}
								
							}
							else if (expectedArguments.at(i - 1) & RegisterInfo(it.at(i))) {
								std::cout << "Approved ! \n";
								datas.push_back(m_RegisterUMap[it.at(i)]->getRef());
								if (special_case) {
									datas.push_back(special_case);
								}
							}
							else if (RegisterInfo(it.at(i)) & Reg_Error ) {
								errorflag |= _ErrorFlag::Register_Value;
								break;
							}
						}
						
						if (errorflag == 0) {
							if (checkShamt)
								datas.push_back(shamt);

							m_InstructionUMap[it.at(0)]->Execute(datas, PC);
							result = string_format("%s\t%s\n", result.c_str(), createCOutput(it, expectedArguments.size(), m_InstructionUMap[it.at(0)]->getOpcode(), m_InstructionUMap[it.at(0)]->getFunct()).c_str());
						}
						else {
							if (errorflag & _ErrorFlag::Segment_Start) {
								result = string_format("%s[Error] Expected segment start.\n", result.c_str());
							}
							if (errorflag & _ErrorFlag::Segment_Duplicate) {
								result = string_format("%s[Error] Duplicate segment start.\n", result.c_str());
							}
							if (errorflag & _ErrorFlag::Instruction_Start) {
								result = string_format("%s[Error] Expected instruction got[ %s ] at Line %d.\n", result.c_str(), it.at(i).c_str(), TokenizedLineCount);
							}
							if (errorflag & _ErrorFlag::Numeric_Value) {
								result = string_format("%s[Error] Expected numeric got[ %s ] at Line %d.\n", result.c_str(), it.at(i).c_str(), TokenizedLineCount);
								std::cout << "res = " << result << "\n";
							}
							if (errorflag & _ErrorFlag::Register_Value) {
								result = string_format("%s[Error] Missuse of register type [ %s ] at Line %d.\n", result.c_str(), it.at(i).c_str(), TokenizedLineCount);
								std::cout << "res = " << result << "\n";
							}
						}

					}
					else {
						std::cout << "size= " << it.size();
					}
					break;

				}

			}

			TokenizedLineCount++;
			
		}
			
		if(~(errorflag & _ErrorFlag::Segment_Start))
			result = string_format("%s\t%s\n}", result.c_str(), "return 0;");
		return result;
	}
	
}