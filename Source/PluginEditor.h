/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RingView.h"

//==============================================================================
/**
*/
class IlaTestAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    IlaTestAudioProcessorEditor (IlaTestAudioProcessor&);
    ~IlaTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    IlaTestAudioProcessor& audioProcessor;


    
    RingView the_ring;
    
    juce::Slider gateLengthSlider;
    juce::Label gateLabel;
    juce::Label gateTitle;

    juce::Slider rateSlider;
    juce::Label rateLabel;
    juce::Label rateTitle;
   
    juce::Slider jitterSlider;
    juce::Label jitterLabel;
    juce::Label jitterTitle;

    juce::Slider minVelocitySlider;
    juce::Label minVelocityLabel;
    juce::Label minVelocityTitle;

    juce::Slider maxVelocitySlider;
    juce::Label maxVelocityLabel;
    juce::Label maxVelocityTitle;

    juce::TextButton resetButton;
    juce::Label resetButtonLabel;

    void sliderValueChanged(juce::Slider* slider) override;
    void resetButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IlaTestAudioProcessorEditor)
};
