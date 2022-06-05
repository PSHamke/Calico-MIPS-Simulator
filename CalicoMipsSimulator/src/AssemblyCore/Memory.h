#pragma once
#include <vector>
#include "Log.h"
#include "unordered_map"
class Memory {
public:
	typedef std::pair<std::string, int> MemoryRegion;
	
	Memory(const Memory&) = delete;

	static Memory& Get() {
		static Memory instance;
		return instance;
	}

	static void FreeTextMemory() {
		Get().IFreeTextMemory();
	}

	static void FreeDataMemory() {
		Get().IFreeDataMemory();
	}

	static std::vector <int >& GetTextMemory() {
		return Get().IGetTextMemory();
	}
	static std::unordered_map < unsigned int, MemoryRegion>& GetDataMemory() {
		return Get().IGetDataMemory();
	}

	static int HandleMemoryByte(unsigned int aCurrentMemoryRow, unsigned int aCurrentMemoryCol,unsigned int aMemoryType, bool& aIsValid) {
		if (aMemoryType == 0) { // Text Memory
			return Get().IHandleTextMemoryByte(aCurrentMemoryRow, aCurrentMemoryCol, aIsValid);
		}
		else {
			return Get().IHandleDataMemoryByte(aCurrentMemoryRow, aCurrentMemoryCol, aIsValid);
		}
			
	}
	static int HandleMemoryBit(unsigned int aCurrentMemoryRow, unsigned int aCurrentMemoryCol,unsigned int aMemoryType, bool& aIsValid) {
		if (aMemoryType == 0) { // Text Memory
			return Get().IHandleTextMemoryBit(aCurrentMemoryRow, aCurrentMemoryCol, aIsValid);
		}
		else {
			return Get().IHandleDataMemoryBit(aCurrentMemoryRow, aCurrentMemoryCol, aIsValid);
		}
		
	}
	static int HandleDataMemory(unsigned int address,unsigned int n, bool& check)  {
		return Get().IHandleDataMemoryByte(address,n,check);
	}
	static bool DataMemoryInsert(std::string label, int value, unsigned int address) {
		return Get().IDataMemoryInsert(label, value, address);
	}
	static int GetDataMemoryValue(unsigned int address, bool &check) {
		return Get().IGetDataMemoryValue(address, check);
	}
	
	static int GetPC() {
		return Get().IGetPC();
	}
	
	static void SetPC(int value) {
		return Get().ISetPC(value);
	}

	static const char* GetAscii(unsigned int mMemoryRow) {
		return Get().IGetAscii(mMemoryRow);
	}

	static void SetTextMemory() {
		
	}
	static int GetVirtualPC() {
		return Get().IGetVirtualPC();
	}
	static void SetVirtualPC(int value) {
		return Get().ISetVirtualPC(value);
	}

	static int GetCallingReason() {
		return Get().IGetCallingReason();
	}
	static void SetCallingReason(int callingReason) {
		return Get().ISetCallingReason(callingReason);
	}
private:
	


	Memory() {

	}

	void IFreeTextMemory() {
		m_TextMemory.clear();
	}
	void IFreeDataMemory() {
		m_DataMemory.clear();
	}
	int IHandleDataMemoryByte(unsigned int aCurrentMemoryRow, unsigned int  aCurrentMemoryCol, bool& aIsValid) {
		int address = (aCurrentMemoryRow * 4 + (aCurrentMemoryCol - 1) / 4);
		if (m_DataMemory.find(address) != m_DataMemory.end()) {
			aIsValid = true;
			return IGetnThByte(m_DataMemory[address].second, (aCurrentMemoryCol - 1) % 4) ;
		}
		else {
			aIsValid = false;
			return 0;
		}
		
	}
	int IHandleDataMemoryBit(unsigned int aCurrentMemoryRow, unsigned int  aCurrentMemoryCol, bool& aIsValid) {
		int address = aCurrentMemoryRow;
		if (m_DataMemory.find(address) != m_DataMemory.end()) {
			aIsValid = true;
			return IGetnThBit(m_DataMemory[address].second, (aCurrentMemoryCol) % 32);
		}
		else {
			aIsValid = false;
			return 0;
		}
	}
	int IGetDataMemoryValue(unsigned int address, bool &check){
		if (m_DataMemory.find(address) != m_DataMemory.end()) {
			check = true;
			return m_DataMemory[address].second;
		}
		else {
			check = false;
			return 0;
		}
	}
	int IDataMemoryInsert(std::string label, int value, unsigned int address) {
		if (m_DataMemory.find(address) != m_DataMemory.end()) {
			return false;
		}
		else {
			m_DataMemory[address] = std::make_pair(label,value);
			return true;
		}
	}
	int IHandleTextMemoryByte(unsigned int mCurrentMemoryRow, unsigned int  mCurrentMemoryCol, bool &mIsValid) {
		if ((mCurrentMemoryRow * 4 + (mCurrentMemoryCol - 1) / 4) < m_TextMemory.size()) {
			mIsValid = true;
			return IGetnThByte(m_TextMemory.at((int)(mCurrentMemoryRow * 4 + mCurrentMemoryCol - 1) / 4), (mCurrentMemoryCol - 1) % 4);
			
		}
		mIsValid = false;
		return 0;
	}
	int IHandleTextMemoryBit(unsigned int mCurrentMemoryRow, unsigned int  mCurrentMemoryCol, bool& mIsValid) {
		if ((mCurrentMemoryRow) < m_TextMemory.size()) {
			mIsValid = true;
			return IGetnThBit(m_TextMemory.at(mCurrentMemoryRow), (mCurrentMemoryCol) % 32);
		}
		mIsValid = false;
		return 0;
	}
	int IGetnThByte(int source, int n ) {
		return (source >> (8 * n) & 0xff);
	}
	int IGetnThBit(int source, int n) {

		return ((source >> n) & 1);
	}
	const char* IGetAscii(unsigned int mMemoryRow) {
		char mAscii[5] = "....";
		if ((mMemoryRow) < m_TextMemory.size()) {
			for (int i= 0; i < 4; i++) {
				mAscii[i] = (char)IGetnThByte(m_TextMemory.at(mMemoryRow), i) >= 32 ? (char)IGetnThByte(m_TextMemory.at(mMemoryRow), i) : '.';
			}
			mAscii[4] = '\0';
		}
		return mAscii;
	}
	std::vector <int >& IGetTextMemory () {
		return m_TextMemory;
	}

	std::unordered_map <unsigned int, MemoryRegion>& IGetDataMemory() {
		return m_DataMemory;
	}
	
	int IGetPC() {
		return m_PC;
	}
	int IGetVirtualPC() {
		return m_VirtualPC;
	}
	
	void ISetPC(int value) {
		m_PC = value;
	}
	void ISetVirtualPC(int value) {
		m_VirtualPC = value;
	}
	void ISetCallingReason(int callingReason) {
		m_CallingReason = callingReason;
	}
	int IGetCallingReason() {
		return m_CallingReason;
	}
private:
	
	std::vector <int> m_TextMemory;
	std::unordered_map <unsigned int, MemoryRegion> m_DataMemory;
	int m_PC;
	int m_VirtualPC;
	int m_CallingReason;
	
};