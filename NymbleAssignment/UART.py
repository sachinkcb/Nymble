import serial
import time

# Configure the serial port
serial_port = 'COM3'  # Replace with your ESP32's serial port
baud_rate = 2400  # Match the baud rate in your ESP32 code
ser = serial.Serial(serial_port, baud_rate)

# Data to send
# Data to send
data_to_send = """Finance Minister Arun Jaitley Tuesday hit out at former RBI governor 
Raghuram Rajan for predicting that the next banking crisis would be triggered by 
MSME lending, saying postmortem is easier than taking action when it was required. 
Rajan, who had as the chief economist at IMF warned of impending financial crisis of 
2008, in a note to a parliamentary committee warned against ambitious credit targets 
and loan waivers, saying that they could be the sources of the next banking crisis. 
Government should focus on sources of the next crisis, not just the last one. 
In particular, government should refrain from setting ambitious credit targets or 
waiving loans. Credit targets are sometimes achieved by abandoning appropriate 
due diligence, creating the environment for future NPAs," Rajan said in the note." 
Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined 
more closely for potential credit risk. Rajan, who was RBI governor for three years 
till September 2016, is currently."""

try:
    # Send the data and check if it was written successfully
    bytes_written = ser.write(data_to_send.encode())
    if bytes_written == len(data_to_send):
        print(f"Successfully wrote {bytes_written} bytes.")
    else:
        print(f"Error: Failed to write all bytes. Wrote {bytes_written} out of {len(data_to_send)} bytes.")
    # Variables for speed calculation (using the third method)
    previous_time = time.time()
    bytes_received = 0
    rate_samples = []
    while True:  # Loop to continuously receive data
        try:
            # Send the data and check if it was written successfully
            bytes_written = ser.write(data_to_send.encode())
            received_data = ser.readline().decode()  # Read a line of data
            current_time = time.time()
            bytes_received += len(received_data)
            if received_data:
                rate = bytes_received / (current_time - previous_time)
                rate_samples.append(rate)
                print("response received from MCU :", received_data)
                print(f"Instantaneous Rate: {rate:.2f} bytes/s")

                # Keep a fixed number of samples (e.g., 10)
                if len(rate_samples) > 10:
                    rate_samples.pop(0)  # Remove the oldest sample

                # Calculate and print average rate
                average_rate = sum(rate_samples) / len(rate_samples)
                print(f"Average Rate: {average_rate:.2f} bytes/s")

                previous_time = current_time
                bytes_received = 0  # Reset for the next measurement
            else:
                break  # Exit loop if no data is received
        except UnicodeDecodeError:
            print("Decoding error. Raw data:", ser.readline())

except serial.SerialException as e:
    print("Serial port error:", e)
except Exception as e:
    print("An error occurred:", e)
finally:
    # Close the serial port
    ser.close()