#ifndef _P2_PACKAGEDATA_H_
#define _P2_PACKAGEDATA_H_

//struct to build a package
typedef struct UART_Package
{
    uint8_t header, command, length, end;
    uint8_t *data;
    uint32_t crc32;
} UART_Package;

//returns a struct which contains package data
UART_Package createPackage(uint8_t header, uint8_t command, uint8_t length, uint8_t *data, uint8_t end);
//function that sends a package to uart
void sendPackage(UART_Package pack);
//function that receives a package from uart
receivePackage(UART_Package pack);

#endif