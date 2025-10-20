#include "Display.h"

#define CHAR_WIDTH    6
#define CHAR_HEIGHT   8
#define POS_OFF_COL   (127 - 4*CHAR_WIDTH - 1)
#define POS_ON_COL    (POS_OFF_COL - 4*CHAR_WIDTH - 3)
#define POS_NUM_COL   (POS_ON_COL - CHAR_WIDTH - 3)

#define GRAPH_WIDTH 64
#define GRAPH_Y_OFFSET 32
#define GRAPH_LINE_HEIGHT 2


Display::Display(PulseSequenceData& pulseSequenceData, Statemachine& statemachine):
  m_display(0),
  m_pulseSequenceData(pulseSequenceData),
  m_statemachine(statemachine)
{
}

Display::~Display() {
}

void Display::init() {
  //m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  m_display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // Show image buffer on the display hardware. Since the buffer is intialized with a splashscreen
  // internally, this will display the splashscreen.
  m_display.display();
  delay(500);
}

void Display::drawScreen() {
  m_display.clearDisplay();

  drawSettings();
  drawChannels();
  drawSelected();
  drawGraph();
  
  m_display.display();
}

void Display::drawSettings() {
  String triggerTypeString[] = {"C","H","L"}; // Strings for: CONTINUOUS, HIGH_EDGE, LOW_EDGE
  String stepFactorString[] = {"s","m","h"};  // Strings for: SEC, MIN, HOUR
  char tempString[30];
    
  m_display.setTextSize(1);
  m_display.setTextColor(WHITE);
  m_display.setCursor(0, 0);
  m_display.println("Trigger:");  // Trigger: P |
  m_display.println("S:");        // S: 000.10s |
  m_display.println("P:");        // P:   1000  |
  m_display.println("Run:");      // Run:00000  |

  m_display.setCursor(9*CHAR_WIDTH, 0*CHAR_HEIGHT);
  m_display.print(triggerTypeString[m_pulseSequenceData.m_triggerType]); // Use strings for trigger type
  
  m_display.setCursor(3*CHAR_WIDTH, 1*CHAR_HEIGHT);
  sprintf(tempString, "%03d.", m_pulseSequenceData.m_stepDuration / 100);
  m_display.print(tempString);
  sprintf(tempString, "%02d", m_pulseSequenceData.m_stepDuration % 100);
  m_display.print(tempString);
  m_display.print(stepFactorString[m_pulseSequenceData.m_selectedStepFactor]); // Use strings for step factor

  m_display.setCursor(5*CHAR_WIDTH, 2*CHAR_HEIGHT);
  sprintf(tempString, "%04d", m_pulseSequenceData.m_period);
  m_display.print(tempString);

  m_display.setCursor(4*CHAR_WIDTH, 3*CHAR_HEIGHT);
  sprintf(tempString, "%01d", m_pulseSequenceData.m_runs / 10000UL);
  m_display.print(tempString);
  sprintf(tempString, "%04d", m_pulseSequenceData.m_runs % 10000UL);
  m_display.print(tempString);
}

void Display::drawChannels() {
  char tempString[10];
  
  for (uint8_t ch = 0 ; ch < PulseSequenceData::CHANNELS ; ch++) {
    m_display.setTextSize(1);
    m_display.setTextColor(WHITE);
    
    m_display.setCursor(POS_NUM_COL, ch*8);
    m_display.print(ch+1); // list channels 1..8 instead of 0..7
    
    m_display.setCursor(POS_ON_COL, ch*8);
    sprintf(tempString, "%04d", m_pulseSequenceData.m_pulseDef[ch].on);
    m_display.print(tempString);
    
    m_display.setCursor(POS_OFF_COL, ch*8);
    sprintf(tempString, "%04d", m_pulseSequenceData.m_pulseDef[ch].off);
    m_display.print(tempString);
  }
}

