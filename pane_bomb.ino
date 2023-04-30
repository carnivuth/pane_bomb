// https://www.tinkercad.com/things/aZbi3OP3wA3-ingenious-jofo-crift/editel?sharecode=zm6AWrX5KtYEYwpjToYd4LguY1Vzc-SiAHwUOB98VHM
/////////////////////////////
//      Pane Bomb       //
/////////////////////////////

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Tone.h>
#define pound 14
Tone tone1;

int Scount = 12; // count seconds
int Mcount = 10; // count minutes
int Hcount = 0; // count hours
int DefuseTimer = 0; // set timer to 0
//admin stuff
int adminpwdl=5;
char adminpwd[5]="12345";


long secMillis = 0; // store last time for second add
long interval = 1000; // interval for seconds
/// bomb password stuff
const int bombpwdsize=7;
char password[bombpwdsize]; // number of characters in our password
int currentLength = 0; //defines which number we are currently writing
int i = 0; 
char entered[bombpwdsize];

int ledPin = 4; //red led
int ledPin2 = 3; //yellow led
int ledPin3 = 2; //green led

LiquidCrystal lcd(7,8,10,11,12,13); // the pins we use on the LCD

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9' ,'C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, 5, A1, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A4, A5, 6};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void clearDisplay(){
  lcd.clear();
  lcd.setCursor(0,0);
}
void getFromKeyPad(int size,char * result){
  int cursize=0;
  while (cursize< size){
    char key2 = keypad.getKey();
    if (key2 != NO_KEY)    
            { 
              lcd.setCursor(cursize + 7, 1);
              lcd.cursor();          
              lcd.print(key2);
              result[cursize] = key2;
              cursize++;
              tone1.play(NOTE_C6, 200);
              delay(100);
              lcd.print('*');
              lcd.noCursor();
              lcd.setCursor(cursize + 6, 1);
              lcd.setCursor(cursize + 7, 1);
              lcd.cursor();
            }
  }
}
void getBombPwd(){
  int lenght=0;
   lcd.print("Enter Code: ");
  while (lenght < bombpwdsize)
  {
    lcd.setCursor(lenght + 6, 1);
    lcd.cursor();
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      if ((key != '*')&&(key != '#'))
      { 
      lcd.print(key);
      password[lenght] = key;
      lenght++;
      tone1.play(NOTE_C6, 200);
      }
    }
  }

  if (lenght == bombpwdsize)
  {
    delay(500);
    lcd.noCursor();
    lcd.clear();
    lcd.home();
    lcd.print("You've Entered: ");
    lcd.setCursor(6,1);
    //print password
    for(int j=0;j<bombpwdsize; j++ ){
              lcd.print(password[j]);
    }
    tone1.play(NOTE_E6, 200);
    delay(3000);
    lcd.clear();
    lenght = 0;
  }

}

void setup(){
  pinMode(ledPin, OUTPUT); // sets the digital pin as output
  pinMode(ledPin2, OUTPUT); // sets the digital pin as output
  pinMode(ledPin3, OUTPUT); // sets the digital pin as output
  tone1.begin(9);
  lcd.begin(16, 2);
  Serial.begin(9600);
  ///request admin pwd
  char input[adminpwdl];
  lcd.print("ho il cazzone");
  delay(3000);
  clearDisplay();

  do{

    clearDisplay();
    lcd.print("Enter admin pwd: ");
    getFromKeyPad(adminpwdl,input);

  }while(input[0] != adminpwd[0] 
  || input[1] != adminpwd[1] 
  || input[2] != adminpwd[2] 
  || input[3] != adminpwd[3] 
  || input[4] != adminpwd[4]);
  clearDisplay();
  char time[4];
  getFromKeyPad(3,time);
  time[4]='\0';
  Mcount=atoi(time);
  getBombPwd();
}

  



 

