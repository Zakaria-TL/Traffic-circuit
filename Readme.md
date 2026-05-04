# Smart Intersection with Tramway Barrier 🚦🚆

An Arduino-based embedded system designed to manage a smart traffic intersection integrated with a tramway safety barrier. The system utilizes **Timer Interrupts** for precise timing and **UART Interrupts** for real-time manual control.

## 🌟 Features

- **Automatic Mode (AUTO):** Standard traffic light cycle (Green -> Yellow -> Red).
- **Manual Override:** Forcing specific light states via Serial commands (e.g., `SET_GREEN1=5000`).
- **Tramway Safety Mode:** Closes the barrier and sets all traffic lights to Red to allow the tram to pass safely.
- **Emergency STOP Mode:** Flashing yellow lights for maintenance or emergency situations.
- **Real-Time Execution:** Built using AVR Timer2 interrupts for 100ms precision.

## 🛠️ Hardware Components

- **Arduino Uno** (or compatible board).
- **Traffic Lights:** 6 LEDs (2x Red, 2x Yellow, 2x Green).
- **Tramway Barrier:** 1 Servo Motor.
- **Resistors:** 220Ω for each LED.
- **Breadboard & Jumper wires.**

## 📟 Serial Commands

You can control the system by typing the following commands in the Serial Monitor (9600 Baud):

| Command        | Action                                     |
| :------------- | :----------------------------------------- |
| `START`        | Switch to Automatic mode.                  |
| `STOP`         | Enable flashing yellow lights (Emergency). |
| `TRAM ON`      | Close barrier and set lights to Red.       |
| `TRAM OFF`     | Open barrier and return to Auto mode.      |
| `SET_GREEN1=X` | Force Light 1 to Green for X milliseconds. |
| `SET_RED1=X`   | Force Light 1 to Red for X milliseconds.   |

> **Note:** The system automatically returns to `AUTO` mode after the manual timeout expires.

## 🔧 Circuit Connection

| Component                | Arduino Pin     |
| :----------------------- | :-------------- |
| **Light 1 (R, Y, G)**    | Pins 13, 12, 11 |
| **Light 2 (R, Y, G)**    | Pins 4, 3, 2    |
| **Servo Motor (Signal)** | Pin 9           |

## 🚀 Getting Started

1.  Clone this repository.
2.  Open the `.ino` file in the **Arduino IDE**.
3.  Connect your Arduino board and upload the code.
4.  Open the **Serial Monitor** (Set to **9600 Baud**).
5.  (Optional) If using **Tinkercad**, simply paste the code into the code editor.

## 📜 Technical Details

The project core logic resides in the `ISR(TIMER2_COMPA_vect)` function, ensuring that light transitions and manual timers are independent of the main `loop()` execution. This guarantees a deterministic real-time behavior.

---

Developed as a university project for **Real-Time Systems**.
