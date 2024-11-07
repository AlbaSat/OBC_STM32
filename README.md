# Alba CubeSat On-Board Computer Emulation Project

<img src="./assets/logo.png" alt="Project logo" width="300" height="150" >

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Stack](#software-stack)
- [Simulation Details](#simulation-details)
- [Setup Instructions](#setup-instructions)
- [Usage Guide](#usage-guide)
- [Coding](#coding)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Overview

Welcome to the CubeSat On-Board Computer (OBC) Simulation project! This initiative aims to emulate the functionalities of the GomSpace NanoMind-based OBC using the STM32 Nucleo F411RE development board. By substituting real peripherals with simulated counterparts like ultrasonic sensors, we can effectively replicate and test the payload interfaces and operations of an actual CubeSat mission. This simulation environment is crucial for validating software and system integration before deploying to the final hardware.

## Features

- **STM32 Nucleo F411RE Integration:** Leverage the powerful STM32 microcontroller to mimic the GomSpace NanoMind architecture.
- **Peripherals Simulation:** Utilize "fake" sensors (e.g., ultrasonic sensors) to emulate CubeSat payloads.
- **Modular Design:** Easily add or modify simulated peripherals to match mission requirements.
- **Real-Time Data Processing:** Handle data streams as they would occur in a real CubeSat environment.
- **Comprehensive Logging:** Track system performance and data for analysis and debugging.
- **Open-Source Framework:** Collaborate and contribute to enhance the simulation capabilities.

## Hardware Components

- **STM32 Nucleo F411RE:** The primary microcontroller board used for simulation.
- **USB Interface:** For programming and serial communication.
- **Simulated Peripherals:**
  - **Ultrasonic Sensors:** Mimic distance and obstacle detection payloads.
  - **Temperature & Pressure Sensors:** Simulate environmental monitoring instruments.
  - **Communication Modules:** Emulate telemetry and command interfaces.
- **Power Supply:** Ensures stable power delivery to the Nucleo board and peripherals.

## Software Stack

- **Firmware Development:**
  - **STM32CubeIDE:** Integrated development environment for writing and debugging firmware.
  - **STM32CubeMX:** Configuration tool for setting up microcontroller peripherals and middleware.
- **Operating System:**
  - **FreeRTOS:** For task management and real-time operations.
- **FAT FileSystem:**
  - **FATFS:**  For efficient and lightweight memory managment.
- **Communication Protocol:**
  - **libcsp:**  Cubesat Space Protocol library for reliable and efficient communication between subsystems and ground stations.
- **Simulation Tools:**
  - **Mock Libraries:** Create fake peripheral drivers to simulate sensor data.
- **Version Control:**
  - **Git:** Manage codebase and collaborate effectively.
- **Documentation:**
  - **Markdown Files:** Maintain clear and structured project documentation.

## Simulation Details

The simulation environment replicates the key functionalities of the CubeSat's OBC by:

1. **Emulating Payload Interfaces:** Replace actual sensors with simulated ones to generate synthetic data streams.
2. **Interfacing with Simulated Peripherals:** Ensure that the OBC can communicate with and process data from these fake sensors as it would with real hardware.
3. **Testing Software Modules:** Validate data acquisition, processing algorithms, and communication protocols within a controlled environment.
4. **Iterative Development:** Allow for continuous testing and refinement of the OBC software before hardware deployment.

## Setup Instructions

### Prerequisites

- **Hardware:**
  - STM32 Nucleo F411RE board
  - USB cable for programming and communication
  - (Optional) Additional components for simulated peripherals

- **Software:**
  - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) installed on your computer
  - Git installed for version control

### Installation Steps

1. **Clone the Repository:**
   ```bash
   # Clone the repository with the recurse-submodules flag
   git clone https://github.com/AlbaSat/OBC_STM32.git -b CUBE_IDE_auto_code --recurse-submodules

   # Rename the folder OBC_STM32 to OBC_new for CUBEIDE consistency
   mv OBC_STM32 OBC_new

   # Navigate into the renamed folder
   cd OBC_new
   ```

2. **Open the Project in STM32CubeIDE:**
   - Launch STM32CubeIDE.
   - Go to `File > Import > Existing Projects into Workspace`.
   - Navigate to the cloned repository folder and select the project.

3. **Configure the Project:**
   - Ensure that all simulated peripherals are correctly set up.

4. **Build the Firmware:**
   - Click on the build icon or go to `Project > Build Project` to compile the firmware.

5. **Flash the Firmware to the Nucleo Board:**
   - Connect the STM32 Nucleo F411RE to your computer via USB.
   - Click on the debug or run icon to upload the firmware.

## Coding

Remember to use the .ioc auto code generator whenever you need to add a new feature.
Modify the following sections only.

| **Section**                              | **Purpose**                                                              | **Common Use Cases**                                             |
|------------------------------------------|--------------------------------------------------------------------------|------------------------------------------------------------------|
| `/* USER CODE BEGIN 0 */`                | Global variables, includes, and macros                                   | Declare global variables, include custom headers                 |
| `/* USER CODE BEGIN 1 */`                | Function prototypes, data structures, and enums                          | Declare custom function prototypes, or define enums and structs   |
| `/* USER CODE BEGIN 2 */`                | Custom initialization code                                               | Initialize custom peripherals, setup hardware-specific functions |
| `/* USER CODE BEGIN 3 */`                | Main application code                                                    | Insert your application logic (main loop)                        |
| `/* USER CODE BEGIN 4 */`                | Custom function definitions                                              | Define custom functions declared in `BEGIN 1`                    |
| `/* USER CODE BEGIN Callback 0 */`       | Custom code inside callback functions                                    | Handle events triggered by interrupts (e.g., timers, GPIO)       |
| `/* USER CODE BEGIN Error_Handler_Debug */` | Custom error handling logic                                              | Handle system errors, blink LEDs, log errors                     |

## Usage Guide

### Running the Simulation

1. **Start the Firmware:**
   - Ensure the Nucleo board is connected and the firmware is flashed.
   - The simulation should begin automatically upon reset.

2. **Monitor Serial Output:**
   - Open a serial terminal (**serial_COM.py**) to view real-time data from the simulated peripherals.
   - Default settings: 115200 baud rate, 8 data bits, no parity, 1 stop bit.

3. **Interacting with Simulated Peripherals:**
   - Use predefined commands or scripts to trigger specific sensor behaviors.
   - Observe how the OBC processes and responds to the simulated data.

4. **Logging and Analysis:**
   - Utilize built-in logging features to record data streams for further analysis.
   - Review logs to identify potential issues or validate system performance.

### Extending the Simulation

- **Adding New Simulated Sensors:**
  - Create mock drivers following existing peripheral examples.
  - Integrate them into the firmware and ensure proper communication with the OBC.

- **Modifying Existing Simulations:**
  - Adjust the behavior of simulated peripherals by tweaking the mock data generation logic.
  - Test different scenarios to assess OBC responsiveness.

## Contributing

We welcome contributions from the community! To contribute:

1. **Fork the Repository:**
   - Click on the "Fork" button at the top-right corner of this page.

2. **Create a New Branch:**
   ```bash
   git checkout -b feature/YourFeatureName
   ```

3. **Make Your Changes:**
   - Implement your feature or bug fix.

4. **Commit Your Changes:**
   ```bash
   git commit -m "Description of your changes"
   ```

5. **Push to Your Fork:**
   ```bash
   git push origin feature/YourFeatureName
   ```

6. **Open a Pull Request:**
   - Navigate to the original repository and open a pull request detailing your changes.

Please ensure your contributions adhere to the project's coding standards and include appropriate documentation.

## License

This project is licensed under the [MIT License](LICENSE).

## Contact

For questions, suggestions, or support, please reach out to:

- **Name:** Alessandro Balzan, Marco Tomasini, Gianluca Pianta ...
- **Email:** balzanalessandro2001@gmail.com, ...
- **Organization:** Alba CubeSat
- **GitHub:** https://github.com/alebal123bal(https://github.com/yourusername)

---

*Happy Coding! 🚀*

