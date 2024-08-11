
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define flameSensorPin A0
#define tempSensorPin A1
#define batterySensorPin 2 
#define servoPin 3

#define p_temp 60
#define p_fogo 200 //0 a 1023
#define abertura 95
Servo servo;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  //Serial.begin(9600);
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

void aciona_ext1(int flag){
  servo.write(abertura);
  while(flag !=0){
    delay(200);
    flag = fogo_temp() + fogo_ir(); 
  }
  servo.write(0);
}

void aciona_ext2(int flag){
  servo.write(abertura);
  delay(flag*1000);
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
  Serial.print("Bat: ");
  Serial.print(map(analogRead(batterySensorPin), 0, 1023, 0, 100));
  Serial.println("%");
  Serial.println();
*/

  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("Temp:");
  char buffer[10];
  dtostrf(temp, 3, 0, buffer);
  lcd.print(buffer);
  lcd.print("C");
  lcd.setCursor(0,1); 
  lcd.print("Bat:");
  dtostrf((2.54 + (analogRead(batterySensorPin) / 1023.0) * (4.25 - 2.54))*100/4.25, 3, 0, buffer);
  lcd.print(buffer);
  lcd.print("%");
  
  lcd.setCursor(10,0);
  lcd.print("Ir:");
  lcd.print(analogRead(flameSensorPin)/100);

   if(flag != 0){
    aciona_ext1(flag);
  }
  delay(500);
}