#pragma once
#include <string>

struct GLFWwindow;
namespace GameEngine
{
  class CWindow
  {
  public:
    CWindow(std::string title, const unsigned int width, const unsigned int height);
    ~CWindow();

    CWindow(const CWindow&) = delete;
    CWindow(CWindow&&) = delete;
    CWindow& operator=(const CWindow&) = delete;
    CWindow& operator=(CWindow&&) = delete;

    virtual void on_update();
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
  private:
    int init();
    void shutdown();
  private:
    GLFWwindow* m_pWindow{ nullptr };
    std::string m_title;
    unsigned int m_width{ 320 };
    unsigned int m_height{ 320 };
  };
}