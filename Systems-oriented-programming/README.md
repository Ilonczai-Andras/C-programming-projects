# Systems Programming Project

## Project Overview
Develop a C program on Linux that generates a 1-bit color depth ("black-and-white", two-color) BMP file representing a graph of a randomly varying quantity over time. The program should operate in two modes: 
- **Sender**: Generates the data
- **Receiver**: Renders the data

Data transfer between the two modes is achieved through inter-process communication using either a file or a socket. 

## Features
1. **Main Program**:  
   - Handles command-line arguments.
   - Supports `--version` and `--help` options.
   - Defaults to sender mode (`-send`) and file communication mode (`-file`), unless specified.

2. **Measurement Simulation**:
   - Generates a sequence of integer values to represent sensor measurements.
   - Implements a 3-state random walk function that produces data based on the current time.

3. **BMP File Creation**:
   - Converts measurement data into a 1-bit BMP file (`chart.bmp`).
   - BMP image shows data as a continuous curve with customizable two-color palette.

4. **Process Management**:
   - Examines `/proc` directory for `status` files and finds specific process IDs.

5. **File and Socket Communication**:
   - Implements data sending/receiving through a text file or UDP sockets.
   - Uses signals to synchronize sender and receiver processes.

6. **Signal Handling**:
   - Supports interrupt (SIGINT), user-defined (SIGUSR1), and alarm (SIGALRM) signals.

7. **Multithreading**:
   - Outputs version information using multiple threads to print each line independently.

## Getting Started

### Prerequisites
- **Compiler**: Use `gcc` with necessary flags (e.g., `-fopenmp` for threading support).
- **Operating System**: Linux-based environment.

### Compilation
Compile the program with:
```bash
gcc project.c -fopenmp helper.c -o chart
