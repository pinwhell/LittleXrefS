#include "ILXTool.h"
#include "FunctionReferenceList.h"

int main()
{
	if (LX::Init() != LX::LX_OK)
	{
		printf("Error while initializing LittleXrefs\n");
		exit(1);
	}

	ILXTool* pIlxTool = new ILXARMTool(LX::g_pLXrefs);

	FunctionReferenceList funcRefsLists;
	pIlxTool->FindReferences("PlayerControl", 0x28, funcRefsLists);
	funcRefsLists.Print();
	
}