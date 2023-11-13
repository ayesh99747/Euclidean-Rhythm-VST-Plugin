/*
  ==============================================================================

    RingView.h
    Created: 25 Jan 2023 10:20:01pm
    Author:  Guy Brown

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RingModel.h"

//==============================================================================
/*
*/
class RingView  : public juce::Component, public juce::Timer
{
public:
    RingView();
    ~RingView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    RingModel *model = NULL;
    
private:
    void drawCircle(juce::Graphics& g, float x, float y, float r, float t);
    void drawOpenDot(juce::Graphics& g, float x, float y, float r);
    void drawHitDot(juce::Graphics& g, float x, float y, float r);
    void drawFilledDot(juce::Graphics& g, float x, float y, float r);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingView)
};
