/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MyColours.h"


IlaTestAudioProcessorEditor::IlaTestAudioProcessorEditor (IlaTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    setSize (500, 400);
    
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour(MY_LIGHTER_BLUE));

    gateLengthSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    gateLengthSlider.setRange (0.1, 1.0, 0.01);
    gateLengthSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gateLengthSlider.setPopupDisplayEnabled (false, false, this);
    gateLengthSlider.setTextValueSuffix (" Gate");
    gateLengthSlider.setValue(0.7);
    gateLengthSlider.addListener(this);

    gateLabel.setText("Gate", juce::dontSendNotification);
    gateLabel.setFont(juce::Font(14.0f));
    gateLabel.setJustificationType(juce::Justification::centred);

    gateTitle.setText("Gate", juce::dontSendNotification);
    gateTitle.setFont(juce::Font(14.0f));
    gateTitle.setJustificationType(juce::Justification::centred);

    rateSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    rateSlider.setRange (1, 5, 1);
    rateSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    rateSlider.setPopupDisplayEnabled (false, false, this);
    rateSlider.setTextValueSuffix (" Rate");
    rateSlider.setValue(3);
    rateSlider.addListener(this);

    rateLabel.setText("A/B", juce::dontSendNotification);
    rateLabel.setFont(juce::Font(14.0f));
    rateLabel.setJustificationType(juce::Justification::centred);

    rateTitle.setText("Rate", juce::dontSendNotification);
    rateTitle.setFont(juce::Font(14.0f));
    rateTitle.setJustificationType(juce::Justification::centred);

    jitterSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    jitterSlider.setRange(0.0,20.0,1.0);
    jitterSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    jitterSlider.setPopupDisplayEnabled(false, false, this);
    jitterSlider.setTextValueSuffix(" Jitter");
    jitterSlider.setValue(0.0);
    jitterSlider.addListener(this);

    jitterLabel.setText("Jitter", juce::dontSendNotification);
    jitterLabel.setFont(juce::Font(14.0f));
    jitterLabel.setJustificationType(juce::Justification::centred);

    jitterTitle.setText("Jitter", juce::dontSendNotification);
    jitterTitle.setFont(juce::Font(14.0f));
    jitterTitle.setJustificationType(juce::Justification::centred);

    minVelocitySlider.setSliderStyle(juce::Slider::LinearBarVertical);
    minVelocitySlider.setRange(0.0, 120.0, 1.0);
    minVelocitySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    minVelocitySlider.setPopupDisplayEnabled(false, false, this);
    minVelocitySlider.setTextValueSuffix("Min Velocity");
    minVelocitySlider.setValue(60.0);
    minVelocitySlider.addListener(this);

    minVelocityLabel.setText("Min \n Vel", juce::dontSendNotification);
    minVelocityLabel.setFont(juce::Font(14.0f));
    minVelocityLabel.setJustificationType(juce::Justification::centred);

    minVelocityTitle.setText("Min \n Vel", juce::dontSendNotification);
    minVelocityTitle.setFont(juce::Font(14.0f));
    minVelocityTitle.setJustificationType(juce::Justification::centred);

    maxVelocitySlider.setSliderStyle(juce::Slider::LinearBarVertical);
    maxVelocitySlider.setRange(0.0, 120.0, 1.0);
    maxVelocitySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    maxVelocitySlider.setPopupDisplayEnabled(false, false, this);
    maxVelocitySlider.setTextValueSuffix("Max Vel");
    maxVelocitySlider.setValue(120.0);
    maxVelocitySlider.addListener(this);

    maxVelocityLabel.setText("Max \n Vel", juce::dontSendNotification);
    maxVelocityLabel.setFont(juce::Font(14.0f));
    maxVelocityLabel.setJustificationType(juce::Justification::centred);

    maxVelocityTitle.setText("Max \n Vel", juce::dontSendNotification);
    maxVelocityTitle.setFont(juce::Font(14.0f));
    maxVelocityTitle.setJustificationType(juce::Justification::centred);

    resetButtonLabel.setText("Reset", juce::dontSendNotification);
    resetButtonLabel.setFont(juce::Font(14.0f));
    resetButtonLabel.setJustificationType(juce::Justification::centred);

    resetButton.setButtonText("RESET");
    resetButton.onClick = [this] {resetButtonClicked(); };
    

    // this function adds the slider to the editor
    addAndMakeVisible (&gateLengthSlider);
    addAndMakeVisible(&gateLabel);
    addAndMakeVisible(&gateTitle);

    addAndMakeVisible (&rateSlider);
    addAndMakeVisible (&rateLabel);
    addAndMakeVisible (&rateTitle);
    
    addAndMakeVisible (&the_ring);

    addAndMakeVisible (&jitterSlider);
    addAndMakeVisible (&jitterLabel);
    addAndMakeVisible (&jitterTitle);

    addAndMakeVisible(&minVelocitySlider);
    addAndMakeVisible(&minVelocityLabel);
    addAndMakeVisible(&minVelocityTitle);

    addAndMakeVisible(&maxVelocitySlider);
    addAndMakeVisible(&maxVelocityLabel);
    addAndMakeVisible(&maxVelocityTitle);

    addAndMakeVisible(&resetButton);

    the_ring.model = audioProcessor.model; // pointer copy refer to same model
    
}

