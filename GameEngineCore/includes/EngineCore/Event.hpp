#pragma once
#include <array>
#include <functional>
namespace GameEngine
{
  enum class EEventType
  {
    eWindowResize = 0,
    eWindowClose,

    eKeyPressed,
    eKeyReleased,

    eMouseButtonPressed,
    eMouseButtonReleased,
    eMouseMoved,
    eEventCount
  };

  struct SBaseEvent
  {
    virtual ~SBaseEvent() = default;
    virtual EEventType getType() const = 0;
  };

  struct SEventMouseMoved : public SBaseEvent
  {
    SEventMouseMoved(const double newX, const double newY)
      : x(newX), y(newY)
    {

    }
    EEventType getType() const override { return m_type; }
    double x;
    double y;
    static const EEventType m_type = EEventType::eMouseMoved;
  };

  struct SEventWindowClose : public SBaseEvent
  {
    virtual EEventType getType() const override
    {
      return m_type;
    }

    static const EEventType m_type = EEventType::eWindowClose;
  };

  struct SEventWindowResize : public SBaseEvent
  {
    SEventWindowResize(unsigned int width, unsigned int height)
      : m_width(width), m_height(height)
    {

    }
    EEventType getType() const override { return m_type; }
    unsigned int m_width;
    unsigned int m_height;
    static const EEventType m_type = EEventType::eWindowResize;
  };

  class SEventDispatcher
  {
  public:
    template<typename EEventType>
    void add_event_listener(std::function<void(EEventType&)> callback)
    {
      auto baseCallback = [func = std::move(callback)](SBaseEvent& e)
      {
        if (e.getType() == EEventType::m_type)
        {
          func(static_cast<EEventType&>(e));
        }
      };
      m_eventCallbacks[static_cast<size_t>(EEventType::m_type)] = std::move(baseCallback);
    }

    void dispatch(SBaseEvent& event)
    {
      auto& callback = m_eventCallbacks[static_cast<size_t>(event.getType())];
      if (callback)
      {
        callback(event);
      }
    }

  private:
    std::array<std::function<void(SBaseEvent&)>, static_cast<size_t>(EEventType::eEventCount)> m_eventCallbacks;
  };
}