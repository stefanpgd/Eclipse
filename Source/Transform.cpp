#include "precomp.h"
#include "Transform.h"

Transform::Transform()
{
	Position = vec3(0.0f);
	Rotation = vec3(0.0f);
	Scale = vec3(1.0f);

	model = mat4(1.0f);
	rotationMatrix = mat4(1.0f);
}

glm::mat4& Transform::GetModelMatrix()
{
	if (!isStatic)
	{
		UpdateRotationMatrix();

		model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model *= rotationMatrix;
		model = glm::scale(model, Scale);
	}

	return model;
}

void Transform::SetStatic(bool isStatic)
{
	if (isStatic)
	{
		model = GetModelMatrix();
	}

	this->isStatic = isStatic;
}

void Transform::UpdateRotationMatrix()
{
	glm::mat4 rotX = glm::mat4(1.0f);
	glm::mat4 rotY = glm::mat4(1.0f);
	glm::mat4 rotZ = glm::mat4(1.0f);

	float radX = glm::radians(Rotation.x);
	float sinX = sinf(radX);
	float cosX = cosf(radX);

	rotX[1][1] = cosX;
	rotX[2][1] = -sinX;
	rotX[1][2] = sinX;
	rotX[2][2] = cosX;

	float radY = glm::radians(Rotation.y);
	float sinY = sinf(radY);
	float cosY = cosf(radY);

	rotY[0][0] = cosY;
	rotY[2][0] = sinY;
	rotY[0][2] = -sinY;
	rotY[2][2] = cosY;

	float radZ = glm::radians(Rotation.z);
	float sinZ = sinf(radZ);
	float cosZ = cosf(radZ);

	rotZ[0][0] = cosZ;
	rotZ[1][0] = -sinZ;
	rotZ[0][1] = sinZ;
	rotZ[1][1] = cosZ;

	rotationMatrix = rotX * rotY * rotZ;
}