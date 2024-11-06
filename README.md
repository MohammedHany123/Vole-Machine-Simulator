# Vole Machine Simulator

**Vole Machine Simulator** is originally an assignment for OOP class at Faculty of Computers and Artificial Intelligence at Cairo University under the supervision of Dr. Mohammad El-Ramly made by Ahmed Samir, Mohammed Hany, Loay Medhat. It is a simulation of a virtual machine designed to execute instructions written in a specific assembly-like language. This program provides a graphical interface that allows users to load, execute, and visualize instructions with memory and register operations.

## Table of Contents

- [Features](#features)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Usage](#usage)
- [Instruction Set](#instruction-set)
- [Code Structure](#code-structure)
- [Contributing](#contributing)
- [License](#license)

---

## Features

- **Load and Execute Instructions**: Load a list of instructions and execute them step-by-step.
- **Memory and Register Visualization**: Display memory and registers in real-time, with color-coded highlights.
- **Hexadecimal and ASCII Output**: View output in both hexadecimal and ASCII format for each executed instruction.
- **Control Unit (CU) and ALU Display**: Visualize how instructions interact with the CU and ALU screens.
- **Conditional and Unconditional Jump Instructions**: Test and implement jump instructions with real-time Program Counter (PC) display.

## Screenshots
![Screenshot (330)](https://github.com/user-attachments/assets/ce089686-6d4c-4777-93be-156e8b2da659)
![Screenshot (331)](https://github.com/user-attachments/assets/86dfe52c-d7bc-4a6d-8d48-b6ee964fc6ad)
![Screenshot (329)](https://github.com/user-attachments/assets/bb4b602a-821e-4c90-8640-e8ea9e70f544)

*A preview from inside the program*

## Installation

1. **Clone the repository**:
   ```
   git clone https://github.com/yourusername/vole-machine-simulator.git
   cd vole-machine-simulator
   ```
2. **Dependencies**:
   - **Qt Framework**: Install Qt 6.8 (needed for graphical interface).
   - **Compiler**: Ensure you have a C++ compiler, such as `g++` or `clang++`.
3. **Build the Project**:
   - Open the project in Qt Creator and built it.
4. **Run the Program**

## Usage

1. **Load Instructions**:
   - Enter the instructions in the input field, using the assembly-like format described below.
   - Instructions can include operations like loading, storing, moving, artithmetic, logical operations, and jumps.
2. **Execute Instructions**:
   - Step through each instruction or run the program until it halts.
   - Observe memory and register updates in real-time.
3. **View Output**:
   - See the output in both hexadecimal and ASCII format.
   - Watch the CU and ALU displays for current operation translations.

## Instruction Set

The following is the set of available instructions. Each instruction is a 4-character hex code representing an operation and operands.

| Opc. | Opr. | Effect                                                                                                                                                                      |
|------|------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1    | RXY  | Copy the content of RAM cell XY to register R.                                                                                                                              |
| 2    | RXY  | Copy the bit-string XY to register R.                                                                                                                                       |
| 3    | RXY  | Copy the content of register R to RAM cell XY.                                                                                                                              |
| 4    | xRS  | Copy the content of register R to register S.                                                                                                                               |
| 5    | RST  | Add the content of register S and register T, and put the result in register R. Data is interpreted as integers in two's-complement notation.                               |
| 6    | RST  | Add the content of register S and register T, and put the result in register R. Data is interpreted as floats in floating point notation.                                   |
| 7    | RST  | Bitwise OR (∨) the content of register S and T, and put the result in register R.                                                                                           |
| 8    | RST  | Bitwise AND (∧) the content of register S and T, and put the result in register R.                                                                                          |
| 9    | RST  | Bitwise XOR (⊕) the content of register S and T, and put the result in register R.                                                                                          |
| A    | RxX  | Rotate the content of register R cyclically right X steps.                                                                                                                  |
| B    | RXY  | Jump to instruction in RAM cell XY if the content of register R equals the content of register 0.                                                                           |
| C    | xxx  | Halt execution.                                                                                                                                                              |
| D    | RXY  | Jump to instruction in RAM cell XY if the content of register R is greater than (>) the content of register 0. Data is interpreted as integers in two's-complement notation. |


## Code Structure
- **main.cpp**: Entry point of the application
- **mainwindow.cpp and mainwindow.h**: Contains the main UI logic and functions for updating memory, registers, and other components.
- **mainwindow.ui**: The UI design for the application.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them with clear messages.
4. Push to your fork and submit a pull request.

Please ensure that your code follows the existing style and that all new features are documented.

## License

This project is licensed under the MIT License.
