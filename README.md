# CSC1104Assignment: Raspberry Pi and Data Visualization

## Overview
This project demonstrates various functionalities, including:
1. Data visualization with GNUplot.
2. GPIO interaction on Raspberry Pi.
3. Sorting algorithms implemented in C and Assembly.

It is designed as an educational tool to explore embedded systems, data visualization, and algorithmic design.

---

## Features

### Data Visualization (`displayPlot.c`)
- Reads LED waveform data generated by a Raspberry Pi.
- Displays graphical plots using GNUplot.
- **How to Run**:
  1. Navigate to the file location:
     ```bash
     cd <path_to_file>
     ```
  2. Transfer the required data file from the Raspberry Pi:
     ```bash
     scp -P 22 pi@<ip_address>:displayPlot.csv .
     ```
  3. Compile and execute the program:
     ```bash
     gcc -o displayPlot displayPlot.c
     ./displayPlot
     ```
  4. Ensure GNUplot is installed. [Installation Guide](https://riptutorial.com/gnuplot/example/11275/installation-or-setup).

### GPIO Interaction (`newStudent.c`)
- Interacts with Raspberry Pi GPIO pins to control LEDs.
- Requires the `wiringPi` library.
- **Setup**:
  - Install `wiringPi`. [Installation Guide](https://learn.sparkfun.com/tutorials/raspberry-gpio/c-wiringpi-setup).
- **Pin Configuration**:
  - GPIO27: Red LED
  - GPIO13: Red LED
  - Ground
- **How to Run**:
  1. Navigate to the file location:
     ```bash
     cd <path_to_file>
     ```
  2. Compile and execute:
     ```bash
     gcc -o newStudent newStudent.c -lwiringPi
     ./newStudent
     ```

### Bubble Sort (`q2.c` and `q2.s`)
- Implements the Bubble Sort algorithm in C and ARM Assembly.
- **C Implementation**:
  - Basic bubble sort algorithm for educational purposes.
  - Demonstrates sorting in ascending order.
- **Assembly Implementation**:
  - ARM/Aarch64 assembly for Raspberry Pi.
  - Leverages ARM registers for array manipulation and comparison.
- **How to Run**:
  1. Compile and run the C implementation:
     ```bash
     gcc -o bubblesort q2.c
     ./bubblesort
     ```
  2. For the Assembly implementation, use an ARM-compatible assembler.

---

## Prerequisites
1. **GNUplot** for data visualization.
2. **wiringPi** for GPIO interactions.
3. **C Compiler** (e.g., GCC).
4. ARM-compatible assembler for `q2.s`.

---

## Code Structure

### Files
- **`displayPlot.c`**: Handles data visualization using GNUplot.
- **`newStudent.c`**: Manages GPIO-based interactions with LEDs on Raspberry Pi.
- **`q2.c`**: C implementation of Bubble Sort.
- **`q2.s`**: ARM Assembly implementation of Bubble Sort.
- **`example_waveform.png`**: Sample output for waveform visualization.

### Key Components
1. **Visualization**:
   - Data reading and plotting using GNUplot.
2. **Embedded Systems**:
   - GPIO control via `wiringPi`.
3. **Sorting Algorithms**:
   - Bubble sort implemented in both C and Assembly for educational comparison.

---

## Usage
1. Follow the setup and running instructions for each file in the **Features** section.
2. Explore the provided files to understand the algorithms and system interactions.

---

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a feature branch.
3. Submit a pull request with your changes.

---

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Acknowledgments
This project is part of the CSC1104 coursework, designed to explore data visualization, embedded systems, and algorithmic design.

