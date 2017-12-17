#include "blinklib.h"
#include "blinkstate.h"

int encounterCount = 0;
int encounterCountSinceInfected = 0;

enum Health { NORMAL = 1,
              DORMANT = 2,
              SYMPTOMATIC = 3
            };

int myHealth = NORMAL;

byte prevNeighborStates[] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  setState(myHealth);
}

void loop() {
  // put your main code here, to run repeatedly:
  if( buttonMultiClicked()) {
    byte count=buttonClickCount();
    if (count==3) {     // Triple click
      myHealth = DORMANT;
      setState(myHealth);
    }
    else if (count==4) {     // 4 clicks reset health (reset game)
      myHealth = NORMAL;
      encounterCount = 0;
      encounterCountSinceInfected = 0;
      setState(myHealth);
    }
  }

  // if I have a new neighbor (one that wasn't there before)
  for(int i=0; i<6; i++) 
  {

    byte newNeighborState = getNeighborState( i );

    if ( newNeighborState != prevNeighborStates[ i ] &&
         newNeighborState != 0 ) {
          
      // increase encounterCount
      encounterCount++;

      // if I am NORMAL
      if ( myHealth == NORMAL ) {
        
        // if neighbor is either DORMANT OR SYMPTOMATIC
        if( newNeighborState == DORMANT || newNeighborState == SYMPTOMATIC ) {
          // I become DORMANT
          myHealth = DORMANT;
          setState(myHealth);
        }
      }

      // if I am DORMANT
      else if( myHealth == DORMANT ) {
        
        // increase my encounters since dormant
        encounterCountSinceInfected++;

        // if my encounterCountSinceInfected is 2 or greater
        if( encounterCountSinceInfected >= 2 ) {
          // I become SYMPTOMATIC
          myHealth = SYMPTOMATIC;
          setState(myHealth);
        }
      }

      prevNeighborStates[ i ] = newNeighborState;
    }
  }

  // show our health state
  switch(myHealth) {
    case NORMAL: setColor(GREEN); break;
    case DORMANT: setColor(GREEN); break;
    case SYMPTOMATIC: setColor(RED); break;
  }
}
