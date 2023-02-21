#include "driver/uart.h"
#include "myUart.h"

void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = (size==8)?3:((size==7)?2:1),
        .parity    = (parity==0)?UART_PARITY_DISABLE:((parity==1)?UART_PARITY_EVEN:UART_PARITY_ODD),
        .stop_bits = (stop==1)? 1:3,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(uart_num, READ_BUF_SIZE, READ_BUF_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, txPin, rxPin,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

}

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void uartPutchar(uart_port_t uart_num, char c)
{
    uart_write_bytes(uart_num, &c, sizeof(c));
}

bool uartKbhit(uart_port_t uart_num)
{
    uint8_t length;
    uart_get_buffered_data_len(uart_num, (size_t*)&length);
    return (length > 0);
}

char uartGetchar(uart_port_t uart_num)
{
    char c;
    // read byte, no wait
    uart_read_bytes(uart_num, &c, sizeof(c), 0);

    return c;
}

void uartPuts(uart_port_t uart_num, char *str){
    uint8_t i=0;
    char c=(*str);
    while(c!=0){
        uartPutchar(uart_num, c);
        i++;
        c=*(str+i);
    }
}

void uartGets(uart_port_t uart_num, char *str){ 
    uint8_t i=0;
    char c=0;
    do{
        c=uartGetchar(uart_num);
        if(c == 13) *(str+i)=0; //fin de la cadena
        else{
            if(c!='.' && c!='-'){
                if(c==8 && i!=0){ //12
                    *(str+i-1)=' ';
                    uartPutchar(uart_num,8);
                    uartPutchar(uart_num,' ');
                    uartPutchar(uart_num,8);
                    i--;
                }
                else{
                    if(c!=8){
                        *(str+i)=c;
                        uartPutchar(uart_num,c);
                        i++;
                    }
                }
                
            }
        }
    }while(c!=13);
}

void uartClrScr(uart_port_t uart_num)
{
    // Uso "const" para sugerir que el contenido del arreglo lo coloque en Flash y no en RAM
    const char caClearScr[] = "\e[2J";
    uart_write_bytes(uart_num, caClearScr, sizeof(caClearScr));
}

void uartSetColor(uart_port_t uart_num, uint8_t color){
    char caColor[] = "\033[38;5;";
     
    uart_write_bytes(uart_num, caColor, sizeof(caColor));
    if(color==GREEN){
        uartPutchar(0,'1');
        uartPutchar(0,'0');
        /*caColor[7] = '1';
        caColor[8] = '0';*/
    }if(color==YELLOW){
        uartPutchar(0,'1');
        uartPutchar(0,'1');
    }
    if(color==BLUE){
        uartPutchar(0,'1');
        uartPutchar(0,'2');
        /*caColor[7] = '1';
        caColor[8] = '2';*/
    }
    if(color==BLACK){
        uartPutchar(0,'0');
    }
    uartPutchar(0,'m');
    uartPutchar(0,'\0');
}

void uartGoto11(uart_port_t uart_num)
{
    // Limpie un poco el arreglo de caracteres, los siguientes tres son equivalentes:
     // "\e[1;1H" == "\x1B[1;1H" == {27,'[','1',';','1','H'}
    const char caGoto11[] = "\e[1;1H";
    uart_write_bytes(uart_num, caGoto11, sizeof(caGoto11));
}

void set_xy(char* str, uint8_t num, uint8_t j){
    //char caGotoxy[] = "\e[xx;yyH";

}
void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y)
{
    // Limpie un poco el arreglo de caracteres, los siguientes tres son equivalentes:
     // "\e[1;1H" == "\x1B[1;1H" == {27,'[','1',';','1','H'}
    char caGotoxy[] = "\e[";
    char x1[]="xx";
    char y1[]="yy";

    uart_write_bytes(uart_num, caGotoxy, sizeof(caGotoxy));

    if(x<10){
        x+=48;
        uartPutchar(uart_num, x);
    } 
    else{
        if(x<100){
            x1[1]=(x%10)+48; 
            x1[0]=(x/10)+48; 
            uartPuts(uart_num, x1);
        }
    }
    
    uartPutchar(uart_num, ';');

    if(y<10){
        y+=48;
        uartPutchar(uart_num, y);
    } 
    else{
        if(y<100){
            y1[1]=(y%10)+48; 
            y1[0]=(y/10)+48; 
            uartPuts(uart_num, y1);
        }
    }
    uartPutchar(uart_num, 'H');
    uartPutchar(uart_num, '\0');
}

void orderArray(char *str, uint8_t i){
    char c=0; //01100
    uint8_t j=0,z=i/2;
    while(j<z){
        c=*(str+i-1);
        *(str+i-1)=*(str+j);
        *(str+j)=c;
        i--; j++;
    }
}
void myItoa(uint32_t number, char *str, uint8_t base){
    uint8_t i=0;
    char c=0;
    uint32_t data=0;
    if(number>0){
        if(base==2){
            while(number!=0){
                if((number%2)==0) c='0';
                else c='1';
                *(str+i)=c;
                number/=2;
                i++;
            }
            if(number==0){
                *(str+i)='0'; i++;
            }
        }
        if(base==10){
            while(number!=0){
                data=number%10;
                data+=48;
                number/=10;
                *(str+i)=data;
                i++;
            }
            if(!number && i==1){
                *(str+i)='0'; i++;
            }
        }
        if(base==16){
            while(number!=0){
                data=number%16;
                if(data>9) data+=55; //A-F HEX
                else data+=48; //0 - 9 DEC
                number/=16;
                *(str+i)=data;
                i++;
            }
            if(!number && i==1){
                *(str+i)='0'; i++;
            }
        }
        orderArray(str,i);//ordena el arreglo -> orden correcto MSB to LSB
    }
    else{
        *(str+i)='0'; i++;
    }
    *(str+i)=0;
}

uint8_t myAtoi(char *str){
    uint8_t data=0;
    int i=0;
    char c=0; //toma el primer Caracter
    c=*(str+i);
    if(c=='0') return 0;

    while(c!=0){ //Mientras no sea un nullo, conviertelo
        if(c<48 || c>57) return data; //manda lo que llevabas
        c=c-48; //ajustac a valor entero
        if(i!=0){
            data*=10; //si es la posicion diferente a las unidades->*10
        }
        data=data + c; //suma el valor ajustado
        i++; 
        c=*(str+i);
    }
    return data; //alfanumerico->entero
}