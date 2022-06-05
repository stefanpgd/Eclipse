#include "Precomp.h"
#include "SceneLoader.h"
#include <iostream>
#include <fstream>
#include "Camera.h"
#include "Object.h"

SceneLoader::SceneLoader()
{
}

void SceneLoader::SaveScene(std::string sceneName, std::vector<Object*>& objects)
{
	std::cout << "Saving scene: " << sceneName << std::endl;
	std::ofstream mySaveFile;
	mySaveFile.open("Assets/Scenes/" + sceneName + ".txt", std::fstream::out);
	mySaveFile.clear();
	mySaveFile << objects.size() << "\n";

	for (int i = 0; i < objects.size(); i++)
	{
		mySaveFile << objects[i]->name << "\n";
		mySaveFile << objects[i]->modelFileName << "\n";

		mySaveFile << objects[i]->transform.Position.x << "\n";
		mySaveFile << objects[i]->transform.Position.y << "\n";
		mySaveFile << objects[i]->transform.Position.z << "\n";

		mySaveFile << objects[i]->transform.Rotation.x << "\n";
		mySaveFile << objects[i]->transform.Rotation.y << "\n";
		mySaveFile << objects[i]->transform.Rotation.z << "\n";

		mySaveFile << objects[i]->transform.Scale.x << "\n";
		mySaveFile << objects[i]->transform.Scale.y << "\n";
		mySaveFile << objects[i]->transform.Scale.z << "\n";

		mySaveFile << objects[i]->materialIndex << "\n";
	}

	Camera* camera = Camera::GetInstance();
	mySaveFile << camera->CameraPosition.x << "\n";
	mySaveFile << camera->CameraPosition.y << "\n";
	mySaveFile << camera->CameraPosition.z << "\n";

	mySaveFile << camera->CameraFront.x << "\n";
	mySaveFile << camera->CameraFront.y << "\n";
	mySaveFile << camera->CameraFront.z << "\n";

	mySaveFile << camera->CameraUp.x << "\n";
	mySaveFile << camera->CameraUp.y << "\n";
	mySaveFile << camera->CameraUp.z << "\n";

	mySaveFile << camera->Yaw << "\n";
	mySaveFile << camera->Pitch << "\n";
	mySaveFile << camera->FOV << "\n";
	mySaveFile << camera->CameraMovementSpeed << "\n";
	mySaveFile << camera->CameraTiltSpeed << "\n";
	mySaveFile << camera->CameraMovementSprintSpeed << "\n";

	mySaveFile.close();
}

bool SceneLoader::LoadScene(std::string sceneName, std::vector<Object*>& objects)
{
	std::string line;
	std::ifstream myScene("Assets/Scenes/" + sceneName + ".txt");

	if (myScene.is_open())
	{
		std::getline(myScene, line);
		int objectCount = std::stoi(line);

		for (int i = 0; i < objectCount; i++)
		{
			ObjectSaveData data;
			
			// Object Name
			std::getline(myScene, line);
			data.name = line;

			// Object Model File location/name
			std::getline(myScene, line);
			data.modelFileName = line;

			for (int j = 0; j < 9; j++)
			{
				std::getline(myScene, line);
				data.transformInfo[j] = std::stof(line);
			}

			std::getline(myScene, line);
			data.materialIndex = std::stoi(line);

			Object* object = new Object(data);
			objects.push_back(object);
		}

		Camera* camera = Camera::GetInstance();
		CameraSaveData cameraData;

		// Camera Position
		for (int i = 0; i < 9; i++)
		{
			std::getline(myScene, line);
			cameraData.cameraPositionalInfo[i] = std::stof(line);
		}

		// Camera Settings
		for (int i = 0; i < 6; i++)
		{
			std::getline(myScene, line);
			cameraData.cameraSettings[i] = std::stof(line);
		}

		camera->SetupCameraFromSaveData(cameraData);
		myScene.close();

		return true;
	}

	printf("Warning: Scene file not found\n");
	return false;
}