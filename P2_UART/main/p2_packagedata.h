#ifndef _P2_PACKAGEDATA_H_
#define _P2_PACKAGEDATA_H_
#include <inttypes.h>
#define N_PACKAGES 4*2
#define PAYLOAD_LEN 4
#define BUFFER_SIZE 256
#define BYTES_PER_PACKAGE 8
#define MSG_TAM_STR 16
//struct to build a package
typedef struct UART_Package
{
    uint8_t header, command, length, end;
    uint8_t data[PAYLOAD_LEN];
    uint32_t crc32;
} UART_Package;

//returns a struct which contains package data
void createPackage(UART_Package *mi_pack, uint8_t header, uint8_t command, uint8_t length, uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t fin);
void PackageToString(UART_Package pack, char *msg_pack);
//function that sends a package to uart
void sendPackage(UART_Package pack);
//function that receives a package from uart
void receivePackage(UART_Package pack);
//void package_to_inputCRC32(uint8_t *data_arr,  UART_Package mi_pack);
void StringToPackage(UART_Package *pack, char *msg_pack);
#endif