#include "IReferenceEngine.h"
#include "FunctionReferenceList.h"
#include "Function.h"
#include "CapstoneHelper.h"

IReferenceEngine::IReferenceEngine(csh _CapstoneDisasm, uint16_t _abiRegBase)
	: CapstoneHandle(_CapstoneDisasm)
	, abiRegBase(_abiRegBase)
{
}

void IReferenceEngine::FindRefereces(Function* pFunc, const std::string& typeName, uint64_t offset, FunctionReferenceList& outRefsLists)
{
	std::vector<uint16_t> matchingTypenameParamsId;

	if (pFunc->getMatchingTypenameParamsId(typeName, matchingTypenameParamsId))
	{
		cs_insn* pDisasmdInst = nullptr;
		uintptr_t count = 0;

		if ((count = cs_disasm(CapstoneHandle, pFunc->entryMem, 0x1000, (uint64_t)(pFunc->entryMem), 0, &pDisasmdInst)) != 0 && pDisasmdInst)
		{
			cs_insn* pDisasmdInstEnd = pDisasmdInst + count;

			for (uint16_t currParamId : matchingTypenameParamsId)
				FindRefereces(pFunc, pDisasmdInst, pDisasmdInstEnd, (uint16_t)(abiRegBase + currParamId), offset, outRefsLists);

			cs_free(pDisasmdInst, count);
		}
	}
}

ArmReferenceEngine::ArmReferenceEngine(csh CapstoneDisasm)
	: IReferenceEngine(CapstoneDisasm, ARM_REG_R0)
{
}
//arm_reg::ARM_REG_R0

void ArmReferenceEngine::FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists)
{
	for (cs_insn* pCurrInst = pStart; pCurrInst < pEnd; pCurrInst++)
	{
		if (ArmCapstoneHelper::HeuristicReturn(pCurrInst)) break;

		switch (pCurrInst->id)
		{
		case ARM_INS_LDR:
		case ARM_INS_LDRB:
		case ARM_INS_LDRBT:
		case ARM_INS_LDRD:
		{
			uint16_t lreg = ArmCapstoneHelper::GetLValueRegType(pCurrInst);
			uint16_t rreg = ArmCapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg)
			{
				uintptr_t disp = pCurrInst->detail->arm.operands[1].mem.disp;

				if (disp == offset) // LDR? R?, [trackReg, #?? == offset?]
					outRefsLists.AddReference(pFunc, pCurrInst, 'r');

			} else if(lreg == trackReg) goto END_FIND;	// Register that contained instance of offset was overriden, 
									// no more work to do
			break;
		}

		case ARM_INS_STR:
		case ARM_INS_STRB:
		case ARM_INS_STRBT:
		case ARM_INS_STRD:
		{
			uint16_t rreg = ArmCapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg)
			{
				uintptr_t disp = pCurrInst->detail->arm.operands[1].mem.disp;

				if (disp == offset) // LDR? R?, [trackReg, #?? == offset?]
					outRefsLists.AddReference(pFunc, pCurrInst, 'w');

			}

			break;
		}

		case ARM_INS_MOV:
		{
			uint16_t lreg = ArmCapstoneHelper::GetLValueRegType(pCurrInst);
			uint16_t rreg = ArmCapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg) FindRefereces(pFunc, pCurrInst + 1, pEnd, lreg, offset, outRefsLists);
			else if (lreg == trackReg) goto END_FIND;	// Again Register that contained instance of offset was overriden, 
									// no more work to do

			break;
		}


		}
	}
END_FIND:
	return;
}

Arm64ReferenceEngine::Arm64ReferenceEngine(csh CapstoneDisasm)
	: IReferenceEngine(CapstoneDisasm, ARM64_REG_X0)
{

}

void Arm64ReferenceEngine::FindRefereces(Function* pFunc, cs_insn* pStart, cs_insn* pEnd, uint16_t trackReg, uint64_t offset, FunctionReferenceList& outRefsLists)
{
	for (cs_insn* pCurrInst = pStart; pCurrInst < pEnd; pCurrInst++)
	{
		if (Arm64CapstoneHelper::HeuristicReturn(pCurrInst)) break;

		switch (pCurrInst->id)
		{

		case ARM64_INS_LDR:
		case ARM64_INS_LDRB:
		{
			uint16_t lreg = Arm64CapstoneHelper::GetLValueRegType(pCurrInst);
			uint16_t rreg = Arm64CapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg)
			{
				uintptr_t disp = pCurrInst->detail->arm64.operands[1].mem.disp;

				if (disp == offset) // LDR? R?, [trackReg, #?? == offset?]
					outRefsLists.AddReference(pFunc, pCurrInst, 'r');

			}
			else if (lreg == trackReg) goto END_FIND;	// Register that contained instance of offset was overriden, 
								 // no more work to do
			break;
		}

		case ARM64_INS_STR:
		case ARM64_INS_STRB:
		{
			uint16_t rreg = Arm64CapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg)
			{
				uintptr_t disp = pCurrInst->detail->arm64.operands[1].mem.disp;

				if (disp == offset) // LDR? X?, [trackReg, #?? == offset?]
					outRefsLists.AddReference(pFunc, pCurrInst, 'w');

			}

			break;
		}

		case ARM64_INS_MOV:
		{
			uint16_t lreg = Arm64CapstoneHelper::GetLValueRegType(pCurrInst);
			uint16_t rreg = Arm64CapstoneHelper::GetRValueRegType(pCurrInst);

			if (trackReg == rreg) FindRefereces(pFunc, pCurrInst + 1, pEnd, lreg, offset, outRefsLists);
			else if (lreg == trackReg) goto END_FIND;	// Again Register that contained instance of offset was overriden, 
														// no more work to do

			break;
		}


		}
	}
END_FIND:
	return;
}