void Display::drawSelected() {
  uint32_t value;
  uint8_t xPos, yPos, width;
  Statemachine::Element selectedElement = m_statemachine.getElementSelected();
  Statemachine::Selection selection = m_statemachine.getSelection();

  m_display.setTextSize(1);
  m_display.setTextColor(BLACK, WHITE);

  if (selection == Statemachine::SelectionAll) {
    width = 4 * CHAR_WIDTH;
  } else {
    width = CHAR_WIDTH;
  }
  
  if ((selectedElement >= Statemachine::ElementOn1) && (selectedElement <= Statemachine::ElementOn8)) {
    xPos = POS_ON_COL;
    yPos = 8 * (selectedElement - Statemachine::ElementOn1);
    if (selection != Statemachine::SelectionAll) {
      xPos += CHAR_WIDTH * (Statemachine::SelectionDigit3 - selection);
    }
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else if ((selectedElement >= Statemachine::ElementOff1) && (selectedElement <= Statemachine::ElementOff8)) {
    xPos = POS_OFF_COL;
    yPos = 8 * (selectedElement - Statemachine::ElementOff1);
    if (selection != Statemachine::SelectionAll) {
      xPos += CHAR_WIDTH * (Statemachine::SelectionDigit3 - selection);
    }
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else if (selectedElement == Statemachine::ElementTrigger) {
    xPos = 9*CHAR_WIDTH;
    yPos = 0*CHAR_HEIGHT;
    width = CHAR_WIDTH;
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else if (selectedElement == Statemachine::ElementStepDuration) {
    xPos = 3*CHAR_WIDTH;
    yPos = 1*CHAR_HEIGHT;
    if (selection == Statemachine::SelectionAll) {
      width = 6 * CHAR_WIDTH;
    } else {
      xPos += CHAR_WIDTH * (Statemachine::SelectionDigit4 - selection);
      if (selection < Statemachine::SelectionDigit2) {
        xPos += CHAR_WIDTH;
      }
    }
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else if (selectedElement == Statemachine::ElementStepFactor) {
    xPos = 9*CHAR_WIDTH;
    yPos = 1*CHAR_HEIGHT;
    width = CHAR_WIDTH;
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else if (selectedElement == Statemachine::ElementPeriode) {
    xPos = 5*CHAR_WIDTH;
    yPos = 2*CHAR_HEIGHT;
    if (selection != Statemachine::SelectionAll) {
      xPos += CHAR_WIDTH * (Statemachine::SelectionDigit3 - selection);
    }
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  } else { // if (selectedElement == Statemachine::ElementRun) {
    xPos = 4*CHAR_WIDTH;
    yPos = 3*CHAR_HEIGHT;
    width = 5*CHAR_WIDTH;
    m_display.fillRect(xPos, yPos, width, CHAR_HEIGHT, INVERSE);
  }
}

void Display::drawGraph() {
  bool isHigh;
  uint16_t drawStepPos;
  uint16_t onPos;
  uint16_t offPos;
  uint8_t xOnPos;
  uint8_t xOffPos;
  uint8_t yPos;
  uint16_t period = calcPeriod();

  // draw the graph
  for (uint8_t ch = 0 ; ch < PulseSequenceData::CHANNELS ; ch++) {
    // calculate positions
    onPos = m_pulseSequenceData.m_pulseDef[ch].on;
    offPos = m_pulseSequenceData.m_pulseDef[ch].off;
    yPos = ch * (GRAPH_LINE_HEIGHT + 2) + GRAPH_Y_OFFSET;
    xOnPos = (onPos * GRAPH_WIDTH + GRAPH_WIDTH / 2) / period;
    xOffPos = (offPos * GRAPH_WIDTH + GRAPH_WIDTH / 2) / period;
    if (xOnPos > GRAPH_WIDTH) { // Out of bounds
      xOnPos = GRAPH_WIDTH;
    }
    if (xOffPos > GRAPH_WIDTH) { // Out of bounds
      xOffPos = GRAPH_WIDTH;
    }
    
    // Draw it
    if (onPos <= offPos) {
      m_display.drawLine(0, yPos + GRAPH_LINE_HEIGHT, xOnPos, yPos + GRAPH_LINE_HEIGHT, WHITE);
      m_display.drawLine(xOnPos, yPos + GRAPH_LINE_HEIGHT, xOnPos, yPos, WHITE);
      m_display.drawLine(xOnPos, yPos, xOffPos, yPos, WHITE);
      m_display.drawLine(xOffPos, yPos, xOffPos, yPos + GRAPH_LINE_HEIGHT, WHITE);
      m_display.drawLine(xOffPos, yPos + GRAPH_LINE_HEIGHT, GRAPH_WIDTH, yPos + GRAPH_LINE_HEIGHT, WHITE);
    } else {
      m_display.drawLine(0, yPos, xOffPos, yPos, WHITE);
      m_display.drawLine(xOffPos, yPos, xOffPos, yPos + GRAPH_LINE_HEIGHT, WHITE);
      m_display.drawLine(xOffPos, yPos + GRAPH_LINE_HEIGHT, xOnPos, yPos + GRAPH_LINE_HEIGHT, WHITE);
      m_display.drawLine(xOnPos, yPos + GRAPH_LINE_HEIGHT, xOnPos, yPos, WHITE);
      m_display.drawLine(xOnPos, yPos, GRAPH_WIDTH, yPos, WHITE);
    }
  }
}

uint16_t Display::calcPeriod() {
  uint16_t period = 0;
  if (m_pulseSequenceData.m_triggerType == PulseSequenceData::CONTINUOUS) {
    period = m_pulseSequenceData.m_period;
  } else {
    for (uint8_t ch = 0 ; ch < PulseSequenceData::CHANNELS ; ch++) {
      if (m_pulseSequenceData.m_pulseDef[ch].on > period) {
        period = m_pulseSequenceData.m_pulseDef[ch].on;
      }
      if (m_pulseSequenceData.m_pulseDef[ch].off > period) {
        period = m_pulseSequenceData.m_pulseDef[ch].off;
      }
    }
  }
  return period;
}


