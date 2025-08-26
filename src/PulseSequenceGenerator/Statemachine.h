#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include "ButtonHandler.h"
#include "PulseSequenceData.h"

class Statemachine {
public:

  enum Element {
    ElementOn1 = 0,
    ElementOn2,
    ElementOn3,
    ElementOn4,
    ElementOn5,
    ElementOn6,
    ElementOn7,
    ElementOn8,
    ElementOff1,
    ElementOff2,
    ElementOff3,
    ElementOff4,
    ElementOff5,
    ElementOff6,
    ElementOff7,
    ElementOff8,
    ElementTrigger,
    ElementStepDuration,
    ElementStepFactor,
    ElementPeriode,
    ElementRun
  };

  enum Selection {
    SelectionAll = 0,
    SelectionDigit0,
    SelectionDigit1,
    SelectionDigit2,
    SelectionDigit3,
    SelectionDigit4
  };
  
  /**
   * Constructor
   */
  explicit Statemachine(PulseSequenceData& pulseSequenceData);

  /**
   * Run Statemachine
   */
  void run(void);
  
  /** Set Up Event*/
  void setButtonState(ButtonHandler::ButtonState btnEvent);

  Element getElementSelected();
  
  Selection getSelection();

  bool isDataChanged();
  
private:

  void resetEvents();
  
  void evaluateNavigation(Element upElement, Element downElement, Element leftElement, Element rightElement);

  bool isButtonEvent();

  void incrementElement();
  void decrementElement();
  uint16_t calcIncrement();

  template<typename ENUM_TYPE> void incrementEnum(ENUM_TYPE &x);
  template<typename ENUM_TYPE> void decrementEnum(ENUM_TYPE &x);

  // constants
  static const uint32_t LONGPRESS_REPEAT_MS = 200;

  // members
  Element m_selected;
  Selection m_selection;
  ButtonHandler::ButtonState m_btnEvent;
  ButtonHandler::ButtonState m_previousBtnEvent;
  PulseSequenceData &m_pulseSequenceData;
  uint32_t m_lastLongPress;
  bool m_isDataChanged;
  uint32_t m_lastRuns;
};
#endif // STATEMACHINE_H
