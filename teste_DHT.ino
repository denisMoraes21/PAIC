#include <DHT.h>
#define DHTPIN A3 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600);
  dht.begin();
}
 
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h)){
    Serial.println("Failed to read from DHT");
  } 
  else{
    Serial.print("Umidade Relativa de Equilíbrio: ");
    Serial.print(h);
    Serial.print("%UR");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print("°C");
    Serial.print(" Atividade da Água: ");
    Serial.println(h/100);

    //Exibir gráfico ln aw vs 1/T
    
    delay(1000);

    
  }
}
