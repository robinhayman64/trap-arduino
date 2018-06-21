#include <SoftwareSerial.h>
SoftwareSerial GSM(2, 3); // RX, TX

boolean debug = true;    // Enable serial communication after startup
char* deviceImei;         // International Mobile Equipment Identity (IMEI)
long int standby = 10800000;   // Time for GSM to turn off in milliseconds (10800000 = 3 hours)

String commands[] = {"AT", "AT+CSQ", "AT+CPIN?", "AT+CPIN=4321", "AT+GSN", "AT+CREG?", "AT+CSQ", "AT+CMGF=1", "AT+CMGS=\"+27820486812\"", "THIS IS A TEST MESSAGE|", "|"};

int startupSet[] = {0, 1, 2, 5, -1};
int enterPin[] = {3, -1};
int sendSmsSet[] = {7, 8, 9, -1};

// Returns the length of an int array that terminates with -1
int intArrLength(int arr[])
{
  int i = 0;
  while (arr[i] != -1)
  {
    ++i;
  }
  return i;
}

// Converts a char array to an int
int charArrToInt(char arr[])
{
  return atoi(arr);
}

// Returns a section of a char array from a char until a separator is detected
char* select(char arr[], int from)
{
  int i = from, j = 0;
  while (arr[i] != ' ' and arr[i] != '\0' and arr[i] != '\n' and arr[i] != ',')
    ++i;
  j = i - from;
  char* sub = new char[j + 1]; // includes terminating null character
  for (int k = 0; k < j + 1; ++k)
    sub[k] = '\0';
  i = from;
  for (int k = 0; k < j; ++k, ++i)
    sub[k] = arr[i];
  return sub;
}

// Returns a char array with only the required result (second line of output)
char* filterResult(char arr[])
{
  if (arr[0] == '\0')
    return '\0';
  int i = 0, j = 0;
  while (arr[i] != '\n')
    ++i;
  while (arr[++i] != '\n')
    ++j;
  char* filtered = new char[j];
  for (int k = 0; k < j; ++k)
    filtered[k] = '\0';
  i = i - j;
  j = 0;
  while (arr[i + 1] != '\n')
    filtered[j++] = arr[i++];
  return filtered;
}

void gsmOn()
{
  digitalWrite(7, HIGH);
  delay(1000); // 800 also works, but 1000 is safe
  digitalWrite(7, LOW);
  delay(10000); // 9 seconds also works, but 10 is safe
}

void gsmOff()
{
  digitalWrite(7, HIGH);
  delay(1000); // 800 also works, but 1000 is safe
  digitalWrite(7, LOW);
  delay(2000); // wait until shutdown is done
  Serial.println("Entering Stand by mode");
}

char* request(int instruction)
{
  int count = 0, timeout = 0;
  bool flag = false;

  char* buffer = new char[64];
  for (int a = 0; a < 64; a++)
  {
    buffer[a] = '\0';
  }
  int i = 0;
  while (commands[instruction][i] != '\0')
  {
    byte b = commands[instruction][i];
    if (b == '|')
      GSM.write(0x1a);
    else
      GSM.write(b);
    ++i;
  }
  GSM.println();
  delay(100);
  while (!flag and timeout < 100)
  {
    while (GSM.available())
    {
      buffer[count++] = GSM.read();
      if (count == 64)
        break;
      flag = true;
    }
    ++timeout;
  }
  return buffer;
}

// Returns -1 for fine or error code if not fine
int check(char ret[])
{
  char err[] = {'\0', '\0', '\0', '\0'};
  if (ret[0] == '\0')
    err[0] = 0;
  for (int a = 0; a < 64; ++a)
  {
    if (ret[a] == '>')
    {
      Serial.println("SUCCESS");
      return -1;
    }
    if (ret[a] == 'O')
      if (ret[a + 1] == 'K')
      {
        Serial.println("SUCCESS");
        return -1;
      }
    if (ret[a] == 'C')
      if (ret[a + 1] == 'M')
        if (ret[a + 2] == 'E')
          for (int i = a; i < a + 4; ++i)
          {
            if (isDigit(ret[i + 11]))
              err[i - a] = ret[i + 11];
          }
  }
  return charArrToInt(err);
}

// Resolves errors based on their error codes
boolean resolve(int errorCode)
{
  Serial.println("ERROR: " + String(errorCode));
  switch (errorCode) {
    case 0:
      Serial.println("GSM module is switched off or missing, attempting to turn it on");
      gsmOn();
      Serial.println("GSM module is turn on");
      break;
    case 10:
      Serial.println("SIM is not inserted");
      return false;
    case 11:
      Serial.println("SIM PIN required, trying default pin once (4321)");
      //execute(enterPin);
      return false;
    case 30:
      Serial.println("Low or no signal");
      return false;
    case 100:
      Serial.println("Unknown command");
      return false;
    default:
      Serial.println("Error case not found for code: " + String(errorCode) + ", error could not be resolved");
      return false;
  }
  Serial.println("Error resolved for code: " + String(errorCode));
  return true;
}

boolean execute(int set[])
{
  boolean completed = false, fine = true;
  char* output;
  int process = 0, errorCode = -1;
  while (!completed and fine and process < intArrLength(set))
  {
    Serial.println("Running command: " + String(process) + " - " + commands[set[process]]);
    output = request(set[process]);
    errorCode = check(output);
    if (errorCode != -1)
      fine = false;
    if (!fine)
    {
      fine = resolve(errorCode);
      --process;
    }
    if (fine)
    {
      errorCode = -1;
      ++process;
    }
    else
    {
      Serial.println("Program broke due to the above unresolved error.");
      break;
    }
    delete [] output;
  }
}

void startup()
{
  // do easy tasks in set
  execute(startupSet);
  // Advance tests:
  // test signal strength, 0 or 1 is to low
  if (charArrToInt(select(request(6), 15)) < 2)
    if (!resolve(30))
      Serial.println("Program broke due to the above unresolved error.");

}

void setup() {
  GSM.begin(9600);
  Serial.begin(9600);
  pinMode(7, OUTPUT);

  startup();
  Serial.println("Startup done");

  deviceImei = filterResult(request(4));
  Serial.println("IMEI Number: " + String(deviceImei));
  Serial.println("Signal Strength: " + String(select(request(6), 15)));
}

void loop() {
  if (debug) {
    // Read and Write from Serial after startup
    unsigned char buffer[64];
    int count = 0;
    while (GSM.available())
    {
      buffer[count++] = GSM.read();
      if (count == 64)
        break;
    }

    Serial.write(buffer, count);
    for (int i = 0; i < count; i++)
    {
      buffer[i] = '\0';
    }
    count = 0;

    if (Serial.available())
    {
      byte b = Serial.read();
      if (b == '|')
        GSM.write(0x1a);
      else
        GSM.write(b);
    }
  }
  else
  {
    execute(sendSmsSet);
    delay(3000);
    gsmOff();
    delay(standby);
    gsmOn();
  }
}












