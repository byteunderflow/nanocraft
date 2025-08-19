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

        debug();

        glfwSwapBuffers(handle);
    }
}

void Window::debug()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::Begin("DEBUG", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("RENDERER", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Resolution: %dx%d", width, height);
        ImGui::Text("FPS: %d", time.fps);
        ImGui::Text("Render: %.2fms", time.delta * 1000.0f);
        ImGui::SliderFloat("FOV", &renderer->settings.fov, 0.0f, 150.0f);
        ImGui::SliderFloat("Near plane", &renderer->settings.near, 0.1f, 1.0f);
        ImGui::SliderFloat("Far plane", &renderer->settings.far, 1000.0f, 5000.0f);
        if (ImGui::Button("Wireframe"))
        {
            renderer->settings.wireframe = !renderer->settings.wireframe;
        }
        if (ImGui::Button("Vsync"))
        {
            settings.vsync = !settings.vsync;
            glfwSwapInterval(settings.vsync ? 1 : 0);
        }
    }

    if (ImGui::CollapsingHeader("PLAYER", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("XYZ: %f %f %f", game->world.player->position.x, game->world.player->position.y, game->world.player->position.z);
        if (game->world.player->chunk)
            ImGui::Text("Chunk XZ: %d %d", game->world.player->chunk->position.x, game->world.player->chunk->position.z);
        ImGui::Text("Yaw: %fdeg", game->world.player->yaw);
        ImGui::Text("Pitch: %fdeg", game->world.player->pitch);
        ImGui::SliderFloat("Height", &game->world.player->settings.height, 1.0f, 2.0f);
        ImGui::SliderFloat("Speed", &game->world.player->settings.speed, 0.0f, 100.0f);
        ImGui::SliderInt("Render distance", reinterpret_cast<int *>(&game->world.player->settings.renderDistance), 1, 16);
    }

    if (ImGui::CollapsingHeader("MOUSE", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Sensitivity", &mouse.settings.sensitivity, 0.1f, 1.0f);
    }

    if (ImGui::CollapsingHeader("GENERATION", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Scale", &game->world.generator->scale, 0.001f, 0.1f);
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
        game->world.player->move(Movement::FORWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_S))
        game->world.player->move(Movement::BACKWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_A))
        game->world.player->move(Movement::LEFT, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_D))
        game->world.player->move(Movement::RIGHT, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_SPACE))
        game->world.player->move(Movement::UPWARD, time.delta);
    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT))
        game->world.player->move(Movement::DOWNWARD, time.delta);
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

    const float32 xOffset = (x - mouse.lastx) * mouse.settings.sensitivity;
    const float32 yOffset = (mouse.lasty - y) * mouse.settings.sensitivity;
    mouse.lastx = x;
    mouse.lasty = y;

    game->world.player->look(xOffset, yOffset);
}