#include "HelperString.h"


bool is_number(const std::string& s)
{
	
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
std::vector<std::string> TokenizeMyString(const std::string& s, const std::string& del)
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
		CL_CORE_INFO("Token = {}", token);
		res.push_back(token);
	}
	return res;
}