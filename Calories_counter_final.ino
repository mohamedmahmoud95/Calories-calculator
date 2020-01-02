
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define buzzer 10
#define menu 5
#define plus10 6
#define plus100 7
#define refresh 8
#define settings 9
#define gnd 13
int p10s  , p10ls , p10counter=0 ;
int p100s, p100ls , p100counter=0;  
int refreshs, refreshls, refreshcounter=0;
int settingss , settingsls , settingscounter=0;
int menus , menuls , menucounter=0;

int calcounter=0 , calmax=1500;
void setup()
{
  
  pinMode(buzzer , OUTPUT);
  pinMode(gnd,OUTPUT);
  digitalWrite(13,LOW);
  pinMode(plus10 , INPUT_PULLUP);
  pinMode(plus100, INPUT_PULLUP);
  pinMode(refresh, INPUT_PULLUP);
  pinMode(settings, INPUT_PULLUP);

  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Good Morning ^_^");
  delay(1000);
  clean();  
  pinMode(4,OUTPUT);
}

void loop()
{
  
  mains();
  addcal();
  cal_check();
  
  /*
  if (menucounter == 1)
  {
    mains();
    delay(100);
  }
  if (menucounter == 2)
  {
    addcal();
    delay(100);
  }  
  
*/
p10s=digitalRead(plus10);
p100s=digitalRead(plus100);
refreshs=digitalRead(refresh);
settingss=digitalRead(settings);
menus=digitalRead(menu);


/*
Serial.print("plus10state = ");
Serial.print(p10s);
Serial.print("    plus100state = ");
Serial.print(p100s);
Serial.print("    refreh state = ");
Serial.print(refreshs);
Serial.print("    settings state = ");
Serial.print(settingss);
Serial.print("    menu state = ");
Serial.println(menus);
delay(200);*/
/////////////////////////////////


//buttons counters
///plus 10
if(p10s!= p10ls)
{
  if(p10s==LOW)
  {
    p10counter+=10;
    calcounter+=10;
  }
  else
  p10counter=p10counter;
}
p10ls=p10s;


///////////////
///plus 100

  if(p100s!= p100ls)
{
  if(p100s==LOW)
  {
    p100counter+=100;
        calcounter+=100;

  }
  else
  p100counter=p100counter;
}
p100ls=p100s;

/*
/////////////
//refresh
if(refreshs!= refreshls)
{
  if(refreshs==LOW)
  {
    refreshcounter++;
  }
  else
  refreshcounter=refreshcounter;
}
refreshls=refreshs;

if (refreshcounter>=2)
refreshc();*/

///////////////////
//settings:

if(settingss!= settingsls)
 {
  if(settingss==LOW)
  {
    settingscounter++;
  }
  else
  settingscounter=settingscounter;
 }
settingsls=settingss;
if (settingscounter%2 ==0)
editMax();
//////////////////////////


///////////////////////////
///menu

if(menus!= menuls)
 {
  if(menus==LOW)
  {
    menucounter++;
  }
  else
  menucounter=menucounter;
 }
menuls=menus;

/*
Serial.print("plus10 = ");
Serial.print(p10counter);
//
Serial.print("    plus100 = ");
Serial.print(p100counter);
//
Serial.print("   refresh = ");
Serial.print(refreshcounter);
//
Serial.print("   settings: = ");
Serial.println(settingscounter);4
*/
}
/////////////////////
///clean lcd
void clean()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");  
}




////////
void addcal()
{
  calcounter = p10counter + p100counter;
}


////////////////////////////////
//edit Max

void editMax()
{
    clean();
    lcd.setCursor(0,0);
    lcd.print("set MAX cal/day");    
    lcd.setCursor(0,1);
    lcd.print(calmax);

    
    if(p10s!= p10ls)
    {
      if(p10s==LOW)
      {
        calmax+=10;
      }
      else
          calmax= calmax;
    }
    p10ls=p10s;
    
    
      if(p100s!= p100ls)
    {
      if(p100s==LOW)
      {
            calmax+=100 ; }
      else
          calmax=calmax;
    }
    p100ls=p100s;
    



        if(settingss!= settingsls)
         {
          if(settingss==LOW)
          {
              clean();
              lcd.setCursor(0,0);
              lcd.print("saving settings..");
              delay(1000);
           
              lcd.setCursor(0,1);
              lcd.print("ready to go..");
              delay(750);
              clean();
              mains();
             }
}
}

void mains()
{
 
  lcd.setCursor(0,0);
  lcd.print("Cal counter:");

  if(calcounter<10)
  {lcd.setCursor(12,0);
  lcd.print("  ");
  lcd.setCursor(14,0);
  lcd.print(calcounter);
  }
  else
  {
  lcd.setCursor(12,0);
  lcd.print(calcounter);
  }


  lcd.setCursor(0,1);
  lcd.print("remaning:");
  if ((calmax-calcounter)<1000)
  {
  lcd.setCursor(11,1);
  lcd.print(" ");
  lcd.setCursor(12,1);
  lcd.print(calmax-calcounter);
  }
  else
  {
  lcd.setCursor(11,1);
  lcd.print(calmax-calcounter);
  }
}

void cal_check()
{
    if ((calmax-calcounter)>50)
            mains();
    else if ((calmax-calcounter)<=50 && (calmax-calcounter)>0)
      {  
         
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Caution!!");
  alarm();
  }

      else if ((calmax-calcounter)<0)
      {  
         
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("limit exceeded!!");
  alarm();
  }
}



void alarm()
{
  
if ((calmax-calcounter)>50)
noTone(buzzer);

    if ((calmax-calcounter)<=50 && (calmax-calcounter)>0)
      {  
         
tone(buzzer,250);
delay(500);
noTone(buzzer);
delay(500);
tone(buzzer,250);
delay(500);
noTone(buzzer);
delay(500);
tone(buzzer,250);
delay(500);
noTone(buzzer);
delay(500);

delay(120000);
cal_check();

  }

      else if ((calmax-calcounter)<0)
      {  
tone(buzzer,300);
delay(700);
noTone(buzzer);
delay(700);

tone(buzzer,300);
delay(700);
noTone(buzzer);
delay(700);


tone(buzzer,300);
delay(700);
noTone(buzzer);
delay(700);

tone(buzzer,300);
delay(700);
noTone(buzzer);
delay(700);
delay(120000);

cal_check();


}
}

void refreshc()
{
    calcounter=0; 
    clean();
    lcd.setCursor(0,0);
    lcd.print("It's a new day^^");
    delay(1500);
    clean();
    delay(50);
    //
    lcd.setCursor(0,0);
    lcd.print("lets lose fats");
    delay(1500);
    clean();
    mains();
}
