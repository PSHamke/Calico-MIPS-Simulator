#pragma once
#include <vector>
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

	static int HandleTextMemory(unsigned int mCurrentMemoryRow, unsigned int mCurrentMemoryCol, bool& mIsValid) {
		return Get().IHandleTextMemory(mCurrentMemoryRow, mCurrentMemoryCol, mIsValid);
	}
	static int HandleDataMemory(unsigned int mMemoryRow, unsigned int mMemoryCol) {
		return Get().IHandleDataMemory(mMemoryRow, mMemoryCol);
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
	int IHandleTextMemory(unsigned int mCurrentMemoryRow, unsigned int  mCurrentMemoryCol, bool &mIsValid) {
		if ((mCurrentMemoryRow * 4 + (mCurrentMemoryCol - 1) / 4) < m_TextMemory.size()) {
			mIsValid = true;
			return IGetnThByte(m_TextMemory.at((int)(mCurrentMemoryRow * 16 + mCurrentMemoryCol - 1) / 4), (mCurrentMemoryCol - 1) % 4);
			
		}
		mIsValid = false;
		return 0;
	}
	int IGetnThByte(int source, int n ) {
		return (source >> (8 * n) & 0xff);
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