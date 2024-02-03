#include "input.h"

#include <common/utils/assertion.h>

#include "GLFW/glfw3.h"

enum class InputStatus
{
    NotPressed = 0,
    Pressed = 1
};

Input Input::s_instance{};

void Input::Initialise()
{
    for (int i = 0; i < static_cast<int>(KeyCode::KeycodeSize); ++i)
    {
        Get().m_input_map[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
        Get().m_input_map_prev[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
    }
}

void Input::Update()
{
    Get().m_input_map_prev = Get().m_input_map;
}

bool Input::GetKey(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed;
}

bool Input::GetKeyDown(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed &&
        Get().m_input_map_prev[key] == InputStatus::NotPressed;
}

bool Input::GetKeyUp(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::NotPressed &&
        Get().m_input_map_prev[key] == InputStatus::Pressed;
}

bool Input::GetKeyHeld(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed &&
        Get().m_input_map_prev[key] == InputStatus::NotPressed;
}

Input& Input::Get()
{
    return s_instance;
}

void Input::CallbackUpdate(const int glfw_keycode, int glfw_action)
{
    SCX_ASSERT(glfw_keycode != -1, "Unknown GLFW key code encountered.");

    const auto key_code = static_cast<KeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_input_map[key_code] = input_status;
}

void KeyCallback(GLFWwindow*, const int keycode, const int scancode, const int action, const int mods)
{
    Input::CallbackUpdate(keycode, action);
}
