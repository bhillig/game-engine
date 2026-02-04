#include <Platform/Windows/WindowsInput.h>

#include <Application.h>
// TODO: Abstract out Window into a WindowsWindow
#include <Window.h>

namespace Core
{

Input* Input::s_instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
    GLFWwindow* window = Application::GetApp()->GetWindow().GetHandle();
    const int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
    GLFWwindow* window = Application::GetApp()->GetWindow().GetHandle();
    const int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 WindowsInput::GetMousePositionImpl()
{
    GLFWwindow* window = Application::GetApp()->GetWindow().GetHandle();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { xpos, ypos };
}

}
