#pragma once

class Shader
{
public:
	unsigned int ID; // program ID

	Shader(const std::string vertexName, const std::string fragementName);
	~Shader();

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec3(const std::string& name, glm::vec3 vec) const;
	void SetMat4(const std::string& name, glm::mat4 mat4) const;
};