#include "ILXTool.h"
#include "FunctionReferenceList.h"

int main()
{
	if (LX::Init() != LX::LX_OK)
	{
		printf("Error while initializing LittleXrefs\n");
		exit(1);
	}

	ILXTool* pIlxTool = new LXARMTool(LX::g_pLXrefs);

	pIlxTool->Run();
}