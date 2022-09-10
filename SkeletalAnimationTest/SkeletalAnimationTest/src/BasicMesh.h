#include "Math.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>

class BasicMesh
{
public:
	BasicMesh() {};

	~BasicMesh();

	// Top level entry point for loading process.
	bool LoadMesh(const std::string& filename);

private:

	void Clear();

	void InitFromScene(const aiScene* pScene, const std::string& Filename);

	void CountVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndeces, unsigned int& numBones);

	void ReserveSpace(unsigned int& numVertices, unsigned int& numIndeces);

	void InitAllMeshes(const aiScene* pScene);

	// Populate vector with vertex and index data.
	void InitSingleMesh(const aiMesh* paiMesh);

	void ParseMeshBones(unsigned int index, const aiMesh* paiMesh);

	void ParseSingleBone(unsigned int index, unsigned int meshIndex, const aiBone* pBone);

	//bool InitMaterials(const aiScene* pScene, const std::string filename);

	// Content of m_buffer.
	enum BUFFER_TYPE {
		INDEX_BUFFER = 0,
		POS_VB = 1,
		TEXCOORD_VB = 2,
		NORMAL_VB = 3,
		WVP_MAT_VB = 4,  // required only for instancing
		WORLD_MAT_VB = 5,  // required only for instancing
		NUM_BUFFERS = 6
	};

	GLuint m_VAO = 0;
	GLuint m_Buffers[NUM_BUFFERS] = { 0 };

	struct Bone {

		std::vector<aiVertexWeight> BoneWeights; // One bone can manipulate multiple vertecies.
	};

	
	struct BasicMeshEntry {
		BasicMeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0; 
			MaterialIndex = 0;// TODO: INVALID__MATERIAL
			NumBones = 0;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
		unsigned int NumBones;
		std::vector<Bone> Bones;
	};

	std::vector<BasicMeshEntry> m_Meshes;

	// Temporary space for vertex stuff before we load them into the GPU
	std::vector<Vector3f> m_Positions;
	std::vector<Vector3f> m_Normals;
	std::vector<Vector2f> m_TexCoords;
	std::vector<unsigned int> m_Indices;
};