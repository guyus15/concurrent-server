#include "input.h"

#include <common/graphics/screen_manager.h>

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
        Get().m_key_input[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
        Get().m_key_input_prev[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
    }
}

void Input::Update()
{
    Get().m_key_input_prev = Get().m_key_input;
}

bool Input::GetKey(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed;
}

bool Input::GetKeyDown(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed &&
        Get().m_key_input_prev[key] == InputStatus::NotPressed;
}

bool Input::GetKeyUp(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::NotPressed &&
        Get().m_key_input_prev[key] == InputStatus::Pressed;
}

bool Input::GetKeyHeld(const KeyCode key)
{
    return Get().m_key_input[key] == InputStatus::Pressed &&
        Get().m_key_input_prev[key] == InputStatus::NotPressed;
}

glm::vec2 Input::GetMousePosition()
{
    return Get().m_mouse_position;
}

bool Input::GetMouse(const MouseButton button)
{
    return Get().m_mouse_input[button] == InputStatus::Pressed;
}

Input::Input()
    : m_mouse_position{ 0, 0 }
{
}

Input& Input::Get()
{
    return s_instance;
}

void Input::KeyCallbackUpdate(const int glfw_keycode, int glfw_action)
{
    SCX_ASSERT(glfw_keycode != -1, "Unknown GLFW key code encountered.");

    const auto key_code = static_cast<KeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_key_input[key_code] = input_status;
}

void Input::MousePositionCallbackUpdate(const double glfw_xpos, const double glfw_ypos)
{
    Get().m_mouse_position = { glfw_xpos, glfw_ypos };
}

void Input::MouseButtonCallbackUpdate(const int glfw_button, const int glfw_action)
{
    SCX_ASSERT(glfw_button != -1, "Unknown GLFW mouse button encountered.");

    const auto mouse_button = static_cast<MouseButton>(glfw_button);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_mouse_input[mouse_button] = input_status;
}

void KeyCallback(GLFWwindow*, const int keycode, int, const int action, int)
{
    Input::KeyCallbackUpdate(keycode, action);
}

void MousePositionCallback(GLFWwindow*, const double xpos, const double ypos)
{
    Input::MousePositionCallbackUpdate(xpos, ypos);
}

void MouseButtonCallback(GLFWwindow*, const int button, const int action, int)
{
    Input::MouseButtonCallbackUpdate(button, action);
}
