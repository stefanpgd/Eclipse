#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// (3D) Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/type_mat.hpp>

#include "ImguiHandler.h"

using namespace glm;
const unsigned int ScreenWidth = 1280;
const unsigned int ScreenHeight = 720;