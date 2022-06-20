#pragma once
#include <string>
#include <algorithm>
#include <regex>
#include "Log.h"
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
bool is_number(const std::string& s);
std::vector<std::string> TokenizeMyString(const std::string& s, const std::string& del);
// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	bool prev_is_space = true;
	std::replace(s.begin(), s.end(), '\t', ' ');
	s.erase(std::remove_if(s.begin(), s.end(), [&prev_is_space](unsigned char curr) {
		bool r = std::isspace(curr) && prev_is_space;
		prev_is_space = std::isspace(curr);
		return r;

		}), s.end());

	return ltrim(rtrim(s, t), t);
}

inline std::string& charCheck(std::string& s, bool& check, char t = ',') {

	int actualSize = s.length();
	bool prev_is_space = false;

	s.erase(std::remove_if(s.begin(), s.end(), [&prev_is_space, &t](unsigned char curr) {
		bool r = curr == t && prev_is_space;
		prev_is_space = curr == t;
		return r;
		}), s.end());


	check = actualSize == s.length();
	return s;
}

inline int AnalyzeString(std::string& data, bool& check) {
	
	enum NumericType {
		None,
		HEX,
		BIN,
		DEC,
		OCT,
		UNKNOWN
	};
	trim(data);
	std::vector < std::pair<std::regex, NumericType>> reg_regex;
	reg_regex.push_back(std::make_pair(std::regex("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?"), NumericType::HEX));
	reg_regex.push_back(std::make_pair(std::regex("0[bB][0-1]+[uU]?[lL]?[lL]?"), NumericType::BIN));
	reg_regex.push_back(std::make_pair(std::regex("[+-]?[0-9]+[Uu]?[lL]?[lL]?"), NumericType::DEC));
	

	NumericType  mType = None;
	
	for (auto& reg : reg_regex) {
		std::smatch match;
		if (std::regex_match(data, match, reg.first)) {
			check = true;
			mType = reg.second;

		}
	}
	if (check) {
		switch (mType) {
		case None:
			CL_CORE_INFO ("None type error");
			return 0;
			break;
		case HEX:
			return std::stol(data, nullptr, 16);
			break;
		case BIN:
			data.erase(0, 2);
			return std::stol(data, nullptr, 2);
			break;
		case DEC:
			return std::stol(data);
			break;
		default:
			return 0;
			break;
		}
	}
	return 0;
}