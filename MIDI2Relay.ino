/*
  MIDI to Relay / Opto Converter
  Arduino Nano + 74HC595 + ULN2803
  Base version: 4 shift registers = 32 outputs
  По умолчанию этот код читает все MIDI-каналы и управляет нотами 36–67, 
  потому что FIRST_NOTE = 36, а выходов 32.

*/

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();


// =========================
// MIDI CHANNEL SETTINGS
// =========================

// Раскомментируй только ОДНУ строку:

#define MIDI_OMNI
// #define MIDI_CHANNEL 1


// =========================
// SHIFT REGISTER SETTINGS
// =========================

#define DATA_PIN   11
#define CLOCK_PIN  13
#define LATCH_PIN  10

// Количество 74HC595
// 4 регистра = 32 выхода
// 8 регистров = 64 выхода
// 16 регистров = 128 выходов
#define NUM_SHIFT_REGISTERS 4

#define TOTAL_OUTPUTS (NUM_SHIFT_REGISTERS * 8)

byte shiftRegisterBuffer[NUM_SHIFT_REGISTERS];


// =========================
// MIDI NOTE SETTINGS
// =========================

// Первая MIDI нота
// Например:
// 36 = C2 / Bass Drum в GM Drum Map
// 60 = C4 / Middle C
#define FIRST_NOTE 36


// =========================
// UPDATE SHIFT REGISTERS
// =========================

void updateShiftRegisters()
{
  digitalWrite(LATCH_PIN, LOW);

  for (int i = NUM_SHIFT_REGISTERS - 1; i >= 0; i--)
  {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, shiftRegisterBuffer[i]);
  }

  digitalWrite(LATCH_PIN, HIGH);
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
// NOTE ON HANDLER
// =========================

void handleNoteOn(byte channel, byte note, byte velocity)
{
#ifndef MIDI_OMNI
  if (channel != MIDI_CHANNEL)
    return;
#endif

  if (velocity == 0)
  {
    handleNoteOff(channel, note, velocity);
    return;
  }

  int output = note - FIRST_NOTE;

  if (output < 0 || output >= TOTAL_OUTPUTS)
    return;

  setOutput(output, true);
}


// =========================
// NOTE OFF HANDLER
// =========================

void handleNoteOff(byte channel, byte note, byte velocity)
{
#ifndef MIDI_OMNI
  if (channel != MIDI_CHANNEL)
    return;
#endif

  int output = note - FIRST_NOTE;

  if (output < 0 || output >= TOTAL_OUTPUTS)
    return;

  setOutput(output, false);
}


// =========================
// SETUP
// =========================

void setup()
{
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (int i = 0; i < NUM_SHIFT_REGISTERS; i++)
  {
    shiftRegisterBuffer[i] = 0;
  }

  updateShiftRegisters();

#ifdef MIDI_OMNI
  MIDI.begin(MIDI_CHANNEL_OMNI);
#else
  MIDI.begin(MIDI_CHANNEL);
#endif

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
}


// =========================
// LOOP
// =========================

void loop()
{
  MIDI.read();
}