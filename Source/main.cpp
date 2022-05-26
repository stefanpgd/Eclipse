#include "Renderer.h"
#include <iostream>
#include <stdexcept>

int main()
{
	Renderer renderer;

	try
	{
		renderer.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}