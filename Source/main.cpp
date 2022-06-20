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
// Right mouse-click to control camera
// Click in scene to select object

// Bugs:
// Objects still spawn in with the wrong material? test.