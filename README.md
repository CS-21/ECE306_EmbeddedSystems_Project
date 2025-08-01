# ECE 306 Embedded Systems Projects – NC State University

This repository contains a series of cumulative embedded systems projects completed as part of ECE 306 (Introduction to Embedded Systems) at North Carolina State University during Spring 2025.

The course focused on programming the **MSP430FR2355** microcontroller in **C**, implementing real-time control of peripherals, and integrating hardware and software components in an autonomous embedded system.

## Project Overview

Throughout the course, I completed 10 embedded projects that progressively built toward a final functional mobile system. Each project was implemented and demonstrated individually.

### Final System: Line-following vehicle with IoT capability

## Features

- **Motor control** (forward/reverse control with timing)
- **LCD display interface** (real-time data feedback)
- **Black line detection and navigation** (using IR sensors)
- **Serial port communication** (debugging and data exchange)
- **IoT integration** with ESP32 module for remote updates
- **Modular design** using C and custom drivers
- **SIPO/PISO shift register implementation** for sensor reading

## Tools & Hardware Used

- **Texas Instruments MSP430FR2355 LaunchPad**
- **Code Composer Studio (CCS)**
- **ESP32-WROOM-32E** IoT module
- **Analog Discovery for debugging**
- Soldering tools, IR sensors, battery pack, LCD module

## File Structure

- `main.c` – Main firmware logic
- `lcd.c` / `lcd.h` – LCD interface driver
- `motor.c` / `motor.h` – Motor control functions
- `serial.c` – UART communication

## Learning Outcomes

- Real-time control of hardware using C
- State machine design for embedded control
- Debugging low-level MCU behavior with scope and serial output
- Modular driver development (LCD, motors, UART)

