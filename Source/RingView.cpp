/*
 ==============================================================================
 
 RingView.cpp
 Created: 25 Jan 2023 10:20:01pm
 Author:  Guy Brown

 This class contains code to draw the rings.
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "RingView.h"
#include "MyColours.h"

#define DOT_RADIUS 3

//==============================================================================
RingView::RingView()
{
    startTimer(100);
}

RingView::~RingView()
{
}

/*
    This function is used to draw the Rings on the GUI.
*/
void RingView::paint (juce::Graphics& g)
{
    if (model==NULL)
        return;
    int c = model->getRadius()+10;

    // check that we have some active rings (more than 0)
    if (model->getNumRings()>0) {
        // Iterate from 0 to number of rings.
        for (int j=0; j<model->getNumRings(); j++) {
            float z = j*10.0;
            g.setColour(juce::Colour(MY_LIGHTER_BLUE));
            drawCircle(g,c,c,model->getRadius()-z,1);
            // Iterate through the number of beats in the ring.
            for (int i=0; i<model->getNumBeats(j); i++) {
                float theta = i*juce::MathConstants<float>::twoPi/(float)model->getNumBeats(j);
                float x = c+(model->getRadius()-z)*std::sin(theta);
                float y = c-(model->getRadius()-z)*std::cos(theta);
                if (model->getBufferAt(j, i) > 0) {
                    drawHitDot(g, x, y, DOT_RADIUS);
                }
                else {
                    drawOpenDot(g, x, y, DOT_RADIUS);
                }
            }
        }
    }
    }

void RingView::resized()
{
}

void RingView::timerCallback()
{
    repaint();
}

void RingView::drawCircle(juce::Graphics& g, float x, float y, float r, float t) {
    g.drawEllipse(x-r,y-r,2.0f*r,2.0f*r,t);
}

/*
    This draws a dot in which there is no hit.
*/
void RingView::drawOpenDot(juce::Graphics& g, float x, float y, float r) {
    float xo = x-r;
    float yo = y-r;
    g.setColour(juce::Colour(MY_MID_BLUE));
    g.fillEllipse(xo,yo,2.0f*r,2.0f*r);
    g.setColour(juce::Colour(MY_CYAN));
    g.drawEllipse(xo,yo,2.0f*r,2.0f*r,1);
}

/*
    This draws a dot in which a hit is present.
*/
void RingView::drawHitDot(juce::Graphics& g, float x, float y, float r) {
    float xo = x-r;
    float yo = y-r;
    g.setColour(juce::Colour(MY_CYAN));
    g.fillEllipse(xo,yo,2.0f*r,2.0f*r);
    g.setColour(juce::Colour(MY_CYAN));
    g.drawEllipse(xo,yo,2.0f*r,2.0f*r,1);
}

/*
    Unused function
*/
void RingView::drawFilledDot(juce::Graphics& g, float x, float y, float r) {
    g.setColour(juce::Colour(MY_NEAR_WHITE));
    g.fillEllipse(x-r,y-r,2.0f*r,2.0f*r);
}
