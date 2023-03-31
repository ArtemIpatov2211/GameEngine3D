#include "EngineCore/Window.hpp"
#include "EngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"

namespace GameEngine
{
  GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };
  GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
  const char* vertex_shader =
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main() {"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}";
  const char* fragment_shader =
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}";
  static bool s_GLFW_initialized = false;
  std::unique_ptr<ShaderProgram> p_shader_program;
  GLuint vao;


  CWindow::CWindow(std::string title, const unsigned int width, const unsigned int height)
    : m_data({ std::move(title), width, height })
  {
    int resultCode = init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
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
      LOG_CRITICAL("Can't initialize GLFW");
    }
    s_GLFW_initialized = true;
    /* Create a windowed mode window and its OpenGL context */
    m_pWindow = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
    if (!m_pWindow)
    {
      LOG_CRITICAL("Can't create window");
      glfwTerminate();
      return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      LOG_CRITICAL("Failed to initialize GLAD");
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
    p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
    if (!p_shader_program->isCompiled())
    {
      return false;
    }

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    return 0;
  }
  void CWindow::shutdown()
  {
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
  }

  void CWindow::on_update()
  {
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT); 
    p_shader_program->bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(getWidth());
    io.DisplaySize.y = static_cast<float>(getHeight());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Backgroud color widget");
    ImGui::ColorEdit4("Backgroud color", m_backgroundColor);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
  }
}