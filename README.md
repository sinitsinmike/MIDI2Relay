# MIDI2Relay
MIDI2Relay

<img width="1536" height="1024" alt="midi2relay" src="https://github.com/user-attachments/assets/a8f42767-862f-42eb-ab29-0320b16244dc" />


<img width="1280" height="960" alt="2026-05-11 13 58 36" src="https://github.com/user-attachments/assets/4a8c1b89-04d6-41be-9eb5-e02cef822bc4" />

<img width="960" height="1280" alt="2026-05-11 13 58 39" src="https://github.com/user-attachments/assets/4d86542a-740f-4eac-877b-83b46933fec2" />

Очень важно по питанию
НЕ питать реле от Arduino Nano.
Правильно:
	• Nano отдельно 5V
	• реле отдельно 12V/24V
	• GND общий

Рекомендую
Для надежности:
	• 100nF возле каждого 74HC595
	• 100nF возле Nano
	• 470uF на линии реле
	• отдельное питание реле

H11L1 vs 6N137
Лучше всего:
	1. 6N137
	2. H11L1
	3. 6N138
	4. 6N139
Для быстрого MIDI:
	• 6N137 самый стандартный вариант

Что можно добавить дальше
Можно легко добавить:
	• Sustain pedal
	• Velocity threshold
	• Toggle mode
	• Latch mode
	• Panic
	• MIDI CC
	• Program Change
	• EEPROM presets
	• OLED меню
	• USB MIDI
	• DIN + USB одновременно
	• MIDI THRU
Если хочешь — следующим сообщением могу сделать:
	• полную схему подключения в виде картинки
	• PCB layout
	• version для 64/128 реле
	• ultra-fast version без библиотеки MIDI
	• version с debounce/anti-chatter для реле
	• опторазвязанные выходы вместо реле
	• MIDI drum mapper GM standard
	• таблицу нот GM Drum Map.

/*
  MIDI to Relay / Opto Converter
  Arduino Nano + 74HC595 + ULN2803
  Base version: 4 shift registers = 32 outputs
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

По умолчанию этот код читает все MIDI-каналы и управляет нотами 36–67, потому что FIRST_NOTE = 36, а выходов 32.

Проект: MIDI → Relay/Opto на Arduino Nano
У тебя получается классическая схема:
	• MIDI IN через оптопару (H11L1 / 6N137 / 6N138 / 6N139)
	• Arduino Nano принимает MIDI
	• 74HC595 расширяют количество выходов
	• ULN2803 коммутируют реле/оптроны/соленоиды
Архитектура очень правильная — Nano использует всего 3 пина для любого количества нот.

Как это работает
MIDI NOTE ON:
	• включает соответствующий выход
MIDI NOTE OFF:
	• выключает соответствующий выход
Можно сделать:
	• 8 нот
	• 16 нот
	• 24
	• 32
	• 64
	• и т.д.
Количество определяется числом:
	• 74HC595
	• ULN2803

Подключение модулей
1. Arduino Nano → 74HC595
Для всех 74HC595:
Arduino Nano	74HC595
D11	DS (SER)
D13	SH_CP (SRCLK)
D10	ST_CP (RCLK)
5V	VCC
GND	GND

2. Цепочка нескольких 74HC595
Первый 595:
	• Q7S → DS следующего
И так далее.
Clock и Latch общие на все микросхемы.

3. 74HC595 → ULN2803
74HC595	ULN2803
Q0	IN1
Q1	IN2
Q2	IN3
Q3	IN4
Q4	IN5
Q5	IN6
Q6	IN7
Q7	IN8

4. ULN2803 → Реле
Пример одного канала:
ULN2803	Реле
OUT1	минус катушки
COM	+V через защитные диоды
GND системы	общий

Другой вывод катушки:
	• на +12V (или твое напряжение)
ULN2803 коммутирует землю.
5. MIDI IN на H11L1 / 6N137
MIDI DIN-5
MIDI DIN	Назначение
Pin 4	+ через 220Ω
Pin 5	сигнал через 220Ω
Pin 2	GND

Для 6N137
Вход:
MIDI pin 4 → 220Ω → anode
MIDI pin 5 → cathode
Выход:
6N137	Arduino
OUT	D0 RX
VCC	5V
GND	GND

Pull-up:
	• 4.7k между OUT и 5V

<img width="1306" height="15404" alt="image" src="https://github.com/user-attachments/assets/518d281e-156c-4541-a8be-6222e9961726" />
