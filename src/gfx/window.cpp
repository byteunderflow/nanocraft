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
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, width / 2.0, height / 2.0);
    glfwSetCursorPosCallback(handle, [](GLFWwindow *handle, double x, double y)
                             {
        Window *window = (Window *)glfwGetWindowUserPointer(handle);
        if (window->state.paused)
            ImGui_ImplGlfw_CursorPosCallback(handle, x, y);
        else
            window->move(x, y); });

    glfwSwapInterval(settings.vsync ? 1 : 0);
    glfwShowWindow(handle);
}

void Window::run()
{
    while (!glfwWindowShouldClose(handle))
    {
        // Process input
        glfwPollEvents();
        input();

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

        // Update game
        game->update();

        // Render game
        glfwGetFramebufferSize(handle, &width, &height);
        renderer->render(width, height, game->world);

        // Render debug information
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Resolution: %dx%d", width, height);
        ImGui::Text("FPS: %d", time.fps);
        ImGui::Text("Render: %.2fms", time.delta * 1000.0f);
        ImGui::Text("XYZ: %f %f %f", game->world.player.position.x, game->world.player.position.y, game->world.player.position.z);
        ImGui::Text("Yaw: %fdeg", game->world.player.yaw);
        ImGui::Text("Pitch: %fdeg", game->world.player.pitch);

        ImGui::SliderFloat("Sensitivity", &mouse.settings.sensitivity, 0.1f, 1.0f);
        ImGui::SliderFloat("FOV (deg)", &renderer->settings.fov, 0.0f, 120.0f);
        ImGui::SliderFloat("Speed", &game->world.player.speed, 0.0f, 20.0f);

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

void Window::pause()
{
    if (state.paused)
        return;
    state.paused = true;

    glfwSetCursorPos(handle, width / 2.0, height / 2.0);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::resume()
{
    if (!state.paused)
        return;
    state.paused = false;

    glfwSetCursorPos(handle, width / 2.0, height / 2.0);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouse.enter = true;
}

void Window::input()
{
    // State
    if (glfwGetKey(handle, GLFW_KEY_1))
        pause();
    if (glfwGetKey(handle, GLFW_KEY_2))
        resume();

    if (state.paused)
        return;

    // Player movement
    if (glfwGetKey(handle, GLFW_KEY_W))
        game->world.player.forward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_S))
        game->world.player.backward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_A))
        game->world.player.left(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_D))
        game->world.player.right(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
        game->world.player.upward(time.delta);
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
        game->world.player.downward(time.delta);
}

void Window::move(double ix, double iy)
{
    if (state.paused)
        return;

    const float x = static_cast<float>(ix);
    const float y = static_cast<float>(iy);

    if (mouse.enter)
    {
        mouse.lastx = x;
        mouse.lasty = y;
        mouse.enter = false;
    }

    const float xoffset = (x - mouse.lastx) * mouse.settings.sensitivity;
    const float yoffset = (mouse.lasty - y) * mouse.settings.sensitivity;
    mouse.lastx = x;
    mouse.lasty = y;

    game->world.player.look(xoffset, yoffset);
}