/***************************************************************************
  This is a library for the NRMC balance experiment.

  Written by Jacob Huesman for Bison Robotics.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
 
#ifndef BALANCE_H
#define BALANCE_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Refresh {
  public :
    Refresh( long rate );
    void delayMicro( long time );
    void restart( void );
    bool cont( void );
  private :
    unsigned long delay;
    unsigned long curTime;
    unsigned long lastTime;
};

#endif