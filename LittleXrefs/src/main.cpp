#include "ILXTool.h"
#include "FunctionReferenceList.h"
#include "Function.h"
#include <iostream>
#include "IReferenceEngine.h"
#include "Function.h"
#include "CapstoneHelper.h"
#include <capstone/capstone.h>


int main()
{
	if (LX::Init() != LX::LX_OK)
	{
		printf("Error while initializing LittleXrefs\n");
		exit(1);
	}

	ILXTool* pIlxTool = new LXARM64Tool(LX::g_pLXrefs);

	pIlxTool->ParseAllFunction();
	pIlxTool->Run();
}