/*
 ==============================================================================
 
    RingModel.h

    This class is used to store all the rings.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Ring.h"

#define MAX_BEATS 16
#define MAX_RINGS 5

class RingModel {
    
public:
    RingModel();
    void tickByRing(int idx);
    void addRandomRing(int note);
    void removeRing(int idx);
    void debug();
    int getNumBeats(int idx);
    int getNumHits(int idx);
    int getCurrentBeat(int idx);
    int getRadius();
    int getBufferAt(int idx, int i);
    int getNumRings();
    int getAccentPeriod(int idx);
    int getNoteByRing(int idx);
    bool hasHitOnThisBeat(int idx);
    bool canAddNote();
    void markForRemoval(int note);
    bool isMarkedForRemoval(int idx);
    void reset();
    
private:
    int current_beat = 0;
    int radius = 100;
        
    juce::Array<Ring> rings;
        
};
