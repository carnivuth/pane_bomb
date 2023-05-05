// https://www.tinkercad.com/things/aZbi3OP3wA3-ingenious-jofo-crift/editel?sharecode=zm6AWrX5KtYEYwpjToYd4LguY1Vzc-SiAHwUOB98VHM
/////////////////////////////
//      Pane Bomb       //
/////////////////////////////

#include <Keypad.h>
#include <Tone.h>
#define pound 14
//buzzer
Tone tone1;

//counters
int Scount = 0; 
int Mcount = 15; 
int Hcount = 0; 

int DefuseTimer = 0; 

long secMillis = 0; 
long interval = 1000;

/// bomb parameters
const int bombpwdsize=8;
char password[bombpwdsize]="1234567"; 
int currentLength = 0;
int i = 0; 
char entered[bombpwdsize];

//LEDs
int ledPin = 4; 
int ledPin2 = 3; 
int ledPin3 = 2;


//KEYPAD
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9' ,'C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, 5, A1, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A4, A5, 6};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//string comparison
int checkString(char * str1, char * str2, int dim){
  int result=1;
  for (int i=0; i<dim; i++){
    if(str1[i]!=str2[i])result=0;
  }
    return result;

}

//get string from keypad
void getFromKeyPad(int size,char * result, char * text,int mask){
  int cursize=0;
  while (cursize< size){
    //get keypress
    char key = keypad.getKey();
    if (key != NO_KEY){ 
      //check  for valid character
      if(key!= '*' && key!= '#'){
         
         result[cursize] = key;
         cursize++;
         tone1.play(NOTE_C6, 200);
      }
      //reset buffer
      if(key == '#'){
        cursize=0;

      }
             
    }
  }
}
// get bomb code

void setup(){

  //LED setup
  pinMode(ledPin, OUTPUT); 
  pinMode(ledPin2, OUTPUT); 
  pinMode(ledPin3, OUTPUT); 

  //buzzer setup
  tone1.begin(9);

  //Serial setup
  Serial.begin(9600);

  //show output display
  delay(3000);
  
  //activate bomb
  char activationCode[bombpwdsize];
  do{
    getFromKeyPad(bombpwdsize,activationCode,"Activate: ",1);
  }while(checkString(activationCode,password,bombpwdsize)==0);
  
}


  



 

void loop()
{
  timer();
  char key2 = keypad.getKey(); // get the key
  
  if (key2 == '*')
    {
     
      
      while (currentLength < bombpwdsize)
        {
          
          timer();
          
          char key2 = keypad.getKey(); 
          if (key2 == '#')
            {
              currentLength = 0;
           
            }
          else                  
          if (key2 != NO_KEY)    
            {
              
        
              
              entered[currentLength] = key2;
              currentLength++;
              tone1.play(NOTE_C6, 200);
              delay(100);
       
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
              ////BOMB DEFUSED

              currentLength = 0;
              digitalWrite(ledPin3, HIGH);
              delay(2500);
             
              delay(1000000);
            }
      else
        {
    
    
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


  if (Hcount >= 10)
    {

    }
  if (Hcount < 10) 
    {
 
    }


  if (Mcount >= 10)
    {
   
    }
  if (Mcount < 10) 
    {
   
    }
    

  if (Scount >= 10) 
    {
     
    }
  if (Scount < 10) 
    {

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
        }
    }
}

