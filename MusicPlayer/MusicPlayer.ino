
#include <LiquidCrystal.h> 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
const int debounceDelay = 20; 
byte songNumber = 0; 

//using aliases for switch and buzzer 
const int switchButton = 6; 
const int buzzer = 8; 

//describing notes for songs 
char notes[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g' }; 

//frequency used to play songs 
unsigned int fHz[] = {262, 294, 330, 349, 392, 440, 494}; 
const byte notesLength = sizeof(notes); 

//songs that I decided to implement 
char littleTeapot[] = "fedcbgagbcceddeefffedcbgagbbgbbcdef"; 
char earsHangLow[] = "gfeeeaabcbabefgggggefgffffgfeeeeeebaabcbabbef g f e"; 
char alouette[] = "cdeedcdecacdeedcdeccbcbceeggggfedcgggaaag"; 

//length of the songs I chose 
const byte lTeapotLength = sizeof(littleTeapot); 
const byte eHangLowLength = sizeof(earsHangLow); 
const byte alouetteLength = sizeof(alouette);
 
//function that displays the start of the program 
void getStarted()
{ 
  lcd.setCursor(0,0); 
  lcd.print("Press the button"); 
  lcd.setCursor(0,1); 
  lcd.print(" to get started"); 
}

void setup() 
{ 
  
  //initiating lcd screen 
  lcd.begin(16,2); 
  getStarted(); 
  pinMode(switchButton, INPUT); 
}

void loop() 
{

  //condition that checks if the button was pushed 
  if( digitalRead(switchButton) == HIGH) 
  { 
    songNumber++; 
    
    //lcd shows the name of the first song 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print(" Playing I'm a "); 
    lcd.setCursor(0,1); 
    lcd.print(" little teapot "); 
    int duration = 333; 
    
    //this loop allows first song to be played 
    for(int i = 0; i<lTeapotLength; i++) 
    { 
      playNote(littleTeapot[i],duration); 
      
      //if button pressed again it moves to the next song 
      if(digitalRead(switchButton) == HIGH) 
      { 
        playSong2();
        i=lTeapotLength; 
      } 
    } 
    if(songNumber == 1) 
    { 
      next(); 
      alert();
       
      //loop used as delay to give 10 seconds to the user to play the next song 
      for(int i = 0; i < 100; i++) 
      { 
        delay(100); 
        if (digitalRead(switchButton) == HIGH) 
        { 
          playSong2(); 
          i = 100; 
        } 
      } 
    } 
    else if(songNumber > 1) 
    { 
      getStarted(); 
      songNumber = 0; 
    } 
  } 
}

//function that plays a note
//code similar to Margolis, M., 
//2011. Arduino Cookbook. 2nd ed. 1005 Gravenstein Highway North,
//Sebastopol: O'Reilly Media.
void playNote(char note, int duration) 
{ 
  for (int i = 0; i<notesLength; i++)
  { 
    if(note == notes[i]) 
      tone(buzzer,fHz[i],duration); 
  } 
  
  delay(duration); 
} 

//function that plays second song in the list 
void playSong2() 
{ 
  int val = LOW; 
  songNumber++; 
  
  //the name of the second song is displayed 
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("Playing Alouette"); 
  int duration = 400; 
  
  //loop that makes the buzzer play second song 
  for(int i = 0; i < alouetteLength; i++) 
  { 
    playNote(alouette[i], duration); 
    
    //if button pressed again next song would be played 
    if(digitalRead(switchButton) == HIGH) 
    { 
      val = digitalRead(switchButton); 
      playSong3(); 
      i = alouetteLength; 
    } 
  } 
  
  if(val == LOW) 
  { 
    next ;
    alert(); 
    //delay to let the user decide whether to play the third song or not 
    for(int i = 0; i < 100; i++) 
    { 
      delay(100); 
      if (digitalRead(switchButton) == HIGH) 
      { 
        playSong3(); 
        i = 100; 
      } 
    } 
  } 
} 

//function that plays third song in the list 
void playSong3() 
{ 
  songNumber++;  
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print(" Playing Do your "); 
  lcd.setCursor(0,1); 
  lcd.print("ears hang low ?"); 
  int duration = 333; 
  int i = 0; 
  do
  { 
    playNote(earsHangLow[i], duration); 
    i++; 
    if(digitalRead(switchButton)==HIGH)
    i = eHangLowLength; 
  } 
  while(i < eHangLowLength); 
  
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print(" No more songs "); 
  lcd.setCursor(2,1); 
  lcd.print("left to play"); 
  delay(6000); 
  getStarted(); 
} 

//function that display a warning 
void alert() 
{ 
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("You have 10 secs"); 
  lcd.setCursor(0,1); 
  lcd.print("Or program resets"); 
} 

//function that asks the user to press button for next song 
void next() 
{ 
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print(" Press the button"); 
  lcd.setCursor(0,1); 
  lcd.print("to play the next song "); 
  for(int i = 0; i < 6; i++) 
  {
    delay(500); 
    lcd.scrollDisplayLeft(); 
  } 
}
