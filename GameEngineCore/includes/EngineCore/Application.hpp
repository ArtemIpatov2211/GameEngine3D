#pragma once
namespace GameEngine
{
class CApplication
{
public:
  CApplication();
  virtual ~CApplication();

  CApplication(const CApplication&) = delete;
  CApplication(CApplication&&) = delete;
  CApplication& operator=(const CApplication&) = delete;
  CApplication& operator=(CApplication&&) = delete;

  virtual int start(unsigned int windowWidth, unsigned int windowHeight, const char* title);

  virtual void on_update() {}
};
}