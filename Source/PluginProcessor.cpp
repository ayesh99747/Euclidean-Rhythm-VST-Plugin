/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


IlaTestAudioProcessor::IlaTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  )
#endif
{
    model = new RingModel();
}

IlaTestAudioProcessor::~IlaTestAudioProcessor()
{
    delete(model);
}

const juce::String IlaTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IlaTestAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool IlaTestAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool IlaTestAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double IlaTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IlaTestAudioProcessor::getNumPrograms()
{
    return 1;   
}

int IlaTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IlaTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IlaTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void IlaTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void IlaTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    time = 0;
    last_ppq = 0.0;
    playing = false;
    gate_length = 1.0;
    sample_rate = sampleRate;
    rate = 1.0;
    jitter = 0.0;
    minVelocity = 60.0;
    maxVelocity = 120.0;

}

void IlaTestAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IlaTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void IlaTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    int offset = 0;
    juce::MidiBuffer output;

    /*
        This for loop is used to add and remove rings.
        This is done based on the MIDI note messaged received.
        If the note is a note on, we add a new ring based on that note.
        If the note is a note off, we remove the relevant ring.
    */
    for (const auto metadata : midiMessages) {
        auto msg = metadata.getMessage();

        if (msg.isNoteOn()) {
            fprintf(stderr, "NOTE ON\n");
            // If we can add another ring
            if (model->canAddNote()) {
                int noteNumber = msg.getNoteNumber();
                model->addRandomRing(noteNumber);
                // Adding a new event to the midi buffer
            }
        }
        else if (msg.isNoteOff()) {
            // When the finger is lifted from the note
            int n = msg.getNoteNumber();
            model->markForRemoval(n);
        }
    }
  
    
    // clear the buffer, we will create notes outselves so the buffer should not be played
    midiMessages.clear();

    // Extracting the playhead from the DAW.
    if (auto* ph = getPlayHead()) {
        // Extracting the current playhead position.
        auto pos = ph->getPosition();
        if (pos) {
            // Extract the current play positon and beats per minute.
            juce::Optional ppq_opt = pos->getPpqPosition();
            juce::Optional bpm_opt = pos->getBpm();
            playing = pos->getIsPlaying();
                
            if (bpm_opt.hasValue()) {
                /*
                    Based on the beats per second, nuber of samples, gate_length and sample rate,
                    we calculate the note_duration, note_interval and offset.

                    The note_duration is the amount of time to play a note.
                    The note_interval is the time in between notes.
                    The offset is the time to leave before playing the MIDI note.
                */
                double bpm = *bpm_opt;
                double beatsPerSecond = sample_rate / (bpm / 60.0);
                int num_samples = buffer.getNumSamples();
                int note_interval = (int)(rate * beatsPerSecond);
                int note_duration = (int)(gate_length * rate * beatsPerSecond);
                auto offset = juce::jmax(0, juce::jmin((int)(note_duration - time), num_samples - 1));

                for (int i = 0; i < model->getNumRings(); i++) {
                    // If the ring is marked for removal, we send a note off and remove the ring.
                    if (model->isMarkedForRemoval(i)) {
                        output.addEvent(juce::MidiMessage::noteOff(1, model->getNoteByRing(i)), offset);
                        model->removeRing(i);
                        continue;
                    }
                    if (playing)  {
                        if (((time + num_samples) >= note_duration)) {
                            // we send a note off if we are playing and the note duration has passed.
                            output.addEvent(juce::MidiMessage::noteOff(1, model->getNoteByRing(i)), offset); 
                        }
                        if (((time + num_samples) >= note_interval)) {
                            // We have to send a note on if the note_interval has passed.
                            if (model->hasHitOnThisBeat(i)) {
                                // We add jitter to the offset if there is some jitter.
                                int jitteredOffset = offset;
                                if (jitter > 0) {
                                    juce::Random my_random;
                                        jitteredOffset += my_random.nextInt(jitter * 2) - jitter;
                                }

                                // We calculate the amount of velocity to send based on the accent period.
                                if ((model->getCurrentBeat(i) % model->getAccentPeriod(i)) == 0) {
                                    output.addEvent(juce::MidiMessage::noteOn(1, model->getNoteByRing(i), (juce::uint8)maxVelocity), jitteredOffset);
                                }
                                else {
                                    output.addEvent(juce::MidiMessage::noteOn(1, model->getNoteByRing(i), (juce::uint8)minVelocity), jitteredOffset);
                                }
                            }
                            model->tickByRing(i);
                        }
                    }
                }
                time = (time + num_samples) % note_interval;

                }
            }
            else {
                model->reset();
            }
        }
    midiMessages.swapWith(output);
    
}

bool IlaTestAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* IlaTestAudioProcessor::createEditor()
{
    return new IlaTestAudioProcessorEditor (*this);
}

void IlaTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void IlaTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IlaTestAudioProcessor();
}
