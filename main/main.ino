//Import our libraries
#include "src/dht_library/DHT.h"
#include "src/gsm_library/GSM.h"

GSM gsm(2,3);
DHT dht(4,5);

void setup()
{
  Serial.begin(9600);
  if(gsm.start())
  {
    Serial.println("Startup Sucsessfull");
  }
}

void loop()
{
  if(gsm.postr())
  {
    Serial.println("Post Sucsessfull");
  }
  delay(600000);
}
