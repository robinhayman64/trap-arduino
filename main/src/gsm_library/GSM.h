/*
GSM.h (formerly gsmTransmitter.ino) -
Multi-instance GSM Transmitter library for Arduino

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

The latest version of this library can always be found at
https://github.com/ebriumgroep/trap-arduino
*/

#ifndef GSM_h
#define GSM_h

#include "GSM.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

class GSM
{
private:
  // Constant Data
  const String commands[18] = { "AT", "AT+CSQ", "AT+CPIN?", "AT+CPIN=4321", "AT+GSN", "AT+CREG?", "AT+CSQ", "AT+CMGF=1", "AT+CMGS=\"+27820486812\"", "THIS IS A TEST MESSAGE|", "|",
                                "AT+QIFGCNT=0", "AT+QICSGP=1,\"internet\"", "AT+QHTTPURL=37,50", "http://erbium.requestcatcher.com/test", "AT+QHTTPPOST=11,50,50", "Hello World", "AT+QHTTPREAD=50"};
  const int startupSet[5] = {0, 1, 2, 5, -1};           	// The commands to execute in sequence to start the gsm modem
  const int postRequestSet[7] = {11, 12, 13, 14, 15, 16, -1}; 	// The commands to execute on sequence to do a post request to an server

  // Internal Methods
  char* request(int);       // Execute an single command drom the commands array
  bool execute(int []);     // Execute a list of functions with the request method
  bool resolve(int);        // Resolves error codes
  int arrLength(int []);    // Derermine the length of an array
  int check(char []);       // Checks for error codes in the output
  void gsmOn();             // Turns the GSM Modem on
  void gsmOf();             // Turns the GSM Modem of

  // Internal Variables
  String adress, mesage;    // Done
  SoftwareSerial *MODEM;    // Done

public:
  // Setter Methods
  void setAdress(String);   // Done
  void setMesage(String);   // Done

  // Controll Methods
  bool start();
  bool postr();

  // Constructor and Destructor
  GSM(int, int);            // Done
  ~GSM();                   // Done
};

#endif
