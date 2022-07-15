#pragma once

struct ATexture;

class Shader;
class Mesh;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
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

public:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<ATexture> texturesLoaded;
};

unsigned int TextureFromFile(const char* path, aiTextureType type, const std::string& directory, bool gamma = false);