
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

enum class MouseMode
{
    Normal = GLFW_CURSOR_NORMAL,
    Disabled = GLFW_CURSOR_DISABLED
};

enum class MouseButton
{
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE
};


class Input
{
public:
    static Input* s_Instance();

public:
    // hack
    void SetGLFWwindow(GLFWwindow* window);

    bool IsMouseButtonPressed(const MouseButton button);
    bool IsKeyPressed(const int keycode);
    glm::vec2 GetMousePosition();
    void SetMouseMode(const MouseMode mode);
private:
    GLFWwindow* m_Window;
};

static Input* m_Instance;