#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include "Adafruit_SH1106.h"

#include "PulseSequenceData.h"
#include "Statemachine.h"

class Display {
public:
  explicit Display(PulseSequenceData& pulseSequenceData, Statemachine& statemachine);
  ~Display();

  void init();
  
  void drawScreen();

private:

  void drawSettings();
  void drawChannels();
  void drawSelected();
  void drawGraph();
  uint16_t calcPeriod();

  // instance members
  //Adafruit_SSD1306 m_display;
  Adafruit_SH1106 m_display;
  PulseSequenceData &m_pulseSequenceData;
  Statemachine& m_statemachine;
};

#endif // DISPLAY_H

