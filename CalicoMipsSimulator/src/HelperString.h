#pragma once
#include <string>
#include <algorithm>
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