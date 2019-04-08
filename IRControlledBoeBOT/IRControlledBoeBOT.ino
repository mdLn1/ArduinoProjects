#include <Servo.h> 
#include <IRremote.h>
/* decimal values for remote control buttons
*  "+" button 2080 or 32 --> move forward
* "-" button 2081 or 33 --> move backward
* "on/off" button 12 or 2060 --> perform/detach servos
* "turn down the volume" button 17 or 2065 --> turn right
* "turn up the volume" button 16 or 2064 --> turn left
* "i" button 15 or 2063 --> horn
* "back up" button 34 or 2082 --> attach sevos
*/
Servo servoLeft;
Servo servoRight;


//using aliases for IRreceiver, buzzer and ultrasonic sensor 
const byte receiver = 6;
const byte buzzer = 2; const byte ping = 4;

IRrecv irrecv(receiver);
decode_results results;


void setup() 
{ 
  //setting the LEDs and buzzer for output 
  pinMode(buzzer, OUTPUT);
  pinMode(7, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT); 
  irrecv.enableIRIn();
}


void loop()
{
  //check for command code
  if (irrecv.decode(&results))
  { 
    // checks for a particular type of command coding RC5 
    if (results.decode_type == RC5)
    { 
        // if ... else if are looking for specific codes that can be found at the
        // beginining of the program together with the buttons that should pe pressed
        int instruction = results.value;
        // move forward, acceleration effect
        if((instruction == 32) || (instruction == 2080))
        {
          digitalWrite(9,HIGH);
          rampUp();
          forward(10);
          rampDown();
          digitalWrite(9,LOW);
          stopServos();
        }
        // move backwards
        else if((instruction == 33) || (instruction == 2081))
        {
          digitalWrite(11,HIGH);
          backward(10);
          digitalWrite(11,LOW);
          stopServos();
        }
        //turn left
        else if((instruction == 17) || (instruction == 2065))
        {
          digitalWrite(7,HIGH); 
          playTone(buzzer, 1000, 700); 
          left(10); digitalWrite(7,LOW); 
          stopServos();
        }
        //turn right
        else if((instruction == 16) || (instruction == 2064))
        {
          digitalWrite(10,HIGH); 
          playTone(buzzer, 1000, 700);
          right(10); digitalWrite(10,LOW);
          stopServos();
        }
        else if((instruction == 15) || (instruction == 2063))
        {
          playTone(buzzer, 350, 400); 
          delay(200); 
          playTone(buzzer, 350, 500);
        }
        else if(instruction == 2060)
        {
          shutServos();
        }
        else if(instruction == 12)
        {
          attachServos();
        }
        else if((instruction == 34) || (instruction == 2082))
        {
          attachServos();
          showOff();
        }
        irrecv.enableIRIn(); // Receive the next value
     }
   }
}


//function that allows the buzzer to be used similar to 'tone' function
void playTone(byte outputPIN,int frequency, int duration)
{
  int period = 1000000L / frequency; int pulse = period/ 2;
  for (long i = 0; i < duration * 1000L; i += period)
  {
    digitalWrite(outputPIN,HIGH); 
    delayMicroseconds(pulse);
    digitalWrite(outputPIN, LOW); 
    delayMicroseconds(pulse);
  }
}


//function that sets the servos still 
void stopServos() 
{
  servoLeft.writeMicroseconds(1500); 
  servoRight.writeMicroseconds(1500);
}


//function that increases the speed from still to maximum 
void rampUp()
{
  for(int speed = 0; speed<=200; speed+=4)
  {
    servoLeft.writeMicroseconds(1500-speed);
    servoRight.writeMicroseconds(1500+speed); 
    delay(10);
  }
}

//function that decreases the speed from maximum to still 
void rampDown()
{
  for(int speed = 0; speed<=200; speed+=10)
  {
    servoLeft.writeMicroseconds(1300+speed); 
    servoRight.writeMicroseconds(1700-speed); 
    delay(10);
  }
}


//function that turn the robot to the left 
void left(int delayValue)
{
  irrecv.resume(); servoLeft.writeMicroseconds(1300); servoRight.writeMicroseconds(1300); 
  for(int i = 0; i < 14; i++)
  {
    if(irrecv.decode(&results))
    {
      if((results.value == 17) || (results.value == 2065))
      {
        left(delayValue);
      }
      else if(results.value == 2060)
      {
        shutServos();
      }
    }
    else
      delay(delayValue);
   }
}


