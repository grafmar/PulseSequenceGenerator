#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "DebounceButton.h"

class ButtonHandler {
public:

  enum ButtonState {
    ButtonEvent_None,
    ButtonEvent_Invalid,
    ButtonEvent_PressedUp,
    ButtonEvent_PressedDown,
    ButtonEvent_PressedLeft,
    ButtonEvent_PressedRight,
    ButtonEvent_PressedEnter,
    ButtonEvent_LongPressedUp,
    ButtonEvent_LongPressedDown,
  };
  
  explicit ButtonHandler(void);

  void init(void);

  void runFilterButtons(void);

  ButtonState getState();
  
private:
  // constants
  const uint16_t  DEBOUNCE_DURATION_MS    = 10;
  const uint16_t  LONG_PRESS_DURATION_MS  = 1000;
  const int       PIN_BUTTON_UP           = 8;
  const int       PIN_BUTTON_DOWN         = 9;
  const int       PIN_BUTTON_RIGHT        = 10;
  const int       PIN_BUTTON_LEFT         = 11;
  const int       PIN_BUTTON_ENTER        = 12;
  
  // members
  DebounceButton m_debounceButtonUp;
  DebounceButton m_debounceButtonDown;
  DebounceButton m_debounceButtonRight;
  DebounceButton m_debounceButtonLeft;
  DebounceButton m_debounceButtonEnter;

  

  ButtonState m_activeState;
  uint32_t m_debounceTimerUp;
  uint32_t m_debounceTimerDown;
  uint32_t m_debounceTimerRight;
  uint32_t m_debounceTimerLeft;
  uint32_t m_debounceTimerEnter;
  bool m_lastStateUp;
  bool m_lastStateDown;
  bool m_lastStateLeft;
  bool m_lastStateRight;
  bool m_lastStateEnter;
  bool m_stateUp;
  bool m_stateDown;
  bool m_stateLeft;
  bool m_stateRight;
  bool m_stateEnter;
};
#endif // BUTTON_HANDLER_H


