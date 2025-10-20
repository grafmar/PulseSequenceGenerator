#include "TriggerHandler.h"

TriggerHandler::TriggerHandler(void) :
  m_trigStateOld(0)
{
}

void TriggerHandler::init(void) {
  pinMode(PIN_EXT_TRIGGER, INPUT_PULLUP);
}

TriggerHandler::TriggerEvent TriggerHandler::activeTriggerState() {
  int trigState = digitalRead(PIN_EXT_TRIGGER);
  TriggerEvent triggerState = TriggerEvent_Invalid;

  if (trigState > m_trigStateOld) {
    triggerState = TriggerEvent_HighEdge;
  } else if (trigState < m_trigStateOld) {
    triggerState = TriggerEvent_LowEdge;
  }
  m_trigStateOld = trigState;

  return triggerState;
}
