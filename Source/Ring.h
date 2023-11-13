/*
  ==============================================================================

    Ring.h

    This class represents one Euclidena Rhythm as a Ring.
    It also contains the parameters for each rhythm.

  ==============================================================================
*/

#pragma once

// This constant defines the maximum number of beats for a ring.
#define MAX_BEATS 16

class Ring {
    
public:
    Ring();
    int numBeats;
    int numHits;
    int currentBeat;
    int buffer[MAX_BEATS];
    int note;
    int phase;
    bool isFlaggedForRemoval;
    int accentPeriod;
};
