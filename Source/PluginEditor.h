#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class KawaiiDriveAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit KawaiiDriveAudioProcessorEditor(KawaiiDriveAudioProcessor&);
    ~KawaiiDriveAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    void configureKnob(juce::Slider& slider, const juce::String& suffix = {});
    void configureButton(juce::ToggleButton& button);
    void drawKnobLabel(juce::Graphics& g, juce::Slider& slider, const juce::String& label);

    KawaiiDriveAudioProcessor& audioProcessor;

    juce::Slider driveSlider;
    juce::Slider toneSlider;
    juce::Slider mixSlider;
    juce::Slider outputSlider;
    juce::ToggleButton autoMixButton;

    std::unique_ptr<SliderAttachment> driveAttachment;
    std::unique_ptr<SliderAttachment> toneAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> outputAttachment;
    std::unique_ptr<ButtonAttachment> autoMixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KawaiiDriveAudioProcessorEditor)
};
