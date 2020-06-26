/* 
 * Valores de KE:
 * Taberebá: -0,073310853
 * Cupuaçu: -0,079398551
 * Maracujá: -0,083678176
 */
 
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define DHTTYPE DHT11
#define DHTPIN A1
#define PHPIN A2
#define TDSPIN A3
#define Amostras 30
#define Tensao 5.0

DHT dht(DHTPIN, DHTTYPE);
OneWire pino(7);
DallasTemperature barramento(&pino);
DeviceAddress sensor;
LiquidCrystal_I2C lcd(0x27,16,2);
float PhSensor = 0, Volt = 0, ph, ce, aw, temp, TensaoMedia = 0, tds, media; 
float KE = -0.073310;
double ValoresTds[29];
int Sinal[Amostras];
int Tempo[Amostras]; 
int i = 0, j = 0, amostra;


void setup() {
  Serial.begin(9600);
  dht.begin();
  barramento.begin();
  barramento.getAddress(sensor, 0);
  pinMode(TDSPIN, INPUT);
  pinMode(PHPIN, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.backlight();
}

// Temperatura
  float Temp(){
    barramento.requestTemperatures();
    float temp = barramento.getTempC(sensor);
    return temp;
  }
  
// Total de Sólidos Dissolvidos
  float Ce(){
    TensaoMedia = (NumeroMedio(Tempo, Amostras) * (float)Tensao)/1024;
    float CompensacaoTensao = TensaoMedia/(1.0 + 0.02*(Temp() - 25.0));
    tds = (133.42*CompensacaoTensao*CompensacaoTensao*CompensacaoTensao - 255.86*CompensacaoTensao*CompensacaoTensao + 857.39*CompensacaoTensao)/2;
    ce = tds/KE;
    return ce;
  }  
  
// Potencial Hidrogeniônico
  float Ph(){
    PhSensor = analogRead(PHPIN);
    Volt = (PhSensor*5.0)/1024;
    ph = 7+((2.5-Volt)/1.8);
    return ph;
  }

// Atividade da água
  float Aw(){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (isnan(t) || isnan(h))
      Serial.println("Aw error");
    else
      float aw = h/100;
    return aw;
  }
  
// Número médio
  int NumeroMedio(int Array[], int Tamanho){
    int box[Tamanho];
    int box2;
    for (int c = 0; c < Tamanho; c++){
      box[c] = Array[c];
    }
    for (int c = 0; c < Tamanho - 1; c++){
      if (box[c] > box[c + 1]){
        box2 = box[c];
        box[c] = box[c + 1];
        box[c + 1] = box2;
      }
    }
    if (Tamanho % 2 < 1)
      box2 = box[Tamanho/2];
    else 
      box2 =  box[(Tamanho + 1)/2];
    return box2;
  }
  
void loop() {
  static unsigned long AmostraTempoLigado = millis();
    if (millis() - AmostraTempoLigado > 0.04){
      AmostraTempoLigado = millis();
      Sinal[i] = analogRead(TDSPIN);
      i++;
      if(i == Amostras){
        i = 0;
      }
    }
  
  static unsigned long TempoLigado = millis();
  if (millis() - TempoLigado > 0.8){
    TempoLigado = millis();
    for (j = 0; j < Amostras; j++){
       Tempo[j] = Sinal[j];
    }
  }

   lcd.setCursor(0,0);
   lcd.print("CE: ");
   lcd.setCursor(4,0);
   lcd.print(Ce());
   lcd.print(" mg/L");

   lcd.setCursor(0,1);
   lcd.print("TE: ");
   lcd.setCursor(4,1);
   lcd.print(Temp());
   lcd.print(" °C");

   delay(1000);
   lcd.clear();
   
   lcd.setCursor(0,0);
   lcd.print("PH: ");
   lcd.setCursor(4,0);
   lcd.print(Ph());

   lcd.setCursor(0,1);
   lcd.print("AW: ");
   lcd.setCursor(4,1);
   lcd.println(Aw());

   delay(1000);
   lcd.clear();
  
}
