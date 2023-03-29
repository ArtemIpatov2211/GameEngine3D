#include <iostream>
#include "EngineCore/Application.hpp"
#include <memory>

class CMyApp : public GameEngine::CApplication
{
  virtual void on_update() override
  {
    std::cout << "Update frame: " << frame++ << std::endl;
  }
  int frame = 0;
};

int main()
{
  auto myApp = std::make_unique<CMyApp>();
  int returnCode = myApp->start(640, 480, "QWERY");
  std::cin.get();
}