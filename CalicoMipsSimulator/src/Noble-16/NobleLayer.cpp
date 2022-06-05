#include "NobleLayer.h"

namespace NobleLayer {
	
	void Noble::InitRegistersMap()
	{

	}

	void Noble::InitInstructionMap()
	{

	}

	std::string Noble::IValidateInput(const std::string& data, const std::string& dataMem)
	{
		return "";
	}

	void Noble::IValidateLabel(const std::string& data, int p_CurrentLine, ErrorFlag& errorFlag)
	{

	}

	void Noble::IValidateInstructions(std::vector <std::string>& data, int p_CurrentLine, ErrorFlag& errorFlag)
	{

	}

	void Noble::IExecute(int pStep)
	{

	}

	bool Noble::ILabelInsert(const std::string label, int address)
	{
		return true;
	}

	bool Noble::ILabelCheck(const std::string label)
	{
		return true;
	}

	std::string Noble::IConstructCResult()
	{
		return "";
	}

}