#include "precomp.h"
#include "Camera.h"

void Camera::Initialize(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch)
{
	if (!isInitialized)
	{
		CameraPosition = position;
		CameraFront = front;
		CameraUp = up;
		Yaw = yaw;
		Pitch = pitch;

		projectionMatrix = glm::perspective(glm::radians(FOV), (float)ScreenWidth / (float)ScreenHeight, Near, Far);
		isInitialized = true;
	}
}

void Camera::Update(float deltaTime)
{
	this->deltaTime = deltaTime;
	viewMatrix = glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
}

glm::mat4& Camera::GetViewMatrix()
{
	return viewMatrix;
}

glm::mat4& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::ProcessKeyboard(CameraDirection direction)
{
	float velocity = Sprinting ? CameraMovementSprintSpeed : CameraMovementSpeed;
	velocity *= deltaTime;

	switch (direction)
	{
	case CameraDirection::Front:
		CameraPosition += CameraFront * velocity;
		break;

	case CameraDirection::Back:
		CameraPosition -= CameraFront * velocity;
		break;

	case CameraDirection::Left:
		CameraPosition -= glm::normalize(glm::cross(CameraFront, CameraUp)) * velocity;
		break;

	case CameraDirection::Right:
		CameraPosition += glm::normalize(glm::cross(CameraFront, CameraUp)) * velocity;
		break;
	}
}

void Camera::ProcessMouseMovement(double xPos, double yPos, bool constrainPitch)
{
	float xpos = static_cast<float>(xPos);
	float ypos = static_cast<float>(yPos);

	if (FirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		FirstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	float speed = CameraTiltSpeed * deltaTime;
	xOffset *= speed;
	yOffset *= speed;

	Yaw += xOffset;
	Pitch += yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	CameraFront = glm::normalize(front);
}

void Camera::ProcessMouseScroll(float yOffset)
{
	FOV -= (float)yOffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 45.0f)
		FOV = 45.0f;

	projectionMatrix = glm::perspective(glm::radians(FOV), (float)ScreenWidth / (float)ScreenHeight, Near, Far);
}

void Camera::SetupCameraFromSaveData(CameraSaveData& data)
{
	CameraPosition.x = data.cameraPositionalInfo[0];
	CameraPosition.y = data.cameraPositionalInfo[1];
	CameraPosition.z = data.cameraPositionalInfo[2];

	CameraFront.x = data.cameraPositionalInfo[3];
	CameraFront.x = data.cameraPositionalInfo[4];
	CameraFront.x = data.cameraPositionalInfo[5];

	CameraUp.x = data.cameraPositionalInfo[6];
	CameraUp.x = data.cameraPositionalInfo[7];
	CameraUp.x = data.cameraPositionalInfo[8];

	Yaw = data.cameraSettings[0];
	Pitch = data.cameraSettings[1];
	FOV = data.cameraSettings[2];

	CameraMovementSpeed = data.cameraSettings[3];
	CameraTiltSpeed = data.cameraSettings[4];
	CameraMovementSprintSpeed = data.cameraSettings[5];

	ProcessMouseMovement(0, 0);
}