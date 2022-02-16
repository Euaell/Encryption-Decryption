 
#include "uart.h"           //for UART functions
#include "system.h"         //for system functions

void main()
{
  // char data;
  char buffer[] = "";
  
  char key; // encoding key
  char key1; // decoding key
     
  clock_init();       //initialize system clock
  uart_init();         //initialize serial port (UART)
  
  send_string("Enter a string to encrypt: ");  //Send string on the UART line
  get_string(buffer); // saves the input string in the 'buffer' char array
  send_string(buffer);
  
  char buffersaved = *buffer;
  
  send_string("Enter a key: "); // promtes the key to be entered
  key = get_char(); // saves a single char key to the 'key' char variable
  send_char(key); // displays the input character
  
  /*The following code encryptes the string with the given key and displays the final result*/
  send_string("The encrypted message is ->");
  int icounter = 0;
  while(*(buffer + icounter) != '\r')
  {
    *(buffer + icounter) += key;
    icounter++;
  }
  send_string(buffer);
  send_string("Encryption succesful");
  
  icounter=0; // resets the counter to 0
  send_decodeString("Enter the key to decrypt with: "); // prompts the user for the decoding key
  key1 = get_dchar(); // saves the decoding key in variable 'key1' char
  
  while(*(buffer + icounter) != '\r') // decrypts the string with the given string
  {
    *(buffer + icounter) -= key1;
    icounter++;
  }
  send_decodeString(buffer); // display the decoded string
  
  // if wrong key used tells display approprate message
  if(buffersaved != *(buffer)){
    send_decodeString("Wrong key");
  }
}
 
