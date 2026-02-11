#include <Platform/Windows/WindowsInput.h>

#include <Application.h>

#include <GLFW/glfw3.h>

namespace Core
{

Input* Input::s_instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow().GetNativeHandle());
    const int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow().GetNativeHandle());
    const int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 WindowsInput::GetMousePositionImpl()
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow().GetNativeHandle());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { xpos, ypos };
}

}
