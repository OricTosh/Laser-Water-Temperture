#include <OneWire.h>
#include <DallasTemperature.h>

//Data wire is plugged into port 7
const int ONE_WIRE_BUS =  7;
const int TEMPERATURE_PRECISION = 9;
// Setup a oneWire Instance
OneWire oneWire(ONE_WIRE_BUS);
//Pass our oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress;

const int latchPin = 4;
const int clockPin = 3;
const int dataPin = 2;
const int digit1 = 5;
const int digit2 = 6;
//byte data;
byte dataArray[13];
const int redLed = 8;


void setup() {
  sensors.begin();
  Serial.begin(9600);
  Serial.println("Laser Water Temp");
  numberOfDevices = sensors.getDeviceCount();
  
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		Serial.print("Found device ");
		Serial.print(i, DEC);
		Serial.print(" with address: ");
		printAddress(tempDeviceAddress);
		Serial.println();
		Serial.print("Setting resolution to ");
		Serial.println(TEMPERATURE_PRECISION, DEC);
		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
        	Serial.print("Resolution actually set to: ");
		Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
		Serial.println();
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(i, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
  }

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  
  dataArray[0] = 3;    // Zero
  dataArray[1] = 159;  // One
  dataArray[2] = 37;   // Two
  dataArray[3] = 13;   // Three
  dataArray[4] = 153;  // Four
  dataArray[5] = 73;   // Five 
  dataArray[6] = 65;   // Six
  dataArray[7] = 31;   // Seven
  dataArray[8] = 1;    // Eight
  dataArray[9] = 9;    // Nine
  
}  // End of setup

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  int temp;
  
  temp = (int) tempC;
     
  Serial.print("Temp C: ");
  Serial.println(tempC);
  
  if (temp > 28)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    delay(500);
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(redLed, LOW);
  }
  
 // if (temp < 28)
  //{
    //digitalWrite(greenLed, HIGH);
 // }
  
  // get units of temp
  int units = temp % 10;
  Serial.print("Units: ");
  Serial.println(units);
  
  // get tens value of temp
  temp = temp/10;
  int tens = temp % 10;
  Serial.print("Tens: ");
  Serial.println(tens);
  displayNum(units, tens);
  
}

void displayNum(int a, int b){
  
  //a = 8; //debug
  //b = 5; 
  digitalWrite(latchPin, HIGH);
  shiftOut(dataPin, clockPin, LSBFIRST, dataArray[a]);
  shiftOut(dataPin, clockPin, LSBFIRST, dataArray[b]);
  digitalWrite(latchPin, LOW);

}


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop() {
  
  delay(1000);
  sensors.requestTemperatures();
  printTemperature(tempDeviceAddress);  

} // End of Loop  
   
  
