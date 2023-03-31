#include "EngineCore/Application.hpp"
#include "EngineCore/Log.hpp"
#include "EngineCore/Window.hpp"
#include "EngineCore/Event.hpp"

#include <iostream>

namespace GameEngine
{
CApplication::CApplication()
{
  LOG_INFO("Start App");
}

CApplication::~CApplication()
{
  LOG_INFO("Close App");
}

int CApplication::start(unsigned int windowWidth, unsigned int windowHeight, const char* title)
{
  m_pWindow = std::make_unique<CWindow>(title, windowWidth, windowHeight);
  m_eventDispatcher.add_event_listener<SEventMouseMoved>(
    [](SEventMouseMoved& event)
    {
    });

  m_eventDispatcher.add_event_listener<SEventWindowResize>(
    [](SEventWindowResize& event)
    {
    });

  m_eventDispatcher.add_event_listener<SEventWindowClose>(
    [&](SEventWindowClose& event)
    {
      m_bCloseWindow = true;
    });
  m_pWindow->setEventCallback(
    [&](SBaseEvent& event)
    {
      m_eventDispatcher.dispatch(event);
    }
  );
  while (!m_bCloseWindow)
  {
    m_pWindow->on_update();
    on_update();
  }
  m_pWindow = nullptr;
  return 0;
}

}