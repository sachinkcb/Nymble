
# UART-Based Data Transmission and Persistent Storage

## Project Overview

This project implements a robust system for data communication between a PC and an ESP32 microcontroller using **UART** (Universal Asynchronous Receiver-Transmitter). It focuses on the reliable reception, incremental storage, and subsequent transmission of data using the **Non-Volatile Storage (NVS)** subsystem on the ESP32. The system is designed to handle data streams of up to 1000 bytes, store the received data incrementally in NVS, and once the complete dataset has been received, retrieve the data from NVS and send it back to the PC over UART. This setup simulates a practical use case where data integrity, efficient storage, and echo transmission are required for serial communication between two devices.

---

## System Features and Capabilities

- **UART-Based Data Communication**: The ESP32 UART is configured to operate at a baud rate of 2400 bps, facilitating serial communication between the microcontroller and a connected PC.
  
- **Incremental NVS Storage**: The incoming data stream is stored in **Non-Volatile Storage (NVS)** incrementally as it is received over UART. This ensures that data is stored efficiently in flash memory in real time.

- **Threshold-Based Data Retrieval**: Once the complete dataset (1000 bytes) has been received, the entire data stored in NVS is retrieved and sent back over UART to the PC for verification or further processing.

- **Real-Time Data Echo**: The received data is echoed back to the PC immediately upon reception, ensuring communication reliability and providing feedback to the sender.

- **Resilience Against Data Loss**: By leveraging NVS for incremental storage, the system ensures that no data is lost in the event of unexpected interruptions, providing a stable mechanism for data retrieval and persistence.

---

## Technical Specifications

### Hardware Requirements
- **ESP32 Development Board**: The ESP32 microcontroller serves as the central processing unit for handling UART communication and NVS storage.
- **PC**: A computer with a serial terminal (or Python script) for sending data over UART and receiving responses.
- **USB-to-Serial Converter**: If not directly using the ESP32’s USB port, a USB-to-Serial adapter may be required for communication.

### Software Requirements
- **ESP-IDF**: Espressif’s IoT Development Framework (ESP-IDF) is required to develop, build, and flash the firmware onto the ESP32. This project has been developed and tested with ESP-IDF v5.0 or later.
  
- **Python (Optional)**: For testing and sending data via UART from the PC, Python can be used along with the `pyserial` library to automate the process.

---

## System Workflow

1. **UART Initialization**: The system configures UART communication between the ESP32 and the PC, with parameters such as baud rate, data bits, stop bits, and parity set to standard values. UART is initialized for both transmission and reception.
  
2. **Incremental Data Reception**: The ESP32 continuously listens for incoming data on its UART interface. As data packets arrive, they are stored in the buffer and subsequently written to NVS in an incremental manner. This ensures real-time data persistence.

3. **NVS Storage**: The received data is divided into smaller chunks, and each chunk is stored incrementally into the ESP32's NVS partition, ensuring that data is preserved even if the system is interrupted.

4. **Echo Feedback**: Each chunk of data received is echoed back immediately to the PC, providing real-time confirmation of reception.

5. **Full Data Retrieval**: Upon reception of the full dataset (1000 bytes), the ESP32 retrieves the stored data from NVS, reassembles it, and transmits it back to the PC over UART.

6. **Repetition**: The system continuously listens for new incoming data, storing and echoing back as per the described process.

---

## Project Components

### UART Configuration
The UART configuration for this project uses the following parameters:
- **Baud Rate**: 2400 bps
- **Data Bits**: 8 bits
- **Stop Bits**: 1 bit
- **Parity**: No parity
- **Flow Control**: Disabled

### Why NVS Instead of EEPROM?
The assignment originally specified the use of **EEPROM** for storing the received data. However, the ESP32 microcontroller does not include dedicated EEPROM memory. Instead, the ESP32 provides a more modern and flexible solution called **Non-Volatile Storage (NVS)**.
The Non-Volatile Storage (NVS) is a robust storage mechanism provided by the ESP32, allowing data persistence across resets. The project stores each received data chunk in flash memory using the NVS API, ensuring safe and reliable storage.
In this project, NVS is used as an alternative to EEPROM for storing data incrementally as it is received from UART.

---

## Implementation Details

### UART Initialization
The UART is initialized with the specified baud rate, TX/RX pins, and buffer sizes. This allows the ESP32 to establish a serial connection with the PC, enabling bidirectional communication.

### Incremental Storage in NVS
Data is stored in chunks as it is received. Instead of waiting for the entire dataset to be available, the system saves each chunk into NVS as it arrives, thereby improving reliability and minimizing memory usage during runtime.

### Threshold-Based Retrieval
Once 1000 bytes have been received, the ESP32 retrieves the data stored in NVS and sends it back to the PC. The 1000-byte threshold ensures that the system operates efficiently within buffer limits while providing robust data transfer capabilities.

---

## Usage Instructions

### ESP32 Setup

1. **Clone or Create the Project**: Ensure the provided `main.c` file is included in your ESP-IDF project directory.
2. **Build the Project**: Use ESP-IDF tools to compile and build the firmware:
    ```bash
    idf.py build
    ```
3. **Flash the Project**: Flash the firmware onto the ESP32:
    ```bash
    idf.py flash
    ```
4. **Monitor the Output**: Use the ESP-IDF monitor tool to observe the UART communication and NVS data storage in action:
    ```bash
    idf.py monitor
    ```

### PC-Side Communication

To send data to the ESP32 and receive the echoed data back, you can use a serial terminal (e.g., PuTTY or Docklight) or automate the process using a Python script with the `pyserial` library.

#### Example Python Script:
```python
import serial

# Configure the serial port and baud rate
ser = serial.Serial('COM3', 2400)

# Data to send (ensure it is close to 1000 bytes)
data_to_send = "Your text data here, make sure it's long enough..."

# Send the data
ser.write(data_to_send.encode())

# Read the echoed data
received_data = ser.read(len(data_to_send))
print("Data received from ESP32:", received_data.decode())

# Close the serial connection
ser.close()
```

---

## Expected Output

- **Data Echo**: As each chunk of data is sent from the PC, the ESP32 will echo it back over UART.
- **Full Data Retrieval**: Once 1000 bytes have been received, the ESP32 will retrieve the entire dataset from NVS and send it back to the PC, ensuring data integrity and confirming proper storage.

Example terminal output (ESP-IDF Monitor):
```bash
Data stored in NVS and echoed back:
<your_text_data>
```

## Conclusion

This project provides a practical solution for storing data incrementally in NVS on the ESP32 while facilitating reliable UART communication with a PC. The combination of real-time data echoing and full data retrieval from NVS makes this system suitable for applications that require robust, persistent data handling and verification over serial communication.
