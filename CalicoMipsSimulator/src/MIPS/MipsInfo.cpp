#include "MipsInfo.h"
namespace MIPSLayer {
	std::pair<std::string, std::vector<int>>  instructions[] = {
		// R Type
		{"add",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"sub",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"and",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"or",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"slt",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"jr",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries| Reg_Return_Address,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Shamt}},

		{"srl",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"sll",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"mult",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		// I Type 
		{"addi",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value}},

		{"andi",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value}},

		{"ori",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value}},

		{"slti",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value}},

		{"beq",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value |Special_Label}},

		{"bne",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value | Special_Label}},

		{"lui",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Negligible,
		Special_Constant | Reg_Constant_Value}},

		{"lw",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Paranthesis|Special_Constant | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value}},

		{"sw",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Paranthesis | Special_Constant | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value}},

		{"lb",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Paranthesis | Special_Constant | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value}},

		{"sb",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Paranthesis | Special_Constant | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value}},

		// J Type jumps
		{"jal",{ Special_Label|Special_Constant}},

		{"j",{ Special_Label |Special_Constant}}


	}; // pair of m_Instruction and expected arguments

	std::pair<std::string, int> registers[] = {
			{"$zero",Reg_Constant_Value}, {"$at",Reg_Assembler_Temporary}, {"$v0",Reg_Expression_Evaluation},
			{"$v1",Reg_Expression_Evaluation}, {"a0",Reg_Arguments},{"a1",Reg_Arguments},{"a2",Reg_Arguments},
			{"a3",Reg_Arguments},{"$t0",Reg_Temporaries}, {"$t1",Reg_Temporaries},{"$t2",Reg_Temporaries},
			{"$t3",Reg_Temporaries},{"$t4",Reg_Temporaries},{"$t5",Reg_Temporaries},{"$t6",Reg_Temporaries},{"$t7",Reg_Temporaries},
			{"$s0",Reg_SavedTemporaries}, {"$s1",Reg_SavedTemporaries},{"$s2",Reg_SavedTemporaries},{"$s3",Reg_SavedTemporaries},
			{"$s4",Reg_SavedTemporaries},{"$s5",Reg_SavedTemporaries},{"$s6",Reg_SavedTemporaries},{"$s7",Reg_SavedTemporaries},
			{"$t8",Reg_ExtraTemporaries}, {"$t9",Reg_ExtraTemporaries}, {"$k0",Reg_OS_Reserved}, {"$k1",Reg_OS_Reserved},
			{"$gp",Reg_Stack_Pointers}, {"$sp",Reg_Global_Pointers}, {"$fp",Reg_Frame_Pointer}, {"$ra",Reg_Return_Address}
	};
	
	std::vector<int>& InstructionInfo(const std::string& token) {

		for (auto& it : instructions) {
			if (token == it.first)	
				return it.second;
		}
		std::vector<int> emptyVec;
		return emptyVec;
	}

	int RegisterInfo(const std::string& token) {
		
		for (auto& it : registers) {
			if (token == it.first)
				return it.second;
		}
		return Reg_Error;
	}
}
