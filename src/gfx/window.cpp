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
    if (handle == nullptr)
    {
        std::cerr << "Unable to create window" << std::endl;
        exit(EXIT_FAILURE);
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode == nullptr)
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

    glfwSwapInterval(1);
    glfwShowWindow(handle);
}

void Window::run()
{
    // DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Program program;
    {
        Shader vs(GL_VERTEX_SHADER);
        vs.compile("res/shaders/default.vs");

        Shader fs(GL_FRAGMENT_SHADER);
        fs.compile("res/shaders/default.fs");

        program.attach(&vs);
        program.attach(&fs);
        program.link();
    }

    VAO vao;
    vao.bind();

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    VBO vbo;
    vbo.bind();
    vbo.fill(vertices, sizeof(vertices));

    GLuint indices[] = {
        3, 0, 1, // first triangle
        3, 2, 1  // second triangle
    };

    EBO ebo;
    ebo.bind();
    ebo.fill(indices, sizeof(indices));

    program.bind();

    while (!glfwWindowShouldClose(handle))
    {
        glfwGetFramebufferSize(handle, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.uniform("color", glm::vec4(sinf((float) glfwGetTime()) / 2.0f + 0.5f, 0.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
}