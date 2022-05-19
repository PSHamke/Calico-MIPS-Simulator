#pragma once
#include <vector>
#include "Log.h"
class Memory {
public:
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
	static std::vector <int >& GetDataMemory() {
		return Get().IGetDataMemory();
	}

	static int HandleTextMemoryByte(unsigned int mCurrentMemoryRow, unsigned int mCurrentMemoryCol, bool& mIsValid) {
		return Get().IHandleTextMemoryByte(mCurrentMemoryRow, mCurrentMemoryCol, mIsValid);
	}
	static int HandleTextMemoryBit(unsigned int mCurrentMemoryRow, unsigned int mCurrentMemoryCol, bool& mIsValid) {
		return Get().IHandleTextMemoryBit(mCurrentMemoryRow, mCurrentMemoryCol, mIsValid);
	}
	static int HandleDataMemory(unsigned int mMemoryRow, unsigned int mMemoryCol) {
		return Get().IHandleDataMemory(mMemoryRow, mMemoryCol);
	}
	static const char* GetAscii(unsigned int mMemoryRow) {
		return Get().IGetAscii(mMemoryRow);
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
	int IHandleDataMemory(unsigned int mMemoryRow, unsigned int mMemoryColumn) {

	}
	int IHandleTextMemoryByte(unsigned int mCurrentMemoryRow, unsigned int  mCurrentMemoryCol, bool &mIsValid) {
		if ((mCurrentMemoryRow * 4 + (mCurrentMemoryCol - 1) / 4) < m_TextMemory.size()) {
			mIsValid = true;
			return IGetnThByte(m_TextMemory.at((int)(mCurrentMemoryRow * 16 + mCurrentMemoryCol - 1) / 4), (mCurrentMemoryCol - 1) % 4);
			
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

	std::vector <int >& IGetDataMemory() {
		return m_DataMemory;
	}
private:
	
	std::vector <int> m_TextMemory;
	std::vector <int> m_DataMemory;
};