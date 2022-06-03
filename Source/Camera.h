#pragma once

enum class CameraDirection
{
    Front,
    Back,
    Left,
    Right
};

class Camera
{
public: 
    static Camera* GetInstance()
    {
        static Camera instance;
        return &instance;
    }

	void Initialize(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw = -90.0f, float pitch = 0.0f);
    
    void Update(float deltaTime);

    glm::mat4& GetViewMatrix();
    glm::mat4& GetProjectionMatrix();

    void ProcessKeyboard(CameraDirection direction);
    void ProcessMouseMovement(double xPos, double yPos, bool constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

public:
    float deltaTime;

    glm::vec3 CameraPosition;
    glm::vec3 CameraFront;
    glm::vec3 CameraUp;

    // euler Angles
    float Yaw;
    float Pitch;
    float FOV = 45.0f;
    
    // camera options
    float CameraMovementSpeed = 2.5f;
    float CameraTiltSpeed = 1.0f;

    // Projection
    float Near = 0.1f;
    float Far = 5000.0f;
    bool FirstMouse = true;

private:
    bool isInitialized = false;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // Camera movement 
    float lastX = 0.0f;
    float lastY = 0.0f;
};