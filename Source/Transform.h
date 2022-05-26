#pragma once

class Transform
{
public:
	Transform();

	glm::mat4& GetModelMatrix();
	void SetStatic(bool isStatic);

private:
	void UpdateRotationMatrix();

public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

private:
	glm::mat4 model;
	glm::mat4 rotationMatrix;

	bool isStatic = false;
};