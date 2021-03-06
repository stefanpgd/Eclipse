#include "Precomp.h"
#include "SceneLoader.h"
#include <iostream>
#include <fstream>
#include "Camera.h"
#include "Object.h"
#include "Editor.h"
#include "Light.h"

void SceneLoader::SaveScene(std::string sceneName, std::vector<Object*>& objects, std::vector<Light*>& lights)
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

	mySaveFile << lights.size() << "\n";
	for (int i = 0; i < lights.size(); i++)
	{
		mySaveFile << lights[i]->Name << "\n";
		mySaveFile << lights[i]->IsPointLight << "\n";

		mySaveFile << lights[i]->Intensity << "\n";
		mySaveFile << lights[i]->LinearFalloff << "\n";
		mySaveFile << lights[i]->ExponentialFalloff << "\n";

		mySaveFile << lights[i]->Position.x << "\n";
		mySaveFile << lights[i]->Position.y << "\n";
		mySaveFile << lights[i]->Position.z << "\n";

		mySaveFile << lights[i]->Color.x << "\n";
		mySaveFile << lights[i]->Color.y << "\n";
		mySaveFile << lights[i]->Color.z << "\n";

		mySaveFile << lights[i]->GlobalLightRotation.x << "\n";
		mySaveFile << lights[i]->GlobalLightRotation.y << "\n";
		mySaveFile << lights[i]->GlobalLightRotation.z << "\n";

		mySaveFile << lights[i]->AmbientAmount.x << "\n";
		mySaveFile << lights[i]->AmbientAmount.y << "\n";
		mySaveFile << lights[i]->AmbientAmount.z << "\n";

		mySaveFile << lights[i]->DiffuseAmount.x << "\n";
		mySaveFile << lights[i]->DiffuseAmount.y << "\n";
		mySaveFile << lights[i]->DiffuseAmount.z << "\n";
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

	Editor* editor = Editor::GetInstance();

	mySaveFile << editor->showSceneObjects << "\n";
	mySaveFile << editor->showObjectCreation << "\n";
	mySaveFile << editor->showCameraSettings << "\n";
	mySaveFile << editor->showObjectDetails << "\n";
	mySaveFile << editor->showStatistics << "\n";
	mySaveFile << editor->showGizmos << "\n";
	mySaveFile << editor->showConsole << "\n";
	mySaveFile << editor->showSceneLights << "\n";

	mySaveFile.close();
}

bool SceneLoader::LoadScene(std::string sceneName, std::vector<Object*>& objects, std::vector<Light*>& lights)
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

		std::getline(myScene, line);
		int lightCount = std::stoi(line);

		for (int i = 0; i < lightCount; i++)
		{
			Light* light = new Light();
			std::getline(myScene, line);
			light->Name = line;

			std::getline(myScene, line);
			light->IsPointLight = std::stoi(line);

			std::getline(myScene, line);
			light->Intensity = std::stof(line);

			std::getline(myScene, line);
			light->LinearFalloff = std::stof(line);

			std::getline(myScene, line);
			light->ExponentialFalloff = std::stof(line);

			vec3 data[5];
			for (int i = 0; i < 5; i++)
			{
				std::getline(myScene, line);
				data[i].x = std::stof(line);
				std::getline(myScene, line);
				data[i].y = std::stof(line);
				std::getline(myScene, line);
				data[i].z = std::stof(line);
			}

			light->Position = data[0];
			light->Color = data[1];
			light->GlobalLightRotation = data[2];
			light->AmbientAmount = data[3];
			light->DiffuseAmount = data[4];
			lights.push_back(light);
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

		std::vector<bool> editorSettings;
		for (int i = 0; i < 8; i++)
		{
			std::getline(myScene, line);
			editorSettings.push_back(std::stoi(line));
		}

		Editor::GetInstance()->LoadEditorSaveProfile(editorSettings);

		myScene.close();

		return true;
	}

	printf("Warning: Scene file not found\n");
	return false;
}