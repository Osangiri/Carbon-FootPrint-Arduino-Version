<img width="4030" height="2070" alt="IMG_0132" src="https://github.com/user-attachments/assets/60b4ac6e-65b3-474c-b13a-db036eb3ea5b" />

# Carbon-FootPrint-Arduino-Version
Arduino + Python carbon footprint calculator using sensor data, real-time CO₂ estimates, LCD feedback, and LED indicators.
# 🌱 Carbon Footprint Calculator

A work-in-progress **Arduino + Python carbon footprint calculator** that integrates sensor data, real-time processing, an LCD display, and LED indicators to explore environmental monitoring and sustainable technology.

## 📌 Project Overview

This project explores how hardware sensors and software can work together to create an interactive sustainability-focused system.

An **LDR (photoresistor)** collects real-world light data, which is read by an Arduino and transmitted to a Python program through serial communication. Python processes the sensor data, generates a prototype CO₂ estimate, determines a footprint level, and sends the result back to the Arduino.

The Arduino then displays the result on an LCD and uses LEDs to visually indicate the footprint level.

## ⚙️ How It Works

```text
LDR Sensor
    ↓
Arduino reads sensor
    ↓
Sensor data sent through Serial
    ↓
Python processes the data
    ↓
Prototype CO₂ estimate calculated
    ↓
LOW / MEDIUM / HIGH classification
    ↓
Result sent back to Arduino
    ↓
LCD Display + LED Indicator
```

## 🛠️ Technologies

* Arduino Uno
* Python
* C/C++
* PySerial
* LDR / Photoresistor
* 16x2 LCD
* LEDs
* Breadboard and electronic components

## 💡 Current Features

* Real-time light sensor readings
* Arduino-to-Python serial communication
* Python-based sensor data processing
* Prototype CO₂ footprint estimation
* LOW, MEDIUM, and HIGH footprint classification
* Real-time LCD output
* Green, yellow, and red LED feedback

## 🧪 Current Prototype

The current version uses an LDR to demonstrate the complete sensor-to-software pipeline.

The LDR measures environmental light intensity, and Python maps the sensor readings to prototype footprint levels. The current CO₂ values are experimental mappings and should **not be interpreted as direct measurements of carbon emissions**.

The goal of this stage is to establish and test the complete communication system between the physical sensor, Arduino, Python processing, and physical output.

## 🚧 Work in Progress

Future development will focus on:

* Adding additional environmental sensors
* Integrating temperature sensing
* Developing a more scientifically grounded carbon-footprint calculation model
* Improving sensor calibration
* Expanding sustainability recommendations
* Improving the user interface and physical prototype

## 🎯 Project Goal

The long-term goal is to create an interactive system that combines **embedded systems, environmental sensing, and software** to help users better understand energy use and carbon footprints.

This project is currently under active development.
<img width="4031" height="1726" alt="IMG_0131" src="https://github.com/user-attachments/assets/be436871-f8c7-457f-8cea-c5126b5c6ec3" />


