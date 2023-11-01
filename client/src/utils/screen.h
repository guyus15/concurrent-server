#pragma once

#include <GLFW/glfw3.h>

#include <utility>
#include <vector>

using Resolution = std::pair<int, int>;

std::vector<Resolution> GetAvailableResolutions(GLFWmonitor* monitor);
Resolution GetCurrentResolution(GLFWmonitor* monitor);
