#include "window.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "vbo.hpp"
#include "vao.hpp"
#include "ebo.hpp"

#include <iostream>

Window::Window() {}

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}

void onerror(int errorcode, const char *description)
{
    std::cerr << errorcode << ": " << description << std::endl;
}

void onkey(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
    glfwSetWindowShouldClose(handle, key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE);
}

void Window::create()
{
    glfwSetErrorCallback(onerror);

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

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode)
    {
        std::cerr << "Unable to retrieve video mode" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(handle, (mode->width - WIDTH) / 2, (mode->height - HEIGHT) / 2);
    glfwSetKeyCallback(handle, onkey);

    glfwMakeContextCurrent(handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Unable to load GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    renderer = std::make_unique<Renderer>();
    renderer->init();

    glfwSwapInterval(1);
    glfwShowWindow(handle);
}

void Window::run()
{
    int width;
    int height;

    double time;
    int frames;

    while (!glfwWindowShouldClose(handle))
    {
        glfwGetFramebufferSize(handle, &width, &height);
        glViewport(0, 0, width, height);

        renderer->render(width, height);

        frames++;
        const double now = glfwGetTime();
        if (now - time >= 1.0)
        {
            std::cout << "fps: " << frames << std::endl;
            time = now;
            frames = 0;
        }

        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
}