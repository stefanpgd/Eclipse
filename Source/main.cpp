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
// Blinn-Phong ( LearnOpenGL )
// Fix texture loading for models. 
// Save editor window settings ( are windows open, closed, enc. )
// Proper material/shader management
// Click in scene to select object
// Right mouse-click to control camera

// Bugs:
// Objects still spawn in with the wrong material? test.