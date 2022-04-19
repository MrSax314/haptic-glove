#include <Servo.h>
#define LED 13        // On board LED

Servo servos[5];  // create servo object to control a servo

// Pin Values 
int encoderPins[5] = {A0, A1, A2, A3, A4};  // Encoder pin number  
int actuatorPins[5] = {2, 3, 4, 5, 6};      // Actuator pin number

 // Data Storage 
byte incomingBytes[5] = {};        // Incoming serial data [20]
byte actuatorCommands[5] = {};     // Commands for actuators

int encoderVoltages[5] = {};         // Finger encoder data
byte encoderAngles[6] = {};          // Finger calculated angles
byte data[6] = {};

// Extra vars
//char stopChar = 'z';
int temp = 0;
bool _connected = false;
// Add : Encoder and Motor bounds for Init function

// Functions
void pinSetup();
void establishContact();
void readEncoders();
void writeData();
void readData();
void setServos();

void setup() {  
  pinSetup();                         // Sets up encoder and servo pins
  Serial.begin(250000);               // Baudrate
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //Serial.setTimeout(1000);          // If data can't be read, timeout
  //establishContact();               // Waits for callback
  Serial.flush();
}


void loop() {
  //readEncoders();                     // Read encoders
  //writeData();                        // Output finger angles over USB
  readData();
  setServos();
}

//==============================================================================

void pinSetup()
{
  // Encoder pin setup
  for (int i = 0; i < sizeof(encoderPins)/sizeof(encoderPins[0]); i++)
  {
    pinMode(encoderPins[i], INPUT);
    //pinMode(actuatorPins[i], OUTPUT);
  }
  
  // Actuator pin setup
  for (int i = 0; i < sizeof(actuatorPins)/sizeof(actuatorPins[0]); i++)
  {
    servos[i].attach(actuatorPins[i]);
  }
}


void readEncoders()
{
  // Read voltages (0V-5V)
  for (int i = 0; i < sizeof(encoderPins)/sizeof(encoderPins[0]); i++)
  {
    encoderVoltages[i] = analogRead(encoderPins[i]);
  }
  // Convert voltage -> angle (Linear up to 320deg) TODO [Maybe do conversion upon writing data to pc?]
  for (int i = 0; i < sizeof(encoderPins)/sizeof(encoderPins[0]); i++)
  {
    //encoderAngles[i] = encoderVoltages[i] * 360 / 5 - 180;    // +/- 180deg
    encoderAngles[i] = map(encoderVoltages[i], 0, 1023, 1, 255);  // scale it to use it with the servo (value between 0 and 180)
  }
}


void writeData()
{
  encoderAngles[5] = 0;
  Serial.write(encoderAngles, sizeof(encoderAngles)/sizeof(encoderAngles[0]));      // Write data over USB
  //sprintf(string, "%u, %u, %u, %u, %u", encoderAngles[0], encoderAngles[1], encoderAngles[2], encoderAngles[3], encoderAngles[4]);
  //Serial.println(string);
  /*
  for (int i = 0; i < 5; i++)
  {
    //String myString = String((char *)encoderAngles);
    Serial.print(encoderAngles[i]);
    Serial.print(", ");
  }
  Serial.println();
  */
  
  /*
  if (Serial.availableForWrite() > sizeof(encoderAngles)/sizeof(encoderAngles[0]))
  {
    //Serial.write( (uint8_t*)encoderAngles, sizeof(encoderAngles));  
    Serial.write(encoderAngles, sizeof(encoderAngles));  
  }
  */
}
// incommingBytes actuatorCommands
void readData()
{
  if (_connected)
  {
    // Read 6 bytes
    Serial.readBytes(incomingBytes,sizeof(incomingBytes)/sizeof(incomingBytes[0]));
    
    // Disconnect if last byte is zero
    if (incomingBytes[5] != 0) 
    {      
      Serial.println("Last byte not zero - Connection lost");
      _connected = false;
      return;
    }
  }
  else    // Not connected
  {
    temp = Serial.read();
    if (temp == 0)
      _connected = true;      // Connected if 0 is read   
  }
  /*
  while(Serial.read() > 0)
  {
    //delay(500);
  }
  if (Serial.available() >= 5)
  {
    Serial.readBytes(incomingBytes,sizeof(incomingBytes)/sizeof(incomingBytes[0]));  // Read commands from USB
    if (incomingBytes[0] == 0)
      digitalWrite(LED, HIGH);
    //Serial.print(incomingBytes[0]);
    //setServos();                                            // Command actuators
  }
  */
}

void setServos()
{
  for (int i = 0; i < sizeof(actuatorPins)/sizeof(actuatorPins[0]); i++)
  {
    //analogWrite(actuatorPins[i], actuatorCommands[i]);
    servos[i].write(incomingBytes[i]);
    /*
    while(true) 
    {
      Serial.write(incomingBytes[0]);  
    }
    */
    //setServos();
    //servos[i].write(encoderAngles[i]);
  }
}


void establishContact() 
{
  /*
  while (Serial.available() <= 0)   // Loops until some data is received
  {
    Serial.write(0);              // Send a capital A
    //initializeHand();             // Init glove parameters
    delay(300);
  }
  */
  Serial.write(0);              // Send a capital A
  delay(300);
}


/* Steps
 * 1) Update finger pos data
 * 2) Send finger pos data over USB
 * 3) Read braking commands from USB
 * 4) Output to motors if necessary 
 */


 //// Legacy


  //val = map(val, 0, 1023, 0, 256);     // scale it to use it with the servo (value between 0 and 180)
  //Serial.println(val);
  //myservo.write(val);                  // sets the servo position according to the scaled value
  //delay(15);                           // waits for the servo to get there



 // ====================== Read commands from USB ======================
  /*if (Serial.available() > 0) // > 5??? TODO
  {
    Serial.readBytesUntil(stopChar, incomingBytes, 5); // Stores incoming data
  }
  */
  // TODO: Convert to signal for motors to use (depends on actuator used)
  //actuatorCommands[i] = ...
