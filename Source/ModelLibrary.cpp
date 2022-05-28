#include "precomp.h"
#include "Model.h"
#include "ModelLibrary.h"

Model* ModelLibrary::GetModel(std::string location)
{
	Model* model = models[location];

	if (model == nullptr)
	{
		model = new Model(location);
		models[location] = model;
	}

	return model;
}