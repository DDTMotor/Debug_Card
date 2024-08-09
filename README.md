[TOC]

# 0. DDT motor development board

-   achieve control of all models of our prototype motors

motor models currently supported:

|      Type      | communication |
| :------------: | :-----------: |
| M0601C_111/411 |     RS485     |
|   M0602C_112   |     RS485     |
| M0603C_111/211 |     uart      |
|   M1502E_111   |      CAN      |
| M1502D_133/233 |      CAN      |
|   M1505A_222   |      CAN      |
| P1010A/P1010B  |      CAN      |
| M0603A_111/411 |      LIN      |

# 1. How to get our open source code ?

**If you don't have a GitHub account**, you can follow these steps to register and use GitHub:

1.  Visit the official GitHub website: [GitHub](https://github.com/)
2.  Click on the "Sign up" button located at the top right corner of the page to begin the registration process.
3.  After completing the registration,you can log in to GitHub and start browsing or cloning open source projects.

You can log into [GitHub](https://github.com/) to access our open source repository at the following address：

https://github.com/DDTMotor/Debug_Card.git

## 1.1 Clone the source code

To obtain the source code, you can clone the repository using Git commands. **If you haven't installed Git yet**, you can visit the [ Git ](https://git-scm.com/) to download and install it. Here are the steps to clone the repository:

1.  Open your command-line tool (Command Prompt or PowerShell on Windows, Terminal on macOS or Linux).

2.  Clone the repository to your local machine with the following command:

    ```bash
    git clone https://github.com/DDTMotor/Debug_Card.git
    ```

3.  After executing the command, Git will automatically create a new directory on your computer and download all the repository files into that directory.

# 2. How to compile and develop

## 2.1 Get ready

Before you begin, make sure you have the following software or drivers installed:

1.  **Install Keil MDK-ARM or GCC**: Ensure that [Keil MDK-ARM](https://www.keil.com/mdk5) or [GCC](https://gcc.gnu.org/) compiler is installed in your development environment.
2.  **Get the Project Source Code**: Follow the previous steps to obtain the source code of the project.
3.  **STM32 package support**：standard library or HAL library
4.  **An debug emulator**：such as J-Link or STLink or DAP-Link

### Code introduction:

1.  This project iis developed using STM32CubeMX in conjunction with the HAL library.
2.  We have adopted the principles of software layering, object orientation, and decoupling in our development process, dividing each functionality into a device layer (responsible for hardware abstraction and direct manipulation) and a task layer (responsible for specific business logic).
3.  For the STM32 standard library, simply change the HAL library code of the device layer for each feature to the standard library code

## 2.2. Choose IDE and Library Type

Before You can start compiling, you need to select an IDE (integrated development environment) and library type. 

This project is based on the STM32 Hal Library, but for completeness, I will also provide guidance on how to use the standard library.

### 2.2.1 Compile with GCC (HAL Library):

Before compiling a HAL library-based project with GCC, you need to ensure that the project framework has been generated using the STM32CubeMX software. 

Here are the steps to use STM32CubeMX:

1.  **Install GCC**: Ensure the GCC compiler is installed on your system, available from the [GCC website](https://gcc.gnu.org/).
2.  **Download STM32CubeMX**: Visit the official ST website, search for STM32CubeMX, and then download the version suitable for your operating system.
3.  **Install STM32 HAL Library**: the project depends on the HAL library, you need to download and install the appropriate HAL library from the ST official website.
4.  **Get the Source Code**: Follow the previous steps to obtain the project's source code.
5.  **Open STM32CubeMX Project**: Double-click the STM32CubeMX project file in our project, which is . ioc files for suffixes
6.  **Configure Makefile**: Ensure the Makefile is correctly set up, including the compiler path, library path, list of source files, etc.
7.  **Compile the Project**: Open the command line in the project directory and execute the `make` command to start compiling.
8.  **Resolve Compilation Issues**: If compilation errors occur, check the Makefile and source code, and update library files or modify the code as necessary.
9.  **Flash the Program**: Use the appropriate flashing tool to flash the `.elf` or `.bin` file generated from the compilation to the microcontroller.

### 2.2.2 Compile with Keil MDK-ARM(HAL Library):

1.  Ensure Keil MDK-ARM software and the corresponding microcontroller pack are installed.

2.  Ensure STM32CubeMX software and the corresponding microcontroller pack are installed.

3.  This project is generated with STM32CubeMX, and you can directly open the `.ioc` file.

4.  In STM32CubeMX's Project Manager, select the IDE type as MDK-ARM.

    ![image-20240809111542416](Image/image-20240809111542416.png)

5.  Click to generate the project, which will automatically create a Keil project.

6.  Connect a debug emulator (such as J-Link or ST-Link) to your development board.

7.  Click the "Build" button to compile the project, and modify the code according to the prompts if there are compilation errors.

8.  After successful compilation, use the debugger to flash the program to the microcontroller.

### 2.2.3 Compile with Keil MDK-ARM(Standard Library):

If you have been using the STM32 Standard Peripheral Library and are not familiar with the HAL library, you might consider learning about the HAL library by referring to section 2.2.2 on how to develop with STM32CubeMX and the HAL library.

If you prefer to continue using the STM32 Standard Peripheral Library, you may not be able to use our project directly, as it is developed based on the HAL library. However, you can read section 2.2.4 to understand how to migrate our HAL library project to the STM32 Standard Peripheral Library.

Moving the project from the HAL library to the standard library requires the following key considerations:

1.  Hardware abstraction differences: the HAL library provides a higher level of hardware abstraction than the standard library. You need to understand the differences in hardware operations between the two libraries.
2.  API changes: the API of the HAL library differs from the API of the standard library, and the HAL API calls need to be replaced with the API of the standard library.
3.  Initialization Code: the HAL library is typically initialized using HAL () , whereas the standard library uses a different initialization method.
4.  Peripheral configuration: peripheral configuration code needs to be rewritten according to the requirements of the standard library, including clock settings, GPIO configuration, interrupt configuration, RCC
5.  Interrupt handling: the use of interrupts in the project, the need to adjust to the standard library interrupt handling.

# 3. documentation

In the first-level Directory of this repository project, you will find the“Doc” folder with the hardware schematics and Development Board instructions. These resources will be useful to you

# 4. project folders explain

|    file     |                           explain                            |
| :---------: | :----------------------------------------------------------: |
| Application | User code folders, including Board Support packages (packaged as devices) , custom macros, and task scheduling related |
|    OLED     |                  OLED screen driver library                  |
| SEGGER_RTT  | The RTT tool, used by J-Link RTT Viewer software, is used to print logs for easy debugging |
|     doc     |       The use of documentation and hardware schematics       |
|  Makefile   |                   Compile using a makefile                   |

Application folders explain

![image-20240802143911267](Image/image-20240802143911267.png)

|     file      |                           explain                            |
| :-----------: | :----------------------------------------------------------: |
|      BSP      |               MCU hardware peripherals support               |
| Communication | Motor Protocol Layer Code, according to motor specifications prepared |
|    Devices    | It encapsulates modules such as peripherals into devices, object-oriented programming and decoupling |
|     Libs      | Custom Library files and log definitions, such as some commonly used macro definitions |
|     Task      |       Task Scheduling Code and Custom Task Code files        |

# 5. debug problem record

## RS485 communication direction pin control

BL3085N is a half-duplex RS-485 transceiver. A GPIO pin is drawn from the MCU to manually control the direction of communication of the transceiver. When the GPIO outputs a high level, RS-485 is the direction of transmission, that is, the MCU sends data to the bus, when GPIO output is low, RS -485 is the receiving direction, that is, MCU receives the data on the bus

In the code, I write this:

```
// set the rs485 communication direction to send
HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
// send the data to the motor
HAL_UART_Transmit(huart, buff, (uint16_t)AGT_UART_BUFF_LEN,0xffff);
// set the rs485 communication direction to receive
HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
```

Note that I did not add a HAL delay because the motor replies to data very quickly. If you add a delay after the switch level (for example, 20ms) , this will cause the 485 transceiver can not be switched to receive mode in time, MCU lost received motor feedback byte data

## Modify the serial baud rate

​	When using UART communication to control M0603C and M0603A motors, because the baud rate of the two motors is not the same, we need to modify the baud rate of the corresponding serial port peripherals and reinitialize it after the user selects the motor model, this allows a serial port peripheral to support motors with different baud rates

## Motor CAN terminal resistance

​	When I support the motor model M1502E in the debugging process, the 120 terminal ohm resistor on the CAN bus of the motor is disconnected by default, and the terminal resistor of the control board is also disconnected, so the communication fails, the motor is out of control

​	In order to ensure the normal communication in the back, I turned on the CAN bus 120 ohms resistor on the control board (using the dial switch to select, please see the control board schematic for details) , and in the motor initialization time sends the instruction to open the motor terminal resistance, thus can stabilize the communication, and the successful control motor
