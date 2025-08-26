#include "Statemachine.h"

Statemachine::Statemachine(PulseSequenceData& pulseSequenceData) :
  m_selected(ElementTrigger),
  m_selection(SelectionAll),
  m_btnEvent(ButtonHandler::ButtonEvent_None),
  m_previousBtnEvent(ButtonHandler::ButtonEvent_None),
  m_pulseSequenceData(pulseSequenceData),
  m_lastLongPress(0),
  m_isDataChanged(false),
  m_lastRuns(0)
{
}

void Statemachine::run(void) {
  Element oldSelected = m_selected;
  Selection oldSelection = m_selection;

  m_isDataChanged = false;
  if (m_selection == SelectionAll) {
    switch(m_selected) {
      case ElementOn1:
        evaluateNavigation(ElementOn1, ElementOn2, ElementTrigger, ElementOff1);
        break;

      case ElementOn2:
        evaluateNavigation(ElementOn1, ElementOn3, ElementStepFactor, ElementOff2);
        break;

      case ElementOn3:
        evaluateNavigation(ElementOn2, ElementOn4, ElementPeriode, ElementOff3);
        break;

      case ElementOn4:
        evaluateNavigation(ElementOn3, ElementOn5, ElementPeriode, ElementOff4);
        break;

      case ElementOn5:
        evaluateNavigation(ElementOn4, ElementOn6, ElementPeriode, ElementOff5);
        break;

      case ElementOn6:
        evaluateNavigation(ElementOn5, ElementOn7, ElementPeriode, ElementOff6);
        break;

      case ElementOn7:
        evaluateNavigation(ElementOn6, ElementOn8, ElementPeriode, ElementOff7);
        break;

      case ElementOn8:
        evaluateNavigation(ElementOn7, ElementOn8, ElementPeriode, ElementOff8);
        break;

      case ElementOff1:
        evaluateNavigation(ElementOff1, ElementOff2, ElementOn1, ElementOff1);
        break;

      case ElementOff2:
        evaluateNavigation(ElementOff1, ElementOff3, ElementOn2, ElementOff2);
        break;

      case ElementOff3:
        evaluateNavigation(ElementOff2, ElementOff4, ElementOn3, ElementOff3);
        break;

      case ElementOff4:
        evaluateNavigation(ElementOff3, ElementOff5, ElementOn4, ElementOff4);
        break;

      case ElementOff5:
        evaluateNavigation(ElementOff4, ElementOff6, ElementOn5, ElementOff5);
        break;

      case ElementOff6:
        evaluateNavigation(ElementOff5, ElementOff7, ElementOn6, ElementOff6);
        break;

      case ElementOff7:
        evaluateNavigation(ElementOff6, ElementOff8, ElementOn7, ElementOff7);
        break;

      case ElementOff8:
        evaluateNavigation(ElementOff7, ElementOff8, ElementOn8, ElementOff8);
        break;

      case ElementTrigger:
        evaluateNavigation(ElementTrigger, ElementStepDuration, ElementTrigger, ElementOn1);
        break;

      case ElementStepDuration:
        evaluateNavigation(ElementTrigger, ElementPeriode, ElementStepDuration, ElementStepFactor);
        break;

      case ElementStepFactor:
        evaluateNavigation(ElementTrigger, ElementPeriode, ElementStepDuration, ElementOn2);
        break;

      case ElementPeriode:
        evaluateNavigation(ElementStepDuration, ElementRun, ElementPeriode, ElementOn3);
        break;
        
      case ElementRun:
        evaluateNavigation(ElementPeriode, ElementRun, ElementRun, ElementOn4);
        if (m_selection == SelectionDigit0) {
          m_selection = SelectionAll;
          m_pulseSequenceData.m_runs = 0;
        }
        break;
        
      default:
        break;        
    }
  } else { // if (m_selection == SelectionAll) {
    // changeValue depending on selection and Selected area;
    if (isButtonEvent()) {
      if (m_btnEvent == ButtonHandler::ButtonEvent_PressedUp) {
        incrementElement();
      } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedDown) {
        decrementElement();
      } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedLeft) {
        if ((m_selection < SelectionDigit3) || ((m_selected == ElementStepDuration) && (m_selection < SelectionDigit4))) {
          incrementEnum(m_selection);
        }
      } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedRight) {
        if (m_selection > SelectionDigit0) {
          decrementEnum(m_selection);
        }
      } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedEnter) {
        m_selection = SelectionAll;
      }
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_LongPressedUp) {
      if (m_lastLongPress - millis() >= LONGPRESS_REPEAT_MS) {
        incrementElement();
        m_lastLongPress = millis();
      }
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_LongPressedDown) {
      if (m_lastLongPress - millis() >= LONGPRESS_REPEAT_MS) {
        decrementElement();
        m_lastLongPress = millis();
      }
    }
  }
  
  if ((oldSelected != m_selected) || (oldSelection != m_selection) || (m_pulseSequenceData.m_runs != m_lastRuns)) {
    m_isDataChanged = true;
  }
  
  m_lastRuns = m_pulseSequenceData.m_runs;
  m_previousBtnEvent = m_btnEvent;
}


  
void Statemachine::setButtonState(ButtonHandler::ButtonState btnEvent) {
  m_btnEvent = btnEvent;
}

