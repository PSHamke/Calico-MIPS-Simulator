#pragma once
#include "vector"
#include "string"
#define BIT(X) 1<<X
namespace MIPSLayer {
	enum {
		Reg_Constant_Value = BIT(0),
		Reg_Assembler_Temporary = BIT(1),
		Reg_Expression_Evaluation = BIT(2),
		Reg_Arguments = BIT(3),
		Reg_Temporaries = BIT(4),
		Reg_SavedTemporaries = BIT(5),
		Reg_ExtraTemporaries = BIT(6),
		Reg_OS_Reserved = BIT(7),
		Reg_Global_Pointers = BIT(8),
		Reg_Stack_Pointers = BIT(9),
		Reg_Frame_Pointer = BIT(10),
		Reg_Return_Address = BIT(11),
		Special_Shamt = BIT(12),
		Special_Constant = BIT(13),
		Special_Negligible= BIT(14),
		Reg_Error = BIT(15)
	};
	std::vector<int>& InstructionInfo(const std::string& token);
	int RegisterInfo(const std::string& token);
		
}