void loop()
{
  timer();
  char key2 = keypad.getKey(); // get the key
  
  if (key2 == '*')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Code: ");
      
      while (currentLength < bombpwdsize)
        {
          
          timer();
          
          char key2 = keypad.getKey(); 
          if (key2 == '#')
            {
              currentLength = 0;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Code: ");
            }
          else                  
          if (key2 != NO_KEY)    
            {
              
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
              
              lcd.print(key2);
              entered[currentLength] = key2;
              currentLength++;
              tone1.play(NOTE_C6, 200);
              delay(100);
              lcd.noCursor();
              lcd.setCursor(currentLength + 6, 0);
              lcd.print("*");
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
            }
        }

      if (currentLength == bombpwdsize) 
        {
          int flag=1;
          for(int j=0;j<bombpwdsize;j++){
            if(entered[j] != password[j]) flag=0;
          }
          if (flag==1)
            {
              lcd.noCursor();
              lcd.clear();
              lcd.home();
              lcd.print("Bomb Defused");
              currentLength = 0;
              digitalWrite(ledPin3, HIGH);
              delay(2500);
              lcd.setCursor(0,1);
              lcd.print("Reset the Bomb");
              delay(1000000);
            }
      else
        {
          lcd.noCursor();
          lcd.clear();
          lcd.home();
          lcd.print("Wrong Password!");
    
          if (Hcount > 0)
            {
              Hcount = Hcount - 1;
            }
    
          if (Mcount > 0)
            {
              Mcount = Mcount - 59;
            }
          if (Scount > 0)
            {
              Scount = Scount - 59;
            }
        delay(1500);
        currentLength = 0;
  
        }
      }  
    }
}

void timer()
{
  Serial.print(Scount);
  Serial.println();
  
  if (Hcount <= 0)
  {
    if ( Mcount < 0 )
    {
      lcd.noCursor();
      lcd.clear();
      lcd.home();
      lcd.print("The Bomb Has ");
      lcd.setCursor (0,1);
      lcd.print("Exploded!");
      
      while (Mcount < 0) 
      {
        digitalWrite(ledPin, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
      }
    } 
  }

  lcd.setCursor (0,1); // sets cursor to 2nd line
  lcd.print ("Timer:");

  if (Hcount >= 10)
    {
      lcd.setCursor (7,1);
      lcd.print (Hcount);
    }
  if (Hcount < 10) 
    {
      lcd.setCursor (7,1);
      lcd.write ("0");
      lcd.setCursor (8,1);
      lcd.print (Hcount);
    }

  lcd.print (":");

  if (Mcount >= 10)
    {
      lcd.setCursor (10,1);
      lcd.print (Mcount);
    }
  if (Mcount < 10) 
    {
      lcd.setCursor (10,1);
      lcd.write ("0");
      lcd.setCursor (11,1);
      lcd.print (Mcount);
    }
    
  lcd.print (":");

  if (Scount >= 10) 
    {
      lcd.setCursor (13,1);
      lcd.print (Scount);
    }
  if (Scount < 10) 
    {
      lcd.setCursor (13,1);
      lcd.write ("0");
      lcd.setCursor (14,1);
      lcd.print (Scount);
    }

  if (Hcount <0) 
    {
      Hcount = 0; 
    }

  if (Mcount <0) 
    {
      Hcount --; 
      Mcount = 59; 
    }

  if (Scount <1) // if 60 do this operation
    {
      Mcount --; // add 1 to Mcount
      Scount = 59; // reset Scount
    }

  if (Scount > 0) // do this oper. 59 times
    {
      unsigned long currentMillis = millis();
  
      if(currentMillis - secMillis > interval) 
        {
          tone1.play(NOTE_G5, 200);
          secMillis = currentMillis;
          Scount --; // add 1 to Scount
          digitalWrite(ledPin2, HIGH); // sets the LED on
          delay(10); // waits for a second
          digitalWrite(ledPin2, LOW); // sets the LED off
          delay(10); // waits for a second
          //lcd.clear();
        }
    }
}

