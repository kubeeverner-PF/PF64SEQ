#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class PF64SEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StepSequencerAudioProcessorEditor (StepSequencerAudioProcessor&);
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent&) override;

private:
    StepSequencerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencerAudioProcessorEditor)
};
