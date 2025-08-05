#include "window.hpp"

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

    // Set window position
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode)
    {
        std::cerr << "Unable to retrieve video mode" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(handle, (mode->width - width) / 2, (mode->height - height) / 2);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup ImGui backends
    if (!ImGui_ImplGlfw_InitForOpenGL(handle, true))
    {
        std::cerr << "Unable to initialize ImGui-GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
    {
        std::cerr << "Unable to initialize ImGui-OpenGL" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Setup mouse input
    mouse = std::make_unique<Mouse>(width / 2.0f, height / 2.0f);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, width / 2.0, height / 2.0);
    glfwSetCursorPosCallback(handle, [](GLFWwindow *handle, double x, double y)
                             {
        Window *window = (Window *)glfwGetWindowUserPointer(handle);
        if (window->state.paused)
            ImGui_ImplGlfw_CursorPosCallback(handle, x, y);
        else
            window->move(x, y); });

    // Setup camera
    camera = std::make_unique<Camera>();
    camera->init();

    // Setup camera
    renderer = std::make_unique<Renderer>();
    renderer->init();

    glfwSwapInterval(settings.vsync ? 1 : 0);
    glfwShowWindow(handle);
}

void Window::pause()
{
    if (state.paused)
        return;

    state.paused = true;
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::resume()
{
    if (!state.paused)
        return;

    state.paused = false;
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouse->first = true;
}

void Window::run()
{
    int width;
    int height;

    while (!glfwWindowShouldClose(handle))
    {
        // Process input
        glfwPollEvents();
        input();
        camera->update(renderer->view);

        // Timing
        const float now = static_cast<float>(glfwGetTime());
        time.delta = static_cast<float>(now - time.lasttime);
        time.lasttime = now;
        time.frames++;

        if (now - time.lastframe > 1.0f)
        {
            time.fps = time.frames;
            time.frames = 0;
            time.lastframe = now;
        }

        // Render world
        glfwGetFramebufferSize(handle, &width, &height);
        renderer->render(width, height);

        // Render debug information
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Resolution: %dx%d", width, height);
        ImGui::Text("FPS: %d", time.fps);
        ImGui::Text("Delta: %.2fms", time.delta * 1000.0f);
        ImGui::Text("XYZ: %f %f %f", camera->position.x, camera->position.y, camera->position.z);
        ImGui::Text("Yaw: %fdeg", camera->yaw);
        ImGui::Text("Pitch: %fdeg", camera->pitch);

        if (ImGui::Button("Wireframe"))
        {
            renderer->settings.wireframe = !renderer->settings.wireframe;
        }
        if (ImGui::Button("Vsync"))
        {
            settings.vsync = !settings.vsync;
            glfwSwapInterval(settings.vsync ? 1 : 0);
        }

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(handle);
    }
}

void Window::input()
{
    // State
    if (glfwGetKey(handle, GLFW_KEY_P))
        pause();
    if (glfwGetKey(handle, GLFW_KEY_R))
        resume();

    if (state.paused)
        return;

    // Camera movement
    if (glfwGetKey(handle, GLFW_KEY_W))
        camera->moveForward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_S))
        camera->moveBackward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_A))
        camera->moveLeft(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_D))
        camera->moveRight(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
        camera->moveUpward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
        camera->moveDownward(time.delta);
}

void Window::move(double x, double y)
{
    if (state.paused)
        return;

    mouse->update(static_cast<float>(x), static_cast<float>(y));
    camera->move(mouse->xoffset, mouse->yoffset);
}