#include "DebounceButton.h"

DebounceButton::DebounceButton(const int pin, uint32_t debounceTime, uint32_t longPressTime) :
m_state(StateReleased),
m_pin(pin),
m_debounceTime(debounceTime),
m_longPressTime(longPressTime),
m_lastDebounceTime(0),
m_previousState(false)
{
}

DebounceButton::~DebounceButton() {
}

void DebounceButton::debounce(void) {
  uint32_t now = millis();
  bool state = digitalRead(m_pin);
  
  if (state != m_previousState) {
    m_lastDebounceTime = now;
    m_previousState = state;
  } else if ((now - m_lastDebounceTime) > m_debounceTime) {
    if (state == HIGH) {
      m_state = StateReleased;
    } else if ((now - m_lastDebounceTime) > m_longPressTime) {
      m_state = StateLongPressed;
    } else {
      m_state = StatePressed;
    }
  }
}

DebounceButton::State DebounceButton::getState(void) {
  return m_state;
}

