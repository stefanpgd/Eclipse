#include "Renderer.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
	Renderer* renderer = Renderer::GetInstance();

	try
	{
		renderer->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// Things to do:
// Understand and Implement Blinn-Phong ( LearnOpenGL )
// consider making a static enum for the slots textures can use so it's always consistent ( OpenGlDev )
// Fix texture loading for models. 
// Proper material/shader management ( techniques )
// Whenever a model is unable to load in, instead of closing the app, just don't load in the model/skip it.
// When removing objects, the data attached to it doesn't get used at all. These are likely textures & Models, removes these.
// Lights should have a little image, part of gizmos toggle
// Right mouse-click to control camera
// Click in scene to select object