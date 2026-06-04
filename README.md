# STM32 Bare-Metal Driver Development

A complete STM32 bare-metal driver development project built from scratch using Embedded C and register-level programming.<br>
No HAL libraries were used for peripheral operation.<br>
Developed as part of the MCU1 Embedded Systems learning journey.

---

## 🚀 Features

### GPIO Driver

* GPIO Initialization and Deinitialization
* Input Mode Configuration
* Output Mode Configuration
* Alternate Function Configuration
* External Interrupt Configuration
* GPIO Read/Write APIs
* GPIO Toggle APIs

### SPI Driver

* Master and Slave Mode Support
* Full Duplex Communication
* Blocking Transmit and Receive APIs
* Interrupt-Based Communication
* SPI Interrupt Handling

### I2C Driver

* Master Transmit
* Master Receive
* ACK Control
* Start/Stop Condition Handling
* Event and Error Interrupt Handling
* Interrupt-Based Communication

### USART Driver

* USART Initialization
* Transmission and Reception
* Interrupt-Based Communication
* Baud Rate Configuration
* USART Interrupt Handling

---

## 🛠️ Technologies Used

* Embedded C
* STM32F4 Series Microcontroller
* ARM Cortex-M4 Architecture
* Register-Level Programming
* STM32CubeIDE
* Git & GitHub

---

## 📂 Repository Structure

```text
.
├── Drivers
│   ├── Inc
│   └── Src
│
├── Applications
│   ├── GPIO_Test
│   ├── SPI_Test
│   ├── I2C_Test
│   └── USART_Test
│
└── README.md
```

---

## 🧪 Test Applications

The drivers were verified through multiple hardware test applications:

* LED Control using GPIO
* Push Button Interrupt Handling
* SPI Communication between STM32 and Arduino
* I2C Peripheral Communication
* USART Transmission and Reception
* Interrupt-Based Communication Tests

---

## 📚 What I Learned

* ARM Cortex-M Fundamentals
* STM32 Peripheral Architecture
* Driver Development Methodology
* Register-Level Programming
* Interrupt Handling
* Embedded C Programming
* Debugging Embedded Applications
* Communication Protocols (SPI, I2C, USART)

---

## 🎯 Future Improvements

* SysTick Driver
* RTC Driver
* General Purpose Timers
* PWM Driver
* ADC Driver
* DMA Driver
* LCD Interfacing
* Complete Embedded Projects Using These Drivers

---

## 👨‍💻 Author

**Subarno Pal**<br>
B.Tech, Electronics & Communication Engineering<br>
National Institute of Technology Durgapur<br>

Interested in Embedded Systems, Firmware Development, Robotics, IoT and VLSI.
