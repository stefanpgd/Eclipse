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
	mySaveFile << objects.size() << "\n";

	for (int i = 0; i < objects.size(); i++)
	{
		mySaveFile << objects[i]->transform.Position.x << "\n";
		mySaveFile << objects[i]->transform.Position.y << "\n";
		mySaveFile << objects[i]->transform.Position.z << "\n";

		mySaveFile << objects[i]->transform.Rotation.x << "\n";
		mySaveFile << objects[i]->transform.Rotation.y << "\n";
		mySaveFile << objects[i]->transform.Rotation.z << "\n";

		mySaveFile << objects[i]->transform.Scale.x << "\n";
		mySaveFile << objects[i]->transform.Scale.y << "\n";
		mySaveFile << objects[i]->transform.Scale.z << "\n";
	}

	mySaveFile.close();
}

void SceneLoader::LoadScene(Object* temp)
{
	std::string line;
	std::ifstream myFile("Assets/Scenes/testScene.txt");

	if (myFile.is_open())
	{
		std::getline(myFile, line);
		int objectCount = std::stoi(line);

		// loop over every object, create it... enc...
		for (int i = 0; i < objectCount; i++)
		{
			float transformInfo[9];

			for (int j = 0; j < 9; j++)
			{
				std::getline(myFile, line);
				transformInfo[j] = std::stof(line);
			}

			temp->transform.SetTransformDataFromArray(transformInfo);
		}

		myFile.close();
	}
	else
	{
		throw std::runtime_error("ERROR: Unable to find/load scene file");
	}
}