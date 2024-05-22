#include <Wire.h>  // Biblioteca para Comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca com as funções do display
#include <OneWire.h> // Biblioteca para comunicação com o sensor DS18B20
#include <DallasTemperature.h> // Biblioteca para o sensor DS18B20

LiquidCrystal_I2C lcd(0x27, 20, 4); // Inicializa o display no endereço 0x27

#define ONE_WIRE_BUS 2 // Define o pino digital ao qual o sensor está conectado

OneWire oneWire(ONE_WIRE_BUS); // Cria uma instância do objeto OneWire para se comunicar com o sensor
DallasTemperature sensors(&oneWire); // Passa o objeto OneWire para a biblioteca DallasTemperature

void setup() {
  lcd.init();
  lcd.begin(16, 2); 
  lcd.clear(); 
  lcd.backlight();

  sensors.begin(); 
}

void loop() {
  lcd.clear(); 

  sensors.requestTemperatures(); // Solicita a leitura da temperatura ao sensor

  float temperatura = sensors.getTempCByIndex(0); // Obtém a temperatura em graus Celsius

  lcd.setCursor(0, 0); 
  lcd.print("Temperatura: ");
  lcd.print(temperatura); 
  lcd.print("C");

  delay(2000); 
}
