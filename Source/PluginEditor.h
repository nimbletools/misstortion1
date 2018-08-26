/*
	==============================================================================

		This file was auto-generated!

		It contains the basic framework code for a JUCE plugin editor.

	==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "PluginLookAndFeel.h"


//==============================================================================
/**
*/
class MisstortionAudioProcessorEditor : public AudioProcessorEditor,
	private Slider::Listener,
	private Timer
{
public:
	MisstortionAudioProcessorEditor(MisstortionAudioProcessor&);
	~MisstortionAudioProcessorEditor();

	void InitializeSlider(Slider &slider, Slider::SliderStyle style, AudioParameterFloat* param, String suffix);
	void InitializeSlider(Slider &slider, Slider::SliderStyle style, AudioParameterInt* param, double skew, String suffix);

	//==============================================================================
	void paintFilterButton(ToggleButton &, Graphics &);
	void paint(Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MisstortionAudioProcessor& processor;

	MisstortionLookAndFeel m_lookAndFeel;
	TooltipWindow m_tooltip;

	Image m_backgroundImage;

	Slider m_sliderMix;
	Slider m_sliderGainIn;
	Slider m_sliderGainOut;

	Slider m_sliderDriveHard;
	Slider m_sliderDriveSoft;
	Slider m_sliderToneHP;
	Slider m_sliderToneLP;
	Slider m_sliderSymmetry;

	ToggleButton m_buttonFilterModeLegacy;
	ToggleButton m_buttonFilterMode6db;
	ToggleButton m_buttonFilterMode12db;

#if DEBUG
	Label m_labelDebug;
#endif

	void setControlBounds(Component &component, int x, int y, int w, int h);
	void setSliderParam(Slider* sliderChanged, Slider &actualSlider, AudioParameterFloat* param);
	void setSliderParam(Slider* sliderChanged, Slider &actualSlider, AudioParameterInt* param);
	void sliderValueChanged(Slider* slider) override;
	void setFilterMode(int newMode);
	virtual void timerCallback() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MisstortionAudioProcessorEditor)
};
