#include "PluginProcessor.h"
#include "PluginEditor.h"

PF64SEQAudioProcessor::PF64SEQAudioProcessor() {}
PF64SEQAudioProcessor::~PF64SEQAudioProcessor() {}

void StepSequencerAudioProcessor::prepareToPlay (double sr, int)
{
    sampleRate = sr;
}

void StepSequencerAudioProcessor::sendLaunchpadInit(juce::MidiBuffer& midi)
{
    // Programmer mode ON
    uint8 sysex[] = { 0xF0,0x00,0x20,0x29,0x02,0x18,0x0E,0x01,0xF7 };
    midi.addEvent(juce::MidiMessage(sysex, sizeof(sysex)), 0);
}

void StepSequencerAudioProcessor::processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer& midi)
{
    midi.clear();

    // DAW sync
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo pos;
        if (playHead->getCurrentPosition(pos))
        {
            if (pos.isPlaying)
            {
                double ppq = pos.ppqPosition;
                int step = (int)(ppq * 4.0) % 64;

                if (step != currentStep)
                {
                    currentStep = step;
                    advance(midi);
                }
            }
        }
    }
}

void StepSequencerAudioProcessor::advance(juce::MidiBuffer& midi)
{
    for (int i = 0; i < 64; i++)
    {
        if (i == currentStep)
            sendLED(i, 0, 63, 0, midi); // green
        else if (steps[i])
            sendLED(i, 63, 0, 0, midi); // red
        else
            sendLED(i, 0, 0, 0, midi);

        if (i == currentStep && steps[i])
            sendDrum(i, midi);
    }
}

void StepSequencerAudioProcessor::sendLED(int step, uint8 r, uint8 g, uint8 b, juce::MidiBuffer& midi)
{
    int note = (step / 8) * 16 + (step % 8);

    uint8 msg[] = {
        0xF0,0x00,0x20,0x29,0x02,0x18,0x0B,
        (uint8)note,
        r, g, b,
        0xF7
    };

    midi.addEvent(juce::MidiMessage(msg, sizeof(msg)), 0);
}

void StepSequencerAudioProcessor::sendDrum(int step, juce::MidiBuffer& midi)
{
    int row = step / 8;

    int note = 36; // kick default

    if (row == 1) note = 38; // snare
    if (row == 2) note = 42; // hihat

    midi.addEvent(juce::MidiMessage::noteOn(1, note, (uint8)100), 0);
    midi.addEvent(juce::MidiMessage::noteOff(1, note), 120);
}

void StepSequencerAudioProcessor::setStep(int i, bool s) { steps[i] = s; }
bool StepSequencerAudioProcessor::getStep(int i) const { return steps[i]; }

juce::AudioProcessorEditor* PF64SEQAudioProcessor::createEditor()
{
    return new PF64SEQAudioProcessorEditor (*this);
}
