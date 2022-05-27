#pragma once

class Mesh;
class aiNode;
class aiMesh;
class aiScene;

class Model
{
public:
	Model(std::string path);

	void Draw();
	std::vector<glm::vec3> GetAllVertices();

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	std::vector<Mesh> meshes;
	std::string directory;
};