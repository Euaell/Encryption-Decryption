 
#include "NXP/iolpc2148.h"  //for register definitions

/*-------------------------------------------------------------------------------
Function:    uart_init  
Description: Initializes the UART module
Arguments:   None
Returns:     Nothing
Notes:       Port config: 1 stop bit, No parity, packet size=8-bit, baud rate = 19200bps
----------------------------------------------------------------------------------*/
void uart_init(void)
{
  PINSEL0_bit.P0_0 = 1;     //select pin function as uart (chapter6-pin connect block) -- TXD
  PINSEL0_bit.P0_1 = 1;     //select pin function as uart (chapter6-pin connect block) -- RXD
  
  // definition for the secondary UART
  PINSEL0_bit.P0_8 = 1;
  PINSEL0_bit.P0_9 = 1;
  
  U0LCR_bit.DLAB = 1;       //Divisor latch enabled (reqired only during configuration)
  U1LCR_bit.DLAB = 1;   // definition for the secondary UART
  
  U0DLM = 0;                
  U0DLL = 156;              //Baud rate dividing ratio = 156 (for 19200bps - see manual on how to calculate)
  
  // definition for the secondary UART
  U1DLM = 0;                
  U1DLL = 156;
  
  U0FDR_bit.MULVAL = 1;     //Fractional baud rate multiplier = 1  (optional)
  U0FDR_bit.DIVADDVAL=0;    //Fractional baud rae divider = 0+1    (optional)
  
  // definition for the secondary UART
  U1FDR_bit.MULVAL = 1;
  U1FDR_bit.DIVADDVAL=0;
  
  U0FCR_bit.FCRFE = 1;      //RX and TX FIFO enabled
  U0LCR |= 0x03;            //8bits data, no parity, 1 stop bit
  U0TER_bit.TxEn = 1;       //Enable Transmitter
  
  // definition for the secondary UART
  U1FCR_bit.FCRFE = 1;      
  U1LCR |= 0x03;            
  U1TER_bit.TxEn = 1;
  
  U0LCR_bit.DLAB = 0;       //Divisor latch disabled (no more needed)
  
  // definition for the secondary UART
  U1LCR_bit.DLAB = 0;
}

/*----------------------------------------------------------
Function:   Transmits a single byte on the UART serial line
Returns:    Nothing
Arguments:  Character to be transmitted
Notes:      waits until the transmit holding register (THR) becomes empty
            (interrupts and timeout can be used for more efficient implementation)  
------------------------------------------------------------*/
void send_char(char data)
{
  while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
  U0THR = data;                  //Shift out the data serially on the TX line
  U0THR = '\r';
}

/*----------------------------------------------------------
Function:   Transmits a string of bytes on the UART serial line
Returns:    Nothing
Arguments:  a pointer to the string
Notes:      waits until the transmit holding register (THR) becomes empty 
            (interrupts and timeout can be used for more efficient implementation)    
------------------------------------------------------------*/
void send_string(char *str)
{
 
  while(*str != 0)
  {
    while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
    U0THR = *str++;              //Shift out the data serially on the TX line
  }
  U0THR = '\r';
}

void send_decodeString(char *str)
{
 
  while(*str != 0)
  {
    while(U1LSR_bit.TEMT ==0);      
    U1THR = *str++;              
  }
  U1THR = '\r';
}

/*----------------------------------------------------------
Function:   Receives a single byte from the UART serial line
Returns:    The character received
Arguments:  None
Notes:      waits until valid data is received from the line   
            (interrupts and timeout can be used for efficient implementation)    
------------------------------------------------------------*/
char get_char(void)
{
  char data;                     //variable to hold incoming byte 
  
  while(U0LSR_bit.DR == 0);     //wait for data to come in
  data = U0RBR;                 //copy byte from the receive register to a variable
  return data;                  //return data received to the calling program
}
char get_dchar(void)
{
  char data;                     
  
  while(U1LSR_bit.DR == 0);     
  data = U1RBR; 
  U1THR = U1RBR;
  U1THR = '\r';
  return data;                  
}
/*----------------------------------------------------------
Function:   Receives a string of bytes on the UART serial line and fills in the given buffer
Returns:    Nothing
Arguments: a pointer to the string buffer to be filled and the length of the string in bytes
Notes:      waits until data is received
            (interrupts and timeout can be used for efficient implementation)    
------------------------------------------------------------*/
void get_string(char *buffer)
{
  int count=0;
  while(1){;
    while(U0LSR_bit.DR == 0);    //wait for data to come in
    U0THR = U0RBR;
    
    *(buffer+count) = U0RBR;         //copy byte from the receive register to buffer
    count++;
    
    if (U0RBR == '\r') // breaks if enter is pressed(ascii = 10)
    {
      *(buffer+count) = '\0'; 
      break;
    }
  }
}
