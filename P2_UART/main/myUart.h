#ifndef _MY_UART_H_
#define _MY_UART_H_
 
#include <inttypes.h>
#include <string.h>
#include "driver/uart.h"

// UART 0
#define PC_UART_PORT    (0)
#define PC_UART_RX_PIN  (3)
#define PC_UART_TX_PIN  (1)
// UART 1
#define UART1_PORT      (1)
#define UART1_RX_PIN    (18)
#define UART1_TX_PIN    (19)
// UART 2
#define UART2_PORT      (2)
#define UART2_RX_PIN    (16)
#define UART2_TX_PIN    (17)

#define LED (GPIO_NUM_25)

#define UARTS_BAUD_RATE         (115200)
#define TASK_STACK_SIZE         (1048)
#define READ_BUF_SIZE           (1024)

#define YELLOW  11 // Fixme 
#define GREEN   10 // Fixme 
#define BLUE    12 // Fixme 
#define BLACK   0 // Fixme 

void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin);
void delayMs(uint16_t ms);
// Send
void uartPuts(uart_port_t uart_num, char *str);
void uartPutchar(uart_port_t uart_num, char data);

// Receive
bool uartKbhit(uart_port_t uart_num);
char uartGetchar(uart_port_t uart_num );
void uartGets(uart_port_t uart_num, char *str);

// Escape sequences
void uartClrScr( uart_port_t uart_num );
void uartSetColor(uart_port_t uart_num, uint8_t color);
void set_xy(char* str, uint8_t num, uint8_t j);
void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y);

// Utils
void orderArray(char *str, uint8_t i);
void myItoa(uint32_t number, char* str, uint8_t base) ;
uint8_t myAtoi(char *str);

#endif