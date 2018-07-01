/*
	==============================================================================

		This file was auto-generated!

		It contains the basic framework code for a JUCE plugin processor.

	==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class MisstortionAudioProcessor : public AudioProcessor
{
public:
	AudioParameterFloat* m_paramMix;
	AudioParameterFloat* m_paramGainIn;
	AudioParameterFloat* m_paramGainOut;

	AudioParameterFloat* m_paramDriveHard;
	AudioParameterFloat* m_paramDriveSoft;
	AudioParameterInt* m_paramToneHP;
	AudioParameterInt* m_paramToneLP;
	AudioParameterFloat* m_paramSymmetry;

	IIRFilter m_filtersHP[2];
	IIRFilter m_filtersLP[2];

#if DEBUG
	String m_debugText;
#endif

public:
	//==============================================================================
	MisstortionAudioProcessor();
	~MisstortionAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MisstortionAudioProcessor)
};
