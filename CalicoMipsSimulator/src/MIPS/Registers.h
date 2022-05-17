#pragma once

#include <unordered_map>
#include <string.h>
enum class registerStatus {
	free, store, first, second
};
class Register {

public:
	Register() {}
	Register(std::string name, uint8_t number, int value, registerStatus status) :
		m_Name(name), m_Number(number), m_Value(value), m_Status(status) {}
	~Register() {}
public:
	std::string getName() {
		return m_Name;
	}
	uint8_t getNumber() {
		return m_Number;
	}
	int getValue() {
		return m_Value;
	}
	std::reference_wrapper<int> getRef() {
		return std::reference_wrapper<int>(m_Value);
	}
	registerStatus getStatus() {
		return m_Status;
	}
private:
	std::string m_Name;
	uint8_t m_Number;
	int m_Value;
	registerStatus m_Status;
};




