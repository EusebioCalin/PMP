#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// Masca pentru nava, fiecare linie de biĹ�i reprezintÄ� o linie a caracterului
byte nava[8] = {
  B00000,
  B10000,
  B11100,
  B11110,
  B11111,
  B11110,
  B11100,
  B10000,
};
byte om[8] = {
  B00000,
  B00100,
  B11111,
  B10101,
  B00100,
  B01110,
  B01010,
  B01010,
};
byte glonte[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B00000,
  B00000,
  B00000,
};
byte reload[8] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
};
int nr_glonte = 5;
int linie_nava;
long randNumber;
long randLinie;
int count = 0;
int c = 0;
int trage=1; //1 pentru true, 0 pentru false
int i, j;
volatile int matrice[2][16];

static void
printMat() {
  int i, j;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 16; j++) {
      Serial.print(matrice[i][j]);
      Serial.print(" ");
    }
    Serial.print("\r\n");
  }
  Serial.print("\r\n");
}

void setup() {
  pinMode(A0, INPUT); // setarea pinului analogic A0 ca si pin de input
  digitalWrite(A0, HIGH); //activarea rezistorului pull up pentru pinul A0
  pinMode(A1, INPUT); // setarea pinului analogic A1 ca si pin de input
  digitalWrite(A1, HIGH); //activarea rezistorului pull up pentru pinul A1
  lcd.begin(16, 2);
  Serial.begin(9600);
  int i, j;
  for (i = 0; i <= 1; i++) {
    for (j = 0; j <= 15; j++) {
      matrice[i][j] = 0;
    }
  }
  // caracterele sunt stocate Ă®n CGROM, zona utilizator, pozitiile 0,1,2,3
  lcd.createChar(0, nava);
  lcd.createChar(1, om);
  lcd.createChar(2, glonte);
  lcd.createChar(3, reload);
  // AfiĹźare prima linie, un text standard urmat de primul caracter utilizator
  //5 gloante initial
  lcd.setCursor(0, 0);
  lcd.print(nr_glonte);
  lcd.setCursor(1, 0);
  lcd.write(byte(0));
  linie_nava = 0;
  //  lcd.setCursor(2, 0);
  //  lcd.write(byte(2));
  //  matrice[0][2] = 2;
  lcd.setCursor(7, 0);
  lcd.write(byte(1));
  matrice[0][7] = 1;
  lcd.setCursor(5, 0);
  lcd.write(byte(1));
  matrice[0][5] = 1;
  lcd.setCursor(10, 1);
  lcd.write(byte(1));
  matrice[1][10] = 1;
  lcd.setCursor(3, 1);
  lcd.write(byte(1));
  matrice[1][3] = 1;
  lcd.setCursor(14, 1);
  lcd.write(byte(1));
  matrice[1][14] = 1;
  lcd.setCursor(12, 0);
  lcd.write(byte(1));
  matrice[0][12] = 1;
  //reload
  lcd.setCursor(15, 0);
  lcd.write(byte(3));
  matrice[0][15] = 3;
  printMat();
}
void loop()
{
  delay(50);
  c++;
  int val1 = analogRead(A1);
  int val = analogRead(A0);
  if (val > 640)
  {
    linie_nava = 0;
  }
  else
  {
    linie_nava = 1;
  }
  if ((val1 < 640) && (trage==1))
  {
    trage=0;
    if (nr_glonte > 0)
    {
      nr_glonte--;
      if (linie_nava == 0)
      {
        matrice[0][2] = 2;
      }
      else matrice[1][2] = 2;
    }
  }
  
  if (val1>=650)
    {
      trage=1;
    }
  if (c == 20)
  { c = 0;
    for (i = 0; i <= 1; i++) {
      for (j = 15; j >= 0; j--) {
        if (matrice[i][j] == 2)
        {
          if (matrice[i][j + 1] == 1)
          {
            matrice[i][j] = 0;
            matrice[i][j + 1] = 0;
            // nr_glonte--;
          }
          else {
            if (j < 15) {
              matrice[i][j] = 0;
              matrice[i][j + 1] = 2;
            }
          }
        }
      }
    }
    for (i = 0; i <= 1; i++) {
      for (j = 0; j <= 15; j++) {
        if (matrice[i][j] == 1)
        {
          matrice[i][j] = 0;
          
          if (j == 1 && linie_nava == i)
          {
            lcd.clear();
            lcd.print("Ai pierdut");
            delay(2000);
            exit(0);
          }
//           if (j > 1)
//          {
//            matrice[i][j - 1] = 1;
//          }
          if (j > 1 && matrice[i][j-1]!=2)
          {
            matrice[i][j - 1] = 1;
          }
          if (j > 1 && matrice[i][j-1]==2)
          {
             matrice[i][j - 1] = 0;
          }
          
        }
        if (matrice[i][j] == 3)
        {
          matrice[i][j] = 0;
          if ((j == 2) && (linie_nava == i))
          {
            nr_glonte = 5;
          }
          if (j > 2)
            matrice[i][j - 1] = 3;
        }
      }
    }
    printMat();
    // generare oameni
    randNumber = random(0, 2);
    if (randNumber == 1)
    {
      randLinie = random(0, 2);
      if (randLinie == 0)
      {
        matrice[0][15] = 1;
      }
      else if (randLinie == 1)
      {
        matrice[1][15] = 1;
      }
    }
    //generare stamina
    count += 1;
    if (count >= 20)
    { randNumber = random(0, 2);
      if (randNumber == 0)
        matrice[0][15] = 3;
      else matrice[1][15] = 3;
      count = 0;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(nr_glonte);
  if (linie_nava == 0)
  {
    lcd.setCursor(1, 0);
  }
  else
  {
    lcd.setCursor(1, 1);
  }
  lcd.write(byte(0));
  for (i = 0; i <= 1; i++) {
    for (j = 0; j <= 15; j++) {
      if (matrice[i][j] == 1)
      {
        lcd.setCursor(j, i);
        lcd.write(byte(1));
      }
      if (matrice[i][j] == 2)
      {
        lcd.setCursor(j, i);
        lcd.write(byte(2));
      }
      if (matrice[i][j] == 3)
      {
        lcd.setCursor(j, i);
        lcd.write(byte(3));
      }
    }
  }
  //delay(1000);
   
}

