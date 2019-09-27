// FM Радио версия 1.0
// http://www.forklg.ru
//http://forklg.ru/viewtopic.php?f=98&t=1173


#include <Wire.h>               // I2C-Library для управления радиомодулем RDA5807
//#include <EEPROM.h>
// SDA приемника RDA5807 подключаем к аналоговому входу 4 pb0
// SCL приемника RDA5807 подключаем к аналоговому входу 5 pb2
//#include "TM1637.h" // Библиотека для управления LED дисплеем TM1637
//#define CLK 4     // вывод CLC дисплея pb3
//#define DIO 5     // вывод DIO дисплея  pb4
//TM1637 disp(CLK,DIO);
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();
double f_ini=101.100;// Стартовая частота настройки приемника RDA5807. (Можно изменить на нужную).
  
void setup()
{
   matrix.begin(0x70);
   matrix.setBrightness(6);
 /// disp.set(2); // Устанавливаем яркость дисплея на максимум
  //disp.init(D4056A); // Проводим инициализацию дисплея
  Wire.begin();   // Инициализация двухпроводной шины I2C.
 setFrequency(f_ini);  // Присваиваем значение частоты радиомодулю RDA5807
  pinMode(PB3, INPUT); // вход для подключения кнопки "Частота вниз" pb3
  pinMode(PB4, INPUT);// вход для подключения кнопки "Частота вверх"  pb4
 
}

void loop()                  
{
float fin1=(f_ini*100); // производим вычисления для отображения установленной частоты на дисплее
float fin2=fin1/100; // производим вычисления для отображения установленной частоты на дисплее
//disp.display(fin2); // выводим значение частоты на дисплей
matrix.print(fin2);matrix.writeDisplay();

// проверяем нажатие кнопки "Частота вниз"
if (digitalRead(PB3) == HIGH){
// если кнопка "Частота вниз" нажата
f_ini=f_ini-0.1; // уменьшаем значение частоты на 0,1 МГц
if(f_ini<87.5) f_ini=87.5;
 setFrequency(f_ini); // передаем новое значение частоты радиомодулю
 //EEPROM.write(1, (f_ini-825));
 delay(500); // задержка 0,5 сек. 
};
// проверяем нажатие кнопки "Частота вверх"
if (digitalRead(PB4) == HIGH){
// если кнопка "Частота вверх" нажата
f_ini=f_ini+0.1;// увеличиваем значение частоты на 0,1 МГц
if(f_ini>108) f_ini=108;
 setFrequency(f_ini);// передаем новое значение частоты радиомодулю

//EEPROM.write(1, (f_ini-825));};
delay(300); // задержка 0,5 сек. 
}}
  
void setFrequency(double fmhz)  // Функция загрузки частоты настройки в модуль RDA5807.
{
  int  frequencyB = 4 * (fmhz * 1000000 + 225000) / 32768;
  char frequencyH = frequencyB >> 8;     // Старший байт.
  char frequencyL = frequencyB & 0XFF;   // Накладываем маску 0xFF на младший байт.
  Wire.beginTransmission(0x60);          // Адрес чипа RDA5807M
  Wire.write(frequencyH);                // Старший байт.
  Wire.write(frequencyL);                // Младший байт.
  Wire.write(0xB8);                      // 1011 1000     =Стерео
  Wire.write(0x10);                      // 0001 0000
  Wire.endTransmission();                // формируем I2C-Stop.
}
