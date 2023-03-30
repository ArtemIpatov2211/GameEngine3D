#include "EngineCore/Application.hpp"
#include "EngineCore/Log.hpp"
#include "EngineCore/Window.hpp"

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
  while (true)
  {
    m_pWindow->on_update();
    on_update();
  }
  return 0;
}

}