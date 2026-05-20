/*
  MIDI to Relay / Opto
  Arduino Nano + MIDI IN + 74HC595 + ULN2803

  MIDI in: RX / D0

  SPI wiring:
  D11 = DATA / MOSI
  D13 = CLOCK / SCK
  D10 = LATCH

  Safe start:
  LDEN → D9 Arduino (LDEN это вход на модулях с 595й микросхемой)
  LDEN → 10k → +5V

  Panic button:
  D2 → кнопка → GND

  LED подключаются на выход драйвера через 1к резистор
  
  На плате драйвера:
  COM: +5 или +12
  GND: общий с Frduino, платами сдвиговых регистров, платами драйверов.
  На Arduino больше 5в подавать нельзя.
*/

#include <MIDI.h>
#include <SPI.h>

MIDI_CREATE_DEFAULT_INSTANCE();


// =========================
// MIDI CHANNEL SETTINGS
// =========================

//#define MIDI_OMNI
#define MIDI_CHANNEL 1


// =========================
// PLAY MODE SETTINGS
// =========================

//#define TOGGLE_MODE (Для тестирования, после получения Note On нота остается нажатой, 
// выключается только после повторного получения Note On)


// =========================
// SUSTAIN SETTINGS
// =========================

//#define SUSTAIN_ENABLE (Без этой опции midi сообщения о нажатии педали sustain игнорируются)
#define SUSTAIN_IGNORE // (Раскомментировать эту опцию, закомментировать предыдущую, чтоб педать sustain работала)


// =========================
// TEST MODE
// =========================

//#define OUTPUT_TEST_MODE (Если раскомментировать, будут по очереди загораться светодиоды, 
// с самого нижнего до самого верхнего)
#define TEST_DELAY_MS 250


// =========================
// PIN SETTINGS
// =========================

#define DATA_PIN        11
#define CLOCK_PIN       13
#define LATCH_PIN       10
#define LDEN_ENABLE_PIN 9
#define PANIC_BUTTON_PIN 2


// =========================
// SHIFT REGISTER SETTINGS
// =========================

// 1 розовый модуль = 3 микросхемы 74HC595 = 24 выхода
// 2 розовых модуля = 6 микросхем 74HC595 = 48 выходов
#define NUM_SHIFT_REGISTERS 4
#define TOTAL_OUTPUTS (NUM_SHIFT_REGISTERS * 8)

byte shiftRegisterBuffer[NUM_SHIFT_REGISTERS];


// =========================
// MIDI NOTE SETTINGS
// =========================

#define FIRST_NOTE 53 // 36=C2, 48=C3, F3-53


// =========================
// SUSTAIN PEDAL SETTINGS
// =========================

#define SUSTAIN_CC 64

bool sustainActive = false;
bool noteIsOn[TOTAL_OUTPUTS];
bool sustained[TOTAL_OUTPUTS];


// =========================
// OUTPUT ENABLE / DISABLE
// =========================

void outputsDisable()
{
  digitalWrite(LDEN_ENABLE_PIN, HIGH);
}

void outputsEnable()
{
  digitalWrite(LDEN_ENABLE_PIN, LOW);
}


// =========================
// UPDATE SHIFT REGISTERS
// =========================

void updateShiftRegisters()
{
  digitalWrite(LATCH_PIN, LOW);

  for (int i = NUM_SHIFT_REGISTERS - 1; i >= 0; i--)
  {
    SPI.transfer(shiftRegisterBuffer[i]);
  }

  digitalWrite(LATCH_PIN, HIGH);
}


// =========================
// ALL OUTPUTS OFF
// =========================

void allOutputsOff()
{
  for (int i = 0; i < NUM_SHIFT_REGISTERS; i++)
  {
    shiftRegisterBuffer[i] = 0;
  }

  for (int i = 0; i < TOTAL_OUTPUTS; i++)
  {
    noteIsOn[i] = false;
    sustained[i] = false;
  }

  sustainActive = false;

  updateShiftRegisters();
}


// =========================
// MIDI PANIC
// =========================

void midiPanic()
{
  allOutputsOff();
}


// =========================
// SET OUTPUT
// =========================

void setOutput(int output, bool state)
{
  if (output < 0 || output >= TOTAL_OUTPUTS)
    return;

  int reg = output / 8;
  int bit = output % 8;

  if (state)
    bitSet(shiftRegisterBuffer[reg], bit);
  else
    bitClear(shiftRegisterBuffer[reg], bit);

  updateShiftRegisters();
}


