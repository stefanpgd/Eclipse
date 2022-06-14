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
// Expand the menu bar with: Scene Details, Camera (within scene details?) and Create Objects menu, Make it a checklist like Examples?
// Start locking window positions
// Console in game for logging/messages
// Statistics menu ( FPS/Draw calls/Objects enc... ), check Demo/Examples/Simple Overlay
// Right mouse-click to control camera