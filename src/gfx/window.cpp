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

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode)
    {
        std::cerr << "Unable to retrieve video mode" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(handle, (mode->width - width) / 2, (mode->height - height) / 2);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
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
        glfwPollEvents();
        input();

        const float32 now = static_cast<float32>(glfwGetTime());
        time.delta = static_cast<float32>(now - time.lasttime);
        time.lasttime = now;
        time.frames++;
        if (now - time.lastframe > 1.0f)
        {
            time.fps = time.frames;
            time.frames = 0;
            time.lastframe = now;
        }

        game->update();

        glfwGetFramebufferSize(handle, &width, &height);
        renderer->render(width, height, game->world);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Resolution: %dx%d", width, height);
        ImGui::Text("FPS: %d", time.fps);
        ImGui::Text("Render: %.2fms", time.delta * 1000.0f);

        ImGui::Text("XYZ: %f %f %f", game->world.player.position.x, game->world.player.position.y, game->world.player.position.z);
        ImGui::Text("Chunk XZ: %d %d", game->world.player.chunkX, game->world.player.chunkY);
        ImGui::Text("Yaw: %fdeg", game->world.player.yaw);
        ImGui::Text("Pitch: %fdeg", game->world.player.pitch);

        ImGui::SliderFloat("Sensitivity", &mouse.settings.sensitivity, 0.1f, 1.0f);
        ImGui::SliderFloat("Speed", &game->world.player.speed, 0.0f, 20.0f);
        ImGui::SliderFloat("FOV (deg)", &renderer->settings.fov, 0.0f, 150.0f);
        ImGui::SliderFloat("Near plane", &renderer->settings.near, 0.1f, 1.0f);
        ImGui::SliderFloat("Far plane", &renderer->settings.far, 10.0f, 1000.0f);

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
    if (glfwGetKey(handle, GLFW_KEY_1))
        pause();
    if (glfwGetKey(handle, GLFW_KEY_2))
        resume();

    if (state.paused)
        return;

    if (glfwGetKey(handle, GLFW_KEY_W))
        game->world.player.move(Movement::FORWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_S))
        game->world.player.move(Movement::BACKWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_A))
        game->world.player.move(Movement::LEFT, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_D))
        game->world.player.move(Movement::RIGHT, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
        game->world.player.move(Movement::UPWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
        game->world.player.move(Movement::DOWNWARD, time.delta);
}

void Window::move(double ix, double iy)
{
    if (state.paused)
        return;

    const float32 x = static_cast<float32>(ix);
    const float32 y = static_cast<float32>(iy);

    if (mouse.enter)
    {
        mouse.lastx = x;
        mouse.lasty = y;
        mouse.enter = false;
    }

    const float32 xoffset = (x - mouse.lastx) * mouse.settings.sensitivity;
    const float32 yoffset = (mouse.lasty - y) * mouse.settings.sensitivity;
    mouse.lastx = x;
    mouse.lasty = y;

    game->world.player.look(xoffset, yoffset);
}