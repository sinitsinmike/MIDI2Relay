# MIDI → Relay / Opto Converter
## Arduino Nano + 74HC595 + ULN2803

# Project Description

This project converts incoming MIDI signals into discrete outputs for controlling:

- relays
- optocouplers
- solenoids
- lamps
- automation systems
- electromechanical devices

The MIDI input uses standard DIN-5 MIDI IN with opto-isolation.

## Project Architecture

- MIDI IN through optocoupler
- Arduino Nano receives MIDI
- 74HC595 expand outputs
- ULN2803 drive external loads

Main advantage:
Arduino Nano uses only 3 digital pins regardless of output count.

---

# Operating Principle

## MIDI NOTE ON

When MIDI NOTE ON is received:
- corresponding output turns ON

## MIDI NOTE OFF

When MIDI NOTE OFF is received:
- corresponding output turns OFF

---

# System Scaling

| 74HC595 | ULN2803 | Outputs |
|---|---|---|
| 1 | 1 | 8 |
| 2 | 2 | 16 |
| 4 | 4 | 32 |
| 8 | 8 | 64 |
| 16 | 16 | 128 |

Base version:
- 4x 74HC595
- 4x ULN2803
- 32 outputs

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

# Multiple 74HC595 Chain

First 74HC595:
- Q7S → next register DS

Shared lines:
- CLOCK
- LATCH
- GND
- 5V

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

# ULN2803 → Relay

| ULN2803 | Relay |
|---|---|
| OUT1 | Coil negative |
| COM | +V through protection diodes |
| GND | System ground |

Other relay coil terminal:
- +12V or +24V

ULN2803 switches ground.

---

# MIDI IN

## MIDI DIN-5

| MIDI DIN | Function |
|---|---|
| Pin 4 | + through 220Ω |
| Pin 5 | Signal through 220Ω |
| Pin 2 | GND |

---

# 6N137 Connection

## Input

- MIDI pin 4 → 220Ω → anode
- MIDI pin 5 → cathode

## Output

| 6N137 | Arduino Nano |
|---|---|
| OUT | D0 RX |
| VCC | 5V |
| GND | GND |

Pull-up:
- 4.7k between OUT and 5V

---

# Recommended Optocouplers

1. 6N137
2. H11L1
3. 6N138
4. 6N139

For fast and reliable MIDI, 6N137 is recommended.

---

# Power Supply

## IMPORTANT

Do NOT power relays directly from Arduino Nano.

## Correct Power Scheme

| Device | Power |
|---|---|
| Arduino Nano | 5V |
| 74HC595 | 5V |
| ULN2803 | 5V |
| Relays | separate 12V / 24V |

Required:
- common GND between Arduino and relay supply

---

# Reliability Recommendations

- 100nF near every 74HC595
- 100nF near Arduino Nano
- 470uF on relay power rail
- separate relay power supply
- thick relay power traces
- solid GND polygon

---

# Firmware Settings

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

# Note Settings

```cpp
#define FIRST_NOTE 36
```

The note range is calculated automatically from register count.

Example:
- 4 registers
- 32 outputs
- MIDI notes 36–67

---

# Arduino Pin Usage

| Function | Arduino Nano |
|---|---|
| MIDI RX | D0 |
| DATA | D11 |
| CLOCK | D13 |
| LATCH | D10 |

---

# Future Expansion

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
- DIN + USB simultaneous
- MIDI THRU
- GM Drum Map
- Ultra-fast firmware
- Anti-chatter relay mode