IlaTestAudioProcessorEditor::~IlaTestAudioProcessorEditor()
{
}

//==============================================================================
void IlaTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(MY_DARK_BLUE));
    g.setColour (juce::Colour(MY_PURPLE));
    g.setFont (14.0f);
    g.drawFittedText ("E U C L I D E A N | R H Y T H M S", 0, 10, getWidth(), 30, juce::Justification::centred, 1);
}

void IlaTestAudioProcessorEditor::resized()
{
    gateLengthSlider.setBounds (40, 100, 20, 200);
    gateLabel.setBounds(20, 310, 60, 30);
    gateTitle.setBounds(20, 65, 60, 30);

    rateSlider.setBounds (80, 100, 20, 200);
    rateLabel.setBounds(60, 310, 60, 30);
    rateTitle.setBounds(60, 65, 60, 30);

    jitterSlider.setBounds(120, 100, 20, 200);
    jitterLabel.setBounds(100, 310, 60, 30);
    jitterTitle.setBounds(100, 65, 60, 30);

    minVelocitySlider.setBounds(160, 100, 20, 200);
    minVelocityLabel.setBounds(140, 310, 60, 30);
    minVelocityTitle.setBounds(140, 65, 60, 30);

    maxVelocitySlider.setBounds(200, 100, 20, 200);
    maxVelocityLabel.setBounds(180, 310, 60, 30);
    maxVelocityTitle.setBounds(180, 65, 60, 30);

    the_ring.setBounds(260,90,310,310);
    
     
    resetButton.setBounds(350, 330, 50, 25);
}

void IlaTestAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    audioProcessor.gate_length = gateLengthSlider.getValue();
    gateLabel.setText(juce::String::formatted("%d%%", (int)(audioProcessor.gate_length * 100)), juce::dontSendNotification);

    int r = rateSlider.getValue();
    int n = pow(2,r);
    audioProcessor.rate = 4.0/(float)n; // this is the rate adjustment
    rateLabel.setText(juce::String::formatted("1/%d",n), juce::dontSendNotification);

    audioProcessor.jitter = jitterSlider.getValue();
    jitterLabel.setText(juce::String::formatted("%d% ms", (int)(audioProcessor.jitter)), juce::dontSendNotification);

    audioProcessor.minVelocity = minVelocitySlider.getValue();
    minVelocityLabel.setText(juce::String::formatted("%d%", (int)(audioProcessor.minVelocity)), juce::dontSendNotification);

    audioProcessor.maxVelocity = maxVelocitySlider.getValue();
    maxVelocityLabel.setText(juce::String::formatted("%d%", (int)(audioProcessor.maxVelocity)), juce::dontSendNotification);

    repaint();
}

void IlaTestAudioProcessorEditor::resetButtonClicked()
{
    audioProcessor.model->reset();
}

