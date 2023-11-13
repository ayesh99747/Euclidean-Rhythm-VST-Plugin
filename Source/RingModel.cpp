/*
  ==============================================================================

    RingModel.cpp

    This class is used to store all the rings.

  ==============================================================================
*/

#include "RingModel.h"

/*
    Ensures that an array of size 10 is allocated and cleared
*/
RingModel::RingModel() {
    rings.ensureStorageAllocated(MAX_RINGS);
    rings.clear();
}

/*
    The tickByRing() function advances the current beat of the ring index provided.
    If the current beat exceeds the number of beats for a ring (r.num_beats), it resets the beat back to 0.
*/
void RingModel::tickByRing(int idx) {
    rings.getReference(idx).currentBeat++;
    if (rings.getReference(idx).currentBeat >= rings.getReference(idx).numBeats) {
        rings.getReference(idx).currentBeat = 0;
    }
   
}

/*
    hasHitOnThisBeat() returns a boolean value indicating whether a ring at a specific index (idx) has a hit on the current beat.
*/
int RingModel::getNoteByRing(int idx) {
    return rings.getReference(idx).note;
}

/*
    hasHitOnThisBeat() returns a boolean value indicating whether a ring at a specific index (idx) has a hit on the current beat. 
*/
bool RingModel::hasHitOnThisBeat(int idx) {
    return rings.getReference(idx).buffer[rings[idx].currentBeat]>0;
}


/*
    This method can be used to get the number of beats.
*/
int RingModel::getNumBeats(int note) {
    return rings.getReference(note).numBeats;
}
/*
    This method can be used to get the number of hits.
*/
int RingModel::getNumHits(int idx) {
    return rings.getReference(idx).numHits;
}
/*
    This method can be used to get the current beat.
*/
int RingModel::getCurrentBeat(int idx) {
    return rings.getReference(idx).currentBeat;
}

/*
    This method can be used to get the radius
*/
int RingModel::getRadius() {
    return radius;
}

/*
    This method can be used to get the buffer at the current index.
*/
int RingModel::getBufferAt(int idx, int i) {
    return rings.getReference(idx).buffer[i];
}

/*
    This method can be used to get the number of rings in the rings array.
*/
int RingModel::getNumRings() {
    return rings.size();
}

int RingModel::getAccentPeriod(int idx)
{
   return rings.getReference(idx).accentPeriod;
    
}

/*
    This method can be used to validate if we can add more rings.
*/
bool RingModel::canAddNote() {
    return rings.size()<MAX_RINGS;
}

/*
    This method can be used to add a random ring.
*/
void RingModel::addRandomRing(int note) {

    // If we have reached the maximum number of rings we return.
    if (rings.size()==MAX_RINGS)
        return;

    // We are generating random numbers here and initializing the ring object
    juce::Random my_random;
    Ring the_ring;
    the_ring.numBeats = my_random.nextInt(juce::Range(2,MAX_BEATS));
    the_ring.phase = my_random.nextInt(juce::Range(0,the_ring.numBeats-1));
    the_ring.numHits = my_random.nextInt(juce::Range(1,the_ring.numBeats));
    the_ring.currentBeat = 0;
    the_ring.note = note;

    if (the_ring.numBeats>4) {
        the_ring.accentPeriod = my_random.nextInt(juce::Range(0, the_ring.numBeats - 1));
    }
    else {
        the_ring.accentPeriod = 0;
    }

    // fill the buffer
    float slope = the_ring.numHits/(float)the_ring.numBeats;
    int previous = -1;
    /*
    This loop iterates over each beat within the ring. For each beat, it calculates the corresponding 
    current index based on the slope value multiplied by the loop counter (i). The floor function is 
    used to round down the resulting floating-point value to the nearest integer.

    The value at the_ring.buffer[i] is set to 127 if the current value is different from the previous value. 
    This signifies a hit or onset at that beat. Otherwise, the value is set to 0, indicating no hit or onset.

    The previous variable is then updated with the current value, preparing it for the next iteration.
    */
    for (int i=0; i<the_ring.numBeats; i++) {
      int current = floor(i*slope);
      the_ring.buffer[(i+the_ring.phase)%the_ring.numBeats] = (current!=previous) ? 127 : 0;
      previous = current;
    }

    // add the ring
    rings.add(the_ring);
}

/*
    This method can be used to get the number of beats.
*/
void RingModel::removeRing(int idx) {
    rings.remove(idx);
}

/*
    This method can be used to mark that a ring should be removed.
*/
void RingModel::markForRemoval(int note) {
    for (int i=0; i<rings.size(); i++) {
        if (rings.getReference(i).note==note) {
            rings.getReference(i).isFlaggedForRemoval = true;
            break;
        }
    }
}

/*
    This method is used to check whether a ring has been marked for removal.
*/
bool RingModel::isMarkedForRemoval(int idx)
{
    return rings.getReference(idx).isFlaggedForRemoval;
}

/*
    This method is used to reset all the rings in the Rings array.
    Each ring is marked for removal individually.
*/
void RingModel::reset()
{
    for (int i = 0; i < rings.size(); i++) {
        rings.getReference(i).isFlaggedForRemoval = true;
    }
}

/*
    This method is used to debug the rings array.
*/
void RingModel::debug() {
    fprintf(stdout,"number of rings: %d\n",rings.size());
    for (Ring r : rings) {
        fprintf(stdout,"[note=%d beats=%d hits=%d current_beat=%d]\n",r.note,r.numBeats,r.numHits,r.currentBeat);
        for (int k=0; k<r.numBeats; k++)
            fprintf(stdout,"%d ",r.buffer[k]);
        fprintf(stdout,"\n");
    }
}
