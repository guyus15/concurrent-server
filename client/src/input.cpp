/**
 * input.cpp
 */

#include "input.h"

#include <common/utils/assertion.h>

#include "GLFW/glfw3.h"

enum class InputStatus
{
    NotPressed = 0,
    Pressed = 1
};

Input Input::s_instance{};

/**
 * \brief Initialises the input system by setting relevant keys to \code NotPressed\endcode.
 */
void Input::Initialise()
{
    for (int i = 0; i < static_cast<int>(KeyCode::KeycodeSize); ++i)
    {
        Get().m_input_map[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
        Get().m_input_map_prev[static_cast<KeyCode>(i)] = InputStatus::NotPressed;
    }
}

/**
 * \brief Called when the handling the GLFW key callback.
 * \param glfw_keycode The GLFW key code for the key event.
 * \param glfw_action The GLFW action for the key event.
 */
void Input::CallbackUpdate(const int glfw_keycode, int glfw_action)
{
    SCX_ASSERT(glfw_keycode != -1, "Unknown GLFW key code encountered.");

    const auto key_code = static_cast<KeyCode>(glfw_keycode);
    const auto input_status = glfw_action == GLFW_REPEAT ? InputStatus::Pressed : static_cast<InputStatus>(glfw_action);

    Get().m_input_map[key_code] = input_status;
}

/**
 * \brief Called each frame of the application.
 * \note This must be placed at the end of main loop,
 * but before \code glfwPollEvents()\endcode.
 */
void Input::Update()
{
    Get().m_input_map_prev = Get().m_input_map;
}

/**
 * \brief Determines if the given key is being pressed.
 * \param key The given key.
 * \return True or false value depending on whether the given key is being pressed.
 */
bool Input::GetKey(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed;
}

/**
 * \brief Determines if the given key has been pressed down in the current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key has been pressed
 * down in the current frame.
 */
bool Input::GetKeyDown(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed &&
        Get().m_input_map_prev[key] == InputStatus::NotPressed;
}

/**
 * \brief Determines if the given key has been released in the current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key has been
 * released in the current frame.
 */
bool Input::GetKeyUp(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::NotPressed &&
        Get().m_input_map_prev[key] == InputStatus::Pressed;
}

/**
 * \brief Determines if the given key is being held down by looking at the state
 * of the key in the previous and current frame.
 * \param key The given key.
 * \return True or false value depending on whether the given key is being
 * held down.
 */
bool Input::GetKeyHeld(const KeyCode key)
{
    return Get().m_input_map[key] == InputStatus::Pressed &&
        Get().m_input_map_prev[key] == InputStatus::NotPressed;
}

Input& Input::Get()
{
    return s_instance;
}

void KeyCallback(GLFWwindow*, const int keycode, const int scancode, const int action, const int mods)
{
    Input::CallbackUpdate(keycode, action);
}
