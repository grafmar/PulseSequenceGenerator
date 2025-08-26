#ifndef DEBOUNCE_BUTTON_H
#define DEBOUNCE_BUTTON_H

#include <Arduino.h>


class DebounceButton {
public:
  enum State {
    StateReleased,
    StatePressed,
    StateLongPressed
  };
  
  explicit DebounceButton (const int pin, uint32_t debounceTime, uint32_t longPressTime);

  ~DebounceButton();

  void debounce(void);

  State getState(void);

private:

  State m_state;
  const int m_pin;
  uint16_t m_debounceTime;
  uint16_t m_longPressTime;
  uint32_t m_lastDebounceTime;
  bool m_previousState;
};
#endif // DEBOUNCE_BUTTON_H

