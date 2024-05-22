#include <Wire.h>  // Biblioteca para Comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca com as funções do display
#include <Servo.h>

#define flameSensorPin A0
#define tempSensorPin A1
#define batterySensorPin A2 // Supondo que o sensor de carga da bateria está conectado ao pino A2
#define servoPin 3

#define p_temp 60
#define p_fogo 200
Servo servo;

LiquidCrystal_I2C lcd(0x27,20,4); // Inicializa o display no endereço 0x27

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
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
    }
  } else {
    return 0;
  }
}

int fogo_temp() {
  if (map(analogRead(tempSensorPin), 0, 1023, 125, -55) > p_temp) {
    delay(250);
    if (map(analogRead(tempSensorPin), 0, 1023, 125, -55) > p_temp) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

void aciona_ext(int flag){
  int tempo = flag * 1000;
  servo.write(360);
  delay (tempo);
  servo.write(0);
}

void loop() {
  int flag = fogo_temp() + fogo_ir();
  
  Serial.print("Flag = ");
  Serial.println(flag);
  
  Serial.print("Temperatura: ");
  Serial.print(map(analogRead(tempSensorPin), 0, 1023, 125, -55));
  Serial.println(" C ");
  
  Serial.print("Bat: ");
  Serial.print(map(analogRead(batterySensorPin), 0, 1023, 0, 100));
  Serial.println("%");
  
  Serial.println();
  
  delay(1000);

  lcd.clear(); // Apaga todos os caracteres do Display
  lcd.setCursor(0,0); // Marca o cursor na posição coluna 0, linha 0
  lcd.print("Temperatura:");
  lcd.print(map(analogRead(tempSensorPin), 0, 1023, 125, -55));
  lcd.print("C");

  lcd.setCursor(0,1); // Marca o cursor na posição coluna 0, linha 1
  lcd.print("Bat:");
  lcd.print(map(analogRead(batterySensorPin), 0, 1023, 0, 100));
  lcd.print("%");
  lcd.print(" Ir: ");
  lcd.print(analogRead(flameSensorPin));
}
