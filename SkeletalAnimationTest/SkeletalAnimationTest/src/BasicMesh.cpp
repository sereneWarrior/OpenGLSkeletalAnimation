#include "BasicMesh.h"
#include <iostream>

using namespace std;

#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices )
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

bool BasicMesh::LoadMesh(const string& filename)
{
	// Releasepreviously loaded mesh.
	// TODO: Add Clear() functionality. Delets all data in object.
	// Clear();

	// Create VAO:
	glGenVertexArrays( 1, &m_VAO );
	glBindVertexArray( m_VAO);

	// Create the buffer for the vertices attributes:
	// 6 buffer objects are defined in enum BUFFER_TYPE.
	// Storing data in AOS structucture.
	glGenBuffers( ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);

	if (!pScene)
	{
		printf("Error parsing '%s': '%s'\n", filename, Importer.GetErrorString());
		return 1;
	}
	
	InitFromScene( pScene, filename);

	// Make sure VAO is not changed from the outside.
	glBindVertexArray(0);

	return 0;
}

void BasicMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	return;
}
