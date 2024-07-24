#define BLYNK_TEMPLATE_ID "TMPL3Xp2d_bZh"
#define BLYNK_TEMPLATE_NAME "Health care device"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
const int IR_PIN3 = D7;

#define laser 12

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
double temp_amb;
double temp_obj;

char auth[] = "xHIiFoywI_eD4xBurhc6snJlFlk9husW";
char ssid[] = "HK";
char pass[] = "aaaaaaaa";
bool objectDetected3 = false;

const int ecgPin = A0; // Analog pin connected to AD8232 output

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
   mlx.begin();
  pinMode(laser, OUTPUT);
  digitalWrite(laser, LOW);
}

void loop()
{
  Blynk.run();

    int sensorValue3 = digitalRead(IR_PIN3);
  Serial.print("Sensor Value 3: ");
  Serial.println(sensorValue3);

  if (sensorValue3 == HIGH && !objectDetected3) {
    Serial.println("Object Detected on Sensor 3");
    Blynk.logEvent("sos","Emergency, patient number x needs assistance.!!!!!");
    objectDetected3 = true;
  }

  // If no object detected by sensor 3, update label value and reset flag
  if (sensorValue3 == LOW && objectDetected3) {
    Serial.println("Object Not Detected on Sensor 3");
    objectDetected3 = false;
  }

  int ecgValue = analogRead(ecgPin); // Read ECG value from analog pin A0

  Serial.print("Raw ECG Value: ");
  Serial.println(ecgValue);
  
  // Map the ECG value to an appropriate range (adjust these values based on your sensor)
  int mappedECG = map(ecgValue, 0, 1023, 0, 1023);

  Serial.print("Mapped ECG Value: ");
  Serial.println(mappedECG);
  
  Blynk.virtualWrite(V1, mappedECG); // Send mapped ECG value to Blynk app
  
  if (mappedECG < 100) // Adjust the threshold value as needed
  {
    //Blynk.notify("ECG level is low!");
    Blynk.logEvent("ECG", "Level is low");
  }
  
  // Debug output for Serial plotter
  Serial.print("0,");
  Serial.println(mappedECG);
  delay(100); // Add a small delay to avoid flooding Blynk with data

   temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();
  digitalWrite(laser, HIGH);

  Serial.print("Room Temp = ");
  Serial.println(temp_amb);
  Serial.print("Object temp = ");
  Serial.println(temp_obj);

  Blynk.virtualWrite(V0, temp_obj); // Send object temperature to Blynk

  delay(1000);
}
