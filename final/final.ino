/**************************************************************************
  Aruino Accelerometer Mouse
  
  Written by Ben Gerber, Jacob Huesman, and David Jedynak 
**************************************************************************/

/**************************************************************************
  Include Libraries
  - Balance.h - Custom classes developed for this project
**************************************************************************/

#include <Balance.h>

/**************************************************************************
  Define global objects and variables
**************************************************************************/

/* Sensitivity */
int sens = 40;

/* Pins for Arduino */
const int xPin = 0;
const int yPin = 1;
const int zPin = 2;

/* Angle Variables */
int xAng, yAng, zAng;

/*Analog Read Variables */
int xRead,yRead,zRead;

/* Mouse variables */
int xMove, yMove,xZero1,yZero1;

/* Button Press - Left/Right Button */
int buttonPinL = 5;
int buttonPinR = 4;
int buttonPinZ = 6;

/* Button Press Debounce Booleans */
/* Left Debounce */
boolean currentStateL = LOW;                                        //stroage for current measured button state
boolean lastStateL = LOW;                                           //storage for last measured button state
boolean debouncedStateL = LOW;                                      //debounced button state
/* Right Debouce */
boolean currentStateR = LOW;                                        //stroage for current measured button state
boolean lastStateR = LOW;                                           //storage for last measured button state
boolean debouncedStateR = LOW;                                      //debounced button state
/* Zero Debounce */
boolean currentStateZ = LOW;                                        //stroage for current measured button state
boolean lastStateZ = LOW;                                           //storage for last measured button state
boolean debouncedStateZ = HIGH;                                      //debounced button state

/* Debouncing intervals */
int debounceInterval = 20;                                          //wait 20 ms for button pin to settle
unsigned long timeOfLastButtonEventL = 0;                           //store the last time the button state changed
unsigned long timeOfLastButtonEventR = 0;
unsigned long timeOfLastButtonEventZ = 0;

/* Zero Value Defaults */
double xZero = 0;
double yZero = 0;

//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
int minVal = 265;
int maxVal = 402;

//to hold the caculated values
double x;
double y;

double zX;
double zY;

void zero(){
  zX = x;
  zY = y;
}

int difference(int a, int b){ 
  return a - b;
}


/* Declare Refresh Objects */
Refresh ref50( 50 );
Refresh ref1( 1 );

/* Accelerometer Data */
float AccelX;
float AccelY;
float AccelZ;


/* Test Code */
int count = 0;

/**************************************************************************
  Prepare environment
**************************************************************************/

void setup() 
{
  /**************************************************************************
    Initialize serial
  **************************************************************************/
  Serial.begin(115200);
  Serial.println("Starting up!");
  /* Defining pin inputs */
  pinMode(buttonPinL, INPUT);
  pinMode(buttonPinR, INPUT); 
  pinMode(buttonPinZ, INPUT);
}

/**************************************************************************
  Main program execution
**************************************************************************/
void loop(void) 
{
  /**************************************************************************
    Code to run at 50 Hz.
  **************************************************************************/
  if(ref50.cont()) {
      /**************************************************************************
        Accelerometer Code
      **************************************************************************/
      // Read the analog values from the accelerometer
      xRead = analogRead(xPin);
      yRead = analogRead(yPin);
      zRead = analogRead(zPin);
        
        /* Zeroing Function */
        currentStateZ = digitalRead(buttonPinZ);
        unsigned long currentTimeZ = millis();
  
  
        if (currentStateZ != lastStateZ){
          timeOfLastButtonEventZ = currentTimeZ;
        }
  
        if (currentTimeZ - timeOfLastButtonEventZ > debounceInterval){//if enough time has passed
          if (currentStateZ != debouncedStateZ){//if the current state is still different than our last stored debounced state
            debouncedStateZ = currentStateZ;//update the debounced state
          }
        }
  
        lastStateZ = currentStateZ;\
        
      /* Zeroing Function */  
      if(debouncedStateZ == HIGH)
        {
          xZero = 335 - xRead;
          yZero = 335 - yRead;
          debouncedStateZ = LOW;
        }
        
      xRead = xRead + xZero;
      yRead = yRead + yZero;
      
      // Convert read values to degrees -90 to 90 - Needed for atan2
      xAng = map(xRead, minVal, maxVal, -90, 90);
      yAng = -map(yRead, minVal, maxVal, -90, 90);
      zAng = -map(zRead, minVal, maxVal, -90, 90);
      
      
      
      // Calculate 360deg values like so: atan2(-yAng, -zAng)
      // atan2 outputs the value of -Ï€ to Ï€ (radians)
      // We are then converting the radians to degrees
      x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
      y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
      
      
      /**************************************************************************
        Mouse Code
      **************************************************************************/
      /* Check if mouse is on */
      if(digitalRead(7)){
         /*
       if(x > 180)
          x = abs(-x + 180);
        if(y > 180)
          y = abs(-y + 180);
          */ 
      /* X and Y mapping */
          xMove = map(x,0,360,-sens,sens);
          yMove = map(y,0,360,-sens,sens);


      
      // Output the calculations
      Serial.print("x: ");
      Serial.print(x);
      Serial.print(" | y: ");
      Serial.println(y);
      //Serial.print(" | z: ");
      //Serial.println(zRead);  
        
      /* Move cursor */
      if(xMove != 0 || yMove != 0)
        Mouse.move(yMove,xMove);
        /*Serial.print("y values: ");
        Serial.print(yMove);
        Serial.print(" X values: ");
        Serial.println(xMove);*/
        
        currentStateL = digitalRead(buttonPinL);
        unsigned long currentTimeL = millis();
  
  
        if (currentStateL != lastStateL){
          timeOfLastButtonEventL = currentTimeL;
        }
  
        if (currentTimeL - timeOfLastButtonEventL > debounceInterval){//if enough time has passed
          if (currentStateL != debouncedStateL){//if the current state is still different than our last stored debounced state
            debouncedStateL = currentStateL;//update the debounced state
      
          }
        }
  
        lastStateL = currentStateL;
        
        /* Right Mouse Button Click */
        currentStateR = digitalRead(buttonPinR);
        unsigned long currentTimeR = millis();
  
  
        if (currentStateR != lastStateR){
          timeOfLastButtonEventR = currentTimeR;
        }
  
        if (currentTimeR - timeOfLastButtonEventR > debounceInterval){//if enough time has passed
          if (currentStateR != debouncedStateR){//if the current state is still different than our last stored debounced state
            debouncedStateR = currentStateR;//update the debounced state
      
          }
        }
  
        lastStateR = currentStateR;
        
        if(debouncedStateL == HIGH)
          Mouse.press();
        else
          Mouse.release();
        
        if(debouncedStateR == HIGH)
          Mouse.press(MOUSE_RIGHT);
        else
          Mouse.release(MOUSE_RIGHT);
         
      }
      
      
    /**************************************************************************
      This code runs every second
      - Note: Above code has priority.
    **************************************************************************/
    if( ref1.cont() ) {
      
    }
    
    
  }
  /**************************************************************************
    This code runs every loop
  ***************************************************************************/
}

