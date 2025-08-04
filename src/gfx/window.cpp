#include "window.hpp"

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}

void Window::create()
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

    handle = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
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
    glfwSetWindowPos(handle, (mode->width - WIDTH) / 2, (mode->height - HEIGHT) / 2);

    mouse = std::make_unique<Mouse>(WIDTH / 2.0, HEIGHT / 2.0);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, WIDTH / 2.0, HEIGHT / 2.0);
    glfwSetCursorPosCallback(handle, [](GLFWwindow *window, double x, double y)
                             { ((Window *)glfwGetWindowUserPointer(window))->move(x, y); });

    camera = std::make_unique<Camera>(this);
    camera->init();

    renderer = std::make_unique<Renderer>(this);
    renderer->init();

    glfwSwapInterval(0);
    glfwShowWindow(handle);
}

void Window::run()
{
    double time = glfwGetTime();
    while (!glfwWindowShouldClose(handle))
    {
        const double now = glfwGetTime();
        delta = static_cast<float>(now - time);
        time = now;

        glfwGetFramebufferSize(handle, &width, &height);
        camera->update();
        renderer->render();

        glfwSwapBuffers(handle);
        glfwPollEvents();

        input();
    }
}

void Window::input()
{
    if (glfwGetKey(handle, GLFW_KEY_W))
    {
        camera->moveForward();
    }
    if (glfwGetKey(handle, GLFW_KEY_S))
    {
        camera->moveBackward();
    }
    if (glfwGetKey(handle, GLFW_KEY_A))
    {
        camera->moveLeft();
    }
    if (glfwGetKey(handle, GLFW_KEY_D))
    {
        camera->moveRight();
    }
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
    {
        camera->moveUpward();
    }
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
    {
        camera->moveDownward();
    }
}

void Window::move(double x, double y)
{
    mouse->update(static_cast<float>(x), static_cast<float>(y));
    camera->move();
}