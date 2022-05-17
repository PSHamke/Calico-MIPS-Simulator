#include "Registers.h"
RegisterInfos* Register::GetRegisterInfos()
{
	RegisterInfos* registerInfos = new RegisterInfos();
	registerInfos->push_back(std::make_pair("$zero", "The Constant Value 0"));
	registerInfos->push_back(std::make_pair("$at", "The Assembler Temporary Register"));
	registerInfos->push_back(std::make_pair("$v0", "Values for Function Results and Expression Evaluation"));
	registerInfos->push_back(std::make_pair("$v1", "Values for Function Results and Expression Evaluation"));
	registerInfos->push_back(std::make_pair("$a0", "Function Arguments"));
	registerInfos->push_back(std::make_pair("$a1", "Function Arguments"));
	registerInfos->push_back(std::make_pair("$a2", "Function Arguments"));
	registerInfos->push_back(std::make_pair("$a3", "Function Arguments"));
	registerInfos->push_back(std::make_pair("$t0", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t1", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t2", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t3", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t4", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t5", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t6", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t7", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$s0", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s1", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s2", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s3", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s4", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s5", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s6", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$s7", "Saved Temporary Register"));
	registerInfos->push_back(std::make_pair("$t8", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$t9", "Temporary Register"));
	registerInfos->push_back(std::make_pair("$k0", "Kernel Temporary Register"));
	registerInfos->push_back(std::make_pair("$k1", "Kernel Temporary Register"));
	registerInfos->push_back(std::make_pair("$gp", "Global Pointer"));
	registerInfos->push_back(std::make_pair("$sp", "Stack Pointer"));
	registerInfos->push_back(std::make_pair("$fp", "Frame Pointer"));
	registerInfos->push_back(std::make_pair("$ra", "Return Address"));
	return registerInfos;
}