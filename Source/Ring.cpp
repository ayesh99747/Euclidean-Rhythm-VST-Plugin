/*
  ==============================================================================

    Ring.cpp

    This class represents one Euclidena Rhythm as a Ring.
    It also contains the parameters for each rhythm.

  ==============================================================================
*/

#include "Ring.h"

/*
    This constructor is used to initialize a Ring.
*/
Ring::Ring() {
    numBeats = 16;
    numHits = 4;
    currentBeat = 0;
    note = 0;
    phase = 0;
    isFlaggedForRemoval = false;
    accentPeriod = 0;
}
