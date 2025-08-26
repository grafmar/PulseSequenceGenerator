#ifndef PULSESEQUENCEGENERATOR_H
#define PULSESEQUENCEGENERATOR_H


class PulseSequenceData {
public:

  struct PulseDef {
    uint16_t on;
    uint16_t off;
  } pulseDef_t;
  
  enum SelectedStepFactor {
    SEC = 0,
    MIN,
    HOUR
  };
  
  enum TriggerType {
    PERIOD = 0,
    HIGH_EDGE,
    LOW_EDGE
  };
  
  //// Constants
  static const uint8_t    CHANNELS          = 8;
  // Step factor to achieve two decimal place (000.00s)
  static const uint32_t   STEP_FACTOR_SEC   = 10;
  static const uint32_t   STEP_FACTOR_MIN   = (STEP_FACTOR_SEC * 60);
  static const uint32_t   STEP_FACTOR_HOUR  = (STEP_FACTOR_MIN * 60);
  static const uint32_t   MIN_STEP_DURATION = 1UL;
  static const uint32_t   MAX_STEP_DURATION = 99999UL;
  static const uint16_t   MIN_PERIOD        = 2;
  static const uint16_t   MAX_PERIOD        = 9999;

  //// Instance members
  TriggerType m_triggerType = PERIOD;
  uint32_t m_stepDuration     = 100; // 1s : 001.00 -> 00100
  SelectedStepFactor m_selectedStepFactor = SEC;
  uint16_t m_period           = 10;
  uint32_t m_runs             = 0;
  PulseDef m_pulseDef[CHANNELS] = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 4},
      {5, 4},
      {6, 5},
      {7, 6},
      {8, 7},
    };
} ;




#endif // PULSESEQUENCEGENERATOR_H

