#include <dht.h>                // The library for the temperature sensor

const char* deviceImei;         // International Mobile Equipment Identity
const boolean debug = false;    // Enable serial communication after startup
                                //
dht DHT;

void setup()
{
  // The required functions for serial connections to work and to change the state of the pins
  GSM.begin(9600);
  Serial.begin(9600);
  pinMode(7, OUTPUT);

  // Run the startup routine for the GSM modem. After this section is done the GSM modem will be ready for use.
  startup();
  Serial.println("Startup done");

  // Outputs statistics regarding the GSM modem. This may not really be nessesary information and may be deleted if space becomes a constraint.
  deviceImei = filterResult(request(4));
  Serial.println("IMEI Number: " + String(deviceImei));
  char* sub = select(request(6), 15);
  Serial.println("Signal Strength: " + String(sub));

  // Garbage Collection
  delete [] sub;
}

void loop()
{
  if (debug)
    // Read and Write from Serial after startup
    serialComminication()
  else
  {
    // This will be the main program loop. Temperature, Humidity and FSM data will be be gathered here.
    execute(postRequestSet);

	  Serial.println(request(17));

    delay(3000);
    gsmOff();
    delay(standby);
    gsmOn();
  }
}
