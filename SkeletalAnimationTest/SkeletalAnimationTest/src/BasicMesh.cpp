#include "BasicMesh.h"
#include <iostream>

using namespace std;

#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices )
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

bool BasicMesh::LoadMesh(const string& filename)
{
	// Releasepreviously loaded mesh.
	// TODO: Add Clear() functionality. Delets all data in object.
	//Clear();

	// Create VAO:
	//glGenVertexArrays( 1, &m_VAO );
	//glBindVertexArray( m_VAO);

	// Create the buffer for the vertices attributes:
	// 6 buffer objects are defined in enum BUFFER_TYPE.
	// Storing data in AOS structucture.
	//glGenBuffers( ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);

	if (!pScene)
	{
		printf("Error parsing '%s': '%s'\n", filename, Importer.GetErrorString());
		return 1;
	}
	
	InitFromScene( pScene, filename);

	// Make sure VAO is not changed from the outside.
	//glBindVertexArray(0);

	return 0;
}

BasicMesh::~BasicMesh()
{
	Clear();
}

void BasicMesh::Clear()
{
	if (m_Buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void BasicMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	// Model might be split into sub-components. For each an aiMesh stucture is created.
	// Adjust m_Meshes size.
	m_Meshes.resize(pScene->mNumMeshes);

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;
	unsigned int NumBones = 0;

	// Start filling m_Meshes.
	CountVerticesAndIndices(pScene, NumVertices, NumIndices, NumBones);

	ReserveSpace(NumVertices, NumIndices);

	InitAllMeshes(pScene);

	// Allocate texture.
	//if (!InitMaterials(pScene, filename)) return false;
}

void BasicMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndeces, unsigned int& numBones)
{
	// Read mesh data.
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3; // Triangulated verteces.
		m_Meshes[i].BaseVertex = numVertices;
		m_Meshes[i].MaterialIndex = numIndeces;
		m_Meshes[i].NumBones = pScene->mMeshes[i]->mNumBones ;

		printf(" Mesh %d : VERTICES %d INDICES %d BONES %d \n\n", i, m_Meshes[i].BaseVertex, m_Meshes[i].NumIndices, m_Meshes[i].NumBones);
		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndeces += m_Meshes[i].NumIndices;
		numBones += m_Meshes[i].NumBones;

		if (pScene->mMeshes[i]->HasBones()) {
			// TODO: Resize Bone vector.
			ParseMeshBones(i, pScene->mMeshes[i]);
		}
	}
	printf("\nTotal vertices %d total indices %d total bones %d\n", numVertices, numIndeces, numBones);
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
	printf("Parse %d meshes \n\n", pScene->mNumMeshes);

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

void BasicMesh::ParseMeshBones(unsigned int index, const aiMesh* paiMesh)
{
	for (unsigned int i = 0; i < paiMesh->mNumBones; i++) {
		ParseSingleBone(i, index, paiMesh->mBones[i]);
	}
}

void BasicMesh::ParseSingleBone(unsigned int index, unsigned int meshIndex, const aiBone* pBone)
{
	printf(" Bone %d: '%s' num vertices affected by this bone: %d\n", index, pBone->mName.C_Str(), pBone->mNumWeights);

	Bone bone;

	for (unsigned int i = 0; i < pBone->mNumWeights; i++)
	{
		bone.BoneWeights.push_back(pBone->mWeights[i]);
		printf(" %d: vertex id %d weight %.2f\n", i, pBone->mWeights[i].mVertexId, pBone->mWeights[i].mWeight);
	}
	// TODO: Use reference of bone?
	
	m_Meshes[meshIndex].Bones.push_back(bone);
}
