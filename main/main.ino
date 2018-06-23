//Import our libraries
#include "src/dht_library/DHT.h"
#include "src/gsm_library/GSM.h"

//GSM gsm(2, 3);
GSM *gsm;
DHT dht(4, 22);

void setup()
{
	// Starts the Serial port for debugging
	Serial.begin(9600);

  gsm = new GSM(2, 3);

	// Starts the temperature sensor
	dht.begin();

	// Starts the GSM modem
	if (gsm->start())
	{
		// Initialize the test post address and message. This may be changed throughout the run of the program in the void loop function.
		gsm->setAddress("http://erbium.requestcatcher.com/test");
		gsm->setMessage("Aangeskakel");
		if (gsm->postRequest())
		{
			Serial.println("Startup Successful");
		}
	}
}

void loop()
{
	// Wait for the sensor
	delay(2000);

	// Read the temperature and assign it to the GSM modem
	float t = dht.readTemperature();
	gsm->setMessage(String("Die Temperatuur is: ") + String(t));

	// Use the GSM modem to post the temperature to the server
	if (gsm->postRequest())
	{
		Serial.println("Temperature Posted Successfully");
	}
}
