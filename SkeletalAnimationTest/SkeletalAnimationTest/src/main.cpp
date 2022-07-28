#include "BasicMesh.h"

#include <string>

int main()
{
    BasicMesh* pMesh = new BasicMesh();

    if (!pMesh->LoadMesh("data/testObj.dae")) {
        return false;
    }

    delete pMesh;
	return 0;
}