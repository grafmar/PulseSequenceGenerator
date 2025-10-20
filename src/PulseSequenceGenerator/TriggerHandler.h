#ifndef TRIGGER_HANDLER_H
#define TRIGGER_HANDLER_H

#include <Arduino.h>


class TriggerHandler {
  public:
    TriggerHandler();

    void init(void);

    enum TriggerEvent {
      TriggerEvent_Invalid,
      TriggerEvent_LowEdge,
      TriggerEvent_HighEdge
    };

    TriggerEvent activeTriggerState();

private:
  const int       PIN_EXT_TRIGGER        = A0;

  int m_trigStateOld;
};

#endif // TRIGGER_HANDLER_H