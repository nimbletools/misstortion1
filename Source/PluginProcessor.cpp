/*
	==============================================================================

		This file was auto-generated!

		It contains the basic framework code for a JUCE plugin processor.

	==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Utils.h"


//==============================================================================
MisstortionAudioProcessor::MisstortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	addParameter(m_paramMix = new AudioParameterFloat("mix", "Mix", NormalisableRange<float>(0.0f, 100.0f), 50.0f));
	addParameter(m_paramGain = new AudioParameterFloat("gain", "Gain", NormalisableRange<float>(-50.0f, 50.0f, 0.0f, 0.5f, true), 0.0f));

	addParameter(m_paramDriveHard = new AudioParameterFloat("drive", "Drive", NormalisableRange<float>(0.0f, 50.0f, 0.0f, 0.5f), 0.0f));
	addParameter(m_paramDriveSoft = new AudioParameterFloat("drive2", "Drive 2", NormalisableRange<float>(0.0f, 50.0f, 0.0f, 0.5f), 0.0f));
	addParameter(m_paramDriveSoftPre = new AudioParameterFloat("drive2pre", "Drive 2 Pre", NormalisableRange<float>(0.0f, 100.0f), 50.0f));
	addParameter(m_paramToneHP = new AudioParameterInt("tone", "Tone", 20, 20000, 20));
	addParameter(m_paramToneLP = new AudioParameterInt("tonepost", "Tone Post", 0, 20000, 20000));
	addParameter(m_paramSymmetry = new AudioParameterFloat("symmetry", "Symmetry", NormalisableRange<float>(0.0f, 100.0f), 50.0f));
}

MisstortionAudioProcessor::~MisstortionAudioProcessor()
{
}

//==============================================================================
const String MisstortionAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MisstortionAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MisstortionAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MisstortionAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MisstortionAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MisstortionAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
							// so this should be at least 1, even if you're not really implementing programs.
}

int MisstortionAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MisstortionAudioProcessor::setCurrentProgram(int index)
{
}

const String MisstortionAudioProcessor::getProgramName(int index)
{
	return {};
}

void MisstortionAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void MisstortionAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void MisstortionAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MisstortionAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MisstortionAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; i++) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	double sampleRate = getSampleRate();
	int numSamples = buffer.getNumSamples();

	float mix = (*m_paramMix / 100.0f);
	float gain = Decibels::decibelsToGain((float)*m_paramGain, -50.0f);

	float driveHard = Decibels::decibelsToGain((float)*m_paramDriveHard);
	float driveSoft = Decibels::decibelsToGain((float)*m_paramDriveSoft);
	float driveSoftPre = (*m_paramDriveSoftPre / 100.0f);
	int toneHP = *m_paramToneHP;
	int toneLP = *m_paramToneLP;
	float symmetry = (*m_paramSymmetry / 100.0f);

	m_filtersHP[0].setCoefficients(IIRCoefficients::makeHighPass(sampleRate, (double)toneHP));
	m_filtersHP[1].setCoefficients(IIRCoefficients::makeHighPass(sampleRate, (double)toneHP));

	m_filtersLP[0].setCoefficients(IIRCoefficients::makeLowPass(sampleRate, (double)toneLP));
	m_filtersLP[1].setCoefficients(IIRCoefficients::makeLowPass(sampleRate, (double)toneLP));

	for (int channel = 0; channel < Min(2, totalNumInputChannels); ++channel) {
		float* channelData = buffer.getWritePointer(channel);

		for (int i = 0; i < numSamples; i++) {
			float sample = channelData[i];

			if (mix > 0.0f) {
				sample = m_filtersHP[channel].processSingleSampleRaw(sample);

				if (driveHard > 1.0f) {
					sample = Clamp(-1.0f, 1.0f, sample * driveHard);
				}

				if (driveSoft > 1.0f) {
					float driveMul = 1.0f;
					if (sample < 0) {
						driveMul = driveSoftPre - fmin(0.0f, symmetry * 2.0f - 1.0f);
					} else {
						driveMul = driveSoftPre + fmax(0.0f, symmetry * 2.0f - 1.0f);
					}
					sample = atanf(sample * driveSoft * driveMul);
				}

				if (sample < 0.0f) {
					sample *= (1.0f - symmetry) * 2.0f;
				} else {
					sample *= symmetry * 2.0f;
				}

				sample = m_filtersLP[channel].processSingleSampleRaw(sample);

				channelData[i] = Lerp(channelData[i], sample, mix);
			}

			channelData[i] *= gain;
		}
	}
}

//==============================================================================
bool MisstortionAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MisstortionAudioProcessor::createEditor()
{
	return new MisstortionAudioProcessorEditor(*this);
}

//==============================================================================
void MisstortionAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	ScopedPointer<XmlElement> xml = new XmlElement("root");

	XmlElement* xmlVersion = new XmlElement("version");
	xmlVersion->addTextElement(JucePlugin_VersionString);
	xml->addChildElement(xmlVersion);

	XmlElement* xmlSettings = new XmlElement("settings");
	xmlSettings->setAttribute("mix", *m_paramMix);
	xmlSettings->setAttribute("gain", *m_paramGain);

	xmlSettings->setAttribute("drive", *m_paramDriveHard);
	xmlSettings->setAttribute("drive2", *m_paramDriveSoft);
	xmlSettings->setAttribute("drive2pre", *m_paramDriveSoftPre);
	xmlSettings->setAttribute("tone", *m_paramToneHP);
	xmlSettings->setAttribute("tonepost", *m_paramToneLP);
	xmlSettings->setAttribute("symmetry", *m_paramSymmetry);
	xml->addChildElement(xmlSettings);

	copyXmlToBinary(*xml, destData);
}

void MisstortionAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	ScopedPointer<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);

	XmlElement* xmlSettings = xml->getChildByName("settings");
	if (xmlSettings != nullptr) {
		*m_paramMix = (float)xmlSettings->getDoubleAttribute("mix");
		*m_paramGain = (float)xmlSettings->getDoubleAttribute("gain");

		*m_paramDriveHard = (float)xmlSettings->getDoubleAttribute("drive");
		*m_paramDriveSoft = (float)xmlSettings->getDoubleAttribute("drive2");
		*m_paramDriveSoftPre = (float)xmlSettings->getDoubleAttribute("drive2pre");
		*m_paramToneHP = xmlSettings->getIntAttribute("tone");
		*m_paramToneLP = xmlSettings->getIntAttribute("tonepost");
		*m_paramSymmetry = (float)xmlSettings->getDoubleAttribute("symmetry");
	}

	Logger::writeToLog(String::formatted("  Mix: %f", (float)*m_paramMix));
	Logger::writeToLog(String::formatted("  Gain: %f", (float)*m_paramGain));
	Logger::writeToLog(String::formatted("  Drive: %f", (float)*m_paramDriveHard));
	Logger::writeToLog(String::formatted("  Drive2: %f", (float)*m_paramDriveSoft));
	Logger::writeToLog(String::formatted("  Drive2Pre: %f", (float)*m_paramDriveSoftPre));
	Logger::writeToLog(String::formatted("  Tone: %f", (float)*m_paramToneHP));
	Logger::writeToLog(String::formatted("  TonePost: %f", (float)*m_paramToneLP));
	Logger::writeToLog(String::formatted("  Symmetry: %f", (float)*m_paramSymmetry));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MisstortionAudioProcessor();
}
