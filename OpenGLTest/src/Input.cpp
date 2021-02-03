#include "Input.h"


Input* Input::s_Instance()
{
    if (nullptr == m_Instance)
    {
        m_Instance = new Input();
    }
    return m_Instance;
}

void Input::SetGLFWwindow(GLFWwindow* window)
{
    m_Window = window;
}

bool Input::IsMouseButtonPressed(const MouseButton button)
{
    auto state = glfwGetMouseButton(m_Window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

bool Input::IsKeyPressed(const int keycode)
{
    auto state = glfwGetKey(m_Window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 Input::GetMousePosition()
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return { (float)x, (float)y };
}

void Input::SetMouseMode(const MouseMode mode)
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, static_cast<int32_t>(mode));
}
