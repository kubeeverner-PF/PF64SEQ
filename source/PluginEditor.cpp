#include "PluginEditor.h"

StepSequencerAudioProcessorEditor::StepSequencerAudioProcessorEditor (StepSequencerAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    setSize (400, 400);
}

void StepSequencerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    int size = 40;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int i = y * 8 + x;

            g.setColour(processor.getStep(i) ? juce::Colours::red : juce::Colours::darkgrey);
            g.fillRect(x * size, y * size, size - 2, size - 2);
        }
    }
}

void StepSequencerAudioProcessorEditor::resized() {}

void StepSequencerAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    int size = 40;

    int x = e.x / size;
    int y = e.y / size;

    int i = y * 8 + x;

    processor.setStep(i, !processor.getStep(i));
    repaint();
}