// =========================
// GET OUTPUT STATE
// =========================

bool getOutputState(int output)
{
  if (output < 0 || output >= TOTAL_OUTPUTS)
    return false;

  int reg = output / 8;
  int bit = output % 8;

  return bitRead(shiftRegisterBuffer[reg], bit);
}


// =========================
// CHANNEL FILTER
// =========================

bool channelAllowed(byte channel)
{
#ifndef MIDI_OMNI
  if (channel != MIDI_CHANNEL)
    return false;
#endif

  return true;
}


// =========================
// NOTE ON HANDLER
// =========================

void handleNoteOn(byte channel, byte note, byte velocity)
{
  if (!channelAllowed(channel))
    return;

  if (velocity == 0)
  {
    handleNoteOff(channel, note, velocity);
    return;
  }

  int output = note - FIRST_NOTE;

  if (output < 0 || output >= TOTAL_OUTPUTS)
    return;

#ifdef TOGGLE_MODE

  setOutput(output, !getOutputState(output));

#else

  // Для лиг/повторных Note On:
  // если клапан уже открыт, повторный Note On игнорируем.
  if (getOutputState(output))
    return;

  noteIsOn[output] = true;
  sustained[output] = false;

  setOutput(output, true);

#endif
}


// =========================
// NOTE OFF HANDLER
// =========================

void handleNoteOff(byte channel, byte note, byte velocity)
{
  if (!channelAllowed(channel))
    return;

  int output = note - FIRST_NOTE;

  if (output < 0 || output >= TOTAL_OUTPUTS)
    return;

#ifdef TOGGLE_MODE

  return;

#else

  noteIsOn[output] = false;

#ifdef SUSTAIN_ENABLE
#ifndef SUSTAIN_IGNORE
  if (sustainActive)
  {
    sustained[output] = true;
  }
  else
#endif
#endif
  {
    sustained[output] = false;
    setOutput(output, false);
  }

#endif
}


// =========================
// CONTROL CHANGE HANDLER
// =========================

void handleControlChange(byte channel, byte control, byte value)
{
  // Panic / All Sound Off / Reset Controllers / All Notes Off
  // Panic должен работать независимо от выбранного MIDI-канала.
  if (control == 120 || control == 121 || control == 123)
  {
    midiPanic();
    return;
  }

  if (!channelAllowed(channel))
    return;

  if (control != SUSTAIN_CC)
    return;

#ifdef TOGGLE_MODE
  return;
#endif

#ifdef SUSTAIN_IGNORE
  return;
#endif

#ifndef SUSTAIN_ENABLE
  return;
#endif

  if (value >= 64)
  {
    sustainActive = true;
  }
  else
  {
    sustainActive = false;

    for (int i = 0; i < TOTAL_OUTPUTS; i++)
    {
      if (sustained[i] && !noteIsOn[i])
      {
        sustained[i] = false;
        setOutput(i, false);
      }
    }
  }
}


// =========================
// PANIC BUTTON CHECK
// =========================

void checkPanicButton()
{
  static bool lastReading = HIGH;
  static bool stableState = HIGH;
  static unsigned long lastChangeTime = 0;

  bool reading = digitalRead(PANIC_BUTTON_PIN);

  if (reading != lastReading)
  {
    lastChangeTime = millis();
    lastReading = reading;
  }

  if ((millis() - lastChangeTime) > 30)
  {
    if (reading != stableState)
    {
      stableState = reading;

      if (stableState == LOW)
      {
        midiPanic();
      }
    }
  }
}


// =========================
// SETUP
// =========================

void setup()
{
  pinMode(LDEN_ENABLE_PIN, OUTPUT);
  outputsDisable();

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, HIGH);

  pinMode(PANIC_BUTTON_PIN, INPUT_PULLUP);

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  allOutputsOff();

  delay(100);

  outputsEnable();

#ifndef OUTPUT_TEST_MODE
#ifdef MIDI_OMNI
  MIDI.begin(MIDI_CHANNEL_OMNI);
#else
  MIDI.begin(MIDI_CHANNEL);
#endif

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
#endif
}


// =========================
// LOOP
// =========================

void loop()
{
#ifdef OUTPUT_TEST_MODE

  static int testOutput = 0;

  allOutputsOff();
  setOutput(testOutput, true);

  testOutput++;

  if (testOutput >= TOTAL_OUTPUTS)
  {
    testOutput = 0;
  }

  delay(TEST_DELAY_MS);

#else

  checkPanicButton();
  MIDI.read();

#endif
}