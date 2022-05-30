#pragma once

struct ATexture;

class Shader;
class Mesh;
class aiNode;
class aiMesh;
class aiScene;
class aiMaterial;
enum aiTextureType;

class Model
{
public:
	Model(std::string path);

	void Draw(Shader* shader);
	std::vector<glm::vec3> GetAllVertices();

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<ATexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<ATexture> texturesLoaded;
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);