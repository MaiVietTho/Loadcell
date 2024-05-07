#include "HX711.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);  // EN,RS,D4,D5,D6,D7
HX711 scale(A1, A0); // HX711.DOUT- pin #A0 ;HX711.PD_SCK- pin #A1   
int mode=0;
int mainmode=0;
float hieuchinh = 1.13;
float zero;
int A; // tru?c c?a a
int a;
#define clk  4
#define data 3
#define LOA  5
#define ROLE 6
float g_SET=1;
float cannang;
void ngat()
{
  mode=1;  pip_1();
  for(int j=0;j<100;j++);
}

void setup()
{
    pinMode(LOA,OUTPUT);
    pinMode(ROLE,OUTPUT);
    lcd.begin (16,2);
    scale.set_scale(2280.f);  // this value is obtained by calibrating the scale with known weights; see the README for details
    scale.tare();             // reset the scale to 0
    pinMode(2, INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    attachInterrupt(0, ngat, LOW);
    zero = scale.get_units(1)*hieuchinh;
    digitalWrite(ROLE,LOW);
     g_SET = EEPROM.read(2);
   g_SET = 1000;
}

void loop()
{
    if(mode==1)
  {
    lcd.setCursor(0,0);lcd.print("mode= 1 ");
    mode=0;
    mainmode++;
    if(mainmode>2) mainmode=0;
    while(digitalRead(2)==0);lcd.clear();
  }

  if(mainmode==0) // man hinh chinh
        { can();
          cannang = scale.get_units(1)*hieuchinh-zero;
          if (cannang > g_SET){ digitalWrite(ROLE,HIGH); }
          else {digitalWrite(ROLE,LOW);}
        }
        if(mainmode==1)  // set gia tri mong muon
        {     
          up_down();        
          chop_caidat();  
        }
        if(mainmode==2) //luu gia tri muon cai dat
        {    
         lcd.setCursor(0,0);lcd.print("LUU GIA TRI     ");
         lcd.setCursor(0,1);lcd.print("         CAI DAT");
          delay(1000); lcd.clear();mainmode=0; 
        }

}   

void can()
{ 
  lcd.setCursor(0, 0);  
  lcd.print("  CAN NANG  ");
  lcd.setCursor(13, 1);  
  lcd.print("Gam");  
  lcd.setCursor(4, 1);  
  lcd.print(scale.get_units(1)*hieuchinh-zero);       
}


void up_down()
{
a =digitalRead(clk);
if(a != A)
    {if(digitalRead(data) != a )
      {
        g_SET = g_SET + 1 ;     // chinh len
        if(g_SET > 10000) g_SET = 10000;  
        lcd.setCursor(8,1);
        lcd.print("      "); 
        lcd.setCursor(8,1);
        lcd.print(g_SET); 
        EEPROM.write(2,g_SET);delay(10);
      }
    else {
      g_SET = g_SET - 1; // chinh xuong
      if(g_SET < 0) g_SET = 0;
      lcd.setCursor(8,1);
      lcd.print("      "); 
      lcd.setCursor(8,1);
      lcd.print(g_SET);      
      EEPROM.write(2,g_SET);delay(10);
      }
    }
A=a;
}

void chop_caidat()
{
  lcd.setCursor(0,0);lcd.print("SET GIA TRI:         ");
  lcd.setCursor(8,1);lcd.print(g_SET); lcd.setCursor(13,1);  lcd.print("Gam"); lcd.print("    "); 
}

void pip_1()
{
  digitalWrite(LOA,HIGH);
  delay(50);
  digitalWrite(LOA,LOW);
  delay(50);
}
