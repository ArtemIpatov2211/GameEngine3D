#include "EngineCore/Window.hpp"
#include "EngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace GameEngine
{
  static bool s_GLFW_initialized = false;

  CWindow::CWindow(std::string title, const unsigned int width, const unsigned int height)
    : m_data({ std::move(title), width, height })
  {
    int resultCode = init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
  }
  CWindow::~CWindow()
  {
    shutdown();
  }

  int CWindow::init()
  {
    LOG_INFO("Creating window {0} width size {1}x{2}", m_data.m_title, m_data.m_width, m_data.m_height);
    /* Initialize the library */
    if (!glfwInit() && !s_GLFW_initialized)
    {
      return -1;
      LOG_CRIT("Can't initialize GLFW");
    }
    s_GLFW_initialized = true;
    /* Create a windowed mode window and its OpenGL context */
    m_pWindow = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
    if (!m_pWindow)
    {
      LOG_CRIT("Can't create window");
      glfwTerminate();
      return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      LOG_CRIT("Failed to initialize GLAD");
      return -1;
    }
    glfwSetWindowUserPointer(m_pWindow, &m_data);
    glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height)
      {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
        data.m_width = width;
        data.m_height = height;
        SEventWindowResize event(width, height);
        data.m_eventCallbackFn(event);
      });
    glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y)
      {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
        SEventMouseMoved event(x, y);
        data.m_eventCallbackFn(event);
      });
    glfwSetWindowCloseCallback(m_pWindow,
      [](GLFWwindow* pWindow)
      {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

        SEventWindowClose event;
        data.m_eventCallbackFn(event);
      }
    );
    return 0;
  }
  void CWindow::shutdown()
  {
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
  }

  void CWindow::on_update()
  {
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(getWidth());
    io.DisplaySize.y = static_cast<float>(getHeight());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
  }
}