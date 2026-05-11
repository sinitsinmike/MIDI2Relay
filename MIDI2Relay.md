MIDI to Relay / Opto Converter

Arduino Nano + 74HC595 + ULN2803

⸻

MD_RU

MIDI → Relay / Opto Converter

Arduino Nano + 74HC595 + ULN2803

Описание проекта

Проект предназначен для преобразования входящего MIDI-сигнала в набор дискретных выходов для управления:

* реле
* оптронами
* соленоидами
* лампами
* внешними устройствами автоматики
* электромеханическими системами

В качестве MIDI-интерфейса используется стандартный DIN-5 MIDI IN через опторазвязку.

Архитектура проекта:

* MIDI IN через оптопару
* Arduino Nano принимает MIDI
* 74HC595 расширяют количество выходов
* ULN2803 управляют нагрузкой

Главное преимущество архитектуры:
Arduino Nano использует всего 3 цифровых пина независимо от количества выходов.

⸻

Принцип работы

MIDI NOTE ON

При получении MIDI NOTE ON:

* соответствующий выход включается

MIDI NOTE OFF

При получении MIDI NOTE OFF:

* соответствующий выход выключается

⸻

Масштабирование системы

Количество выходов зависит от числа:

* 74HC595
* ULN2803

Примеры

Количество 74HC595	Количество ULN2803	Выходов
1	1	8
2	2	16
4	4	32
8	8	64
16	16	128

Базовая версия проекта:

* 4x 74HC595
* 4x ULN2803
* 32 выхода

⸻

Подключение модулей

Arduino Nano → 74HC595

Arduino Nano	74HC595
D11	DS / SER
D13	SH_CP / SRCLK
D10	ST_CP / RCLK
5V	VCC
GND	GND

⸻

Цепочка нескольких 74HC595

Первый 74HC595:

* Q7S → DS следующего регистра

Следующий:

* Q7S → DS следующего

И так далее.

Линии:

* CLOCK
* LATCH
* GND
* 5V

общие для всех регистров.

⸻

74HC595 → ULN2803

74HC595	ULN2803
Q0	IN1
Q1	IN2
Q2	IN3
Q3	IN4
Q4	IN5
Q5	IN6
Q6	IN7
Q7	IN8

⸻

ULN2803 → Реле

Пример подключения одного канала:

ULN2803	Реле
OUT1	Минус катушки
COM	+V через защитные диоды
GND	Общий GND

Другой вывод катушки реле:

* на +12V или +24V

ULN2803 коммутирует землю.

⸻

MIDI IN

MIDI DIN-5

MIDI DIN	Назначение
Pin 4	+ через 220Ω
Pin 5	Signal через 220Ω
Pin 2	GND

⸻

Подключение 6N137

Вход

* MIDI pin 4 → 220Ω → anode
* MIDI pin 5 → cathode

Выход

6N137	Arduino Nano
OUT	D0 RX
VCC	5V
GND	GND

Pull-up:

* 4.7k между OUT и 5V

⸻

Рекомендуемые оптопары

Лучшие варианты

1. 6N137
2. H11L1
3. 6N138
4. 6N139

Рекомендация

Для максимально надежного и быстрого MIDI:

* рекомендуется 6N137

⸻

Питание

ВАЖНО

НЕЛЬЗЯ питать реле напрямую от Arduino Nano.

Правильная схема питания

Устройство	Питание
Arduino Nano	5V
74HC595	5V
ULN2803 (логика)	5V
Реле	отдельное 12V / 24V

Обязательно:

* общий GND между Arduino и питанием реле

⸻

Рекомендации по надежности

Конденсаторы

Рекомендуется:

* 100nF возле каждого 74HC595
* 100nF возле Arduino Nano
* 470uF на линии питания реле

Дополнительно

* отдельное питание реле
* короткие линии GND
* толстые дорожки питания реле
* общий земляной полигон на PCB

⸻

Настройки прошивки

MIDI Omni

#define MIDI_OMNI
// #define MIDI_CHANNEL 1

Чтение MIDI со всех каналов.

⸻

MIDI Channel

// #define MIDI_OMNI
#define MIDI_CHANNEL 1

Чтение только выбранного MIDI-канала.

⸻

Настройки нот

#define FIRST_NOTE 36

Первая MIDI-нота.

Количество нот вычисляется автоматически по количеству регистров.

Пример

4 регистра:

* 32 выхода
* MIDI ноты 36–67

⸻

Используемые Arduino пины

Функция	Arduino Nano
MIDI RX	D0
DATA	D11
CLOCK	D13
LATCH	D10

⸻

Возможности расширения

Проект легко расширяется:

* Sustain pedal
* Velocity threshold
* Toggle mode
* Latch mode
* Panic
* MIDI CC
* Program Change
* EEPROM presets
* OLED menu
* USB MIDI
* DIN + USB одновременно
* MIDI THRU
* Drum mapper
* GM Drum Map
* Опторазвязанные выходы
* Anti-chatter relay mode
* Ultra-fast firmware

⸻

PCB рекомендации

Рекомендуется

