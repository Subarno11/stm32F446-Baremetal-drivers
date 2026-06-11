# STM32 Bare-Metal Driver Development

A complete STM32 bare-metal driver development project built from scratch using Embedded C and register-level programming.<br>
No HAL libraries were used for peripheral operation.<br>
Developed as part of the  Embedded Systems learning journey.

---

## 🚀 Features of Driver Layer

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

### BSP (Board Support Package)

The BSP layer contains board-level peripheral modules built on top of the generic drivers. It abstracts hardware-specific details from the respective datasheets and provides easy-to-use APIs for application development.

Implemented modules:
- **DS1307 RTC Driver** – Time and date configuration and retrieval over I2C.
- **16x2 LCD Driver** – LCD initialization, command handling, and data display functions.

The BSP layer utilizes the underlying GPIO and I2C drivers while exposing higher-level application-friendly interfaces.
---

## 🛠️ Technologies Used

* Embedded C
* STM32F446ReTx Nucleo Series Microcontroller
* ARM Cortex-M4 Architecture
* Register-Level Programming
* STM32CubeIDE
---

## 🧪 Test Applications

The drivers were verified through multiple hardware test applications:

* LED Control using GPIOs
* Push Button Interrupt Handling
* SPI Communication between STM32 and Arduino by both polling and non polling APIs
* I2C Peripheral Communication by both polling and non polling APIs
* USART Transmission and Reception by both polling and non polling APIs
* Interrupt-Based Communication Test using the method of Event driven State Machines
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

## 👨‍💻 Author

**Subarno Pal**<br>
B.Tech, Electronics & Communication Engineering<br>
National Institute of Technology Durgapur<br>

Interested in Embedded Systems, Firmware Development.
