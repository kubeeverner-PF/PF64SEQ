#pragma once
#include <JuceHeader.h>

class PF64SEQAudioProcessor  : public juce::AudioProcessor
{
public:
    StepSequencerAudioProcessor();
    ~StepSequencerAudioProcessor() override;

    void prepareToPlay (double, int) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }

    void sendLaunchpadInit(juce::MidiBuffer&);
    void setStep(int index, bool state);
    bool getStep(int index) const;

private:
    bool steps[64] = { false };
    int currentStep = 0;

    double bpm = 120.0;
    double sampleRate = 44100.0;
    int samplesPerStep = 0;
    int counter = 0;

    void advance(juce::MidiBuffer&);
    void sendLED(int step, juce::uint8 r, juce::uint8 g, juce::uint8 b, juce::MidiBuffer&);
    void sendDrum(int step, juce::MidiBuffer&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencerAudioProcessor)
};
