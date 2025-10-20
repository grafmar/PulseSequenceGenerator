// include libraries so they are found in the .cpp/.h files
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>

#include "PulseSequenceData.h"
#include "Statemachine.h"
#include "ButtonHandler.h"
#include "TriggerHandler.h"
#include "Display.h"

PulseSequenceData pulseSequenceData;
ButtonHandler buttonHandler;
Statemachine statemachine(pulseSequenceData);
Display display(pulseSequenceData, statemachine);
TriggerHandler triggerHandler;


uint8_t outputDefinition = B11110000;
uint16_t stepCounter = 0;
uint32_t lastStepTime;
volatile uint32_t stepDurationMs = 1000;

bool goOnce = false;

void setup() {
  DDRD = B11111111;   // set PORTD (digital 7~0) to outputs
  PORTD = outputDefinition;  // initialize output levels to LOW
  buttonHandler.init();
  triggerHandler.init();
  
  // Serial.begin(9600);

  display.init();
  display.drawScreen();
  
  lastStepTime = millis();

  Timer1.initialize(1000); // 1ms timer
  Timer1.attachInterrupt(evaluateOutputs); // evaluateOutputs to run every 1 milliseconds
}

void loop() {
  switch (pulseSequenceData.m_selectedStepFactor) {
    case PulseSequenceData::SEC:
      stepDurationMs = pulseSequenceData.m_stepDuration * PulseSequenceData::STEP_FACTOR_SEC;
      break;
    case PulseSequenceData::MIN:
      stepDurationMs = pulseSequenceData.m_stepDuration * PulseSequenceData::STEP_FACTOR_MIN;
      break;
    case PulseSequenceData::HOUR:
    default:
      stepDurationMs = pulseSequenceData.m_stepDuration * PulseSequenceData::STEP_FACTOR_HOUR;
      break;
  }
    
  buttonHandler.runFilterButtons();
  statemachine.setButtonState(buttonHandler.getState());
  statemachine.run();
  if (statemachine.isDataChanged()) {
    display.drawScreen();
  }
}

void evaluateOutputs() {
  int currentTrigger = triggerHandler.activeTriggerState();
  if ((pulseSequenceData.m_triggerType == PulseSequenceData::CONTINUOUS) || ((pulseSequenceData.m_triggerType == PulseSequenceData::HIGH_EDGE) && (currentTrigger == TriggerHandler::TriggerEvent_HighEdge)) || ((pulseSequenceData.m_triggerType == PulseSequenceData::LOW_EDGE) && (currentTrigger == TriggerHandler::TriggerEvent_LowEdge))) {
    if (!goOnce && (pulseSequenceData.m_triggerType != PulseSequenceData::CONTINUOUS)) {
      lastStepTime = millis();
    }
    goOnce = true;
  }

  if((goOnce == true) ) {
    uint32_t now = millis();
    if (now - lastStepTime >= stepDurationMs) {
      lastStepTime += stepDurationMs;
      stepCounter = (stepCounter + 1) % pulseSequenceData.m_period;
      if ((stepCounter == 0) && (pulseSequenceData.m_runs < 99999UL)) {
        pulseSequenceData.m_runs++;
        goOnce = false;
      }
     updateOutputs();
    }
  }
}

void updateOutputs() {
  for (int i = 0 ; i < PulseSequenceData::CHANNELS ; i++) {
    if (stepCounter == pulseSequenceData.m_pulseDef[i].on) {
      outputDefinition |= 0x01 << i;
    }
    if (stepCounter == pulseSequenceData.m_pulseDef[i].off) {
      outputDefinition &= ~(0x01 << i);
    }
  }
  PORTD = outputDefinition;
}

