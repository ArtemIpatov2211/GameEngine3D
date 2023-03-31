#pragma once
#include <string>
#include "EngineCore/Event.hpp"
#include <functional>

struct GLFWwindow;
namespace GameEngine
{
  class CWindow
  {
  public:
    using EventCallbackFn = std::function<void(SBaseEvent&)>;
    CWindow(std::string title, const unsigned int width, const unsigned int height);
    ~CWindow();

    CWindow(const CWindow&) = delete;
    CWindow(CWindow&&) = delete;
    CWindow& operator=(const CWindow&) = delete;
    CWindow& operator=(CWindow&&) = delete;

    virtual void on_update();
    unsigned int getWidth() const { return m_data.m_width; }
    unsigned int getHeight() const { return m_data.m_height; }
    void setEventCallback(const EventCallbackFn& callback) { m_data.m_eventCallbackFn = callback; }
  private:
    struct WindowData
    {
      std::string m_title;
      unsigned int m_width;
      unsigned int m_height;
      EventCallbackFn m_eventCallbackFn;
    };
    int init();
    void shutdown();
  private:
    GLFWwindow* m_pWindow{ nullptr };
    WindowData m_data; 
    float m_backgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.f };
  };
}