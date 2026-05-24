#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

KawaiiDriveAudioProcessor::KawaiiDriveAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    driveParam = parameters.getRawParameterValue("drive");
    toneParam = parameters.getRawParameterValue("tone");
    mixParam = parameters.getRawParameterValue("mix");
    outputParam = parameters.getRawParameterValue("output");
    autoMixParam = parameters.getRawParameterValue("autoMix");
}

juce::AudioProcessorValueTreeState::ParameterLayout KawaiiDriveAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", juce::NormalisableRange<float>(1.0f, 24.0f, 0.01f, 0.55f), 4.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "tone", "Tone", juce::NormalisableRange<float>(800.0f, 18000.0f, 1.0f, 0.35f), 6500.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 70.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "output", "Output", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), -3.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "autoMix", "Auto Mix", false));

    return { params.begin(), params.end() };
}

void KawaiiDriveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    toneFilter.prepare(spec);
    toneFilter.reset();
}

void KawaiiDriveAudioProcessor::releaseResources() {}

bool KawaiiDriveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainOutput = layouts.getMainOutputChannelSet();
    const auto& mainInput = layouts.getMainInputChannelSet();

    if (mainOutput != juce::AudioChannelSet::mono()
        && mainOutput != juce::AudioChannelSet::stereo())
        return false;

    return mainInput == mainOutput;
}

void KawaiiDriveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const auto totalInputChannels = getTotalNumInputChannels();
    const auto totalOutputChannels = getTotalNumOutputChannels();

    for (auto channel = totalInputChannels; channel < totalOutputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    const auto drive = driveParam->load();
    const auto requestedWet = mixParam->load() / 100.0f;
    const auto driveBalance = static_cast<float>(std::log(drive) / std::log(24.0f));
    const auto autoMixCompensation = juce::jmap(juce::jlimit(0.0f, 1.0f, driveBalance), 1.0f, 0.42f);
    const auto wet = autoMixParam->load() > 0.5f ? requestedWet * autoMixCompensation : requestedWet;
    const auto dry = 1.0f - wet;
    const auto outputGain = juce::Decibels::decibelsToGain(outputParam->load());
    const auto makeup = 1.0f / std::tanh(drive);

    *toneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        currentSampleRate, toneParam->load());

    for (int channel = 0; channel < totalInputChannels; ++channel)
    {
        auto* data = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const auto clean = data[sample];
            const auto driven = std::tanh(clean * drive) * makeup;
            data[sample] = (clean * dry + driven * wet) * outputGain;
        }
    }

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    toneFilter.process(context);
}

void KawaiiDriveAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto state = parameters.copyState(); state.isValid())
    {
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    }
}

void KawaiiDriveAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorEditor* KawaiiDriveAudioProcessor::createEditor()
{
    return new KawaiiDriveAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KawaiiDriveAudioProcessor();
}
