
/*--------------------------------------------------------------
 File:      uart.h
 Purpose:   Header file for UART driver
 Compiler:  IAR EW 5.5
 System:    Philips LPC2148 MCU with ARM7TDMI-s core
----------------------------------------------------------------*/

//function prototypes

void uart_init(void);
void send_char(char data);
void send_string(char *str);
char get_char(void);
char get_dchar(void);
void get_string(char *buffer);
void send_decodeString(char *str);