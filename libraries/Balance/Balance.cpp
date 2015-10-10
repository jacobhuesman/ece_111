/***************************************************************************
  This is a library for the NRMC balance experiment.

  Written by Jacob Huesman for Bison Robotics.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
 
 #include "Balance.h"
 
 /* Constructs a refresh object with a rate given in Hz */
Refresh::Refresh( long rate ) {
  delay = 1000000 / rate;
  curTime = micros();
  lastTime = micros();
}

/* Updates delay */
void Refresh::delayMicro( long time ) {
  delay = time;
}

void Refresh::restart( void ) {
  lastTime = micros();
}

/* Returns true if it's time to fresh the IMU readings */
bool Refresh::cont( void ) {
  curTime = micros();
  if( (lastTime + delay) <= curTime ) {
    lastTime = curTime;
    return true;
  } 
  return false;
}
