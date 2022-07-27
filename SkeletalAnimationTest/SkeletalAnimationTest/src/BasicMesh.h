#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <string>

class BasicMesh
{
public:
	BasicMesh() {};

	//~BasicMesh();

	// Top level entry point for loading process.
	bool LoadMesh(const std::string& filename);

private:

	void InitFromScene(const aiScene* pScene, const std::string& Filename);

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
};