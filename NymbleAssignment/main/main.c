/**
 * 
 * 
\file main.c
\UART and NVS(instead of EEPROM) Echo for ESP32
This program receives data over UART, stores it incrementally in NVS(Non-Volatile Storage), and
echoes the received data back to the PC. When 1000 bytes are received,
the MCU retrieves the data from NVS and sends it back to the PC.
*
*/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"

/**
 * \brief UART configuration parameters.
 */
#define UART_NUM UART_NUM_0 // Using UART0
#define TXD_PIN (GPIO_NUM_1)
#define RXD_PIN (GPIO_NUM_3)
#define BAUD_RATE 2400

/**
 * \brief Buffer sizes.
 */
#define RX_BUFFER_SIZE 1024
#define BUFFER 1000
static uint8_t data[BUFFER];
const int uart_buffer_size = RX_BUFFER_SIZE * 2;

/**
 * \brief NVS partition name and key for data storage.
 */
#define NVS_PARTITION_NAME "storage"
#define NVS_KEY "uart_data"

QueueHandle_t uart_queue;
nvs_handle_t my_nvs_handle;

/**
 * \brief Initialize the UART interface.
 * 
 * This function configures the UART driver, including baud rate, data bits, parity,
 * stop bits, and flow control. It also installs the UART driver and sets the 
 * UART TX and RX pins.
 */
static void init_uart(void)
{
    const uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Install UART driver, and get the queue.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

/**
 * \brief Initialize the NVS partition.
 * 
 * This function initializes the NVS (Non-Volatile Storage) flash and opens 
 * the NVS partition for reading and writing data. It checks for any 
 * NVS errors and erases the storage if required.
 */
static void init_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    err = nvs_open(NVS_PARTITION_NAME, NVS_READWRITE, &my_nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE("NVS", "Error opening NVS: %s", esp_err_to_name(err));
    }
}

/**
 * \brief Store data in NVS.
 * 
 * This function stores the received UART data in NVS under the specified key.
 * It also commits the data to NVS storage to ensure persistence.
 * 
 * \param data The data buffer to be stored in NVS.
 * \param length The length of the data to be stored.
 * \return esp_err_t The status of the NVS operation (ESP_OK if successful).
 */
static esp_err_t store_data_in_nvs(uint8_t *data, size_t length)
{
    esp_err_t err = nvs_set_blob(my_nvs_handle, NVS_KEY, data, length);
    if (err == ESP_OK)
    {
        err = nvs_commit(my_nvs_handle);
        if (err == ESP_OK)
        {
            ESP_LOGI("NVS", "Data stored in NVS successfully.");
        }
        else
        {
            ESP_LOGE("NVS", "Error committing data to NVS: %s", esp_err_to_name(err));
        }
    }
    else
    {
        ESP_LOGE("NVS", "Error writing to NVS: %s", esp_err_to_name(err));
    }
    return err;
}

/**
 * \brief Retrieve data from NVS.
 * 
 * This function retrieves the stored data from NVS under the specified key.
 * It loads the data into the provided buffer and returns the length of the data.
 * 
 * \param data The buffer to store the retrieved data.
 * \param length The length of the retrieved data.
 * \return esp_err_t The status of the NVS operation (ESP_OK if successful).
 */
static esp_err_t retrieve_data_from_nvs(uint8_t *data, size_t *length)
{
    esp_err_t err = nvs_get_blob(my_nvs_handle, NVS_KEY, data, length);
    if (err == ESP_OK)
    {
        ESP_LOGI("NVS", "Data retrieved from NVS successfully.");
    }
    else
    {
        ESP_LOGE("NVS", "Error reading from NVS: %s", esp_err_to_name(err));
    }
    return err;
}

/**
 * \brief UART echo task.
 * 
 * This FreeRTOS task receives data over UART, stores it incrementally in NVS, 
 * and echoes the received data back to the PC. Once 1000 bytes have been received, 
 * the entire stored data is retrieved from NVS and sent back to the PC.
 * 
 * \param pvParameters Task parameters (not used).
 */
static void echo_task(void *pvParameters)
{
    // Initialize UART and NVS
    init_uart();
    init_nvs();
    
    int total_received = 0; // Total number of bytes received

    while (1)
    {
        // Write data to UART
        int len = uart_read_bytes(UART_NUM, data, BUFFER - 1, 10);
        if (len > 0)
        {
            total_received += len;

            // Store received data in NVS (incrementally)
            store_data_in_nvs(data, len);

            // Echo the received data back over UART
            uart_write_bytes(UART_NUM, (const char *)data, len);

            // If we've received 1000 bytes or more, we retrieve and send it back from NVS
            if (total_received >= 1000)
            {
                // Retrieve data from NVS
                size_t data_len = RX_BUFFER_SIZE;
                retrieve_data_from_nvs(data, &data_len);

                // Send data back to PC
                uart_write_bytes(UART_NUM, (const char *)data, data_len);
                printf("Data stored in NVS and echoed back:\n%s\n", data);

                // Reset the total received bytes for the next cycle
                total_received = 0;
            }
        }
    }
}

/**
 * \brief Main application entry point.
 * 
 * This function creates the echo task that handles UART communication 
 * and data storage in NVS.
 */
void app_main(void)
{
    // Create the echo task
    xTaskCreate(echo_task, "echo_task", 4096, NULL, 10, NULL);
}
