//correto
#define sensor A0


int sensorValue = 0;
float voltage = 0, PH = 0;


void setup(){
  pinMode(sensor, INPUT);
 
  Serial.begin(9600);
}

void loop(){
  sensorValue = analogRead(sensor);
  voltage = sensorValue*(5.0/1024.0);
  PH = 7 +((2.5 - voltage)/1.8);
  Serial.print("Sensor value: ");
  Serial.print(sensorValue);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print(" PH: ");
  Serial.println(PH);
 
  delay(2000);
}
