#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(void) :
  m_debounceButtonUp(PIN_BUTTON_UP, DEBOUNCE_DURATION_MS, LONG_PRESS_DURATION_MS),
  m_debounceButtonDown(PIN_BUTTON_DOWN, DEBOUNCE_DURATION_MS, LONG_PRESS_DURATION_MS),
  m_debounceButtonRight(PIN_BUTTON_RIGHT, DEBOUNCE_DURATION_MS, LONG_PRESS_DURATION_MS),
  m_debounceButtonLeft(PIN_BUTTON_LEFT, DEBOUNCE_DURATION_MS, LONG_PRESS_DURATION_MS),
  m_debounceButtonEnter(PIN_BUTTON_ENTER, DEBOUNCE_DURATION_MS, LONG_PRESS_DURATION_MS),

  m_activeState(ButtonEvent_None),
  m_lastStateUp(false),
  m_lastStateDown(false),
  m_lastStateLeft(false),
  m_lastStateRight(false),
  m_lastStateEnter(false),
  m_stateUp(false),
  m_stateDown(false),
  m_stateLeft(false),
  m_stateRight(false),
  m_stateEnter(false)
{
}

void ButtonHandler::init(void) {
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ENTER, INPUT_PULLUP);
}

void ButtonHandler::runFilterButtons(void) {
  m_debounceButtonUp.debounce();
  m_debounceButtonDown.debounce();
  m_debounceButtonRight.debounce();
  m_debounceButtonLeft.debounce();
  m_debounceButtonEnter.debounce();

  DebounceButton::State stateUp     = m_debounceButtonUp.getState();
  DebounceButton::State stateDown   = m_debounceButtonDown.getState();
  DebounceButton::State stateRight  = m_debounceButtonRight.getState();
  DebounceButton::State stateLeft   = m_debounceButtonLeft.getState();
  DebounceButton::State stateEnter  = m_debounceButtonEnter.getState();

  uint8_t numberReleased = 0;
  if (stateUp     == DebounceButton::StateReleased) {
    numberReleased++;
  }
  if (stateDown   == DebounceButton::StateReleased) {
    numberReleased++;
  }
  if (stateRight  == DebounceButton::StateReleased) {
    numberReleased++;
  }
  if (stateLeft   == DebounceButton::StateReleased) {
    numberReleased++;
  }
  if (stateEnter  == DebounceButton::StateReleased) {
    numberReleased++;
  }
  
  if (numberReleased == 5) {
    m_activeState = ButtonEvent_None;
  } else if (numberReleased == 4) {
    if (stateUp == DebounceButton::StatePressed) {
      m_activeState = ButtonEvent_PressedUp;
    } else if (stateDown == DebounceButton::StatePressed) {
      m_activeState = ButtonEvent_PressedDown;
    } else if (stateLeft == DebounceButton::StatePressed) {
      m_activeState = ButtonEvent_PressedLeft;
    } else if (stateRight == DebounceButton::StatePressed) {
      m_activeState = ButtonEvent_PressedRight;
    } else if (stateEnter == DebounceButton::StatePressed) {
      m_activeState = ButtonEvent_PressedEnter;
    } else if (stateUp == DebounceButton::StateLongPressed) {
      m_activeState = ButtonEvent_LongPressedUp;
    } else if (stateDown == DebounceButton::StateLongPressed) {
      m_activeState = ButtonEvent_LongPressedDown;
    } else {
      m_activeState = ButtonEvent_Invalid;
    }
  } else {
    m_activeState = ButtonEvent_Invalid;
  }
}

ButtonHandler::ButtonState ButtonHandler::getState() {
  return m_activeState;
}

