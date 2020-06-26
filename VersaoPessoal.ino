#include <OneWire.h>
#include <DallasTemperature.h>
#define SensorTDS A0
#define Amostras 30
#define Tensao 5.0

int Sinal[Amostras];
int Tempo[Amostras];
int i = 0, j =0;
float TensaoMedia = 0, ValorTDS = 0;

//relógio
int hora = 10, minuto = 59, segundo = 58, amostra;
double media;
double valoresTDS[29];
//

//Sensor de Temeperatura
OneWire pino(7);
DallasTemperature barramento(&pino);
DeviceAddress sensor;
//

void setup() {
  Serial.begin(9600);
  pinMode(SensorTDS, INPUT);
  barramento.begin();
  barramento.getAddress(sensor, 0);  
}

void loop() {
  barramento.requestTemperatures(); 
  float temperatura = barramento.getTempC(sensor);
  
  static unsigned long AmostraTempoLigado = millis();
  if (millis() - AmostraTempoLigado > 0.04){
    AmostraTempoLigado = millis();
    Sinal[i] = analogRead(SensorTDS);
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

    TensaoMedia = (NumeroMedio(Tempo, Amostras) * (float)Tensao)/1024;
    float CompensacaoTensao = TensaoMedia/(1.0 + 0.02*(temperatura - 25.0));
    ValorTDS = (133.42*CompensacaoTensao*CompensacaoTensao*CompensacaoTensao - 255.86*CompensacaoTensao*CompensacaoTensao + 857.39*CompensacaoTensao)/2;
   
    
  }


segundo++;

  if (segundo == 60){
    segundo = 0;
    minuto += 1; 
  }

  if (minuto == 60){
    minuto = 0;
    hora += 1;
  }
  
  amostra = minuto%5;
  

  if (amostra == 0){
    if (segundo == 0){
    
      //Serial.println();
      //Serial.print("VALOR Adquirido");
      Serial.print("Valor TDS obtido: ");
      Serial.print(ValorTDS);
      Serial.println(" ppm");
      Serial.print("  Temperatura: ");
      Serial.print(temperatura, 3);
      Serial.println("°C");
      delay(100);
      
    }
  }

  if (hora == 24){
    hora = 0;
  }

  
  Serial.print("Horário atual: ");
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);
  Serial.print(":");
  Serial.println(segundo);
  delay(100);
}

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
