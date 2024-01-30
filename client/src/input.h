#pragma once

#include <unordered_map>

struct GLFWwindow;

enum class KeyCode
{
    Spacebar = 32,
    A = 65,
    D = 68,
    S = 83,
    W = 87,
    RightArrow = 262,
    LeftArrow = 263,
    DownArrow = 264,
    UpArrow = 265,
    Escape = 256,
    KeycodeSize
};

enum class InputStatus;

class Input
{
public:
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    Input(Input&&) noexcept = default;
    Input& operator=(Input&&) noexcept = default;

    /**
    * \brief Initialises the input system by setting relevant keys to NOT_PRESSED.
    */
    static void Initialise();

    /**
     * \brief Called each frame of the application.
     */
    static void Update();

    /**
    * \brief Determines if the given key is being pressed.
    * \param key The given key.
    * \return True or false value depending on whether the given key is being pressed.
    */
    static bool GetKey(KeyCode key);

    /**
     * \brief Determines if the given key has been pressed down in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been pressed
     * down in the current frame.
     */
    static bool GetKeyDown(KeyCode key);

    /**
     * \brief Determines if the given key has been released in the current frame.
     * \param key The given key.
     * \return True or false value depending on whether the given key has been
     * released in the current frame.
     */
    static bool GetKeyUp(KeyCode key);

    /**
    * \brief Determines if the given key is being held down by looking at the state
    * of the key in the previous and current frame.
    * \param key The given key.
    * \return True or false value depending on whether the given key is being
    * held down.
    */
    static bool GetKeyHeld(KeyCode key);

private:
    Input() = default;
    ~Input() = default;

    static Input& Get();
    static Input s_instance;

    std::unordered_map<KeyCode, InputStatus> m_input_map;
    std::unordered_map<KeyCode, InputStatus> m_input_map_prev;

    /**
     * \brief Called when the handling the GLFW key callback.
     * \param glfw_keycode The GLFW key code for the key event.
     * \param glfw_action The GLFW action for the key event.
     */
    static void CallbackUpdate(int glfw_keycode, int glfw_action);

    friend void KeyCallback(GLFWwindow*, int, int, int, int);
};

void KeyCallback(GLFWwindow*, int, int, int, int);
