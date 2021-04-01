#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(400, 300);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    
    g.fillAll(juce::Colours::lightblue);

    g.setColour(juce::Colours::darkblue);
    g.setFont(20.0f);
    g.drawText("Hello, World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
