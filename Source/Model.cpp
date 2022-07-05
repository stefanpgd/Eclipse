#include "precomp.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "stb_image.h"
#include "Model.h"

Model::Model(std::string path)
{
	LoadModel(path);
}

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	unsigned int processFlags = 
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		//aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		0;

	const aiScene* scene = import.ReadFile(path, processFlags);

	// if there is no scene, errors or no root node, throw error
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error("ERROR: Assimp" + (std::string)import.GetErrorString());
	}

	directory = path.substr(0, path.find_last_of('/'));

	if (directory == "Assets/Models")
	{
		directory = path.substr(0, path.find_last_of('\\'));
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes ( if available )
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// For every child node do the same process
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ATexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		AVertex vertex;

		// Process vertex positions, normals and texture coordinates
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// does the mesh contain texture coords?
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords.x = 0.0f;
			vertex.TexCoords.y = 0.0f;
		}

		vertices.push_back(vertex);
	}

	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	vec3 Ambient = vec3(0.0f);
	vec3 Diffuse = vec3(0.0f);
	vec3 Specular = vec3(0.0f);
	float shininess = 1.0f;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<ATexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<ATexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		aiColor3D ambientColor;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
		{
			Ambient.r = ambientColor.r;
			Ambient.g = ambientColor.g;
			Ambient.b = ambientColor.b;
		}

		aiColor3D diffuseColor;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
		{
			Diffuse.r = diffuseColor.r;
			Diffuse.g = diffuseColor.g;
			Diffuse.b = diffuseColor.b;
		}

		aiColor3D specularColor;
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
		{
			Specular.r = specularColor.r;
			Specular.g = specularColor.g;
			Specular.b = specularColor.b;
		}

		float shine;
		if (material->Get(AI_MATKEY_SHININESS, shine) == AI_SUCCESS)
		{
			shininess = shine;
		}
	}
	
	return Mesh(vertices, indices, textures, Ambient, Diffuse, Specular, shininess);
}

std::vector<ATexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<ATexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{   // if texture hasn't been loaded already, load it
			ATexture texture;
			texture.ID = TextureFromFile(str.C_Str(), this->directory);
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

std::vector<glm::vec3> Model::GetAllVertices()
{
	std::vector<vec3> vertices;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		for (unsigned int j = 0; j < meshes[i].Vertices.size(); j++)
		{
			vertices.push_back(meshes[i].Vertices[j].Position);
		}
	}

	return vertices;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + "/" + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}