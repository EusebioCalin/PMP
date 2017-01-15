#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
void setup()
{
analogReference(DEFAULT); //setarea tensiunii de referinta la tensiunea default
lcd.begin(16, 2); //initializarea LCD ului
lcd.setCursor(0,0);
lcd.print("Cititi senzor");
pinMode(A0, INPUT); // setarea pinului analogic A0 ca si pin de input
digitalWrite(A0, HIGH); //activarea rezistorului pull up pentru pinul A0
pinMode(A1, INPUT); // setarea pinului analogic A1 ca si pin de input
digitalWrite(A1, HIGH); //activarea rezistorului pull up pentru pinul A1
}
void loop()
{
int val = analogRead(A0); //citirea valorii analogice
int val1 = analogRead(A1);
lcd.setCursor(0,1);
lcd.print(val);
lcd.setCursor(5,1);
lcd.print(val1);
} 

