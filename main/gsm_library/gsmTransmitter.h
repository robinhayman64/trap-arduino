/*
gsmTransmitter.h (formerly gsmTransmitter.ino) - 
Multi-instance GSM Transmitter library for Arduino

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

The latest version of this library can always be found at
https://github.com/ebriumgroep/trap-arduino
*/

#ifndef gsmTransmitter_h
#define gsmTransmitter_h

#include <SoftwareSerial.h>

class gsmTransmitter
{
private:
  // static data
  const String commands[];
  static int startupSet[];
  static int enterPin[];
  static int postRequestSet[];
  static int sendSmsSet[];
  static long int standby;
  
  // private data
  bool active;
  bool debug;
  
  // private methods
  int intArrLength(int arr[]);
  int charArrToInt(char arr[]);
  char* select(char arr[], int from);
  char* filterResult(char arr[]);
  void gsmOn();
  void gsmOff();
  
  int check(char ret[]);

public:
  // public methods
  gsmTransmitter(bool runStartup = false);
  ~gsmTransmitter();
  char* filterResult(char arr[]);
  char* request(int instruction);
  bool resolve(int errorCode);
  bool execute(int set[]);
  void startup();
  char* getDeviceImei() { return deviceImei; }

  const char* deviceImei;
  
  using Print::write;

};
#endif
