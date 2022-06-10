#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// (3D) Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/type_mat.hpp>

#include "ImguiHandler.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

using namespace glm;
const unsigned int ScreenWidth = 1280;
const unsigned int ScreenHeight = 720;

static bool objectMenu = false;