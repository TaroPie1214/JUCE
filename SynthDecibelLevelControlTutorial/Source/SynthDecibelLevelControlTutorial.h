/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             SynthDecibelLevelControlTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Synthesiser with level control in dB.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class DecibelSlider : public juce::Slider
{
public:
    DecibelSlider() {}

    double getValueFromText (const juce::String& text) override
    {
        auto minusInfinitydB = -100.0;

        auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();    // [1]

        return decibelText.equalsIgnoreCase ("-INF") ? minusInfinitydB
                                                     : decibelText.getDoubleValue();  // [2]
    }

    juce::String getTextFromValue (double value) override
    {
        return juce::Decibels::toString (value);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
};

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent
{
public:
    MainContentComponent()
    {
        decibelSlider.setRange (-100, -12);
        decibelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
        decibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain ((float) decibelSlider.getValue()); };
        decibelSlider.setValue (juce::Decibels::gainToDecibels (level));
        decibelLabel.setText ("Noise Level in dB", juce::dontSendNotification);

        addAndMakeVisible (decibelSlider);
        addAndMakeVisible (decibelLabel);

        setSize (600, 100);
        setAudioChannels (0, 2);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int, double) override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto currentLevel = level;
        auto levelScale = currentLevel * 2.0f;

        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                buffer[sample] = random.nextFloat() * levelScale - currentLevel;
        }
    }

    void releaseResources() override {}

    void resized() override
    {
        decibelLabel .setBounds (10, 10, 120, 20);
        decibelSlider.setBounds (130, 10, getWidth() - 140, 20);
    }

private:
    juce::Random random;
    DecibelSlider decibelSlider;
    juce::Label decibelLabel;
    float level = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
