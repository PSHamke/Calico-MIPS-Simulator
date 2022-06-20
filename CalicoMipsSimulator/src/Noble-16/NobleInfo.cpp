#include "NobleInfo.h"
namespace NobleLayer {
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

		{"jr",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries| Reg_Return_Address}},

		{"srl",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"sll",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		{"mul",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Shamt}},

		// I Type 
		{"addi",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value}},
		{"muli",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
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
		Special_Constant | Reg_Constant_Value | Special_Label}},

		{"bne",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries | Reg_Constant_Value,
		Special_Constant | Reg_Constant_Value | Special_Label}},

		{"lui",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Negligible,
		Special_Constant | Reg_Constant_Value}},

		{"lw",{ Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries,
		Special_Paranthesis | Special_Constant | Reg_Constant_Value,
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
		{"jal",{ Special_Label | Special_Constant}},

		{"j",{ Special_Label | Special_Constant}},

		// pseudo instruction to get output
		{"nout",{Reg_Expression_Evaluation | Reg_Arguments | Reg_Temporaries | Reg_SavedTemporaries | Reg_ExtraTemporaries}}

	}; // pair of instruction and expected arguments

	std::pair<std::string, int> registers[] = {
			{"$zero",Reg_Constant_Value},{"$v0",Reg_Expression_Evaluation},
			{"$t0",Reg_Temporaries}, {"$t1",Reg_Temporaries},{"$t2",Reg_Temporaries},
			{"$s0",Reg_SavedTemporaries}, {"$s1",Reg_SavedTemporaries},{"$ra",Reg_Return_Address}
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
