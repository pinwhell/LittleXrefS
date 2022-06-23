#pragma once

#include <capstone/capstone.h>
#include <string>

struct Function;
struct FunctionReferenceList;

struct ReferenceEngine
{
	csh CapstoneHandle;

	ReferenceEngine(csh CapstoneDisasm);
	virtual void FindRefereces(Function* pFunc, const std::string& typeName, uint64_t offset, FunctionReferenceList& outRefsLists) = 0; // (candidateFunc, className, offset, ppOutReferenceList)
};

struct ArmReferenceEngine : ReferenceEngine {
	ArmReferenceEngine(csh CapstoneDisasm);

	void FindRefereces(Function* pFunc, const std::string& typeName, uint64_t offset, FunctionReferenceList& outRefsLists) override;
	void FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists);
	void FindRefereces(unsigned char* pEntry, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists);
};

