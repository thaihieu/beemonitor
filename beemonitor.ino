

#include <CurieBle.h>
#include "DHT.h"
#include "HX711.h"

// HX711.DOUT  - pin #10
// HX711.PD_SCK - pin #11

#define DHTPIN A0     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
HX711 scale(10, 11);    // parameter "gain" is ommited; the default value 128 is used by the library

BLEPeripheral blePeripheral;
BLEService BeeSensorService("181A");
DHT dht(DHTPIN, DHTTYPE);

// BLE  Level Characteristic"
BLEUnsignedCharCharacteristic TempLevelChar("2A6E",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);     // remote clients will be able to get notifications if this characteristic changes
BLEUnsignedCharCharacteristic HumLevelChar("2A6F",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);     
BLEUnsignedCharCharacteristic MassLevelChar("2A9E",  // standard 16-bit characteristic UUID
BLERead | BLENotify);     



void setup()
  {
    // put your setup code here, to run once:
    Serial.begin(9600);    // initialize serial communication
    pinMode(13, OUTPUT);   // initialize the LED on pin 13 to indicate when a central is connected  

 /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet */
  blePeripheral.setLocalName("BeeMonitorSketch");
  blePeripheral.setAdvertisedServiceUuid(BeeSensorService.uuid());  // add the service UUID
  blePeripheral.addAttribute(BeeSensorService);   // Add the BLE Temperture/Humility/Mass) service
  
  blePeripheral.addAttribute(TempLevelChar); // add the Temp level characteristic
  blePeripheral.addAttribute(HumLevelChar);
  blePeripheral.addAttribute(MassLevelChar); // add the Mass level characteristic
 

  mass_calibrate(); //start the calibration of the scale
      

  /* Now activate the BLE device.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */
  blePeripheral.begin();
  Serial.println("Bluetooth device active, waiting for connections...");
  
    
  }
void loop() {

 
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(13, HIGH);
    
  
    // as long as the central is still connected:
    while (central.connected()) {   
        beesensor();      
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(13, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    
  }
  delay(2000); //2 second delay but will increase for reading real application to low power consumption
}

    

void beesensor() {

// Wait a few seconds between measurements.
  delay(2000);
 
  
  
 

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  float m = scale.get_units()*0.1; //set KG
  
 // TempLevelChar.setValue(0xabcd);
  TempLevelChar.setValue(t);
  HumLevelChar.setValue(h);
  MassLevelChar.setValue(m);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f) || isnan(m))  {
    Serial.println("Failed to read from sensors!");
    return;
  }
  if (m < 0  ){ // recalibrate the scale again if value go below 0
  mass_calibrate();
        delay(2000);
 };

  // Compute heat index in Fahrenheit (the default)evi
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  Serial.print("Weight in Kg:\t");
  Serial.print(m, 1);
  Serial.print("\t| Average Weight in Kg:\t");
  Serial.println(scale.get_units(10)*0.1, 1);  //get 10 reading and multipy 0.1 to get 1kg

  scale.power_down();             // put the ADC in sleep mode
  delay(4000);
  scale.power_up();
 
}

void mass_calibrate() {

  
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());     // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided 
            // by the SCALE parameter (not set yet)  

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}


