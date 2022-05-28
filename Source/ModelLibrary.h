#pragma once

class Model;

#include <map>

class ModelLibrary
{
public:
	static ModelLibrary* GetInstance()
	{
		static ModelLibrary instance;
		return &instance;
	}

	Model* GetModel(std::string location);

private:
	std::map<std::string, Model*> models;
};