Statemachine::Element Statemachine::getElementSelected() {
  return m_selected;
}
  
Statemachine::Selection Statemachine::getSelection() {
  return m_selection;
}

bool Statemachine::isDataChanged() {
  return m_isDataChanged;
}


// --- Private section ---

void Statemachine::resetEvents() {
}

void Statemachine::evaluateNavigation(Element upElement, Element downElement, Element leftElement, Element rightElement) {
  if (isButtonEvent()) {
    if (m_btnEvent == ButtonHandler::ButtonEvent_PressedUp) {
      m_selected = upElement;
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedDown) {
      m_selected = downElement;
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedLeft) {
      m_selected = leftElement;
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedRight) {
      m_selected = rightElement;
    } else if (m_btnEvent == ButtonHandler::ButtonEvent_PressedEnter) {
      m_selection = SelectionDigit0;
    }
  }
}

bool Statemachine::isButtonEvent() {
  return (m_btnEvent != m_previousBtnEvent);
}

void Statemachine::incrementElement() {
  if ((m_selected >= ElementOn1) && (m_selected <= ElementOn8)) {
    if (m_pulseSequenceData.m_pulseDef[m_selected - ElementOn1].on <= (m_pulseSequenceData.m_period - 1 - calcIncrement())) {
       m_pulseSequenceData.m_pulseDef[m_selected - ElementOn1].on += calcIncrement();
    }

  } else if ((m_selected >= ElementOff1) && (m_selected <= ElementOff8)) {
    if (m_pulseSequenceData.m_pulseDef[m_selected - ElementOff1].off <= (m_pulseSequenceData.m_period - 1 - calcIncrement())) {
       m_pulseSequenceData.m_pulseDef[m_selected - ElementOff1].off += calcIncrement();
    }
  
  } else if (m_selected == ElementTrigger) {
    if (m_pulseSequenceData.m_triggerType < PulseSequenceData::LOW_EDGE) {
      incrementEnum(m_pulseSequenceData.m_triggerType);
    }
  
  } else if (m_selected == ElementStepDuration) {
    if (m_pulseSequenceData.m_stepDuration <= PulseSequenceData::MAX_STEP_DURATION - calcIncrement()) {
      m_pulseSequenceData.m_stepDuration += calcIncrement();
    }
    
  } else if (m_selected == ElementStepFactor) {
    if (m_pulseSequenceData.m_selectedStepFactor < PulseSequenceData::HOUR) {
      incrementEnum(m_pulseSequenceData.m_selectedStepFactor);
    }
  
  } else if (m_selected == ElementPeriode) {
    if (m_pulseSequenceData.m_period <= PulseSequenceData::MAX_PERIOD - calcIncrement()) {
      m_pulseSequenceData.m_period += calcIncrement();
    }
  }
  m_isDataChanged = true;
}

void Statemachine::decrementElement() {
  if ((m_selected >= ElementOn1) && (m_selected <= ElementOn8)) {
    if (m_pulseSequenceData.m_pulseDef[m_selected - ElementOn1].on >= calcIncrement()) {
       m_pulseSequenceData.m_pulseDef[m_selected - ElementOn1].on -= calcIncrement();
    }

  } else if ((m_selected >= ElementOff1) && (m_selected <= ElementOff8)) {
    if (m_pulseSequenceData.m_pulseDef[m_selected - ElementOff1].off >= calcIncrement()) {
       m_pulseSequenceData.m_pulseDef[m_selected - ElementOff1].off -= calcIncrement();
    }
  
  } else if (m_selected == ElementTrigger) {
    if (m_pulseSequenceData.m_triggerType > PulseSequenceData::PERIOD) {
      decrementEnum(m_pulseSequenceData.m_triggerType);
    }
  
  } else if (m_selected == ElementStepDuration) {
    if (m_pulseSequenceData.m_stepDuration >= PulseSequenceData::MIN_STEP_DURATION + calcIncrement()) {
      m_pulseSequenceData.m_stepDuration -= calcIncrement();
    }
    
  } else if (m_selected == ElementStepFactor) {
    if (m_pulseSequenceData.m_selectedStepFactor > PulseSequenceData::SEC) {
      decrementEnum(m_pulseSequenceData.m_selectedStepFactor);
    }
  
  } else if (m_selected == ElementPeriode) {
    if (m_pulseSequenceData.m_period >= PulseSequenceData::MIN_PERIOD + calcIncrement()) {
      m_pulseSequenceData.m_period -= calcIncrement();
    }
  }
  m_isDataChanged = true;
}

uint16_t Statemachine::calcIncrement() {
  uint16_t increment = 1;
  for ( Selection i = SelectionDigit0 ; i < m_selection ; incrementEnum(i)) {
    increment *= 10;
  }
  return increment;
}

template<typename ENUM_TYPE> void Statemachine::incrementEnum(ENUM_TYPE &x) {
  x = static_cast<ENUM_TYPE>(static_cast<uint16_t>(x) + 1);
  m_isDataChanged = true;
}


template<typename ENUM_TYPE> void Statemachine::decrementEnum(ENUM_TYPE &x) {
  x = static_cast<ENUM_TYPE>(static_cast<uint16_t>(x) - 1);
  m_isDataChanged = true;
}

