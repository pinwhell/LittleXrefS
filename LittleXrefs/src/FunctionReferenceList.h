#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

struct Reference;
struct Function;
struct cs_insn;

struct FunctionReferenceList
{
private:
	std::unordered_map<Function*, std::vector<Reference>> RefsLists;
	std::mutex addMtx;

public:
	FunctionReferenceList();
	~FunctionReferenceList();

	void Print();
	void AddReference(Function* pFunc, cs_insn* pCurrInst, char accessMode = '\0');
};