//function that turn the robot to the right 
void right(int delayValue)
{
  irrecv.resume();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);
  for(int i = 0; i < 14; i++)
  {
    if(irrecv.decode(&results))
    {
      if((results.value == 16) || (results.value == 2064 ))
      {
        right(delayValue);
      }
      else if(results.value == 2060)
      {
        shutServos();
      }
    }
    else
      delay(delayValue);
  }
}


//function that makes the robot move backward
void backward(int delayValue)
{
  irrecv.resume();
  servoLeft.writeMicroseconds(1700); 
  servoRight.writeMicroseconds(1300);
  for(int i = 0; i < 14; i++)
  {
    if(irrecv.decode(&results))
    {
      if((results.value == 33) || (results.value == 2081))
      {
        backward(delayValue);
      }
      else if(results.value == 2060)
      {
        shutServos();
      }
    }
    else
      delay(delayValue);
  }
}


//function that makes the robot move forward and
//prevents it from bumping into other objects 
void forward(int delayValue)
{
  irrecv.resume(); 
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700); 
  for(int i = 0; i < 14; i++)
  {
    long distance = testForwardDistance(); if(distance < 15)
    if(distance < 15)
    {
      shutServos();
      i = 14;
    }
    else
    {
      if(irrecv.decode(&results))
      {
        if((results.value == 32) || (results.value == 2080))
        {
          forward(delayValue);
        }
        else if(results.value == 2060)
        {
          shutServos();
        }
      }
      else
        delay(delayValue);
    }
  }
}


//function that will show what the robot can do 
void showOff()
{
  digitalWrite(7,HIGH); 
  digitalWrite(9,HIGH); 
  digitalWrite(10,HIGH); 
  digitalWrite(11,HIGH); 
  playTone(buzzer, 500, 500);
  delay(200);
  playTone(buzzer, 1000, 500);
  delay(200);
  playTone(buzzer, 1500, 500); 
  digitalWrite(7,LOW);
  digitalWrite(9,LOW); 
  digitalWrite(10,LOW); 
  digitalWrite(11,LOW); 
  delay(200);
  digitalWrite(10,HIGH); 
  right(100); 
  digitalWrite(10,LOW); 
  digitalWrite(7,HIGH); 
  left(100); 
  digitalWrite(7,LOW); 
  digitalWrite(9,HIGH); 
  forward(100); 
  digitalWrite(9,LOW); 
  digitalWrite(11,HIGH);
  backward(100);
  digitalWrite(11,LOW); 
  stopServos();
}


//function that gives a warning that the servos are detached 
void shutServos()
{
  servoLeft.detach(); 
  servoRight.detach(); 
  digitalWrite(11, HIGH);
  digitalWrite(9, HIGH);
  delay(400); 
  digitalWrite(11, LOW); 
  digitalWrite(9, LOW); 
  digitalWrite(10, HIGH);
  digitalWrite(7, HIGH); 
  delay(400);
  digitalWrite(10, LOW); 
  digitalWrite(7, LOW);
}


//function thst returns distance in centimeters
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}


//function that gives a warning that the servos are attached 
void attachServos()
{
  digitalWrite(11, HIGH); 
  delay(200); 
  digitalWrite(10, HIGH); 
  delay(200); 
  digitalWrite(9, HIGH); 
  delay(200); 
  digitalWrite(7, HIGH); 
  delay(200); digitalWrite(11, LOW); 
  digitalWrite(10, LOW); 
  digitalWrite(9, LOW); 
  digitalWrite(7, LOW); 
  servoLeft.attach(13); 
  servoRight.attach(12);
}


//function that tests the distance between the robot
//and the nearest object in front of it 
long testForwardDistance()
{
  long duration, cm; 
  pinMode(ping, OUTPUT); 
  digitalWrite(ping, LOW); 
  delayMicroseconds(2); 
  digitalWrite(ping, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(ping, LOW);
  pinMode(ping, INPUT);
  duration = pulseIn(ping, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}
