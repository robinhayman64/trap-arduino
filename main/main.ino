//Import our libraries
#include "src/dht_library/DHT.h"
#include "src/gsm_library/GSM.h"

GSM gsm(2,3 );
DHT dht(4,22);

void setup()
{
  // Starts the gsm modem for debugging
  Serial.begin(9600);

  // Starts the temperature sensor
  dht.begin();

  // Starts the gsm module
  if(gsm.start())
  {
    // Initialise the test post adress and message. This may be changed throughout the run of the program in the void loop function.
    gsm.setAdress("http://erbium.requestcatcher.com/test");
    gsm.setMesage("Aangeskakel");
    if(gsm.postr())
    {
      Serial.println("Startup Sucsessfull");
    }
  }
}

void loop()
{
  // Wait for the sensor
  delay(2000);

  // Read the temperature and assign it to the gsm modem
  float t = dht.readTemperature();
  gsm.setMesage(String("Die Temperatuur is: ")+String(t));

  // Use the gsm modem to post the temperature to the server
  if(gsm.postr())
  {
    Serial.println("Temperature Posted Sucsessfully");
  }
}
