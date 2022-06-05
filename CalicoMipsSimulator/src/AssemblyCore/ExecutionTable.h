#pragma once
struct ExecutionTable { // Focus on that
	int address;
	std::string instruction;
	std::vector<std::reference_wrapper<int>> datas;
	int immediate;
	std::vector<int> registerNames;
};