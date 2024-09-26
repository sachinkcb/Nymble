Firmware Task - UART-Based Data Transmission and Speed Measurement
Problem Statement
The task is to implement a firmware solution that facilitates the transmission of a specified text between a PC and an MCU over UART, while also measuring and displaying the real-time data transmission speed in bits per second.

Text to be Transmitted
The text to be transmitted consists of approximately 1000 characters (including spaces), which can be represented as 1000 bytes.

Text:

“Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one.

In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs," Rajan said in the note." Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently."

Technical Requirements
UART Communication:

The transmission of the text data will be performed over UART between the PC and the MCU.
A baud rate of 2400 bps is used for the UART communication.
Incremental Data Reception and Storage:

As the data is received by the MCU, it is to be stored incrementally into memory (in this case, NVS due to the lack of native EEPROM on the ESP32).
The data should be written to memory progressively, not all at once.
Data Transmission from MCU to PC:

Once the entire dataset has been received and stored on the MCU, the stored data should be retrieved and sent back to the PC via UART.
Real-time Speed Measurement:

The real-time data transmission speed, in bits per second, must be displayed both when sending data from the PC to the MCU and when sending data from the MCU back to the PC.
The speed measurement should reflect the actual number of bits transmitted per second, independent of the UART baud rate.
Key Objectives
UART-based Communication: The system must reliably transmit and receive the given text data between the PC and MCU.
Incremental Data Handling: Data must be written to memory in real-time as it is being received from the PC.
NVS as EEPROM Replacement: Since the ESP32 lacks an EEPROM, the Non-Volatile Storage (NVS) system is used to handle data storage.
Transmission Speed Monitoring: Accurately measure the transmission speed during both PC-to-MCU and MCU-to-PC communication phases and display it on the console.
System Workflow
Data Transmission from PC to MCU:

The PC sends the specified text data (1000 bytes) to the MCU over UART.
The MCU receives the data incrementally, storing it in NVS as it is received.
Data Storage on the MCU:

As each chunk of data is received, it is stored in NVS. The process continues until the entire dataset is received.
Data Retrieval and Echo Back to PC:

Once the full dataset is received, the MCU retrieves the stored data from NVS.
The retrieved data is then transmitted back to the PC over UART.
Real-Time Speed Display:

During both the transmission from the PC to MCU and MCU to PC, the system measures and displays the actual transmission speed in bits per second.
Conclusion
This firmware task demonstrates efficient data transmission and reception over UART between a PC and an MCU, coupled with real-time speed measurement. By utilizing NVS for persistent storage on the ESP32, the system ensures reliability and data integrity, even across power cycles, making it a practical and robust solution for real-world applications.

# Nymble
