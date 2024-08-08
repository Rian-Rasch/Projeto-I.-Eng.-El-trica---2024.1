
#include <Wire.h>  // Biblioteca para Comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca com as funções do display
#include <Servo.h>

#define flameSensorPin A0
#define tempSensorPin A1
#define batterySensorPin 2 // Supondo que o sensor de carga da bateria está conectado ao pino A2
#define servoPin 3

#define p_temp 60
#define p_fogo 200
#define abertura 95
Servo servo;

LiquidCrystal_I2C lcd(0x27,20,4); // Inicializa o display no endereço 0x27

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(tempSensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);
  servo.write(0);
  lcd.init(); 
  lcd.begin(16, 2); // Identifica que o display possui 16 colunas e 2 linhas
  lcd.clear(); // Apaga todos os caracteres do Display
  lcd.backlight();
}

int fogo_ir() {
  if (analogRead(flameSensorPin) < p_fogo) {
    delay(250);
    if (analogRead(flameSensorPin) < p_fogo) {
      return 1;
    } else return 0;
    }else return 0;
}

int fogo_temp() {
  int v_input = analogRead(tempSensorPin);//1024 VALUE
  float temp = 125.315-0.175529*v_input-2;
  if (temp > p_temp) {
    delay(250);
    int v_input = analogRead(tempSensorPin);//1024 VALUE
    float temp = 125.315-0.175529*v_input-2;
    if (temp > p_temp) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

void aciona_ext(int flag){
  //int tempo = flag * 1000;
  servo.write(abertura);
  
  while(flag !=0){
    flag = fogo_temp() + fogo_ir();
    
  }
  //delay (tempo);
  servo.write(0);
}

void loop() {
  int flag = fogo_temp() + fogo_ir();
  int v_input = analogRead(tempSensorPin);//1024 VALUE
  float temp = 125.315-0.175529*v_input-2;

/*
  Serial.print("Flag = ");
  Serial.println(flag);
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" C ");
  Serial.print("IR: ");
  Serial.println(analogRead(flameSensorPin));
  /*
  Serial.print("Bat: ");
  Serial.print(map(analogRead(batterySensorPin), 0, 1023, 0, 100));
  Serial.println("%");
  Serial.println();
  */

  lcd.clear(); // Apaga todos os caracteres do Display
  lcd.setCursor(0,0); // Marca o cursor na posição coluna 0, linha 0
  lcd.print("Temp:");
  char buffer[10];
  dtostrf(temp, 3, 0, buffer);
  lcd.print(buffer);
  lcd.print("C");
  lcd.setCursor(0,1); // Marca o cursor na posição coluna 0, linha 1
  lcd.print("Bat:");
  dtostrf((2.54 + (analogRead(batterySensorPin) / 1023.0) * (4.25 - 2.54))*100/4.25, 3, 0, buffer);
  lcd.print(buffer);
  lcd.print("%");
  
  //*(5.0/1023.0))
  
  lcd.setCursor(10,0);
  lcd.print("Ir:");
  lcd.print(analogRead(flameSensorPin)/100);

   if(flag != 0){
    aciona_ext(flag);
  }

  delay(500);
  
}
