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
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_stdlib.h"

// Gizmo package that links into ImGui
#include "ImGuizmo/ImGuizmo.h"

using namespace glm;
const float AspectRatio = (16.0f / 9.0f);
const unsigned int ScreenWidth = 1600;
const unsigned int ScreenHeight = (unsigned int)(ScreenWidth / AspectRatio);

static bool objectMenu = false;