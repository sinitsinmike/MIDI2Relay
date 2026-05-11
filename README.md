# MIDI2Relay
MIDI2Relay

<img width="1536" height="1024" alt="midi2relay" src="https://github.com/user-attachments/assets/a8f42767-862f-42eb-ab29-0320b16244dc" />


<img width="1280" height="960" alt="2026-05-11 13 58 36" src="https://github.com/user-attachments/assets/4a8c1b89-04d6-41be-9eb5-e02cef822bc4" />

<img width="960" height="1280" alt="2026-05-11 13 58 39" src="https://github.com/user-attachments/assets/4d86542a-740f-4eac-877b-83b46933fec2" />

# MIDI → Relay / Opto Converter
## Arduino Nano + 74HC595 + ULN2803

# Описание проекта

Проект предназначен для преобразования входящего MIDI-сигнала в набор дискретных выходов для управления:

- реле
- оптронами
- соленоидами
- лампами
- внешними устройствами автоматики
- электромеханическими системами

В качестве MIDI-интерфейса используется стандартный DIN-5 MIDI IN через опторазвязку.

## Архитектура проекта

- MIDI IN через оптопару
- Arduino Nano принимает MIDI
- 74HC595 расширяют количество выходов
- ULN2803 управляют нагрузкой

Главное преимущество архитектуры:
Arduino Nano использует всего 3 цифровых пина независимо от количества выходов.

---

# Принцип работы

## MIDI NOTE ON

При получении MIDI NOTE ON:
- соответствующий выход включается

## MIDI NOTE OFF

При получении MIDI NOTE OFF:
- соответствующий выход выключается

---

# Масштабирование системы

| 74HC595 | ULN2803 | Выходов |
|---|---|---|
| 1 | 1 | 8 |
| 2 | 2 | 16 |
| 4 | 4 | 32 |
| 8 | 8 | 64 |
| 16 | 16 | 128 |

Базовая версия:
- 4x 74HC595
- 4x ULN2803
- 32 выхода

---

# Arduino Nano → 74HC595

| Arduino Nano | 74HC595 |
|---|---|
| D11 | DS / SER |
| D13 | SH_CP / SRCLK |
| D10 | ST_CP / RCLK |
| 5V | VCC |
| GND | GND |

---

# Цепочка нескольких 74HC595

Первый 74HC595:
- Q7S → DS следующего регистра

Линии:
- CLOCK
- LATCH
- GND
- 5V

общие для всех регистров.

---

# 74HC595 → ULN2803

| 74HC595 | ULN2803 |
|---|---|
| Q0 | IN1 |
| Q1 | IN2 |
| Q2 | IN3 |
| Q3 | IN4 |
| Q4 | IN5 |
| Q5 | IN6 |
| Q6 | IN7 |
| Q7 | IN8 |

---

# ULN2803 → Реле

| ULN2803 | Реле |
|---|---|
| OUT1 | Минус катушки |
| COM | +V через защитные диоды |
| GND | Общий GND |

Другой вывод катушки:
- +12V или +24V

ULN2803 коммутирует землю.

---

# MIDI IN

## MIDI DIN-5

| MIDI DIN | Назначение |
|---|---|
| Pin 4 | + через 220Ω |
| Pin 5 | Signal через 220Ω |
| Pin 2 | GND |

---

# Подключение 6N137

## Вход

- MIDI pin 4 → 220Ω → anode
- MIDI pin 5 → cathode

## Выход

| 6N137 | Arduino Nano |
|---|---|
| OUT | D0 RX |
| VCC | 5V |
| GND | GND |

Pull-up:
- 4.7k между OUT и 5V

---

# Рекомендуемые оптопары

1. 6N137
2. H11L1
3. 6N138
4. 6N139

Для быстрого и надежного MIDI рекомендуется 6N137.

---

# Питание

## ВАЖНО

НЕ питать реле напрямую от Arduino Nano.

## Правильная схема питания

| Устройство | Питание |
|---|---|
| Arduino Nano | 5V |
| 74HC595 | 5V |
| ULN2803 | 5V |
| Реле | отдельное 12V / 24V |

Обязательно:
- общий GND между Arduino и питанием реле

---

# Рекомендации по надежности

- 100nF возле каждого 74HC595
- 100nF возле Arduino Nano
- 470uF на линии питания реле
- отдельное питание реле
- толстые дорожки питания реле
- общий GND polygon

---

# Настройки прошивки

## MIDI Omni

```cpp
#define MIDI_OMNI
// #define MIDI_CHANNEL 1
```

## MIDI Channel

```cpp
// #define MIDI_OMNI
#define MIDI_CHANNEL 1
```

---

# Настройки нот

```cpp
#define FIRST_NOTE 36
```

Количество нот вычисляется автоматически по количеству регистров.

Пример:
- 4 регистра
- 32 выхода
- MIDI ноты 36–67

---

# Используемые пины Arduino

| Функция | Arduino Nano |
|---|---|
| MIDI RX | D0 |
| DATA | D11 |
| CLOCK | D13 |
| LATCH | D10 |

---

# Возможности расширения

- Sustain pedal
- Velocity threshold
- Toggle mode
- Latch mode
- Panic
- MIDI CC
- Program Change
- EEPROM presets
- OLED menu
- USB MIDI
- DIN + USB одновременно
- MIDI THRU
- GM Drum Map
- Ultra-fast firmware
- Anti-chatter relay mode
