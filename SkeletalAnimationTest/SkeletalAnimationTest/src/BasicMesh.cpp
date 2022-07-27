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
	// Model might be split into sub-components. For each an aiMesh stucture is created.
	m_Meshes.resize(pScene->mNumMeshes);

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	CountVerticesAndIndices(pScene, NumVertices, NumIndices);

	ReserveSpace(NumVertices, NumIndices);

	InitAllMeshes(pScene);
}

void BasicMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndeces)
{
	// Read mesh data.
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3; // Triangulated verteces.
		m_Meshes[i].BaseVertex = numVertices;
		m_Meshes[i].MaterialIndex = numIndeces;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndeces += m_Meshes[i].NumIndices;
	}
}

void BasicMesh::ReserveSpace(unsigned int& numVertices, unsigned int& numIndeces)
{
	// Allocate space.
	m_Positions.reserve(numVertices);
	m_Normals.reserve(numVertices);
	m_TexCoords.reserve(numVertices);
	m_Indices.reserve(numVertices);
}

void BasicMesh::InitAllMeshes(const aiScene* pScene)
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitSingleMesh(paiMesh);
	}
}

void BasicMesh::InitSingleMesh(const aiMesh* paiMesh)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate vertex attribute vector.
	for ( unsigned int i = 0; i < paiMesh->mNumVertices; i++ )
	{
		const aiVector3D& pPos = paiMesh->mVertices[i];
		const aiVector3D& pNormal = paiMesh->mNormals[i];
		const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;

		m_Positions.push_back(Vector3f(pPos.x, pPos.y, pPos.z));
		m_Normals.push_back(Vector3f(pNormal.x, pNormal.y, pNormal.z));
		m_TexCoords.push_back(Vector2f(pTexCoord.x, pTexCoord.y));
	}

	// Populate index data vector (buffer).
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		//assert(face.mNumIndices == 3);
		m_Indices.push_back(face.mIndices[0]);
		m_Indices.push_back(face.mIndices[1]);
		m_Indices.push_back(face.mIndices[2]);
	}
}
