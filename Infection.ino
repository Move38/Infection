#include "blinklib.h"
#include "blinkstate.h"

int encounterCount = 0;
int encounterCountSinceInfected = 0;

enum Health { NORMAL = 1,
              DORMANT = 2,
              SYMPTOMATIC = 3
            };

int myHealth = NORMAL;
bool isPatientZero = false;
bool isGameOver = false;

byte prevNeighborStates[] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  setState(myHealth);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( buttonMultiClicked()) {
    byte count = buttonClickCount();
    if (count == 3) {   // Triple click
      myHealth = DORMANT;
      setState(myHealth);
      // save that I was the Patient Zero
      isPatientZero = true;
    }
    else if (count == 4) {   // 4 clicks reset health (reset game)
      myHealth = NORMAL;
      encounterCount = 0;
      encounterCountSinceInfected = 0;
      setState(myHealth);
      isPatientZero = false;
      isGameOver = false;
    }
  }

  if (!isGameOver) {
    // if I have a new neighbor (one that wasn't there before)
    for (int i = 0; i < 6; i++)
    {

      byte newNeighborState = getNeighborState( i );

      if ( newNeighborState != prevNeighborStates[ i ] &&
           newNeighborState != 0 ) {

        // increase encounterCount
        encounterCount++;

        // if I am NORMAL
        if ( myHealth == NORMAL ) {

          // if neighbor is either DORMANT OR SYMPTOMATIC
          if ( newNeighborState == DORMANT || newNeighborState == SYMPTOMATIC ) {
            // I become DORMANT
            myHealth = DORMANT;
            setState(myHealth);
          }
        }

        // if I am DORMANT
        else if ( myHealth == DORMANT ) {

          // increase my encounters since dormant
          encounterCountSinceInfected++;

          // if my encounterCountSinceInfected is 2 or greater
          if ( encounterCountSinceInfected >= 2 ) {
            // I become SYMPTOMATIC
            myHealth = SYMPTOMATIC;
            setState(myHealth);
            isGameOver = true;
          }
        }

        prevNeighborStates[ i ] = newNeighborState;
      }
    }
  }

  // show our health state
  switch (myHealth) {
    case NORMAL: displayNormal(); break;
    case DORMANT: displayDormant(); break;
    case SYMPTOMATIC: displaySymptoms(); break;
  }
}

void displayNormal() {
  // show green most of the time
  setColor(GREEN);
  // but occasionally show a yellow flash on a single face...
  if (millis() % 1000 > 900) {
    byte face = int(millis() % 6);
    setFaceColor( face, YELLOW );
  }
}

void displayDormant() {
  // show green most of the time
  setColor(GREEN);
  // but occasionally show a red flash on a single face...
  if (millis() % 1000 > 900) {
    byte face = int(millis() % 6);
    setFaceColor( face, YELLOW );
    
    if((millis()/10) % 1000 > 900) {
      byte face = int(millis() % 6);
      setFaceColor( face, ORANGE );
    }
  }
}

void displaySymptoms() {
  //show the score for number of encounters (or if you were infected from the start... remain flashing red)
  if (isPatientZero) {
    setColor(RED);
    delay(300);
    setColor(OFF);
    delay(300);
  }
  else {
    // show the score
    byte count = encounterCount;
    
    setFaceColor(0,RED);
    setFaceColor(1,RED);
    setFaceColor(2,RED);
    setFaceColor(3,ORANGE);
    setFaceColor(4,ORANGE);
    setFaceColor(5,ORANGE);
    delay(1000);
    setColor(OFF);
    delay(300);

    
    while(count > 0) {
      setColor(WHITE);
      delay(300);
      setColor(OFF);
      delay(300);
      
      count--;
    }
  }
}

