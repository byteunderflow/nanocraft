#include "window.hpp"

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}

void Window::create(int width, int height, const char *title)
{
    glfwSetErrorCallback([](int errorcode, const char *description)
                         { std::cerr << errorcode << ": " << description << std::endl; });

    if (!glfwInit())
    {
        std::cerr << "Unable to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!handle)
    {
        std::cerr << "Unable to create window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Unable to load GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(handle, this);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode)
    {
        std::cerr << "Unable to retrieve video mode" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(handle, (mode->width - width) / 2, (mode->height - height) / 2);

    mouse = std::make_unique<Mouse>(width / 2.0f, height / 2.0f);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, width / 2.0, height / 2.0);
    glfwSetCursorPosCallback(handle, [](GLFWwindow *window, double x, double y)
                             { ((Window *)glfwGetWindowUserPointer(window))->move(x, y); });

    camera = std::make_unique<Camera>();
    camera->init();

    renderer = std::make_unique<Renderer>();
    renderer->init();

    glfwSwapInterval(1);
    glfwShowWindow(handle);
}

void Window::run()
{
    int width;
    int height;
    double time = glfwGetTime();

    while (!glfwWindowShouldClose(handle))
    {
        glfwPollEvents();
        input();
        camera->update(renderer->view);

        const double now = glfwGetTime();
        delta = static_cast<float>(now - time);
        time = now;

        glfwGetFramebufferSize(handle, &width, &height);
        renderer->render(width, height);

        glfwSwapBuffers(handle);
    }
}

void Window::input()
{
    if (glfwGetKey(handle, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
        return;
    }

    if (glfwGetKey(handle, GLFW_KEY_W))
    {
        camera->moveForward(delta);
    }
    if (glfwGetKey(handle, GLFW_KEY_S))
    {
        camera->moveBackward(delta);
    }
    if (glfwGetKey(handle, GLFW_KEY_A))
    {
        camera->moveLeft(delta);
    }
    if (glfwGetKey(handle, GLFW_KEY_D))
    {
        camera->moveRight(delta);
    }
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
    {
        camera->moveUpward(delta);
    }
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
    {
        camera->moveDownward(delta);
    }
}

void Window::move(double x, double y)
{
    mouse->update(static_cast<float>(x), static_cast<float>(y));
    camera->move(mouse->xoffset, mouse->yoffset);
}