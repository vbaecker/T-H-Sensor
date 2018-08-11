// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_NODE_ID 60
#define PRIMARY_CHILD_ID 1

#include <SPI.h>
#include <MySensors.h> 
#include <DHT.h>

// Sleep time between reads (in milliseconds)
unsigned long SLEEP_TIME = 300000; 

#define CHILD_ID_HUM 3
#define CHILD_ID_TEMP 4
#define DHTPIN 4     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

float lastTemp;
float lastHum;
bool metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

int reportInterval = 10;
int reportCount = 1;

void setup()  
{ 

  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature and humidity sensor", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM,"Humidity");
  present(CHILD_ID_TEMP, S_TEMP,"Temperature");
  
  //metric = getConfig().isMetric;
  
  dht.begin();
}

void loop()      
{ 
  sleep(5);
  
  // Temperature
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
      Serial.println("Failed reading temperature from DHT");
  } 
  else {
    //Serial.print("T: ");
    //Serial.println(temperature);
    if(lastTemp != temperature || reportCount == 10) { 
      lastTemp = temperature;
      send(msgTemp.set(temperature, 1));    
    }
  }

  // Humidity
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
  } 
  else {
    //Serial.print("H: ");
    //Serial.println(humidity);
    if(lastHum != humidity || reportCount == 10) { 
      lastHum = humidity;
      send(msgHum.set(humidity, 1));
    }
  }
  
  if(reportCount == 10) {
    reportCount = 1;
  }
  else {
    reportCount++;
  }
  
  sleep(SLEEP_TIME);
}

