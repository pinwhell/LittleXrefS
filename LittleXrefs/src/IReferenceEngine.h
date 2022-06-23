#pragma once

#include <capstone/capstone.h>
#include <string>

struct Function;
struct FunctionReferenceList;

struct IReferenceEngine
{
	csh CapstoneHandle;
	uint16_t abiRegBase;

	IReferenceEngine(csh CapstoneDisasm, uint16_t abiRegBase);

	virtual void FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists) = 0;
	void FindRefereces(Function* pFunc, const std::string& typeName, uint64_t offset, FunctionReferenceList& outRefsLists);
};

struct ArmReferenceEngine : IReferenceEngine {
	ArmReferenceEngine(csh CapstoneDisasm);

	void FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists) override;
};

struct Arm64ReferenceEngine : IReferenceEngine {
	Arm64ReferenceEngine(csh CapstoneDisasm);

	void FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists) override;
};

