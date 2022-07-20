#pragma once
struct ExecutionTable { // Focus on that
	int m_Address;
	std::string m_Instruction;
	std::vector<std::reference_wrapper<int>> m_Datas;
	int m_Immediate;
	std::vector<int> m_RegisterNames;
	std::string m_InputLine;
	int m_EditorLine;
};