* размещать 100nF максимально близко к каждой микросхеме
* использовать сплошной GND polygon
* разделять:
    * логическую землю
    * силовую землю реле
* делать широкие дорожки для:
    * +12V
    * +24V
    * GND реле

Желательно

* TVS защита
* клеммники с шагом 5.08mm
* разъем ISP
* светодиод MIDI activity
* светодиод питания

⸻

MD_EN

MIDI → Relay / Opto Converter

Arduino Nano + 74HC595 + ULN2803

Project Description

This project converts incoming MIDI data into discrete outputs for controlling:

* relays
* optocouplers
* solenoids
* lamps
* automation systems
* electromechanical devices

The MIDI input uses standard DIN-5 MIDI IN with opto-isolation.

Project architecture:

* MIDI IN through optocoupler
* Arduino Nano receives MIDI
* 74HC595 expand outputs
* ULN2803 drive external loads

Main advantage:
Arduino Nano uses only 3 digital pins regardless of output count.

⸻

Operating Principle

MIDI NOTE ON

When MIDI NOTE ON is received:

* corresponding output turns ON

MIDI NOTE OFF

When MIDI NOTE OFF is received:

* corresponding output turns OFF

⸻

System Scaling

The number of outputs depends on:

* 74HC595 count
* ULN2803 count

Examples

74HC595 Count	ULN2803 Count	Outputs
1	1	8
2	2	16
4	4	32
8	8	64
16	16	128

Base version:

* 4x 74HC595
* 4x ULN2803
* 32 outputs

⸻

Arduino Nano → 74HC595

Arduino Nano	74HC595
D11	DS / SER
D13	SH_CP / SRCLK
D10	ST_CP / RCLK
5V	VCC
GND	GND

⸻

Multiple 74HC595 Chain

First 74HC595:

* Q7S → next register DS

Then:

* Q7S → next DS

Shared lines:

* CLOCK
* LATCH
* GND
* 5V

⸻

74HC595 → ULN2803

74HC595	ULN2803
Q0	IN1
Q1	IN2
Q2	IN3
Q3	IN4
Q4	IN5
Q5	IN6
Q6	IN7
Q7	IN8

⸻

ULN2803 → Relay

Single channel example:

ULN2803	Relay
OUT1	Coil negative
COM	+V through protection diodes
GND	System ground

Other relay coil terminal:

* connect to +12V or +24V

ULN2803 switches ground.

⸻

MIDI IN

MIDI DIN-5

MIDI DIN	Function
Pin 4	+ through 220Ω
Pin 5	Signal through 220Ω
Pin 2	GND

⸻

6N137 Connection

Input

* MIDI pin 4 → 220Ω → anode
* MIDI pin 5 → cathode

Output

6N137	Arduino Nano
OUT	D0 RX
VCC	5V
GND	GND

Pull-up:

* 4.7k between OUT and 5V

⸻

Recommended Optocouplers

Best Choices

1. 6N137
2. H11L1
3. 6N138
4. 6N139

Recommendation

For reliable high-speed MIDI:

* 6N137 is recommended

⸻

Power Supply

IMPORTANT

Do NOT power relays directly from Arduino Nano.

Correct Power Scheme

Device	Power
Arduino Nano	5V
74HC595	5V
ULN2803 logic	5V
Relay coils	separate 12V / 24V

Required:

* common GND between Arduino and relay supply

⸻

Reliability Recommendations

Capacitors

Recommended:

* 100nF near every 74HC595
* 100nF near Arduino Nano
* 470uF on relay power rail

Additional

* separate relay power supply
* short GND paths
* thick relay power traces
* solid ground polygon on PCB

⸻

Firmware Settings

MIDI Omni

#define MIDI_OMNI
// #define MIDI_CHANNEL 1

Receive MIDI from all channels.

⸻

MIDI Channel

// #define MIDI_OMNI
#define MIDI_CHANNEL 1

Receive only selected MIDI channel.

⸻

Note Settings

#define FIRST_NOTE 36

First MIDI note.

The total note range is automatically calculated from register count.

Example

4 shift registers:

* 32 outputs
* MIDI notes 36–67

⸻

Arduino Pin Usage

Function	Arduino Nano
MIDI RX	D0
DATA	D11
CLOCK	D13
LATCH	D10

⸻

Future Expansion

Project can easily support:

* Sustain pedal
* Velocity threshold
* Toggle mode
* Latch mode
* Panic
* MIDI CC
* Program Change
* EEPROM presets
* OLED menu
* USB MIDI
* DIN + USB simultaneous
* MIDI THRU
* Drum mapper
* GM Drum Map
* Opto-isolated outputs
* Anti-chatter relay mode
* Ultra-fast firmware

⸻

PCB Recommendations

Recommended

* place 100nF capacitors as close as possible to ICs
* use solid GND polygon
* separate:
    * logic ground
    * relay power ground
* use wide traces for:
    * +12V
    * +24V
    * relay GND

Optional

* TVS protection
* 5.08mm terminal blocks
* ISP header
* MIDI activity LED
* Power LED