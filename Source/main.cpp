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
// Abstract objects & lights into a Scene ( requires a Light class ).
// Blinn-Phong ( LearnOpenGL )
// Fix texture loading for models. 
// Proper material/shader management
// Click in scene to select object
// Right mouse-click to control camera
// Whenever a model is unable to load in, instead of closing the app, just don't load in the model/skip it.
// When removing objects, the data attached to it doesn't get used at all. These are likely textures & Models, removes these.