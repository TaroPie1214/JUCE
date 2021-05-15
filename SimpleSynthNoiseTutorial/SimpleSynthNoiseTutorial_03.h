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

 name:             SimpleSynthNoiseTutorial
 version:          3.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Simple noise synthesiser.

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
class MainContentComponent   : public juce::AudioAppComponent
{
public:
    MainContentComponent()
    {
        targetLevel = 0.125f;

        levelSlider.setRange (0.0, 0.25);
        levelSlider.setValue (targetLevel, juce::dontSendNotification);
        levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
        levelSlider.onValueChange = [this]
        {
            targetLevel = (float) levelSlider.getValue();
            samplesToTarget = rampLengthSamples;
        };

        levelLabel.setText ("Noise Level", juce::dontSendNotification);

        addAndMakeVisible (&levelSlider);
        addAndMakeVisible (&levelLabel);

        setSize (800, 100);

        setAudioChannels (0, 2);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int, double) override
    {
        resetParameters();
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto numSamplesRemaining = bufferToFill.numSamples;
        auto offset = 0;

        if (samplesToTarget > 0)
        {
            auto levelIncrement = (targetLevel - currentLevel) / (float) samplesToTarget;
            auto numSamplesThisTime = juce::jmin (numSamplesRemaining, samplesToTarget);

            for (auto sample = 0; sample < numSamplesThisTime; ++sample)
            {
                for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
                    bufferToFill.buffer->setSample (channel, sample, random.nextFloat() * currentLevel);

                currentLevel += levelIncrement;
                --samplesToTarget;
            }

            offset = numSamplesThisTime;
            numSamplesRemaining -= numSamplesThisTime;

            if (samplesToTarget == 0)
                currentLevel = targetLevel;
        }

        if (numSamplesRemaining > 0)
        {
            for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample + offset);

                for (auto sample = 0; sample < numSamplesRemaining; ++sample)
                    *buffer++ = random.nextFloat() * currentLevel;
            }
        }
    }

    void releaseResources() override {}

    void resized() override
    {
        levelLabel .setBounds (10, 10, 90, 20);
        levelSlider.setBounds (100, 10, getWidth() - 110, 20);
    }

    void resetParameters()
    {
        currentLevel = targetLevel;
        samplesToTarget = 0;
    }

private:
    juce::Random random;
    juce::Slider levelSlider;
    juce::Label levelLabel;
    float currentLevel;
    float targetLevel;
    int samplesToTarget;

    static constexpr auto rampLengthSamples = 128;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
