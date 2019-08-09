#include <SoftwareSerial.h>
void sendMessage(int n);

/*Create software serial object to communicate with SIM800L*/
SoftwareSerial mySerial(8,7); /*SIM800L Tx & Rx is connected to Arduino #3 & #2*/

int soil_moisture_sensor_pin = 0; /* Initialize the soil moisture sensor's pin to A0 of Arduino*/

int light_sensor_pin=5; /* Initialize the Light sensor's pin to A1 of Arduino*/

int air_quality_sensor_pin=2; /* Initialize the Air quality sensor's pin to A2 of Arduino*/

int temp_output;

int soil_moisture_output ; /* Decleration of output variables*/

int light_output;

int air_output=0;

int temp_sensor_pin = 3;/* Initialize the Temperature sensor's pin to A3 of arduino */

const int MOISTURE_LEVEL = 250; /*Initialize the moisture level to 250*/

int led2=12;

int led=13;

int led1=8;


void setup() {  /*For communicating with computer.*/

   
   Serial.begin(9600); /*opens Serial port and sets the data rate 9600 bits per second.*/
   
   mySerial.begin(9600);

   Serial.println("Reading From the Sensor ...");
   
   pinMode(led2,OUTPUT);

   pinMode(led,OUTPUT);
   
   pinMode(led3,OUTPUT);
   
   pinMode(13, OUTPUT);
  
   pinMode(7, OUTPUT);

    delay(2000);

     

 
}

void LedState(int state)
{
  digitalWrite(13,state);
}

void loop() {
   

  soil_moisture_output= analogRead(soil_moisture_sensor_pin); /* anlogRead() reads the input from pin A0 and assign that to ouput value. for soil moisture sensor*/

  Serial.print("Mositure : ");

  Serial.println(soil_moisture_output);

  /*Checks the current moisture level with the initialized moisture level 250*/

   if(soil_moisture_output > MOISTURE_LEVEL)
  {
    LedState(HIGH);
    digitalWrite(7,HIGH);
    sendMessage(soil_moisture_output);
  }
  else 
  {
    LedState(LOW);
    digitalWrite(7,LOW);
  }

 
  

  
   
  light_output=analogRead(light_sensor_pin);   /*reads the input from pin A1. for Light sensor. */

  Serial.print("Light : ");

  Serial.println(light_output);/* prints the ligh sensor output to the serial port in human readable form*/

  if(light_output>650 || light_output<100)
  {
    sendMessage(light_output);
  }

 
   
  air_output=analogRead(air_quality_sensor_pin); /*reads the input from pin A2. For Air quality sensor. */

  Serial.print("Air quality : ");

  Serial.print(air_output); /* prints the air quality output to the serial port in human readable form*/

  Serial.println("  PPM");
   if(air_output>250 || air_output<50)
  {
   sendMessage(air_output);
  }


 

  temp_output = analogRead(temp_sensor_pin);

   float mv = ( temp_output/1024.0)*5000; 
  
  float cel = mv/10;
  
  float farh = (cel*9)/5 + 32;

  Serial.print("TEMPRATURE = ");
  
  Serial.print(cel);
  
  Serial.println("*C");

  

   if(cel>45 || cel<28)
  {
    digitalWrite(led1,HIGH);
    delay(1000);
    digitalWrite(led1,LOW);
  }
  else
  {
    Serial.println("Temperature is normal");
  }
  Serial.println("................"); 
  
  Serial.println();
  
  delay(1000);


}
void sendMessage(int n)
{
  Serial.println("Initializing..."); 
  
  delay(1000);
   mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CREG=1");//
  updateSerial();
 

   mySerial.println("AT+CREG?");//Check that you’re registered on the network. The second # should be 1 or 5. 1 indicates you are registered to home network and 5 indicates roaming network. Other than these two numbers indicate you are not registered to any network.
  updateSerial();
 
   Serial.println("..........");
   mySerial.println("AT+COPS=?");//Return the list of operators present in the network.
  updateSerial();

  mySerial.println("AT+COPS?");//Check that you’re connected to the network, in this case BSNL
  updateSerial();
   Serial.println("..........");
  
  mySerial.println("AT+CSQ");
  updateSerial();

   Serial.println("..........");
  mySerial.println("AT+CMEE=1");//Selects SMS message format as text. Default format is Protocol Data Unit (PDU)
  updateSerial();
 
  
   Serial.println("..........");
 
  mySerial.println("AT+CBC"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CPIN=1"); // Configuring TEXT mode
  updateSerial();
  

 

 mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+917892265360\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.write(26);

   
}


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
   /* 1. println() to give a carriage return or a new line at the end.
      2.Serial. is used to communicate between arduino and computer.
    */
