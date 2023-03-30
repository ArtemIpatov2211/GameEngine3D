#include "EngineCore/Window.hpp"
#include "EngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GameEngine
{
  static bool s_GLFW_initialized = false;

  CWindow::CWindow(std::string title, const unsigned int width, const unsigned int height)
    : m_title(std::move(title))
    , m_width(width)
    , m_height(height)
  {
    int resultCode = init();
  }
  CWindow::~CWindow()
  {
    shutdown();
  }

  int CWindow::init()
  {
    LOG_INFO("Creating window {0} width size {1}x{2}", m_title, m_width, m_height);
    /* Initialize the library */
    if (!glfwInit() && !s_GLFW_initialized)
    {
      return -1;
      LOG_CRIT("Can't initialize GLFW");
    }
    s_GLFW_initialized = true;
    /* Create a windowed mode window and its OpenGL context */
    m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
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
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
  }
}