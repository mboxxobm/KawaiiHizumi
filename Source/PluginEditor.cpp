#include "PluginEditor.h"

KawaiiDriveAudioProcessorEditor::KawaiiDriveAudioProcessorEditor(KawaiiDriveAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    configureKnob(driveSlider, "x");
    configureKnob(toneSlider, " Hz");
    configureKnob(mixSlider, "%");
    configureKnob(outputSlider, " dB");
    configureButton(autoMixButton);

    addAndMakeVisible(driveSlider);
    addAndMakeVisible(toneSlider);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(outputSlider);
    addAndMakeVisible(autoMixButton);

    driveAttachment = std::make_unique<SliderAttachment>(audioProcessor.parameters, "drive", driveSlider);
    toneAttachment = std::make_unique<SliderAttachment>(audioProcessor.parameters, "tone", toneSlider);
    mixAttachment = std::make_unique<SliderAttachment>(audioProcessor.parameters, "mix", mixSlider);
    outputAttachment = std::make_unique<SliderAttachment>(audioProcessor.parameters, "output", outputSlider);
    autoMixAttachment = std::make_unique<ButtonAttachment>(audioProcessor.parameters, "autoMix", autoMixButton);

    setResizable(false, false);
    setSize(520, 330);
}

void KawaiiDriveAudioProcessorEditor::configureKnob(juce::Slider& slider, const juce::String& suffix)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 82, 24);
    slider.setTextValueSuffix(suffix);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff09a9d));
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff6a6761));
    slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff2f2b28));
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff2f2b28));
}

void KawaiiDriveAudioProcessorEditor::configureButton(juce::ToggleButton& button)
{
    button.setButtonText("Auto Mix");
    button.setTooltip("Lower the effective Mix as Drive rises.");
    button.setColour(juce::ToggleButton::textColourId, juce::Colour(0xff2f2b28));
    button.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xfff09a9d));
    button.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(0xff8a827a));
}

void KawaiiDriveAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient background(juce::Colour(0xfffff1e7), 0.0f, 0.0f,
                                    juce::Colour(0xffd7f0ef), bounds.getWidth(), bounds.getHeight(), false);
    g.setGradientFill(background);
    g.fillAll();

    g.setColour(juce::Colour(0xff2f2b28));
    g.setFont(juce::FontOptions(30.0f, juce::Font::bold));
    g.drawText("Kawaii Hizumi", 28, 22, getWidth() - 56, 42, juce::Justification::centredLeft);

    g.setFont(juce::FontOptions(14.0f));
    g.setColour(juce::Colour(0xff5d5751));
    g.drawText("soft clip distortion for AU / VST3", 30, 62, getWidth() - 60, 24,
               juce::Justification::centredLeft);

    drawKnobLabel(g, driveSlider, "Drive");
    drawKnobLabel(g, toneSlider, "Tone");
    drawKnobLabel(g, mixSlider, "Mix");
    drawKnobLabel(g, outputSlider, "Output");
}

void KawaiiDriveAudioProcessorEditor::drawKnobLabel(juce::Graphics& g, juce::Slider& slider, const juce::String& label)
{
    g.setColour(juce::Colour(0xff2f2b28));
    g.setFont(juce::FontOptions(15.0f, juce::Font::bold));
    g.drawText(label, slider.getX(), slider.getY() - 24, slider.getWidth(), 20, juce::Justification::centred);
}

void KawaiiDriveAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(28);
    area.removeFromTop(92);

    auto knobArea = area.withHeight(150);
    const auto knobWidth = knobArea.getWidth() / 4;

    driveSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(10, 0));
    toneSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(10, 0));
    mixSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(10, 0));
    outputSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(10, 0));

    area.removeFromTop(18);
    autoMixButton.setBounds(area.withHeight(28).withTrimmedLeft(10));
}
